
#include <iostream>
#include "SingletonManage.h"

//SingletonManage g_singleton_manage;
SingletonManage * SingletonManage::m_instance = NULL;

SingletonManage::SingletonManage(void)
: m_init_times(0)
{
}

SingletonManage::~SingletonManage(void)
{
	std::cout << "Enter SingletonManage::~SingletonManage\n";
	for (list<Clearup *>::iterator it = m_signleton_list.begin(); it != m_signleton_list.end(); ++it)
	{
		(*it)->clearup();
	}
	m_signleton_list.clear();
	std::cout << "Leave SingletonManage::~SingletonManage\n";
}

void SingletonManage::addSingleton(Clearup * obj)
{
	m_signleton_list.push_front(obj);
}

void SingletonManage::init()
{
	++m_init_times;
}

void SingletonManage::fini()
{
	--m_init_times;
	if (0 == m_init_times)
	{
		delete m_instance;
		m_instance = NULL;
	}
}