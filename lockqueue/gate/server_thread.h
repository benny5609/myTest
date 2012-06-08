#ifndef _SERVER_THREAD_H_
#define _SERVER_THREAD_H_
#include <assert.h>
#include "socket_thread.h"
#include "server_socket.h"
class ServerThread : public SocketThread
{
public:
	ServerThread()
	{
	}
	~ServerThread() {}

	virtual void add_socket(SOCKET s)
	{
		static int a = 0;
		unsigned long  l = 1;
		int n = ioctlsocket(s, FIONBIO, &l);
		Socket *so = new ServerSocket(s);

		sockets_.insert(std::make_pair(s, so));
		printf("server enter %d %d %d",s,sockets_.size(),GetCurrentThreadId());
	}

	virtual void remove_socket(SOCKET s)
	{
		std::map<SOCKET, Socket*>::iterator itr = sockets_.find(s);
		if (itr != sockets_.end())
		{
			(*itr).second->close();
			sockets_.erase(itr);
		}
		printf("server leave %d %d %d",s, GetLastError(),GetCurrentThreadId());
	}
	void add_server(int server_id, SOCKET s)
	{
		servers_.insert(std::make_pair(server_id, s));
	}
	virtual void process_msg()
	{
		int r = WaitForSingleObject(send_queue_.get_event(), 30);
		if (r != 0 && r != 258)
		{
			assert(false);
		}
		Packet p;
		while(send_queue_.pop(p))
		{
			if (p.head_.op == op_new)
			{
				add_socket(p.head_.guid);
			}else if (p.head_.op == op_jump)
			{
				int server_id = 0;
				memcpy(&server_id, &p.buf_[0], 4);
				std::map<int, SOCKET>::iterator itr = servers_.find(server_id);
				if (itr != servers_.end())
				{
					std::map<SOCKET, Socket*>::iterator itr2 = sockets_.find((*itr).second);
					if (itr2 != sockets_.end())
					{
						(*itr2).second->push(p);
						clienttoserver_.insert(std::make_pair(p.head_.guid, (*itr).second));
					}					
				}
			}else
			{
				std::map<int, SOCKET>::iterator itr = clienttoserver_.find(p.head_.guid);
				if (itr != clienttoserver_.end())
				{
					std::map<SOCKET, Socket*>::iterator itr2 = sockets_.find((*itr).second);
					if (itr2 != sockets_.end())
					{
						(*itr2).second->push(p);
					}
				}else
				{
					int a;
					memcpy(&a, &p.buf_[0], p.buf_.size());
					printf("drop id %d op %d sequeue %d", p.head_.guid,p.head_.op, a);
					// 这个消息在跳图之后发的消息，全丢弃
				}
			}
		}
	}
	std::map<int, SOCKET> servers_;
	std::map<int, SOCKET> clienttoserver_;
};
extern ServerThread g_server;
#endif