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
		//�ϴ�û�������꣬���˳����ȴ��´�ѭ��
		return 0;
	}

	//��ʼ�洢
	ACE_Time_Value tvBegin(ACE_OS::gettimeofday());
	m_blRunState = true;
	CPlayerData* pPlayerDataTemp = new CPlayerData();

	//���»�����¹����ڴ��б���Ϣ
	m_UserPool.Init(m_key, MAX_SHAREMEMORY_POOL);

	int nCount = m_UserPool.GetUsedObjectCount();
	for(int i = 0; i < nCount; i++)
	{
		CPlayerData* pData = m_UserPool.GetUsedObject(i);
		if(NULL == pData)
		{
			continue;
		}

		//������ݶ�������д�룬��˴����ݲ����棬�ȵ�д������ٴ�
		if(true == pData->GetWriteSate())
		{
			continue;
		}

		//��һ����Ҫ�Ǿ�����ֹ�ڴ洢�����У������ڴ��е����ݱ����������޸ġ�
		//��Ȼmemorycopy���Ա���ϣ����������ݿ鲻�������£�����������������������������Ҳ��һʱ�ģ��´λ�д���Ǽ��ɡ�
		memcpy(pPlayerDataTemp, pData, sizeof(CPlayerData));

		//��ʼ�ж��Ƿ���Ҫ�洢
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
	//����ʱ��
	KillTimer();

	//���ʱ�䶨ʱ��
	m_nTimerID = (int)m_ActiveTimer.schedule(&m_TimeHeart, (void *)NULL, ACE_OS::gettimeofday() + ACE_Time_Value(nTimeIntervel), ACE_Time_Value(nTimeIntervel));
	if(m_nTimerID == 0)
	{
		return false;
	}

	//������ʱ���߳�
	m_ActiveTimer.activate();
	return true;
}

