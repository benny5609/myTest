#pragma once

#include "ace/Event_Handler.h"
#include "ace/Timer_Heap.h"
#include "ace/Timer_Queue_Adapters.h"

#include "SMPool.h"
#include "IOData.h"

#define MAX_SHAREMEMORY_POOL 1500

typedef ACE_Thread_Timer_Queue_Adapter<ACE_Timer_Heap> ActiveTimer;

//定时器处理类（处理定时数据更新）
class CTimeHeart : public ACE_Event_Handler
{
public:
	CTimeHeart();
	~CTimeHeart();

	void Init();
	virtual int handle_timeout(const ACE_Time_Value &tv, const void *arg);

private:
	CSMPool<CPlayerData> m_UserPool;     //共享内存池
	CIOData m_IOData;                    //IO数据接口
	bool    m_blRunState;                //处理是否正在运行
	SMKey   m_key;                       //共享内存Key
}; 

class CTimeManager
{
public:
	CTimeManager(void);
	~CTimeManager(void);

	void Init();

	bool Start(int nTimeIntervel);
	void KillTimer();

private:
	ActiveTimer m_ActiveTimer;
	CTimeHeart  m_TimeHeart;
	int         m_nTimerID;
};
