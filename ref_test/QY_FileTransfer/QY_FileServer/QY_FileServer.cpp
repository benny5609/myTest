// QY_FileServer.cpp : 定义控制台应用程序的入口点。
//

#include "QY_Acceptor.h"
#include "ace/Reactor.h"
#include "ace/OS.h"
#include <iostream>
using namespace std;


//int _tmain(int argc, _TCHAR* argv[])
int main(int argc, char *argv[])
{
	QY_Acceptor acceptor;
	acceptor.open();
	ACE_Reactor::instance()->run_event_loop();
	return 0;
}

