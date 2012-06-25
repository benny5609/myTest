#include "ClientConnect.h"

void TestServer(_ThreadMsg* pThreadMsg)
{
	char szSendDate[100] = {'\0'};
	int nAllLen = 10;
	int nCommandID = 0x2000;
	//int nCommandID = 0x1000;
	int nStrlen = 0;
	ACE_Time_Value tvNow = ACE_OS::gettimeofday();
	ACE_UINT64 u8Date = 0;
	tvNow.msec(u8Date);

	char szPlayerNick[20] = {'\0'};
	sprintf_s(szPlayerNick, 20, "freeeyes");
	uint8 u1PlayerNick = ACE_OS::strlen(szPlayerNick);

	memcpy(&szSendDate[0], (const void*)&nAllLen, sizeof(int));
	memcpy(&szSendDate[4], (const void*)&nCommandID, 2);
	//memcpy(&szSendDate[6], (const void*)&u8Date, sizeof(ACE_UINT64));
	memcpy(&szSendDate[6], (const void*)&u1PlayerNick, sizeof(uint8));
	memcpy(&szSendDate[7], (const void*)szPlayerNick, u1PlayerNick);

	for(int i = 0; i < pThreadMsg->nCount; i++)
	{
		ACE_Message_Block* pmblk = new ACE_Message_Block(nAllLen + 4);

		ACE_OS::memcpy(pmblk->wr_ptr(), szSendDate, nAllLen + 4);
		pmblk->wr_ptr(nAllLen + 4);

		App_ClientProConnectManager::instance()->SendData(pThreadMsg->nIndex, pmblk->rd_ptr(), pmblk->length());
	}

	//等待接收完成
	int nMaxWaitTime = 0;
	while(nMaxWaitTime <= 500000)
	{
		if(pThreadMsg->pClientMessage->m_nRecvCount <= 0)
		{
			//接收数据全部完成
			break;
		}

		ACE_Time_Value tvSleep(0, 1000);
		ACE_OS::sleep(tvSleep);
		nMaxWaitTime++;
	}

	App_ClientProConnectManager::instance()->Close(pThreadMsg->nIndex);
}

DWORD WINAPI ThreadFunc(PVOID pvParam)
{
	_ThreadMsg* pThreadMsg = (_ThreadMsg *)pvParam;

	TestServer(pThreadMsg);

	delete pThreadMsg;

	return 0;
}

CClientConnect::CClientConnect(void)
{
	//初始化反应器
	bool blState = App_ProactorManager::instance()->AddNewProactor(REACTOR_POSTDEFINE, Proactor_WIN32, 1);
	if(false == blState)
	{
		ACE_DEBUG((LM_ERROR, "[Main]App_ProactorManager::instance()->AddNewProactor() errror.\n"));
		getchar();
	}

	App_ProactorManager::instance()->StartProactor();

	App_ClientProConnectManager::instance()->Init(App_ProactorManager::instance()->GetAce_Proactor(REACTOR_POSTDEFINE));
	App_ClientProConnectManager::instance()->StartConnectTask(10);
}

CClientConnect::~CClientConnect(void)
{
	App_ClientProConnectManager::instance()->Close();
}

bool CClientConnect::Init(const char* pServerIP, int nPort, int nThreadCount, int nSendCount, int& nTimeCost)
{
	HANDLE* hThreadID = new HANDLE[nThreadCount];
	vecClientMessage objvecClientMessage;

	//初始化异步链接
	for(int i = 0; i < nThreadCount; i++)
	{	
		CClientMessage* pClientMessage = new CClientMessage();

		//pClientMessage->m_nRecvCount = nSendCount * 2;
		pClientMessage->m_nRecvCount = nSendCount;

		if(false == App_ClientProConnectManager::instance()->Connect(i, pServerIP, nPort, pClientMessage))
		{
			TRACE("[TestServer]Connect error.(%d)",i);
			return false;
		}

		objvecClientMessage.push_back(pClientMessage);

		ACE_Time_Value tvSleep(0, 50 * 1000);
		ACE_OS::sleep(tvSleep);
	}

	ACE_Time_Value tvBegin = ACE_OS::gettimeofday();

	//开始发送
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
		pThreadMsg->nIndex = i;
		pThreadMsg->pClientMessage = objvecClientMessage[i];

		DWORD dwThreadID = 0;
		HANDLE hThread = CreateThread(NULL, 0, ThreadFunc, (PVOID)pThreadMsg, 0, &dwThreadID);
		hThreadID[i] = hThread;
	}

	WaitForMultipleObjects(nThreadCount, hThreadID, TRUE, INFINITE);

	for(int i = 0; i < nThreadCount; i++)
	{
		CloseHandle(hThreadID[i]);
	}

	ACE_Time_Value tvEnd = ACE_OS::gettimeofday();
	ACE_Time_Value tvDisposeTime(tvEnd - tvBegin);

	nTimeCost = tvDisposeTime.msec();

	return true;
}
