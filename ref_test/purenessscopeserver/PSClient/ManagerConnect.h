#pragma once

#include "define.h"

#include <vector>

#define MAX_BUFF_50  50
#define MAX_BUFF_200 200

using namespace std;

struct _ModuleInfo
{
	char szModuleName[MAX_BUFF_200];
	char szModuleFile[MAX_BUFF_200];
	char szModuleDesc[MAX_BUFF_200];
	char szModuleCreateDate[MAX_BUFF_200];

	_ModuleInfo()
	{
		szModuleName[0]       = '\0';
		szModuleFile[0]       = '\0';
		szModuleDesc[0]       = '\0';
		szModuleCreateDate[0] = '\0';
	}
};

typedef vector<_ModuleInfo> vecModuleInfo;

struct _CommandInfo
{
	char szModuleName[MAX_BUFF_200];
	char szCommandID[MAX_BUFF_200];
	int  m_nCount;                     //当前命令被调用的次数
	int  m_nTimeCost;                  //当前命令总时间消耗

	_CommandInfo()
	{
		szModuleName[0] = '\0';
		szCommandID[0]  = '\0';
		m_nCount        = 0;
		m_nTimeCost     = 0;
 	}
};

typedef vector<_CommandInfo> vecCommandInfo;

struct _WorkThreadInfo
{
	char                m_szThreadName[MAX_BUFF_50];   //线程名称
	int                 m_nThreadID;         //线程ID
	ACE_Time_Value      m_tvUpdateTime;      //线程最后处理数据的时间
	int                 m_nState;            //0为没有数据正在处理，1为正在处理接受数据。
	int                 m_nRecvPacketCount;  //此线程下接收包的处理个数
	int                 m_nSendPacketCount;  //此线程下发送包的处理个数
	int                 m_nCommandID;        //当前处理的信令ID
	int                 m_nPacketTime;       //平均数据包处理时间
	int                 m_nCurrPacketCount;  //单位时间内的包处理数量

	_WorkThreadInfo()
	{
		m_nThreadID        = 0;
		m_nState           = 0;
		m_nRecvPacketCount = 0;
		m_nSendPacketCount = 0;
		m_nCommandID       = 0;
		m_nPacketTime      = 0;
		m_nCurrPacketCount = 0;
	}
};

typedef vector<_WorkThreadInfo> vecWorkThreadInfo;

struct _ClientConnectInfo
{
	char       m_szIP[MAX_BUFF_50]; //IP地址信息
	int        m_nConnectID;        //链接的ID
	int        m_nRecvCount;        //接收包数量
	int        m_nSendCount;        //发送包数量
	int        m_nAllRecvSize;      //接收字节数
	int        m_nAllSendSize;      //发送字节数
	int        m_nBeginTime;        //链接建立时间
	int        m_nAliveTime;        //存活时间秒数
	int        m_nRecvQueueCount;     //接受逻辑处理包的个数
	long long  m_nRecvQueueTimeCost;  //接受逻辑处理包总时间消耗
	long long  m_nSendQueueTimeCost;  //发送数据总时间消耗

	_ClientConnectInfo()
	{
		m_szIP[0]            = '\0';
		m_nConnectID         = 0;
		m_nRecvCount         = 0;
		m_nSendCount         = 0;
		m_nAllRecvSize       = 0;
		m_nAllSendSize       = 0;
		m_nBeginTime         = 0;
		m_nAliveTime         = 0;
		m_nRecvQueueCount    = 0;
		m_nRecvQueueTimeCost = 0;
		m_nSendQueueTimeCost = 0;
	}
};

typedef vector<_ClientConnectInfo> vecClientConnectInfo;

struct _ForbiddenIP
{
	char  m_szIP[MAX_BUFF_50];
	int   m_nType;
	int   m_nBeginTime;
	int   m_nSecond;

	_ForbiddenIP()
	{
		m_szIP[0]    = '\0';
		m_nType      = 0;
		m_nBeginTime = 0;
		m_nSecond    = 0;
	}
};

//服务器运行时信息
struct _ServerRunInfo
{
	int m_nCPU;        //当前CPU占用率
	int m_nMemorySize; //当前内存占用率

	_ServerRunInfo()
	{
		m_nCPU        = 0;
		m_nMemorySize = 0;
	}

	_ServerRunInfo& operator = (const _ServerRunInfo& ar)
	{
		this->m_nCPU        = ar.m_nCPU;
		this->m_nMemorySize = ar.m_nMemorySize;
		return *this;
	}
};

typedef vector<_ForbiddenIP> vecForbiddenIP;

struct _IPAccount
{
	char           m_szIP[MAX_BUFF_50];   //当前链接地址
	int            m_nCount;              //当前链接次数
	int            m_nAllCount;           //指定IP链接次数总和 
	int            m_nMinute;             //当前分钟数
	char           m_szDate[MAX_BUFF_50]; //最后更新时间

	_IPAccount()
	{
		m_szIP[0]    = '\0';
		m_nCount     = 0;
		m_nAllCount  = 0;
		m_szDate[0]  = '\0'; 
	}
};

typedef vector<_IPAccount> vecIPAccount;

class CManagerConnect
{
public:
	CManagerConnect(void);
	~CManagerConnect(void);

	bool Connect(const char* pManagerIP, int nPort);
	void Close();

	bool SendShowModule(vecModuleInfo& VecModuleInfo);
	bool SendUnloadModule(const char* pCommand);
	bool SendLoadModule(const char* pCommand);
	bool SendReLoadModule(const char* pCommand);
	bool SendClientCount(const char* pCommand, int& nCount, int& nPoolCount);
	bool SendCommandInfo(const char* pCommand, vecCommandInfo& VecCommandInfo, int& nCommandCount);
	bool SendCommandAllInfo(const char* pCommand, vecCommandInfo& VecCommandInfo, int& nCommandCount);
	bool SendWorkThreadState(const char* pCommand, vecWorkThreadInfo& VecWorkThreadInfo);
	bool SendClientInfo(const char* pCommand, vecClientConnectInfo& VecClientConnectInfo);
	bool SendUDPClientInfo(const char* pCommand, vecClientConnectInfo& VecClientConnectInfo);
	bool SendCloseClient(const char* pCommand);
	bool SendShowForbiddenIP(const char* pCommand, vecForbiddenIP& VecForbiddenIP);
	bool SendAddForbiddenIP(const char* pCommand);
	bool SendDelForbiddenIP(const char* pCommand);
	bool SendServerConnectTCP(const char* pCommand, vecClientConnectInfo& VecClientConnectInfo);
	bool SendServerConnectUDP(const char* pCommand, vecClientConnectInfo& VecClientConnectInfo);
	_ServerRunInfo SendShowCurrProcessInfo(const char* pCommand);
	bool SendServerConnectHistory(const char* pCommand, vecIPAccount& VecIPAccount);

private:
	ACE_SOCK_Connector m_sckConnectClient;
	ACE_INET_Addr      m_adrClient;
	ACE_SOCK_Stream    m_sckStream;
	bool               m_blConnected;
};
