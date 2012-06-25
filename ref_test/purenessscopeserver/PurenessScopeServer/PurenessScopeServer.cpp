// PurenessScopeServer.cpp : 定义控制台应用程序的入口点。
//
// 有时候的说，重新站起来绝对不是一件容易的事情，但是我现在必须站起来。要做好样的，freeeyes
// add by freeeyes, freebird92
// 2008-12-22(冬至)

#include "MainConfig.h"

#ifndef WIN32
//如果是Linux
#include "ServerManager.h"

int main(int argc, char* argv[])
{
	ACE::init();

	int nServerType = SERVER_ACTOR_REACTOR;

	//第一步，读取配置文件
	if(!App_MainConfig::instance()->Init(MAINCONFIG))
	{
		OUR_DEBUG((LM_INFO, "[main]%s\n", App_MainConfig::instance()->GetError()));
	}
	else
	{
		App_MainConfig::instance()->Display();
	}

	//第二步，启动主服务器监控
	if(!App_ServerManager::instance()->Init())
	{
		OUR_DEBUG((LM_INFO, "[main]App_ServerManager::instance()->Init() error.\n"));
		getchar();
	}

	if(!App_ServerManager::instance()->Start())
	{
		OUR_DEBUG((LM_INFO, "[main]App_ServerManager::instance()->Start() error.\n"));
		getchar();
	}

	OUR_DEBUG((LM_INFO, "[main]Server Run is End.\n"));
	App_ServerManager::instance()->Close();

	ACE::fini();

	return 0;
}

#else
//如果是windows

#include "ProServerManager.h"
#include "WindowsDump.h"

int main(int argc, char* argv[])
{
	ACE::init();
	int nServerType = SERVER_ACTOR_PROACTOR;

	//添加Dump文件
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);   

	//第一步，读取配置文件
	if(!App_MainConfig::instance()->Init(MAINCONFIG))
	{
		OUR_DEBUG((LM_INFO, "[main]%s\n", App_MainConfig::instance()->GetError()));
	}
	else
	{
		App_MainConfig::instance()->Display();
	}

	//第二步，启动主服务器监控
	if(!App_ProServerManager::instance()->Init())
	{
		OUR_DEBUG((LM_INFO, "[main]App_ProServerManager::instance()->Init() error.\n"));
		getchar();
	}

	if(!App_ProServerManager::instance()->Start())
	{
		OUR_DEBUG((LM_INFO, "[main]App_ProServerManager::instance()->Start() error.\n"));
		getchar();
	}

	OUR_DEBUG((LM_INFO, "[main]Server Run is End.\n"));

	ACE::fini();

	return 0;
}

#endif


