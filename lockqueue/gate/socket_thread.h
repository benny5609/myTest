#ifndef _SOCKET_THREAD_H_
#define _SOCKET_THREAD_H_
#include <map>
#include "socket.h"
#include "common.h"

class SocketThread
{
public:
	SocketThread()
	{
		
	}
	virtual ~SocketThread()
	{
	}
	int init()
	{
		::_beginthread(SocketThread::run, 0, this);

		return 0;
	}
	static void run(void * p);

	void push(Packet &pk)
	{
		send_queue_.push(pk);
	}
	virtual void add_socket(SOCKET s) = 0;

	virtual void remove_socket(SOCKET s) = 0;

	virtual void process_msg() = 0;

protected:
	std::map<SOCKET, Socket*> sockets_;
	LockQueue<Packet> send_queue_;
	HANDLE	event_;
};

#endif