#ifndef _CLIENT_THREAD_H_
#define _CLIENT_THREAD_H_
#include "client_socket.h"
#include "socket_thread.h"
#include <assert.h>
class ClientThread : public SocketThread
{
public:
	ClientThread(){}
	~ClientThread() {}

	virtual void add_socket(SOCKET s)
	{
		unsigned long  l = 1;
		int n = ioctlsocket(s, FIONBIO, &l);
		Socket *so = new ClientSocket(s);

		sockets_.insert(std::make_pair(s, so));

		printf("client enter %d %d %d",s, sockets_.size(),GetCurrentThreadId());
	}

	virtual void remove_socket(SOCKET s)
	{
		std::map<SOCKET, Socket*>::iterator itr = sockets_.find(s);
		if (itr != sockets_.end())
		{
			(*itr).second->close();
			sockets_.erase(itr);
		}
		printf("client leave %d %d %d",s, GetLastError(),GetCurrentThreadId());
	}
	virtual void process_msg()
	{
		int r = WaitForSingleObject(send_queue_.get_event(), INFINITE);
		if (r != WAIT_OBJECT_0)
		{
			assert(false);
		}
		Packet p;
		while(send_queue_.pop(p))
		{
			if (p.head_.op == op_new)
			{
				add_socket(p.head_.guid);
			}else
			{
				SOCKET s = get_socket(p.head_.guid);
				if (s != INVALID_SOCKET)
				{
					std::map<SOCKET, Socket*>::iterator itr = sockets_.find(s);
					if (itr != sockets_.end())
					{
						(*itr).second->push(p);
					}
				}

			}
		}
	}
	void add_client(SOCKET s, int id)
	{
		std::map<SOCKET, int>::iterator iditr = client_ids_.find(s);
		if (iditr != client_ids_.end())
		{
			//assert(false);
			//remove_socket(s);
		}else
		{
			client_ids_.insert(std::make_pair(s, id));
			client_sockets_.insert(std::make_pair(id, s));
		}
	}

	int get_guid(SOCKET s)
	{
		std::map<SOCKET, int>::iterator iditr = client_ids_.find(s);
		if (iditr != client_ids_.end())
		{
			return (*iditr).second;
		}
		return -1;
	}
	SOCKET get_socket(int guid)
	{
		std::map<int, SOCKET>::iterator iditr = client_sockets_.find(guid);
		if (iditr != client_sockets_.end())
		{
			return (*iditr).second;
		}
		return INVALID_SOCKET;
	}
	std::map<SOCKET, int> client_ids_;
	std::map<int, SOCKET> client_sockets_;
};

extern ClientThread g_clients;

#endif