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
	int  m_nCount;                     //��ǰ������õĴ���
	int  m_nTimeCost;                  //��ǰ������ʱ������

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
	char                m_szThreadName[MAX_BUFF_50];   //�߳�����
	int                 m_nThreadID;         //�߳�ID
	ACE_Time_Value      m_tvUpdateTime;      //�߳���������ݵ�ʱ��
	int                 m_nState;            //0Ϊû���������ڴ���1Ϊ���ڴ���������ݡ�
	int                 m_nRecvPacketCount;  //���߳��½��հ��Ĵ������
	int                 m_nSendPacketCount;  //���߳��·��Ͱ��Ĵ������
	int                 m_nCommandID;        //��ǰ���������ID
	int                 m_nPacketTime;       //ƽ�����ݰ�����ʱ��
	int                 m_nCurrPacketCount;  //��λʱ���ڵİ���������

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
	char       m_szIP[MAX_BUFF_50]; //IP��ַ��Ϣ
	int        m_nConnectID;        //���ӵ�ID
	int        m_nRecvCount;        //���հ�����
	int        m_nSendCount;        //���Ͱ�����
	int        m_nAllRecvSize;      //�����ֽ���
	int        m_nAllSendSize;      //�����ֽ���
	int        m_nBeginTime;        //���ӽ���ʱ��
	int        m_nAliveTime;        //���ʱ������
	int        m_nRecvQueueCount;     //�����߼�������ĸ���
	long long  m_nRecvQueueTimeCost;  //�����߼��������ʱ������
	long long  m_nSendQueueTimeCost;  //����������ʱ������

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

//����������ʱ��Ϣ
struct _ServerRunInfo
{
	int m_nCPU;        //��ǰCPUռ����
	int m_nMemorySize; //��ǰ�ڴ�ռ����

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
	char           m_szIP[MAX_BUFF_50];   //��ǰ���ӵ�ַ
	int            m_nCount;              //��ǰ���Ӵ���
	int            m_nAllCount;           //ָ��IP���Ӵ����ܺ� 
	int            m_nMinute;             //��ǰ������
	char           m_szDate[MAX_BUFF_50]; //������ʱ��

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
