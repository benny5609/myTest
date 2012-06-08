
#pragma once

#include <list>

using namespace std;

/////////////////////////////////////////////////////////////////////////
///Clearup�����˵�ʵ����ģ����Ļ��࣬���ڶ�����������������
///@author	��С��
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
	///��������ö�����ռ�õ���Դ
	virtual void clearup()
	{
		delete this;
	}
};

class SingletonManage;

//extern SingletonManage g_singleton_manage;

/////////////////////////////////////////////////////////////////////////
///SingletonManage���������ڹ���ʵ����Ĳ�������ע��ĵ�ʵ������������
///@author	��С��
///@version	1.0,20080624
/////////////////////////////////////////////////////////////////////////
class SingletonManage
{
protected:
	SingletonManage(void);
	virtual ~SingletonManage(void);
public:
	///�ö���Ϊ��ʵ���࣬�÷������ڻ�ȡ����ĵ�ʵ������
	static SingletonManage * instance()
	{
		if (NULL == m_instance)
		{
			m_instance = new SingletonManage();
		}
		return m_instance;
	}
public:
	///�÷���������ö������һ����ʵ�������������ú�����������ķ�ʽ
	///@param	obj	Clearup�������
	void addSingleton(Clearup * obj);

	///��ʼ��
	void init();

	///����ʼ��
	void fini();
private:
	///��̬SingletonManage����
	static SingletonManage * m_instance;
private:
	///ϵͳ�е�ʵ�������б�
	list<Clearup *>	m_signleton_list;

	///����ʼ���Ĵ���
	int	m_init_times;
};
