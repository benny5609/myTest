// ace_test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "AppNotifyInterface.h"
#include "ManageLogin.h"

int _tmain(int argc, _TCHAR* argv[])
{
	ManageAppNotifyInterface::instance();
	ManageLogin::instance();

	///��ʼ��������Ҫ����ʼ���Ķ���
	ManageAppNotifyInterface::instance()->initApp(argc, argv);

	AccountInfo * account_info = new AccountInfo();
	account_info->name = "GH000001";
	account_info->password = "299219";
	ManageLogin::instance()->handleAccount(account_info);

	ACE_Reactor::instance()->run_event_loop();
	return 0;
}

