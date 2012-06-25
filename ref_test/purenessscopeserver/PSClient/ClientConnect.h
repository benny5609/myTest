#pragma once

#include "define.h"
#include "ClientProConnectManager.h"
#include <vector>

using namespace std;

class CClientMessage : public IClientMessage
{
public:
	CClientMessage() { m_nRecvCount = 0; };
	~CClientMessage() {};

	bool RecvData(CClientParse* pClientParse)
	{
		ACE_Message_Block* pmbBody = pClientParse->GetMessageBody();

		uint16 u2CommandID = 0;
		uint32 u4PlayerID  = 0;

		memcpy(&u2CommandID, pmbBody->rd_ptr(), sizeof(uint16));
		pmbBody->rd_ptr(sizeof(uint16));
		memcpy(&u4PlayerID, pmbBody->rd_ptr(), sizeof(uint32));
		pmbBody->rd_ptr(sizeof(uint32));

		TRACE("[CClientMessage::RecvData]Command=%d u4PlayerID=%d OK.\n", u2CommandID, u4PlayerID);
		if(m_nRecvCount > 0)
		{
			m_nRecvCount--;
		}
		//TRACE("[CClientMessage::RecvData]Recv OK.");
		return true;
	}

	bool ConnectError(int nError)
	{
		//OUR_DEBUG((LM_ERROR, "[CClientMessage::ConnectError] Error (%d).\n", nError));
		return true;
	}

public:
	int m_nRecvCount;
};

typedef vector<CClientMessage*> vecClientMessage;

class CClientConnect
{
public:
	CClientConnect(void);
	~CClientConnect(void);

	bool Init(const char* pServerIP, int nPort, int nThreadCount, int nSendCount, int& nTimeCost);
};
