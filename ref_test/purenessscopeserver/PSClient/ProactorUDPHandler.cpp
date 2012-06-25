#include "ProactorUDPHandler.h"

CProactorUDPHandler::CProactorUDPHandler(void)
{
}

CProactorUDPHandler::~CProactorUDPHandler(void)
{
}

int CProactorUDPHandler::OpenAddress(const ACE_INET_Addr& AddrRemote, ACE_Proactor* pProactor)
{
	if(m_skRemote.open(AddrRemote) == -1)
	{
		ACE_DEBUG((LM_ERROR, "[CProactorUDPHandler::OpenAddress]Open error(%d).\n", errno));
		return -1;
	}

	char* pCompletionKey = NULL;
	if(m_Read.open(*this, m_skRemote.get_handle(), pCompletionKey, pProactor) == -1)
	{
		ACE_DEBUG((LM_ERROR, "[CProactorUDPHandler::OpenAddress]m_Read error.\n"));
		return -1;
	}

	if(m_Write.open(*this, m_skRemote.get_handle(), pCompletionKey, pProactor) == -1)
	{
		ACE_DEBUG((LM_ERROR, "[CProactorUDPHandler::OpenAddress]m_Write error.\n"));
		return -1;
	}

	char* pAct = NULL;
	ACE_Message_Block* pMBBuff = NULL;   
	ACE_NEW_RETURN(pMBBuff, ACE_Message_Block(MAX_UDP_PACKET_LEN), -1);

	size_t stRecvLen = MAX_UDP_PACKET_LEN;
	int nRecvSize = m_Read.recv(pMBBuff, stRecvLen, 0, PF_INET, pAct);   
	return nRecvSize;   
}  

void CProactorUDPHandler::Close()
{
	m_Read.cancel();
	m_Write.cancel();
	m_skRemote.close();
}

void CProactorUDPHandler::handle_read_dgram(const ACE_Asynch_Read_Dgram::Result& result)
{
	//因为UDP是一次性发送完整的数据包，所以不必做粘包处理
	ACE_Message_Block* pMb = result.message_block();
	int nPacketLen = (int)result.bytes_transferred();
	int nTran = (int)result.bytes_transferred();

	if(nPacketLen != 0 && nTran != 0)
	{
		//测试代码
		char* pData = new char[nTran + 1];
		ACE_OS::memcpy(pData, pMb->rd_ptr(), (int)pMb->length());
		pData[nTran] = '\0';
		ACE_DEBUG((LM_INFO, "[CProactorUDPHandler::handle_read_dgram]%s.\n", pData));
		delete[] pData;
		pMb->release();

		char* pAct = NULL;
		ACE_Message_Block* pMBBuff = new ACE_Message_Block(MAX_UDP_PACKET_LEN);   

		size_t stRecvLen = MAX_UDP_PACKET_LEN;
		int nRecvSize = m_Read.recv(pMBBuff, stRecvLen, 0, PF_INET, pAct);   
	}
	else
	{
		//链接关闭
		pMb->release();
		ACE_DEBUG((LM_INFO, "[CProactorUDPHandler::handle_read_dgram]Close.\n"));
	}
}

bool CProactorUDPHandler::SendMessage(const char* pMessage, int nLen, const ACE_INET_Addr& AddrRemote)
{
	int nSize = (int)m_skRemote.send(pMessage, nLen, AddrRemote);
	if(nSize == nLen)
	{
		return true;
	}
	else
	{
		int nErr = errno;
		ACE_DEBUG((LM_ERROR, "[CProactorUDPHandler::SendMessage]send error(%d).\n", errno));
		return false;
	}
}

