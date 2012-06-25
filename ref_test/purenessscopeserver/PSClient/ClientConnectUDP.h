#pragma once

#include "ProactorUDPHandler.h"

class CClientConnectUDP
{
public:
	CClientConnectUDP(void);
	~CClientConnectUDP(void);

	bool Init(const char* pServerIP, int nPort, int nThreadCount, int nSendCount, int& nTimeCost);
};
