#ifndef QY_ACCEPTOR_H
#define QY_ACCEPTOR_H

#include "ace/Reactor.h"
#include "ace/Event_Handler.h"
#include "ace/SOCK_Acceptor.h"

//客户端连接事件处理
class QY_Acceptor : public ACE_Event_Handler
{
public:
	QY_Acceptor(ACE_Reactor *r = ACE_Reactor::instance()) : ACE_Event_Handler(r) {}
	virtual ~QY_Acceptor() { this->handle_close();}

	//打开端口注册，并开始监听，默认端口号是2007
	virtual int open(int = 2007);

	//处理输入连接
	virtual int handle_input(ACE_HANDLE = ACE_INVALID_HANDLE);

	virtual int handle_close(ACE_HANDLE = ACE_INVALID_HANDLE, ACE_Reactor_Mask = 0);

	virtual ACE_HANDLE get_handle() const { return this->sock_acceptor.get_handle(); }

private:
	ACE_SOCK_Acceptor sock_acceptor;
};

#endif