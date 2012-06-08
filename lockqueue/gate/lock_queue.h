#ifndef _LOCK_QUEUE_H_
#define _LOCK_QUEUE_H_
#include <queue>
#include "common.h"
template<class ItemType>
class LockQueue
{
public:
	LockQueue()
	{
		InitializeCriticalSection(&lock);
		event_ = CreateEvent(NULL, true, false, NULL);
	}
	~LockQueue()
	{
		DeleteCriticalSection(&lock);
		CloseHandle(event_);
	}
	void push(const ItemType& item)
	{
		EnterCriticalSection(&lock);
		queue_.push(item);
		SetEvent(event_);
		LeaveCriticalSection(&lock);
	}
	bool pop(ItemType& item)
	{
		EnterCriticalSection(&lock);
		if (queue_.empty())
		{
			ResetEvent(event_);
			LeaveCriticalSection(&lock);
			return false;
		}
		item = queue_.front();
		queue_.pop();
		LeaveCriticalSection(&lock);
		return true;
	}
	bool empty()
	{
		EnterCriticalSection(&lock);
		bool res = queue_.empty();
		LeaveCriticalSection(&lock);
		return true;
	}
	HANDLE get_event()
	{
		return event_;
	}
private:
	typedef std::queue<ItemType> Storge;
	Storge queue_;
	CRITICAL_SECTION lock;
	HANDLE event_;
};
#endif