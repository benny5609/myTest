#include "ManagerConnect.h"

CManagerConnect::CManagerConnect(void)
{
	m_blConnected = false;
}

CManagerConnect::~CManagerConnect(void)
{
	Close();
}

bool CManagerConnect::Connect(const char* pManagerIP, int nPort)
{
	if(m_blConnected == false)
	{
		ACE_Time_Value tv(5);

		//设置远程链接接口
		if(-1 == m_adrClient.set(nPort, pManagerIP))
		{
			ACE_DEBUG((LM_ERROR, "[CManagerConnect::Connec]adrClient(%s:%d) error.\n", pManagerIP, nPort));
			return false;
		}

		//连接远程接口
		if(-1 == m_sckConnectClient.connect(m_sckStream, m_adrClient, &tv))
		{
			int nError = errno;
			ACE_DEBUG((LM_ERROR, "[TestServer]Connect error[%d]. [%s] [%d].\n", errno, pManagerIP, nPort));
			return false;
		}

		m_blConnected = true;
	}
	return true;
}

void CManagerConnect::Close()
{
	if(m_blConnected == true)
	{
		m_sckStream.close();
		m_blConnected = false;
	}
}

bool CManagerConnect::SendShowModule(vecModuleInfo& VecModuleInfo)
{
	if(m_blConnected == false)
	{
		return false;
	}

	char szSendDate[100] = {'\0'};
	char szCommand[100]  = {'\0'}; 
	ACE_Time_Value tv(5);
	size_t stSendEnd = 0;

	sprintf_s(szCommand, 100, "ShowModule -a");
	int nAllLen = (int)strlen(szCommand);
	memcpy(&szSendDate[0], (const void*)&nAllLen, sizeof(int));
	memcpy(&szSendDate[4], (const void*)&szCommand, nAllLen);

	//发送数据
	m_sckStream.send_n(szSendDate, nAllLen + 4, &tv, &stSendEnd);
	if(stSendEnd != nAllLen + 4)
	{
		ACE_DEBUG((LM_ERROR, "[TestServer]Send Data Error.\n"));
		Close();
		return false;
	}

	int nFinishRecv = 0;
	int nRecvLen    = 0;
	int nPacketLen  = 0;
	char* pBuff = new char[1024*100];

	while(true)
	{
		//接收数据
		ssize_t stLen = m_sckStream.recv(&pBuff[nRecvLen], 1024*100 - nRecvLen, &tv);
		if((int)stLen <= 0)
		{
			delete[] pBuff;
			return false;
		}

		if((int)stLen >= 4)
		{
			memcpy(&nPacketLen, pBuff, sizeof(int));
			nFinishRecv = nPacketLen + 4;
		}

		nRecvLen += (int)stLen;

		if(nRecvLen >= nFinishRecv)
		{
			break;
		}
	}

	int nStrLen      = 0;
	int nPos         = 4;
	int nModuleCount = 0;
	memcpy(&nModuleCount, &pBuff[nPos], sizeof(int));
	nPos += sizeof(int);

	for(int i = 0; i < nModuleCount; i++)
	{
		//开始还原数据结构
		_ModuleInfo ModuleInfo;
		memcpy(&nStrLen, &pBuff[nPos], sizeof(char));
		nPos += sizeof(char);

		memcpy(ModuleInfo.szModuleName, &pBuff[nPos], nStrLen);
		nPos += nStrLen;
		ModuleInfo.szModuleName[nStrLen] = '\0';
		
		memcpy(&nStrLen, &pBuff[nPos], sizeof(char));
		nPos += sizeof(char);

		memcpy(ModuleInfo.szModuleFile, &pBuff[nPos], nStrLen);
		nPos += nStrLen;
		ModuleInfo.szModuleFile[nStrLen] = '\0';

		memcpy(&nStrLen, &pBuff[nPos], sizeof(char));
		nPos += sizeof(char);

		memcpy(ModuleInfo.szModuleDesc, &pBuff[nPos], nStrLen);
		nPos += nStrLen;
		ModuleInfo.szModuleDesc[nStrLen] = '\0';

		memcpy(&nStrLen, &pBuff[nPos], sizeof(char));
		nPos += sizeof(char);

		memcpy(ModuleInfo.szModuleCreateDate, &pBuff[nPos], nStrLen);
		nPos += nStrLen;
		ModuleInfo.szModuleCreateDate[nStrLen] = '\0';

		VecModuleInfo.push_back(ModuleInfo);
	}

	delete[] pBuff;
	return true;
}

bool CManagerConnect::SendUnloadModule(const char* pCommand)
{
	if(m_blConnected == false)
	{
		return false;
	}

	char szSendDate[100] = {'\0'};
	char szCommand[100]  = {'\0'}; 
	ACE_Time_Value tv(5);
	size_t stSendEnd = 0;

	sprintf_s(szCommand, 100, "UnLoadModule %s", pCommand);
	int nAllLen = (int)strlen(szCommand);
	memcpy(&szSendDate[0], (const void*)&nAllLen, sizeof(int));
	memcpy(&szSendDate[4], (const void*)&szCommand, nAllLen);

	//发送数据
	m_sckStream.send_n(szSendDate, nAllLen + 4, &tv, &stSendEnd);

	char* pBuff = new char[100];
	ssize_t stLen = m_sckStream.recv(pBuff, 100, &tv);
	if((int)stLen <= 0)
	{
		delete[] pBuff;
		return false;
	}

	int nPacketLen = 0;
	int nReturn    = 0;

	memcpy(&nPacketLen, &pBuff[0], sizeof(int));
	memcpy(&nReturn, &pBuff[4], sizeof(char));

	if(nReturn == 0)
	{
		delete[] pBuff;
		return true;
	}
	else
	{
		delete[] pBuff;
		return false;
	}
}

bool CManagerConnect::SendLoadModule(const char* pCommand)
{
	if(m_blConnected == false)
	{
		return false;
	}

	char szSendDate[100] = {'\0'};
	char szCommand[100]  = {'\0'}; 
	ACE_Time_Value tv(5);
	size_t stSendEnd = 0;

	sprintf_s(szCommand, 100, "LoadModule %s", pCommand);
	int nAllLen = (int)strlen(szCommand);
	memcpy(&szSendDate[0], (const void*)&nAllLen, sizeof(int));
	memcpy(&szSendDate[4], (const void*)&szCommand, nAllLen);

	//发送数据
	m_sckStream.send_n(szSendDate, nAllLen + 4, &tv, &stSendEnd);

	char* pBuff = new char[100];
	ssize_t stLen = m_sckStream.recv(pBuff, 100, &tv);
	if(stLen == 0)
	{
		delete[] pBuff;
		return false;
	}

	int nPacketLen = 0;
	int nReturn    = 0;

	memcpy(&nPacketLen, &pBuff[0], sizeof(int));
	memcpy(&nReturn, &pBuff[4], sizeof(char));

	if(nReturn == 0)
	{
		delete[] pBuff;
		return true;
	}
	else
	{
		delete[] pBuff;
		return false;
	}
}

bool CManagerConnect::SendReLoadModule(const char* pCommand)
{
	if(m_blConnected == false)
	{
		return false;
	}

	char szSendDate[100] = {'\0'};
	char szCommand[100]  = {'\0'}; 
	ACE_Time_Value tv(5);
	size_t stSendEnd = 0;

	sprintf_s(szCommand, 100, "ReLoadModule %s", pCommand);
	int nAllLen = (int)strlen(szCommand);
	memcpy(&szSendDate[0], (const void*)&nAllLen, sizeof(int));
	memcpy(&szSendDate[4], (const void*)&szCommand, nAllLen);

	//发送数据
	m_sckStream.send_n(szSendDate, nAllLen + 4, &tv, &stSendEnd);

	char* pBuff = new char[100];
	ssize_t stLen = m_sckStream.recv(pBuff, 100, &tv);
	if(stLen == 0)
	{
		delete[] pBuff;
		return false;
	}

	int nPacketLen = 0;
	int nReturn    = 0;

	memcpy(&nPacketLen, &pBuff[0], sizeof(int));
	memcpy(&nReturn, &pBuff[4], sizeof(char));

	if(nReturn == 0)
	{
		delete[] pBuff;
		return true;
	}
	else
	{
		delete[] pBuff;
		return false;
	}
}

bool CManagerConnect::SendClientCount(const char* pCommand, int& nCount, int& nPoolCount)
{
	if(m_blConnected == false)
	{
		return false;
	}

	char szSendDate[100] = {'\0'};
	char szCommand[100]  = {'\0'}; 
	ACE_Time_Value tv(5);
	size_t stSendEnd = 0;

	sprintf_s(szCommand, 100, "%s", pCommand);
	int nAllLen = (int)strlen(szCommand);
	memcpy(&szSendDate[0], (const void*)&nAllLen, sizeof(int));
	memcpy(&szSendDate[4], (const void*)&szCommand, nAllLen);

	//发送数据
	m_sckStream.send_n(szSendDate, nAllLen + 4, &tv, &stSendEnd);

	char* pBuff = new char[100];
	ssize_t stLen = m_sckStream.recv(pBuff, 100, &tv);
	if((int)stLen <= 0)
	{
		delete[] pBuff;
		return false;
	}

	int nPacketLen   = 0;
	int nClientCount = 0;

	memcpy(&nPacketLen, &pBuff[0], sizeof(int));
	memcpy(&nCount, &pBuff[4], sizeof(int));
	memcpy(&nPoolCount, &pBuff[8], sizeof(int));

	delete[] pBuff;
	return true;
}

bool CManagerConnect::SendCommandInfo(const char* pCommand, vecCommandInfo& VecCommandInfo, int& nCommandCount)
{
	if(m_blConnected == false)
	{
		return false;
	}

	char szSendDate[100] = {'\0'};
	char szCommand[100]  = {'\0'}; 
	ACE_Time_Value tv(5);
	size_t stSendEnd = 0;

	sprintf_s(szCommand, 100, "%s", pCommand);
	int nAllLen = (int)strlen(szCommand);
	memcpy(&szSendDate[0], (const void*)&nAllLen, sizeof(int));
	memcpy(&szSendDate[4], (const void*)&szCommand, nAllLen);

	//发送数据
	m_sckStream.send_n(szSendDate, nAllLen + 4, &tv, &stSendEnd);

	int nFinishRecv = 0;
	int nRecvLen    = 0;
	int nPacketLen  = 0;
	char* pBuff = new char[1024*100];

	while(true)
	{
		//接收数据
		ssize_t stLen = m_sckStream.recv(&pBuff[nRecvLen], 1024*100 - nRecvLen, &tv);
		if((int)stLen <= 0)
		{
			delete[] pBuff;
			return false;
		}

		if((int)stLen >= 4)
		{
			memcpy(&nPacketLen, pBuff, sizeof(int));
			nFinishRecv = nPacketLen + 4;
		}

		nRecvLen += (int)stLen;

		if(nRecvLen >= nFinishRecv)
		{
			break;
		}
	}

	int nStrLen           = 0;
	int nPos              = 4;
	int nCommandInfoCount = 0;
	memcpy(&nCommandCount, &pBuff[nPos], sizeof(int));
	nPos += sizeof(int);
	memcpy(&nCommandInfoCount, &pBuff[nPos], sizeof(short));
	nPos += sizeof(short);

	for(int i = 0; i < nCommandInfoCount; i++)
	{
		//开始还原数据结构
		_CommandInfo CommandInfo;
		memcpy(&nStrLen, &pBuff[nPos], sizeof(char));
		nPos += sizeof(char);

		memcpy(CommandInfo.szModuleName, &pBuff[nPos], nStrLen);
		nPos += nStrLen;
		CommandInfo.szModuleName[nStrLen] = '\0';

		int nCommandID = 0;
		memcpy(&nCommandID, &pBuff[nPos], sizeof(short));
		nPos += sizeof(short);
		sprintf_s(CommandInfo.szCommandID, MAX_BUFF_200, "0x%04x", nCommandID);

		memcpy(&CommandInfo.m_nCount, &pBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		memcpy(&CommandInfo.m_nTimeCost, &pBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		VecCommandInfo.push_back(CommandInfo);
	}

	delete[] pBuff;
	return true;
}

bool CManagerConnect::SendCommandAllInfo(const char* pCommand, vecCommandInfo& VecCommandInfo, int& nCommandCount)
{
	if(m_blConnected == false)
	{
		return false;
	}

	char szSendDate[100] = {'\0'};
	char szCommand[100]  = {'\0'}; 
	ACE_Time_Value tv(5);
	size_t stSendEnd = 0;

	sprintf_s(szCommand, 100, "%s", pCommand);
	int nAllLen = (int)strlen(szCommand);
	memcpy(&szSendDate[0], (const void*)&nAllLen, sizeof(int));
	memcpy(&szSendDate[4], (const void*)&szCommand, nAllLen);

	//发送数据
	m_sckStream.send_n(szSendDate, nAllLen + 4, &tv, &stSendEnd);

	int nFinishRecv = 0;
	int nRecvLen    = 0;
	int nPacketLen  = 0;
	char* pBuff = new char[1024*100];

	while(true)
	{
		//接收数据
		ssize_t stLen = m_sckStream.recv(&pBuff[nRecvLen], 1024*100 - nRecvLen, &tv);
		if((int)stLen <= 0)
		{
			delete[] pBuff;
			return false;
		}

		if((int)stLen >= 4)
		{
			memcpy(&nPacketLen, pBuff, sizeof(int));
			nFinishRecv = nPacketLen + 4;
		}

		nRecvLen += (int)stLen;

		if(nRecvLen >= nFinishRecv)
		{
			break;
		}
	}

	int nStrLen           = 0;
	int nPos              = 4;
	int nMoudleCount      = 0;
	memcpy(&nMoudleCount, &pBuff[nPos], sizeof(int));
	nPos += sizeof(int);

	for(int j = 0; j < nMoudleCount; j++)
	{
		int nCommandInfoCount = 0;
		memcpy(&nCommandCount, &pBuff[nPos], sizeof(int));
		nPos += sizeof(int);
		for(int i = 0; i < nCommandCount; i++)
		{
			//开始还原数据结构
			_CommandInfo CommandInfo;
			memcpy(&nStrLen, &pBuff[nPos], sizeof(char));
			nPos += sizeof(char);

			memcpy(CommandInfo.szModuleName, &pBuff[nPos], nStrLen);
			nPos += nStrLen;
			CommandInfo.szModuleName[nStrLen] = '\0';

			int nCommandID = 0;
			memcpy(&nCommandID, &pBuff[nPos], sizeof(short));
			nPos += sizeof(short);
			sprintf_s(CommandInfo.szCommandID, MAX_BUFF_200, "0x%04x", nCommandID);

			memcpy(&CommandInfo.m_nCount, &pBuff[nPos], sizeof(int));
			nPos += sizeof(int);

			memcpy(&CommandInfo.m_nTimeCost, &pBuff[nPos], sizeof(int));
			nPos += sizeof(int);

			VecCommandInfo.push_back(CommandInfo);
		}
	}

	delete[] pBuff;
	return true;
}

bool CManagerConnect::SendWorkThreadState(const char* pCommand, vecWorkThreadInfo& VecWorkThreadInfo)
{
	if(m_blConnected == false)
	{
		return false;
	}

	char szSendDate[100] = {'\0'};
	char szCommand[100]  = {'\0'}; 
	ACE_Time_Value tv(5);
	size_t stSendEnd = 0;

	sprintf_s(szCommand, 100, "%s", pCommand);
	int nAllLen = (int)strlen(szCommand);
	memcpy(&szSendDate[0], (const void*)&nAllLen, sizeof(int));
	memcpy(&szSendDate[4], (const void*)&szCommand, nAllLen);

	//发送数据
	m_sckStream.send_n(szSendDate, nAllLen + 4, &tv, &stSendEnd);

	int nFinishRecv = 0;
	int nRecvLen    = 0;
	int nPacketLen  = 0;
	char* pBuff = new char[1024*100];

	while(true)
	{
		//接收数据
		ssize_t stLen = m_sckStream.recv(&pBuff[nRecvLen], 1024*100 - nRecvLen, &tv);
		if((int)stLen <= 0)
		{
			delete[] pBuff;
			return false;
		}

		if((int)stLen >= 4)
		{
			memcpy(&nPacketLen, pBuff, sizeof(int));
			nFinishRecv = nPacketLen + 4;
		}

		nRecvLen += (int)stLen;

		if(nRecvLen >= nFinishRecv)
		{
			break;
		}
	}

	int nStrLen              = 0;
	int nPos                 = 4;
	int nWorkThreadInfoCount = 0;
	//包处理线程
	memcpy(&nWorkThreadInfoCount, &pBuff[nPos], sizeof(char));
	nPos += sizeof(char);

	for(int i = 0; i < nWorkThreadInfoCount; i++)
	{
		//开始还原数据结构
		_WorkThreadInfo WorkThreadInfo;
		sprintf_s(WorkThreadInfo.m_szThreadName, MAX_BUFF_50, "工作线程");

		memcpy(&WorkThreadInfo.m_nThreadID, &pBuff[nPos], sizeof(char));
		nPos += sizeof(char);

		int nTimeValue = 0;
		memcpy(&nTimeValue, &pBuff[nPos], sizeof(int));
		nPos += sizeof(int);
		WorkThreadInfo.m_tvUpdateTime.sec(nTimeValue);

		memcpy(&WorkThreadInfo.m_nState, &pBuff[nPos], sizeof(char));
		nPos += sizeof(char);

		memcpy(&WorkThreadInfo.m_nRecvPacketCount, &pBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		memcpy(&WorkThreadInfo.m_nSendPacketCount, &pBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		memcpy(&WorkThreadInfo.m_nCommandID, &pBuff[nPos], sizeof(short));
		nPos += sizeof(short);

		memcpy(&WorkThreadInfo.m_nPacketTime, &pBuff[nPos], sizeof(short));
		nPos += sizeof(short);

		memcpy(&WorkThreadInfo.m_nCurrPacketCount, &pBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		VecWorkThreadInfo.push_back(WorkThreadInfo);
	}

	delete[] pBuff;
	return true;
}

bool CManagerConnect::SendShowForbiddenIP(const char* pCommand, vecForbiddenIP& VecForbiddenIP)
{
	if(m_blConnected == false)
	{
		return false;
	}

	char szSendDate[100] = {'\0'};
	char szCommand[100]  = {'\0'}; 
	ACE_Time_Value tv(5);
	size_t stSendEnd = 0;

	sprintf_s(szCommand, 100, "%s", pCommand);
	int nAllLen = (int)strlen(szCommand);
	memcpy(&szSendDate[0], (const void*)&nAllLen, sizeof(int));
	memcpy(&szSendDate[4], (const void*)&szCommand, nAllLen);

	//发送数据
	m_sckStream.send_n(szSendDate, nAllLen + 4, &tv, &stSendEnd);

	int nFinishRecv = 0;
	int nRecvLen    = 0;
	int nPacketLen  = 0;
	char* pBuff = new char[1024*100];

	while(true)
	{
		//接收数据
		ssize_t stLen = m_sckStream.recv(&pBuff[nRecvLen], 1024*100 - nRecvLen, &tv);
		if((int)stLen <= 0)
		{
			delete[] pBuff;
			return false;
		}

		if((int)stLen >= 4)
		{
			memcpy(&nPacketLen, pBuff, sizeof(int));
			nFinishRecv = nPacketLen + 4;
		}

		nRecvLen += (int)stLen;

		if(nRecvLen >= nFinishRecv)
		{
			break;
		}
	}

	int nStrLen              = 0;
	int nPos                 = 4;
	int nCount               = 0;
	//包处理线程
	memcpy(&nCount, &pBuff[nPos], sizeof(int));
	nPos += sizeof(int);

	for(int i = 0; i < nCount; i++)
	{
		//开始还原数据结构
		_ForbiddenIP ForbiddenIP;

		memcpy(&nStrLen, &pBuff[nPos], sizeof(char));
		nPos += sizeof(char);

		memcpy(ForbiddenIP.m_szIP, &pBuff[nPos], nStrLen);
		nPos += nStrLen;
		ForbiddenIP.m_szIP[nStrLen] = '\0';

		memcpy(&ForbiddenIP.m_nType, &pBuff[nPos], sizeof(char));
		nPos += sizeof(char);

		memcpy(&ForbiddenIP.m_nBeginTime, &pBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		memcpy(&ForbiddenIP.m_nSecond, &pBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		VecForbiddenIP.push_back(ForbiddenIP);
	}

	delete[] pBuff;
	return true;
}

bool CManagerConnect::SendUDPClientInfo(const char* pCommand, vecClientConnectInfo& VecClientConnectInfo)
{
	if(m_blConnected == false)
	{
		return false;
	}

	char szSendDate[100] = {'\0'};
	char szCommand[100]  = {'\0'}; 
	ACE_Time_Value tv(5);
	size_t stSendEnd = 0;

	sprintf_s(szCommand, 100, "%s", pCommand);
	int nAllLen = (int)strlen(szCommand);
	memcpy(&szSendDate[0], (const void*)&nAllLen, sizeof(int));
	memcpy(&szSendDate[4], (const void*)&szCommand, nAllLen);

	//发送数据
	m_sckStream.send_n(szSendDate, nAllLen + 4, &tv, &stSendEnd);

	int nFinishRecv = 0;
	int nRecvLen    = 0;
	int nPacketLen  = 0;
	char* pBuff = new char[1024*100];

	while(true)
	{
		//接收数据
		ssize_t stLen = m_sckStream.recv(&pBuff[nRecvLen], 1024*100 - nRecvLen, &tv);
		if((int)stLen <= 0)
		{
			delete[] pBuff;
			return false;
		}

		if((int)stLen >= 4)
		{
			memcpy(&nPacketLen, pBuff, sizeof(int));
			nFinishRecv = nPacketLen + 4;
		}

		nRecvLen += (int)stLen;

		if(nRecvLen >= nFinishRecv)
		{
			break;
		}
	}

	int nStrLen              = 0;
	int nPos                 = 4;
	int nClientCount         = 0;
	//包处理线程
	memcpy(&nClientCount, &pBuff[nPos], sizeof(int));
	nPos += sizeof(int);

	for(int i = 0; i < nClientCount; i++)
	{
		//开始还原数据结构
		_ClientConnectInfo ClientConnectInfo;

		memcpy(&nStrLen, &pBuff[nPos], sizeof(char));
		nPos += sizeof(char);

		memcpy(ClientConnectInfo.m_szIP, &pBuff[nPos], nStrLen);
		nPos += nStrLen;
		ClientConnectInfo.m_szIP[nStrLen] = '\0';

		memcpy(&ClientConnectInfo.m_nConnectID, &pBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		memcpy(&ClientConnectInfo.m_nRecvCount, &pBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		memcpy(&ClientConnectInfo.m_nSendCount, &pBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		memcpy(&ClientConnectInfo.m_nAllRecvSize, &pBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		memcpy(&ClientConnectInfo.m_nAllSendSize, &pBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		memcpy(&ClientConnectInfo.m_nBeginTime, &pBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		memcpy(&ClientConnectInfo.m_nAliveTime, &pBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		memcpy(&ClientConnectInfo.m_nRecvQueueCount, &pBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		memcpy(&ClientConnectInfo.m_nRecvQueueTimeCost, &pBuff[nPos], sizeof(long long));
		nPos += sizeof(long long);

		memcpy(&ClientConnectInfo.m_nSendQueueTimeCost, &pBuff[nPos], sizeof(long long));
		nPos += sizeof(long long);

		VecClientConnectInfo.push_back(ClientConnectInfo);
	}

	delete[] pBuff;
	return true;
}

bool CManagerConnect::SendClientInfo(const char* pCommand, vecClientConnectInfo& VecClientConnectInfo)
{
	if(m_blConnected == false)
	{
		return false;
	}

	char szSendDate[100] = {'\0'};
	char szCommand[100]  = {'\0'}; 
	ACE_Time_Value tv(5);
	size_t stSendEnd = 0;

	sprintf_s(szCommand, 100, "%s", pCommand);
	int nAllLen = (int)strlen(szCommand);
	memcpy(&szSendDate[0], (const void*)&nAllLen, sizeof(int));
	memcpy(&szSendDate[4], (const void*)&szCommand, nAllLen);

	//发送数据
	m_sckStream.send_n(szSendDate, nAllLen + 4, &tv, &stSendEnd);

	int nFinishRecv = 0;
	int nRecvLen    = 0;
	int nPacketLen  = 0;
	char* pBuff = new char[1024*100];

	while(true)
	{
		//接收数据
		ssize_t stLen = m_sckStream.recv(&pBuff[nRecvLen], 1024*100 - nRecvLen, &tv);
		if((int)stLen <= 0)
		{
			delete[] pBuff;
			return false;
		}

		if((int)stLen >= 4)
		{
			memcpy(&nPacketLen, pBuff, sizeof(int));
			nFinishRecv = nPacketLen + 4;
		}

		nRecvLen += (int)stLen;

		if(nRecvLen >= nFinishRecv)
		{
			break;
		}
	}

	int nStrLen              = 0;
	int nPos                 = 4;
	int nClientCount         = 0;
	//包处理线程
	memcpy(&nClientCount, &pBuff[nPos], sizeof(int));
	nPos += sizeof(int);

	for(int i = 0; i < nClientCount; i++)
	{
		//开始还原数据结构
		_ClientConnectInfo ClientConnectInfo;

		memcpy(&nStrLen, &pBuff[nPos], sizeof(char));
		nPos += sizeof(char);

		memcpy(ClientConnectInfo.m_szIP, &pBuff[nPos], nStrLen);
		nPos += nStrLen;
		ClientConnectInfo.m_szIP[nStrLen] = '\0';

		memcpy(&ClientConnectInfo.m_nConnectID, &pBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		memcpy(&ClientConnectInfo.m_nRecvCount, &pBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		memcpy(&ClientConnectInfo.m_nSendCount, &pBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		memcpy(&ClientConnectInfo.m_nAllRecvSize, &pBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		memcpy(&ClientConnectInfo.m_nAllSendSize, &pBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		memcpy(&ClientConnectInfo.m_nBeginTime, &pBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		memcpy(&ClientConnectInfo.m_nAliveTime, &pBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		memcpy(&ClientConnectInfo.m_nRecvQueueCount, &pBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		memcpy(&ClientConnectInfo.m_nRecvQueueTimeCost, &pBuff[nPos], sizeof(long long));
		nPos += sizeof(long long);

		memcpy(&ClientConnectInfo.m_nSendQueueTimeCost, &pBuff[nPos], sizeof(long long));
		nPos += sizeof(long long);

		VecClientConnectInfo.push_back(ClientConnectInfo);
	}
		
	delete[] pBuff;
	return true;
}

bool CManagerConnect::SendAddForbiddenIP(const char* pCommand)
{
	if(m_blConnected == false)
	{
		return false;
	}

	char szSendDate[100] = {'\0'};
	char szCommand[100]  = {'\0'}; 
	ACE_Time_Value tv(5);
	size_t stSendEnd = 0;

	sprintf_s(szCommand, 100, "%s", pCommand);
	int nAllLen = (int)strlen(szCommand);
	memcpy(&szSendDate[0], (const void*)&nAllLen, sizeof(int));
	memcpy(&szSendDate[4], (const void*)&szCommand, nAllLen);

	//发送数据
	m_sckStream.send_n(szSendDate, nAllLen + 4, &tv, &stSendEnd);

	char* pBuff = new char[100];
	ssize_t stLen = m_sckStream.recv(pBuff, 100, &tv);
	if((int)stLen <= 0)
	{
		delete[] pBuff;
		return false;
	}

	int nPacketLen   = 0;
	int nOPState     = 0;

	memcpy(&nPacketLen, &pBuff[0], sizeof(int));
	memcpy(&nOPState, &pBuff[4], sizeof(char));

	delete[] pBuff;
	return true;
}

bool CManagerConnect::SendDelForbiddenIP(const char* pCommand)
{
	if(m_blConnected == false)
	{
		return false;
	}

	char szSendDate[100] = {'\0'};
	char szCommand[100]  = {'\0'}; 
	ACE_Time_Value tv(5);
	size_t stSendEnd = 0;

	sprintf_s(szCommand, 100, "%s", pCommand);
	int nAllLen = (int)strlen(szCommand);
	memcpy(&szSendDate[0], (const void*)&nAllLen, sizeof(int));
	memcpy(&szSendDate[4], (const void*)&szCommand, nAllLen);

	//发送数据
	m_sckStream.send_n(szSendDate, nAllLen + 4, &tv, &stSendEnd);

	char* pBuff = new char[100];
	ssize_t stLen = m_sckStream.recv(pBuff, 100, &tv);
	if((int)stLen <= 0)
	{
		delete[] pBuff;
		return false;
	}

	int nPacketLen   = 0;
	int nOPState     = 0;

	memcpy(&nPacketLen, &pBuff[0], sizeof(int));
	memcpy(&nOPState, &pBuff[4], sizeof(char));

	delete[] pBuff;
	return true;
}

bool CManagerConnect::SendCloseClient(const char* pCommand)
{
	if(m_blConnected == false)
	{
		return false;
	}

	char szSendDate[100] = {'\0'};
	char szCommand[100]  = {'\0'}; 
	ACE_Time_Value tv(5);
	size_t stSendEnd = 0;

	sprintf_s(szCommand, 100, "%s", pCommand);
	int nAllLen = (int)strlen(szCommand);
	memcpy(&szSendDate[0], (const void*)&nAllLen, sizeof(int));
	memcpy(&szSendDate[4], (const void*)&szCommand, nAllLen);

	//发送数据
	m_sckStream.send_n(szSendDate, nAllLen + 4, &tv, &stSendEnd);

	char* pBuff = new char[100];
	ssize_t stLen = m_sckStream.recv(pBuff, 100, &tv);
	if((int)stLen <= 0)
	{
		delete[] pBuff;
		return false;
	}

	int nPacketLen   = 0;
	int nOPState     = 0;

	memcpy(&nPacketLen, &pBuff[0], sizeof(int));
	memcpy(&nOPState, &pBuff[4], sizeof(char));

	delete[] pBuff;
	return true;
}

bool CManagerConnect::SendServerConnectTCP(const char* pCommand, vecClientConnectInfo& VecClientConnectInfo)
{
	if(m_blConnected == false)
	{
		return false;
	}

	char szSendDate[100] = {'\0'};
	char szCommand[100]  = {'\0'}; 
	ACE_Time_Value tv(5);
	size_t stSendEnd = 0;

	sprintf_s(szCommand, 100, "%s", pCommand);
	int nAllLen = (int)strlen(szCommand);
	memcpy(&szSendDate[0], (const void*)&nAllLen, sizeof(int));
	memcpy(&szSendDate[4], (const void*)&szCommand, nAllLen);

	//发送数据
	m_sckStream.send_n(szSendDate, nAllLen + 4, &tv, &stSendEnd);

	int nFinishRecv = 0;
	int nRecvLen    = 0;
	int nPacketLen  = 0;
	char* pBuff = new char[1024*100];

	while(true)
	{
		//接收数据
		ssize_t stLen = m_sckStream.recv(&pBuff[nRecvLen], 1024*100 - nRecvLen, &tv);
		if((int)stLen <= 0)
		{
			delete[] pBuff;
			return false;
		}

		if((int)stLen >= 4)
		{
			memcpy(&nPacketLen, pBuff, sizeof(int));
			nFinishRecv = nPacketLen + 4;
		}

		nRecvLen += (int)stLen;

		if(nRecvLen >= nFinishRecv)
		{
			break;
		}
	}

	int nStrLen              = 0;
	int nPos                 = 4;
	int nClientCount         = 0;
	//包处理线程
	memcpy(&nClientCount, &pBuff[nPos], sizeof(int));
	nPos += sizeof(int);

	for(int i = 0; i < nClientCount; i++)
	{
		//开始还原数据结构
		_ClientConnectInfo ClientConnectInfo;

		memcpy(&nStrLen, &pBuff[nPos], sizeof(char));
		nPos += sizeof(char);

		memcpy(ClientConnectInfo.m_szIP, &pBuff[nPos], nStrLen);
		nPos += nStrLen;
		ClientConnectInfo.m_szIP[nStrLen] = '\0';

		memcpy(&ClientConnectInfo.m_nConnectID, &pBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		memcpy(&ClientConnectInfo.m_nRecvCount, &pBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		memcpy(&ClientConnectInfo.m_nSendCount, &pBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		memcpy(&ClientConnectInfo.m_nAllRecvSize, &pBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		memcpy(&ClientConnectInfo.m_nAllSendSize, &pBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		memcpy(&ClientConnectInfo.m_nBeginTime, &pBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		memcpy(&ClientConnectInfo.m_nAliveTime, &pBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		memcpy(&ClientConnectInfo.m_nRecvQueueCount, &pBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		memcpy(&ClientConnectInfo.m_nRecvQueueTimeCost, &pBuff[nPos], sizeof(long long));
		nPos += sizeof(long long);

		memcpy(&ClientConnectInfo.m_nSendQueueTimeCost, &pBuff[nPos], sizeof(long long));
		nPos += sizeof(long long);

		VecClientConnectInfo.push_back(ClientConnectInfo);
	}

	delete[] pBuff;
	return true;
}

bool CManagerConnect::SendServerConnectUDP(const char* pCommand, vecClientConnectInfo& VecClientConnectInfo)
{
	if(m_blConnected == false)
	{
		return false;
	}

	char szSendDate[100] = {'\0'};
	char szCommand[100]  = {'\0'}; 
	ACE_Time_Value tv(5);
	size_t stSendEnd = 0;

	sprintf_s(szCommand, 100, "%s", pCommand);
	int nAllLen = (int)strlen(szCommand);
	memcpy(&szSendDate[0], (const void*)&nAllLen, sizeof(int));
	memcpy(&szSendDate[4], (const void*)&szCommand, nAllLen);

	//发送数据
	m_sckStream.send_n(szSendDate, nAllLen + 4, &tv, &stSendEnd);

	int nFinishRecv = 0;
	int nRecvLen    = 0;
	int nPacketLen  = 0;
	char* pBuff = new char[1024*100];

	while(true)
	{
		//接收数据
		ssize_t stLen = m_sckStream.recv(&pBuff[nRecvLen], 1024*100 - nRecvLen, &tv);
		if((int)stLen <= 0)
		{
			delete[] pBuff;
			return false;
		}

		if((int)stLen >= 4)
		{
			memcpy(&nPacketLen, pBuff, sizeof(int));
			nFinishRecv = nPacketLen + 4;
		}

		nRecvLen += (int)stLen;

		if(nRecvLen >= nFinishRecv)
		{
			break;
		}
	}

	int nStrLen              = 0;
	int nPos                 = 4;
	int nClientCount         = 0;
	//包处理线程
	memcpy(&nClientCount, &pBuff[nPos], sizeof(int));
	nPos += sizeof(int);

	for(int i = 0; i < nClientCount; i++)
	{
		//开始还原数据结构
		_ClientConnectInfo ClientConnectInfo;

		memcpy(&nStrLen, &pBuff[nPos], sizeof(char));
		nPos += sizeof(char);

		memcpy(ClientConnectInfo.m_szIP, &pBuff[nPos], nStrLen);
		nPos += nStrLen;
		ClientConnectInfo.m_szIP[nStrLen] = '\0';

		memcpy(&ClientConnectInfo.m_nConnectID, &pBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		memcpy(&ClientConnectInfo.m_nRecvCount, &pBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		memcpy(&ClientConnectInfo.m_nSendCount, &pBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		memcpy(&ClientConnectInfo.m_nAllRecvSize, &pBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		memcpy(&ClientConnectInfo.m_nAllSendSize, &pBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		memcpy(&ClientConnectInfo.m_nBeginTime, &pBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		memcpy(&ClientConnectInfo.m_nAliveTime, &pBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		memcpy(&ClientConnectInfo.m_nRecvQueueCount, &pBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		memcpy(&ClientConnectInfo.m_nRecvQueueTimeCost, &pBuff[nPos], sizeof(long long));
		nPos += sizeof(long long);

		memcpy(&ClientConnectInfo.m_nSendQueueTimeCost, &pBuff[nPos], sizeof(long long));
		nPos += sizeof(long long);

		VecClientConnectInfo.push_back(ClientConnectInfo);
	}

	delete[] pBuff;
	return true;
}

bool CManagerConnect::SendServerConnectHistory(const char* pCommand, vecIPAccount& VecIPAccount)
{
	if(m_blConnected == false)
	{
		return false;
	}

	VecIPAccount.clear();

	char szSendDate[100] = {'\0'};
	char szCommand[100]  = {'\0'}; 
	ACE_Time_Value tv(5);
	size_t stSendEnd = 0;

	sprintf_s(szCommand, 100, "%s", pCommand);
	int nAllLen = (int)strlen(szCommand);
	memcpy(&szSendDate[0], (const void*)&nAllLen, sizeof(int));
	memcpy(&szSendDate[4], (const void*)&szCommand, nAllLen);

	//发送数据
	m_sckStream.send_n(szSendDate, nAllLen + 4, &tv, &stSendEnd);

	int nFinishRecv = 0;
	int nRecvLen    = 0;
	int nPacketLen  = 0;
	char* pBuff = new char[1024*100];

	while(true)
	{
		//接收数据
		ssize_t stLen = m_sckStream.recv(&pBuff[nRecvLen], 1024*100 - nRecvLen, &tv);
		if((int)stLen <= 0)
		{
			delete[] pBuff;
			return false;
		}

		if((int)stLen >= 4)
		{
			memcpy(&nPacketLen, pBuff, sizeof(int));
			nFinishRecv = nPacketLen + 4;
		}

		nRecvLen += (int)stLen;

		if(nRecvLen >= nFinishRecv)
		{
			break;
		}
	}

	int nStrLen              = 0;
	int nPos                 = 4;
	int nClientCount         = 0;
	//包处理线程
	memcpy(&nClientCount, &pBuff[nPos], sizeof(int));
	nPos += sizeof(int);

	for(int i = 0; i < nClientCount; i++)
	{
		//开始还原数据结构
		_IPAccount IPAccount;

		memcpy(&nStrLen, &pBuff[nPos], sizeof(char));
		nPos += sizeof(char);

		memcpy(IPAccount.m_szIP, &pBuff[nPos], nStrLen);
		nPos += nStrLen;
		IPAccount.m_szIP[nStrLen] = '\0';

		memcpy(&IPAccount.m_nCount, &pBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		memcpy(&IPAccount.m_nAllCount, &pBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		memcpy(&nStrLen, &pBuff[nPos], sizeof(char));
		nPos += sizeof(char);

		memcpy(IPAccount.m_szDate, &pBuff[nPos], nStrLen);
		nPos += nStrLen;
		IPAccount.m_szDate[nStrLen] = '\0';

		VecIPAccount.push_back(IPAccount);
	}

	delete[] pBuff;
	return true;
}

_ServerRunInfo CManagerConnect::SendShowCurrProcessInfo(const char* pCommand)
{
	_ServerRunInfo ServerRunInfo;
	if(m_blConnected == false)
	{
		return ServerRunInfo;
	}

	char szSendDate[100] = {'\0'};
	char szCommand[100]  = {'\0'}; 
	ACE_Time_Value tv(5);
	size_t stSendEnd = 0;

	sprintf_s(szCommand, 100, "%s", pCommand);
	int nAllLen = (int)strlen(szCommand);
	memcpy(&szSendDate[0], (const void*)&nAllLen, sizeof(int));
	memcpy(&szSendDate[4], (const void*)&szCommand, nAllLen);

	//发送数据
	m_sckStream.send_n(szSendDate, nAllLen + 4, &tv, &stSendEnd);

	int nFinishRecv = 0;
	int nRecvLen    = 0;
	int nPacketLen  = 0;
	char* pBuff = new char[1024*100];

	while(true)
	{
		//接收数据
		ssize_t stLen = m_sckStream.recv(&pBuff[nRecvLen], 1024*100 - nRecvLen, &tv);
		if((int)stLen <= 0)
		{
			delete[] pBuff;
			return ServerRunInfo;
		}

		if((int)stLen >= 4)
		{
			memcpy(&nPacketLen, pBuff, sizeof(int));
			nFinishRecv = nPacketLen + 4;
		}

		nRecvLen += (int)stLen;

		if(nRecvLen >= nFinishRecv)
		{
			break;
		}
	}

	int nStrLen              = 0;
	int nPos                 = 4;
	int nClientCount         = 0;
	//包处理线程
	memcpy(&ServerRunInfo.m_nCPU, &pBuff[nPos], sizeof(int));
	nPos += sizeof(int);
	memcpy(&ServerRunInfo.m_nMemorySize, &pBuff[nPos], sizeof(int));
	nPos += sizeof(int);

	delete[] pBuff;
	return ServerRunInfo;
}


