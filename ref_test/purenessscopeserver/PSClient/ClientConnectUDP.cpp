#include "ClientConnectUDP.h"

DWORD WINAPI ThreadFuncUDP(PVOID pvParam)
{
	_ThreadMsg* pThreadMsg = (_ThreadMsg *)pvParam;

	CProactorUDPHandler UDP1;

	char szSendDate[100] = {'\0'};
	char szData[20]      = {'\0'};

	sprintf(szData, "freeeyes");

	int nAllLen = 11;
	int nCommandID = 0x1000;
	int nStrlen = 8;
	memcpy(&szSendDate[0], (const void*)&nAllLen, sizeof(int));
	memcpy(&szSendDate[4], (const void*)&nCommandID, 2);
	memcpy(&szSendDate[6], (const void*)&nStrlen, 1);
	memcpy(&szSendDate[7], szData, (int)strlen(szData));

	ACE_INET_Addr AddrLocal(pThreadMsg->nPort + 10, pThreadMsg->szIP);
	ACE_INET_Addr AddrRemote(pThreadMsg->nPort, pThreadMsg->szIP);

	UDP1.OpenAddress(AddrLocal, ACE_Proactor::instance());

	for(int i = 0; i < pThreadMsg->nCount; i++)
	{
		UDP1.SendMessage(szSendDate, 15, AddrRemote);
		ACE_DEBUG((LM_INFO, "Send OK(%d).\n", i));
		ACE_Time_Value tvSleep(0, 100);
		ACE_OS::sleep(tvSleep);
	}

	UDP1.Close();

	delete pThreadMsg;

	return 0;
}

CClientConnectUDP::CClientConnectUDP(void)
{
}

CClientConnectUDP::~CClientConnectUDP(void)
{
}

bool CClientConnectUDP::Init(const char* pServerIP, int nPort, int nThreadCount, int nSendCount, int& nTimeCost)
{
	HANDLE* hThreadID = new HANDLE[nThreadCount];   

	ACE_Time_Value tvBegin = ACE_OS::gettimeofday();
	//while(true)   //这里可以测试无限次循环
	{
		for(int i = 0; i < nThreadCount; i++)
		{
			_ThreadMsg* pThreadMsg = new _ThreadMsg();
			if(NULL == pThreadMsg)
			{
				ACE_DEBUG((LM_ERROR, "[Main]pThreadMsg is NULL.\n"));
				return false;
			}

			sprintf_s(pThreadMsg->szIP, 20, "%s", pServerIP);
			pThreadMsg->nPort  = nPort;
			pThreadMsg->nCount = nSendCount;

			DWORD dwThreadID = 0;
			HANDLE hThread = CreateThread(NULL, 0, ThreadFuncUDP, (PVOID)pThreadMsg, 0, &dwThreadID);
			hThreadID[i] = hThread;
		}

		WaitForMultipleObjects(nThreadCount, hThreadID, TRUE, INFINITE);

		for(int i = 0; i < nThreadCount; i++)
		{
			CloseHandle(hThreadID[i]);
		}
	}
	ACE_Time_Value tvEnd = ACE_OS::gettimeofday();
	ACE_Time_Value tvDisposeTime(tvEnd - tvBegin);

	nTimeCost = tvDisposeTime.msec();

	return true;
}

