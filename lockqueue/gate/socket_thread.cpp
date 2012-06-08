#include <assert.h>
#include "socket_thread.h"
#include "logic_thread.h"

void SocketThread::run(void * p)
{
	while (1)
	{
		SocketThread *st = (SocketThread *)p;
		st->process_msg();
		
		if (st->sockets_.empty())
		{
			continue;
		}

		fd_set rd_set;
		fd_set wd_set;
		FD_ZERO(&rd_set);
		FD_ZERO(&wd_set);
		std::map<SOCKET, Socket*>::iterator itr = st->sockets_.begin();
		for (; itr != st->sockets_.end(); ++itr)
		{
			FD_SET((*itr).second->sock_, &rd_set);
			int r = (*itr).second->update();
			if ( r < 0)
			{
				if (GetLastError() != WSAEWOULDBLOCK)
				{
					SOCKET s = (*itr).second->sock_;
					++itr;
					st->remove_socket(s);
				}else
				{
					FD_SET((*itr).second->sock_, &wd_set);
				}
			}
		}
		timeval tm;
		tm.tv_sec = 0;
		tm.tv_usec = 30000;
		int n = select(0, &rd_set, &wd_set, NULL, &tm);
		if (n > 0)
		{
			itr = st->sockets_.begin();
			for (; itr != st->sockets_.end(); )
			{
				if(FD_ISSET((*itr).second->sock_, &rd_set))
				{
					int r = (*itr).second->recv();
					if ( r < 0)
					{
						SOCKET s = (*itr).second->sock_;
						++itr;
						st->remove_socket(s);
						continue;
					}else if ( r == 0)
					{
						SOCKET s = (*itr).second->sock_;
						++itr;
						st->remove_socket(s);
						continue;
					}
				}

				if(FD_ISSET((*itr).second->sock_, &wd_set))
				{
					int r = (*itr).second->update();
					if ( r < 0)
					{
						if (GetLastError() != WSAEWOULDBLOCK)
						{
							SOCKET s = (*itr).second->sock_;
							++itr;
							st->remove_socket(s);
							continue;
						}
					}
				}

				++itr;
			}
		}else if (n == 0)
		{
		}else
		{

		}
	}
}