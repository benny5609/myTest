#include "TimeManager.h"

CTimeHeart::CTimeHeart()
{
	m_blRunState = false;
}

CTimeHeart::~CTimeHeart()
{

}

void CTimeHeart::Init()
{
#ifdef _WIN32	
	m_key = 1111;
#else
	m_key = ftok("/home/freeeyes/SMData/", 0);
#endif

	printf("[Test]%d.\n", m_key);
}

int CTimeHeart::handle_timeout(const ACE_Time_Value &tv, const void *arg)
{
	if(m_blRunState == true)
	{
		//上次没有运行完，则不退出，等待下次循环
		return 0;
	}

	//开始存储
	ACE_Time_Value tvBegin(ACE_OS::gettimeofday());
	m_blRunState = true;
	CPlayerData* pPlayerDataTemp = new CPlayerData();

	//重新获得以下共享内存列表信息
	m_UserPool.Init(m_key, MAX_SHAREMEMORY_POOL);

	int nCount = m_UserPool.GetUsedObjectCount();
	for(int i = 0; i < nCount; i++)
	{
		CPlayerData* pData = m_UserPool.GetUsedObject(i);
		if(NULL == pData)
		{
			continue;
		}

		//如果数据对象正在写入，则此次数据不保存，等到写入完成再存
		if(true == pData->GetWriteSate())
		{
			continue;
		}

		//这一步主要是尽量防止在存储过程中，共享内存中的数据被其他进程修改。
		//虽然memorycopy可以被打断，但是在数据块不大的情况下，基本不会出现这个情况。就算出现了也是一时的，下次回写覆盖即可。
		memcpy(pPlayerDataTemp, pData, sizeof(CPlayerData));

		//开始判断是否需要存储
		if(m_UserPool.GetObjectHeadTimeStamp(pData) <= pPlayerDataTemp->GetTimeStamp())
		{
			m_IOData.SavePlayer(pPlayerDataTemp);
			m_UserPool.SetObjectHeadTimeStamp(pData);
		}
	}

	m_blRunState = false;
	ACE_Time_Value tvEnd(ACE_OS::gettimeofday());
	ACE_Time_Value tvInterval(tvEnd - tvBegin);
	ACE_DEBUG((LM_INFO, "[CTimeHeart::handle_timeout]Run OK Cost time(%d).\n", tvInterval.msec()));
	return 0;
}

CTimeManager::CTimeManager(void)
{
	m_nTimerID = 0;
}

CTimeManager::~CTimeManager(void)
{
}

void CTimeManager::Init()
{
	m_TimeHeart.Init();
}

void CTimeManager::KillTimer()
{
	if(m_nTimerID != 0)
	{
		m_ActiveTimer.cancel(m_nTimerID);
		m_nTimerID = 0;
	}
}

bool CTimeManager::Start(int nTimeIntervel)
{
	//清理定时器
	KillTimer();

	//添加时间定时器
	m_nTimerID = (int)m_ActiveTimer.schedule(&m_TimeHeart, (void *)NULL, ACE_OS::gettimeofday() + ACE_Time_Value(nTimeIntervel), ACE_Time_Value(nTimeIntervel));
	if(m_nTimerID == 0)
	{
		return false;
	}

	//启动定时器线程
	m_ActiveTimer.activate();
	return true;
}

