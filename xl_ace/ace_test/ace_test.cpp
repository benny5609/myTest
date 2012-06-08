// ace_test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "AppNotifyInterface.h"
#include "ManageLogin.h"

int _tmain(int argc, _TCHAR* argv[])
{
	ManageAppNotifyInterface::instance();
	ManageLogin::instance();

	///初始化所有需要被初始化的对象
	ManageAppNotifyInterface::instance()->initApp(argc, argv);

	AccountInfo * account_info = new AccountInfo();
	account_info->name = "GH000001";
	account_info->password = "299219";
	ManageLogin::instance()->handleAccount(account_info);

	ACE_Reactor::instance()->run_event_loop();
	return 0;
}

