
#include "QY_Acceptor.h"
#include "QY_Event_Handler.h"
#include "ace/INET_Addr.h"
#include "ace/OS.h"

#include <iostream>
using namespace std;

int QY_Acceptor::open(int port)
{
	ACE_INET_Addr addr(port);

	if(this->sock_acceptor.open(addr) == -1)
	{
		cout << "出错: 打开端口" << port << "失败\n";
		return -1;
	}
	else if(this->reactor()->register_handler(this, ACE_Event_Handler::ACCEPT_MASK) == -1)
	{
		cout << "出错: 打开端口" << port << "失败\n";
		return -1;
	}
	else
	{
		cout << "启动文件服务器(" << addr.get_host_name() << ")\n开始监听端口" << addr.get_port_number() << "...\n";
	}
	return 0;
}

int QY_Acceptor::handle_input(ACE_HANDLE)
{
	QY_Event_Handler *eh;
	ACE_NEW_RETURN(eh, QY_Event_Handler(this->reactor()), -1);
	if(this->sock_acceptor.accept(eh->peer()) == -1)
	{
		return -1;
	}
	else if(eh->open() == -1)
	{
		return -1;
	}
	else
	{
		ACE_INET_Addr clientAddr;
		eh->peer().get_remote_addr(clientAddr);
		cout << clientAddr.get_host_addr() << "连入\n";
		return 0;
	}
}

int QY_Acceptor::handle_close(ACE_HANDLE, ACE_Reactor_Mask)
{
	if (this->sock_acceptor.get_handle () != ACE_INVALID_HANDLE)
    {
		ACE_Reactor_Mask m = ACE_Event_Handler::ACCEPT_MASK | ACE_Event_Handler::DONT_CALL;
		this->reactor ()->remove_handler (this, m);
		this->sock_acceptor.close();
		cout << "服务器关闭\n";
    }
	return 0;
}