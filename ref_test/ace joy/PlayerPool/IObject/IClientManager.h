#ifndef _ICLIENTMANAGER_H
#define _ICLIENTMANAGER_H

#include "ClientMessage.h"

//��������������ͨѶ�����ݹ���
class IClientManager
{
public:
	virtual ~IClientManager() {};

	virtual bool Connect(int nServerID, const char* pIP, int nPort, IClientMessage* pClientMessage) = 0;
	virtual bool Close(int nServerID)                                                               = 0;
	virtual bool SendData(int nServerID, ACE_Message_Block* pmblk)                                  = 0;
	virtual bool StartConnectTask(int nIntervalTime)                                                = 0;                             
	virtual void CancelConnectTask()                                                                = 0;                                                                  
	virtual void Close()                                                                            = 0;
};

#endif