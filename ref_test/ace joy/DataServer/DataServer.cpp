// DataServer.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "TimeManager.h"

int main(int argc, char* argv[])
{
	CTimeManager m_TimeManager;

	m_TimeManager.Init();
	m_TimeManager.Start(10);

	ACE_Thread_Manager::instance ()->wait();

	return 0;
}

