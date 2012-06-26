#include "ace/Message_Queue.h"
#include "ace/Get_Opt.h"
#include "ace/OS.h"
#include  <ace/Thread_Manager.h>
#include <ace/Synch.h>

#pragma comment(lib, "aced.lib")

//消息队列指针
ACE_Message_Queue<ACE_MT_SYNCH>* mq;

const char S1[] = "C++";
const char S2[] = "Java";
const char S3[] = "PHP";
const char S4[] = "C#";

//四个消息指针
ACE_Message_Block* mb1, * mb2, * mb3, * mb4;

//生产者
static void* produce(void *arg)
{
	static int loop = 1;

	while(true)
	{
		ACE_OS::sleep(2);

		ACE_DEBUG((LM_DEBUG, "(%P : %t) producer...\n"));

		while(true)
		{
			if(loop == 1)
			{
				//将高水位设置为10， S1+S2的长度为3+4+2=9<10，因此可以将S3放进去
				//但是再放入S4时生产者将会被阻塞
				//需要注意的是水位的大小并不是消息的个数，而是消息队列中消息里面的数据量之和
				//如果也能以消息的个数作为高低水位的值就好了
				mq->high_water_mark(10);

				mq->enqueue_prio (mb1);
				mq->enqueue_prio (mb2);
				mq->enqueue_prio (mb3);

				ACE_DEBUG((LM_DEBUG, "(%P : %t) producer will pending!!\n"));

				//因为消费者在睡眠6秒之后才会调用deactivate，因此生产者会在这儿阻塞几秒钟
				//可以不断地将msg_bytes打印出来观察观察
				int ret = mq->enqueue_prio (mb4);

				ACE_DEBUG((LM_DEBUG, "(%P : %t) producer waken up by  deactivate, ret = %d!!\n", ret));

				++loop;
			}

			if(loop == 2)
			{
				ACE_OS::sleep(6);

				//将低水位设置为5，因为高水位仍然为10，当前的数据量又超过了10，
				//所以下面的入队操作仍会将生产者阻塞
				//这样消费者消费消息，当数据量小于5时，将唤醒生产者
				//生产者在此处等待被消费者唤醒
				mq->low_water_mark(5);

				ACE_DEBUG((LM_DEBUG, "(%P : %t) producer will pending again!!\n"));

				int ret = mq->enqueue_prio (mb4);

				ACE_DEBUG((LM_DEBUG, "(%P : %t) producer waken up by consumer!! ret = %d\n", ret));

				++loop;
			}
		}
	}

	return NULL;
}

//消费者
void* consume(void *arg)
{
	static int loop = 1;

	while(true)
	{
		ACE_OS::sleep(2);

		ACE_DEBUG((LM_DEBUG, "(%P : %t) consumer...\n"));

		if(loop == 1)
		{
			//等待6秒，此时生产者和消费者都将被阻塞
			ACE_OS::sleep(6);

			//deactivate会唤醒所有的线程，将消息队列设置为不可用
			//以后所存取操作都会返回-1
			//这个操作会唤醒生产者
			mq->deactivate();
			ACE_DEBUG((LM_DEBUG, "(%P : %t) consumer...loop 1\n"));
			++loop;
		}

		if(loop == 2)
		{
			ACE_OS::sleep(2);

			//将消息队列的状态设置成ACTIVATED
			//消息又可以使用了
			/mq->activate();
			ACE_DEBUG((LM_DEBUG, "(%P : %t) consumer...loop 2\n"));
			++loop;
		}

		if(loop == 3)
		{
			ACE_OS::sleep(10);

			//消费两个消息之后，数据量就小于5了，低于低水位将唤醒生产者
			ACE_Message_Block *mb;
			mq->dequeue_head (mb);
			mq->dequeue_head (mb);

			ACE_DEBUG((LM_DEBUG, "(%P : %t) consumer wake up producer!!\n"));

			++loop;
		}
	}

	return NULL;
}

int main(int argc, char* argv[])
{
	mq = ACE_Message_Queue_Factory<ACE_MT_SYNCH>::create_static_message_queue();

	int priority;

	//使用随机数作为消息的优先级
	//数字越高，优先级越高
	priority = ACE_OS::rand() % 100;
	mb1 = new ACE_Message_Block(S1, sizeof S1, priority);

	priority = ACE_OS::rand() % 100;
	mb2 = new ACE_Message_Block(S2, sizeof S2, priority);

	priority = ACE_OS::rand() % 100;
	mb3 = new ACE_Message_Block(S3, sizeof S3, priority);

	priority = ACE_OS::rand() % 100;
	mb4 = new ACE_Message_Block(S4, sizeof S4, priority);

	//将消息压入队列中，enqueue_prio根据消息的优先级将消息放到适当的位置上
	//enqueue_head只是简单地将数据存入队列中，而不考虑消息的优先级
	//使用enqueue_prio压入消息后，可以简单通过dequeue_head和dequeue_tail
	//分别按优先级从高到低和从低到高取消息
	//如果使用enqueue_head和enqueue_tail压入消息
	//则需要通过dequeue_prio来按照消息的优先级依次将消息出队列
	//没有必要既使用enqueue_prio压入消息，又实用dequeue_prio来取消息
	mq->enqueue_prio (mb1);
	mq->enqueue_prio (mb2);
	mq->enqueue_prio (mb3);
	mq->enqueue_prio (mb4);

	//输出静态消息队列的相关信息
	//高低水位默认值均为16384
	ACE_DEBUG((LM_DEBUG, "count : %d, bytes : %d, length : %d, high_water_mark : %d, low_water_mark : %d, status : %d\n",
		mq->message_count(), mq->message_bytes(), mq->message_length(),
		mq->high_water_mark(), mq->low_water_mark(),
		mq->state()));

	ACE_Message_Block *mb;

	//使用next遍历消息，遍历的顺序为高优先级到底优先级
	ACE_DEBUG((LM_DEBUG, "===========next=============\n"));
	//peek一下，并不弹出消息，类似Windows的PeekMessage
	mq->peek_dequeue_head(mb);
	do 
	{
		ACE_DEBUG((LM_DEBUG, "message: %s, priority: %d\n", mb->rd_ptr(), mb->msg_priority()));
	}while(mb = mb->next());

	//使用迭代器遍历消息队列，遍历的顺序为高优先级到底优先级
	ACE_DEBUG((LM_DEBUG, "=========iterator=============\n"));
	ACE_Message_Queue<ACE_MT_SYNCH>::ITERATOR iterator (*mq);

	for (ACE_Message_Block *entry = 0;
		iterator.next (entry) != 0;
		iterator.advance ())
	{
		ACE_DEBUG((LM_DEBUG, "message: %s, priority: %d\n", entry->rd_ptr(), entry->msg_priority()));
	}


	ACE_DEBUG((LM_DEBUG, "============dequeue_head==========\n"));
	while(mq->dequeue_head (mb) != -1)
	{
		ACE_DEBUG((LM_DEBUG, "message: %s, priority: %d\n", mb->rd_ptr(), mb->msg_priority()));

		//这里如果不判断的话，消息队列空时会导致主线程被阻塞
		if(mq->is_empty())
			break;
	}

	ACE_DEBUG((LM_DEBUG, "\n\n"));

	//////////////////////////////////测试高低水位和队列的state使用，进行测试之前mq队列已空///////////////////////////
	//产生一个生产者线程
	ACE_Thread_Manager::instance()->spawn_n
		(
		1,
		(ACE_THR_FUNC) produce
		);

	////产生两个消费者线程
	ACE_Thread_Manager::instance()->spawn_n
		(
		1,
		(ACE_THR_FUNC) consume
		);

	//挂起主线程
	ACE_Thread_Manager::instance()->wait();

	return 0;
}