#ifndef _IMESSAGE_H
#define _IMESSAGE_H

#include "IBuffPacket.h"

//记录消息的一些参数
struct _MessageBase
{
	uint32 m_u4ConnectID;
	uint32 m_u4PacketID;
	uint16 m_u2Cmd;
	uint32 m_u4MsgTime;

	_MessageBase()
	{
		m_u4ConnectID = 0;
		m_u4PacketID  = 0;
		m_u2Cmd       = 0;
		m_u4MsgTime   = 0;
	}
};

//消息信息类的接口
class IMessage
{
public:
	virtual ~IMessage() {};

	virtual void Close() = 0;
	virtual void Clear() = 0;

	virtual void SetMessageBase(_MessageBase* pMessageBase)              = 0;
	virtual bool SetRecvPacket(IBuffPacket* pRecvPacket)                 = 0;
	virtual bool SetMessageInfo(const char* szClientIP, int nClientPort) = 0;

	virtual bool GetPacketHead(_PacketInfo& PacketInfo)    = 0;
	virtual bool GetPacketBody(_PacketInfo& PacketInfo)    = 0;
	virtual bool SetPacketHead(ACE_Message_Block* pmbHead) = 0;
	virtual bool SetPacketBody(ACE_Message_Block* pmbBody) = 0;

	virtual _MessageBase* GetMessageBase() = 0;
	virtual IBuffPacket*  GetRecvPacket()  = 0;
	virtual char* GetClientIP()            = 0;
	virtual int   GetClientPort()          = 0;

	virtual const char* GetError()         = 0;
};

//中间服务器消息类接口
class IPostMessage
{
public:
	IPostMessage() 
	{ 
		m_pRecvPacket = NULL; 
		m_pSendPacket = NULL; 
		m_u4ServerID  = 0;
		m_u2CommandID = 0;
		m_blDelete    = true;
	};

	virtual ~IPostMessage()
	{
		Close();
	};

	virtual void Close()
	{
		if(NULL != m_pRecvPacket)
		{
			delete m_pRecvPacket;
			m_pRecvPacket = NULL;
		}

		if(NULL != m_pSendPacket)
		{
			delete m_pSendPacket;
			m_pSendPacket = NULL;
		}

		m_u4ServerID  = 0;
		m_u2CommandID = 0;
		m_blDelete    = true;
	};

	virtual bool SetRecvPacket(IBuffPacket* pRecvPacket)
	{
		if(NULL == pRecvPacket)
		{
			return false;
		}

		m_pRecvPacket = pRecvPacket;
		return true;
	};

	virtual bool SetSendPacket(IBuffPacket* pSendPacket)
	{
		if(NULL == pSendPacket)
		{
			return false;
		}

		m_pSendPacket = pSendPacket;
		return true;
	};

	virtual IBuffPacket*  GetRecvPacket()
	{
		return m_pRecvPacket;
	};

	virtual IBuffPacket*  GetSendPacket()
	{
		return m_pSendPacket;
	};

	virtual void CallBefore()              = 0;
	virtual void CallBack()                = 0;

	virtual void   SetServerID(uint32 u4ServerID)
	{
		m_u4ServerID = u4ServerID;
	};

	virtual uint32 GetServerID()
	{
		return m_u4ServerID;
	};

	virtual void   SetCommandID(uint16 u2CommandID)
	{
		m_u2CommandID = u2CommandID;
	};

	virtual uint16 GetCommandID()
	{
		return m_u2CommandID;
	};

private:
	IBuffPacket* m_pRecvPacket;
	IBuffPacket* m_pSendPacket;
	uint32       m_u4ServerID;
	uint16       m_u2CommandID;
	bool         m_blDelete;
};

#endif
