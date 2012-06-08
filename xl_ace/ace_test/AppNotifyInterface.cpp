
#include "TraceLogger.h"
#include "AppNotifyInterface.h"

AppNotifyInterface::AppNotifyInterface()
: m_stop(false)
, m_wait(false)
{
	ManageAppNotifyInterface::instance()->addApp(this);
}

//////////////////////////////////////////////////////////////////////////

ManageAppNotifyInterface::ManageAppNotifyInterface()
{
}

ManageAppNotifyInterface::~ManageAppNotifyInterface()
{
}

void ManageAppNotifyInterface::addApp(AppNotifyInterface * app_notify)
{
	m_app_notify_vec.push_back(app_notify);
}

int ManageAppNotifyInterface::initApp(int argc, ACE_TCHAR * argv[])
{
	TRACELog(LM_NOTICE, ACE_TEXT("Start to init all object\n"));
	int result = 0;

	///�����Ⱥ�˳���ʼ��
	for (AppNotifyVec_t::iterator it = m_app_notify_vec.begin(); it != m_app_notify_vec.end(); ++it)
	{
		if ((*it)->init(argc, argv) == -1)
		{
			TRACELog(LM_ERROR, ACE_TEXT("Failed to call ManageAppNotifyInterface::doInit\n"));
			result = 1;
			break;
		}
	}
	return result;
}

void ManageAppNotifyInterface::finiApp()
{
	TRACELog(LM_NOTICE, ACE_TEXT("Start to fini all object\n"));
	///���պ��ʼ���ȷ���ʼ����˳��
	for (AppNotifyVec_t::reverse_iterator it = m_app_notify_vec.rbegin(); it != m_app_notify_vec.rend(); ++it)
	{
		(*it)->fini();
	}
}

void ManageAppNotifyInterface::waitApp()
{
	TRACELog(LM_NOTICE, ACE_TEXT("Start to wait all object\n"));
	///���պ��ʼ���ȷ���ʼ����˳��
	for (AppNotifyVec_t::reverse_iterator it = m_app_notify_vec.rbegin(); it != m_app_notify_vec.rend(); ++it)
	{
		(*it)->wait();
	}
}

void ManageAppNotifyInterface::clear()
{
	m_app_notify_vec.clear();
}