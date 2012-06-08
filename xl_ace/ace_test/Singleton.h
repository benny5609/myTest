#ifndef _SINGLETON_HPP
#define _SINGLETON_HPP

#include <ace/Thread_Mutex.h>
#include <ace/Guard_T.h>
#include <ace/Global_Macros.h>
#include "SingletonManage.h"

/////////////////////////////////////////////////////////////////////////
///Singleton定义了单实例类的模板类。
///@author	汪小龙
///@version	1.0,20080624
/////////////////////////////////////////////////////////////////////////
template<class TYPE>
class Singleton : public Clearup
{
public:
	virtual ~Singleton(void);
protected:
	Singleton(void);
public:
	///用于获取TYPE类型的单实例对象
	static TYPE *instance ();
protected:
	///TYPE类型对象
	TYPE  m_instance;

	///该类单实例对象指针
	static Singleton<TYPE> * m_singleton;
private:
	///用于同步的对象
	static ACE_Thread_Mutex	m_mutex;
};

template<class TYPE>
Singleton<TYPE> * Singleton<TYPE>::m_singleton = NULL;

template<class TYPE>
ACE_Thread_Mutex Singleton<TYPE>::m_mutex;

template<class TYPE>
Singleton<TYPE>::Singleton(void)
{
}

template<class TYPE>
Singleton<TYPE>::~Singleton(void)
{
	m_singleton = NULL;
}

template<class TYPE>
TYPE * Singleton<TYPE>::instance ()
{
	if (NULL == m_singleton)
	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, mutex_guard, m_mutex, NULL);
		if (NULL == m_singleton)
		{
			m_singleton = new Singleton();
			SingletonManage::instance()->addSingleton(m_singleton);
		}
	}
	return &m_singleton->m_instance;
}

#endif