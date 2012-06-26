#pragma once

#include "ace/Event_Handler.h"
#include "ace/Timer_Heap.h"
#include "ace/Timer_Queue_Adapters.h"

#include "SMPool.h"
#include "IOData.h"

#define MAX_SHAREMEMORY_POOL 1500

typedef ACE_Thread_Timer_Queue_Adapter<ACE_Timer_Heap> ActiveTimer;

//��ʱ�������ࣨ����ʱ���ݸ��£�
class CTimeHeart : public ACE_Event_Handler
{
public:
	CTimeHeart();
	~CTimeHeart();

	void Init();
	virtual int handle_timeout(const ACE_Time_Value &tv, const void *arg);

private:
	CSMPool<CPlayerData> m_UserPool;     //�����ڴ��
	CIOData m_IOData;                    //IO���ݽӿ�
	bool    m_blRunState;                //�����Ƿ���������
	SMKey   m_key;                       //�����ڴ�Key
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
