#ifndef _LOGIC_THREAD_H_
#define _LOGIC_THREAD_H_
#include <set>
#include "lock_queue.h"
#include "packet.h"
class LogicThread
{
public:
	void push(Packet& p)
	{
		recv_queue_.push(p);
	}

	void run();
private:
	LockQueue<Packet> recv_queue_;
	std::set<SOCKET> players_;
};
extern LogicThread g_logic;
#endif