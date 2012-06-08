
#pragma once

#include <list>

using namespace std;

/////////////////////////////////////////////////////////////////////////
///Clearup定义了单实例类模板类的基类，用于对象的自我清理操作。
///@author	汪小龙
///@version	1.0,20080624
/////////////////////////////////////////////////////////////////////////
class Clearup
{
public:
	Clearup()
	{
	}
	virtual ~Clearup()
	{
	}
public:
	///用于清理该对象所占用的资源
	virtual void clearup()
	{
		delete this;
	}
};

class SingletonManage;

//extern SingletonManage g_singleton_manage;

/////////////////////////////////////////////////////////////////////////
///SingletonManage定义了用于管理单实例类的操作，后注册的单实例类先析构。
///@author	汪小龙
///@version	1.0,20080624
/////////////////////////////////////////////////////////////////////////
class SingletonManage
{
protected:
	SingletonManage(void);
	virtual ~SingletonManage(void);
public:
	///该对象为单实例类，该方法用于获取该类的单实例对象
	static SingletonManage * instance()
	{
		if (NULL == m_instance)
		{
			m_instance = new SingletonManage();
		}
		return m_instance;
	}
public:
	///该方法用于向该对象添加一个单实例对象句柄，采用后添加先析构的方式
	///@param	obj	Clearup子类对象
	void addSingleton(Clearup * obj);

	///初始化
	void init();

	///反初始化
	void fini();
private:
	///静态SingletonManage对象
	static SingletonManage * m_instance;
private:
	///系统中单实例对象列表
	list<Clearup *>	m_signleton_list;

	///被初始化的次数
	int	m_init_times;
};
