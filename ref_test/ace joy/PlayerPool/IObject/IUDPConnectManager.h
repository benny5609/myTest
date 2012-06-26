#ifndef _IUDPCONNECTMANAGER_H
#define _IUDPCONNECTMANAGER_H

#include "IBuffPacket.h"

class IUDPConnectManager
{
public:
	virtual ~IUDPConnectManager() {};
	virtual bool SendMessage(uint32 u4ConnectID, IBuffPacket* pBuffPacket, const char* szIP, int nPort) = 0;
};

#endif
