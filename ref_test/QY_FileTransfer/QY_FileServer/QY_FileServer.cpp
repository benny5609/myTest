// QY_FileServer.cpp : �������̨Ӧ�ó������ڵ㡣
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

