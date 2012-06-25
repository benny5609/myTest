// BuffPacket.h
// 处理消息的类，用于将消息分发到需要处理的类中
// 学习是必要的，但更重要的是自我的实践。当疲乏来临是，需要的是战胜它。
// add by freeeyes
// 2009-01-10

#include "Message.h"

CMessage::CMessage(void)
{
	m_pRecvPacket   = NULL;
	m_pMessageBase  = NULL;
	m_pmbHead       = NULL;
	m_pmbBody       = NULL;
	m_szError[0]    = '\0';

	m_pMessageBase = new _MessageBase();
}

CMessage::~CMessage(void)
{
	Close();
}

const char* CMessage::GetError()
{
	return m_szError;
}

void CMessage::SetMessageBase(_MessageBase* pMessageBase)
{
	if(NULL != m_pMessageBase)
	{
		delete m_pMessageBase;
		m_pMessageBase = NULL;
	}

	m_pMessageBase = pMessageBase;
}

bool CMessage::SetRecvPacket(IBuffPacket* pRecvPacket)
{
	if(NULL == pRecvPacket)
	{
		sprintf_safe(m_szError, MAX_BUFF_500, "[CMessage::SetRecvPacket]Set RecvPacket is NULL");
		return false;
	}

	if(NULL != m_pRecvPacket)
	{
		delete m_pRecvPacket;
		m_pRecvPacket = NULL;
	}
	
	m_pRecvPacket = pRecvPacket;
	return true;
}

_MessageBase* CMessage::GetMessageBase()
{
	return m_pMessageBase;
}

IBuffPacket* CMessage::GetRecvPacket()
{
	return m_pRecvPacket;
}

bool CMessage::GetPacketHead(_PacketInfo& PacketInfo)
{
	if(m_pmbHead != NULL)
	{
		PacketInfo.m_pData    = m_pmbHead->rd_ptr();
		PacketInfo.m_nDataLen = (int)m_pmbHead->length();
		return true;
	}
	else
	{
		return false;
	}
}

bool CMessage::GetPacketBody(_PacketInfo& PacketInfo)
{
	if(m_pmbBody != NULL)
	{
		PacketInfo.m_pData    = m_pmbBody->rd_ptr();
		PacketInfo.m_nDataLen = (int)m_pmbBody->length();
		return true;
	}
	else
	{
		return false;
	}
}

bool CMessage::SetPacketHead(ACE_Message_Block* pmbHead)
{
	m_pmbHead = pmbHead;
	return true;
}

bool CMessage::SetPacketBody(ACE_Message_Block* pmbBody)
{
	m_pmbBody = pmbBody;
	return true;
}

void CMessage::Close()
{
	if(NULL != m_pMessageBase)
	{
		delete m_pMessageBase;
		m_pMessageBase = NULL;
	}

	if(NULL != m_pRecvPacket)
	{
		delete m_pRecvPacket;
		m_pRecvPacket = NULL;
	}

	if(NULL != m_pmbHead)
	{
		m_pmbHead->release();
		m_pmbHead = NULL;
	}

	if(NULL != m_pmbBody)
	{
		m_pmbBody->release();
		m_pmbBody = NULL;
	}
}

void CMessage::Clear()
{
	if(NULL != m_pmbHead)
	{
		App_MessageBlockManager::instance()->Close(m_pmbHead);
		m_pmbHead = NULL;
	}

	if(NULL != m_pmbBody)
	{
		App_MessageBlockManager::instance()->Close(m_pmbBody);
		m_pmbBody = NULL;
	}
}

CMessagePool::CMessagePool()
{
}

CMessagePool::~CMessagePool()
{
	Close();
}

void CMessagePool::Init(uint32 u4PacketCount)
{
	Close();

	for(int i = 0; i < (int)u4PacketCount; i++)
	{
		CMessage* pPacket = new CMessage();
		if(NULL != pPacket)
		{
			//添加到Free map里面
			mapMessage::iterator f = m_mapMessageFree.find(pPacket);
			if(f == m_mapMessageFree.end())
			{
				m_mapMessageFree.insert(mapMessage::value_type(pPacket, pPacket));
			}
		}
	}
}

void CMessagePool::Close()
{
	//清理所有已存在的指针
	mapMessage::iterator itorFreeB = m_mapMessageFree.begin();
	mapMessage::iterator itorFreeE = m_mapMessageFree.end();

	for(itorFreeB; itorFreeB != itorFreeE; itorFreeB++)
	{
		CMessage* pPacket = (CMessage* )itorFreeB->second;
		SAFE_DELETE(pPacket);
	}

	mapMessage::iterator itorUsedB = m_mapMessageUsed.begin();
	mapMessage::iterator itorUsedE = m_mapMessageUsed.end();

	for(itorUsedB; itorUsedB != itorUsedE; itorUsedB++)
	{
		CMessage* pPacket = (CMessage* )itorUsedB->second;
		SAFE_DELETE(pPacket);
	}

	m_mapMessageFree.clear();
	m_mapMessageUsed.clear();
}

int CMessagePool::GetUsedCount()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	return (int)m_mapMessageUsed.size();
}

int CMessagePool::GetFreeCount()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	return (int)m_mapMessageFree.size();
}

CMessage* CMessagePool::Create()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	//如果free池中已经没有了，则添加到free池中。
	if(m_mapMessageFree.size() <= 0)
	{
		CMessage* pPacket = new CMessage();

		if(pPacket != NULL)
		{
			//添加到Free map里面
			mapMessage::iterator f = m_mapMessageFree.find(pPacket);
			if(f == m_mapMessageFree.end())
			{
				m_mapMessageFree.insert(mapMessage::value_type(pPacket, pPacket));
			}
		}
		else
		{
			return NULL;
		}
	}

	//从free池中拿出一个,放入到used池中
	mapMessage::iterator itorFreeB = m_mapMessageFree.begin();
	CMessage* pPacket = (CMessage* )itorFreeB->second;
	m_mapMessageFree.erase(itorFreeB);
	//添加到used map里面
	mapMessage::iterator f = m_mapMessageUsed.find(pPacket);
	if(f == m_mapMessageUsed.end())
	{
		m_mapMessageUsed.insert(mapMessage::value_type(pPacket, pPacket));
	}

	return (CMessage* )pPacket;
}

bool CMessagePool::Delete(CMessage* pBuffPacket)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	CMessage* pBuff = (CMessage* )pBuffPacket;
	if(NULL == pBuff)
	{
		return false;
	}

	pBuffPacket->Clear();

	mapMessage::iterator f = m_mapMessageUsed.find(pBuff);
	if(f != m_mapMessageUsed.end())
	{
		m_mapMessageUsed.erase(f);

		//添加到Free map里面
		mapMessage::iterator f = m_mapMessageFree.find(pBuff);
		if(f == m_mapMessageFree.end())
		{
			m_mapMessageFree.insert(mapMessage::value_type(pBuff, pBuff));
		}
	}

	return true;
}



