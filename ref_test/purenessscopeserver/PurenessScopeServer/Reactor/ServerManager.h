// Define.h
// 这里定义Server的主要接口（启动监听端口等等）
// 悠然自得，即使什么都不做，也是一种雅兴。
// add by freeeyes
// 2008-12-23


#ifndef _SERVERMANAGER_H
#define _SERVERMANAGER_H

#include "define.h"
#include "MainConfig.h"
#include "ForbiddenIP.h"
#include "ConnectAccept.h"
#include "AceReactorManager.h"
#include "MessageService.h"
#include "SigHandle.h"
#include "LoadModule.h"
#include "LogManager.h"
#include "FileLogger.h"
#include "IObject.h"
#include "BuffPacketManager.h"
#include "ClientReConnectManager.h"
#include "ReUDPManager.h"

class CServerManager
{
public:
	CServerManager(void);
	~CServerManager(void);

	bool Init();
	bool Start();
	bool Close();

private:
	CConnectAcceptorManager m_ConnectAcceptorManager;    //普通客户端链接
	ConnectConsoleAcceptor  m_ConnectConsoleAcceptor;    //后台管理链接
};

typedef ACE_Singleton<CServerManager, ACE_Null_Mutex> App_ServerManager;

#endif
