#include "ace/Message_Queue.h"
#include "ace/Get_Opt.h"
#include "ace/OS.h"
#include  <ace/Thread_Manager.h>
#include <ace/Synch.h>

#pragma comment(lib, "aced.lib")

//��Ϣ����ָ��
ACE_Message_Queue<ACE_MT_SYNCH>* mq;

const char S1[] = "C++";
const char S2[] = "Java";
const char S3[] = "PHP";
const char S4[] = "C#";

//�ĸ���Ϣָ��
ACE_Message_Block* mb1, * mb2, * mb3, * mb4;

//������
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
				//����ˮλ����Ϊ10�� S1+S2�ĳ���Ϊ3+4+2=9<10����˿��Խ�S3�Ž�ȥ
				//�����ٷ���S4ʱ�����߽��ᱻ����
				//��Ҫע�����ˮλ�Ĵ�С��������Ϣ�ĸ�����������Ϣ��������Ϣ�����������֮��
				//���Ҳ������Ϣ�ĸ�����Ϊ�ߵ�ˮλ��ֵ�ͺ���
				mq->high_water_mark(10);

				mq->enqueue_prio (mb1);
				mq->enqueue_prio (mb2);
				mq->enqueue_prio (mb3);

				ACE_DEBUG((LM_DEBUG, "(%P : %t) producer will pending!!\n"));

				//��Ϊ��������˯��6��֮��Ż����deactivate����������߻����������������
				//���Բ��ϵؽ�msg_bytes��ӡ�����۲�۲�
				int ret = mq->enqueue_prio (mb4);

				ACE_DEBUG((LM_DEBUG, "(%P : %t) producer waken up by  deactivate, ret = %d!!\n", ret));

				++loop;
			}

			if(loop == 2)
			{
				ACE_OS::sleep(6);

				//����ˮλ����Ϊ5����Ϊ��ˮλ��ȻΪ10����ǰ���������ֳ�����10��
				//�����������Ӳ����ԻὫ����������
				//����������������Ϣ����������С��5ʱ��������������
				//�������ڴ˴��ȴ��������߻���
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

//������
void* consume(void *arg)
{
	static int loop = 1;

	while(true)
	{
		ACE_OS::sleep(2);

		ACE_DEBUG((LM_DEBUG, "(%P : %t) consumer...\n"));

		if(loop == 1)
		{
			//�ȴ�6�룬��ʱ�����ߺ������߶���������
			ACE_OS::sleep(6);

			//deactivate�ỽ�����е��̣߳�����Ϣ��������Ϊ������
			//�Ժ�����ȡ�������᷵��-1
			//��������ỽ��������
			mq->deactivate();
			ACE_DEBUG((LM_DEBUG, "(%P : %t) consumer...loop 1\n"));
			++loop;
		}

		if(loop == 2)
		{
			ACE_OS::sleep(2);

			//����Ϣ���е�״̬���ó�ACTIVATED
			//��Ϣ�ֿ���ʹ����
			/mq->activate();
			ACE_DEBUG((LM_DEBUG, "(%P : %t) consumer...loop 2\n"));
			++loop;
		}

		if(loop == 3)
		{
			ACE_OS::sleep(10);

			//����������Ϣ֮����������С��5�ˣ����ڵ�ˮλ������������
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

	//ʹ���������Ϊ��Ϣ�����ȼ�
	//����Խ�ߣ����ȼ�Խ��
	priority = ACE_OS::rand() % 100;
	mb1 = new ACE_Message_Block(S1, sizeof S1, priority);

	priority = ACE_OS::rand() % 100;
	mb2 = new ACE_Message_Block(S2, sizeof S2, priority);

	priority = ACE_OS::rand() % 100;
	mb3 = new ACE_Message_Block(S3, sizeof S3, priority);

	priority = ACE_OS::rand() % 100;
	mb4 = new ACE_Message_Block(S4, sizeof S4, priority);

	//����Ϣѹ������У�enqueue_prio������Ϣ�����ȼ�����Ϣ�ŵ��ʵ���λ����
	//enqueue_headֻ�Ǽ򵥵ؽ����ݴ�������У�����������Ϣ�����ȼ�
	//ʹ��enqueue_prioѹ����Ϣ�󣬿��Լ�ͨ��dequeue_head��dequeue_tail
	//�ֱ����ȼ��Ӹߵ��ͺʹӵ͵���ȡ��Ϣ
	//���ʹ��enqueue_head��enqueue_tailѹ����Ϣ
	//����Ҫͨ��dequeue_prio��������Ϣ�����ȼ����ν���Ϣ������
	//û�б�Ҫ��ʹ��enqueue_prioѹ����Ϣ����ʵ��dequeue_prio��ȡ��Ϣ
	mq->enqueue_prio (mb1);
	mq->enqueue_prio (mb2);
	mq->enqueue_prio (mb3);
	mq->enqueue_prio (mb4);

	//�����̬��Ϣ���е������Ϣ
	//�ߵ�ˮλĬ��ֵ��Ϊ16384
	ACE_DEBUG((LM_DEBUG, "count : %d, bytes : %d, length : %d, high_water_mark : %d, low_water_mark : %d, status : %d\n",
		mq->message_count(), mq->message_bytes(), mq->message_length(),
		mq->high_water_mark(), mq->low_water_mark(),
		mq->state()));

	ACE_Message_Block *mb;

	//ʹ��next������Ϣ��������˳��Ϊ�����ȼ��������ȼ�
	ACE_DEBUG((LM_DEBUG, "===========next=============\n"));
	//peekһ�£�����������Ϣ������Windows��PeekMessage
	mq->peek_dequeue_head(mb);
	do 
	{
		ACE_DEBUG((LM_DEBUG, "message: %s, priority: %d\n", mb->rd_ptr(), mb->msg_priority()));
	}while(mb = mb->next());

	//ʹ�õ�����������Ϣ���У�������˳��Ϊ�����ȼ��������ȼ�
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

		//����������жϵĻ�����Ϣ���п�ʱ�ᵼ�����̱߳�����
		if(mq->is_empty())
			break;
	}

	ACE_DEBUG((LM_DEBUG, "\n\n"));

	//////////////////////////////////���Ըߵ�ˮλ�Ͷ��е�stateʹ�ã����в���֮ǰmq�����ѿ�///////////////////////////
	//����һ���������߳�
	ACE_Thread_Manager::instance()->spawn_n
		(
		1,
		(ACE_THR_FUNC) produce
		);

	////���������������߳�
	ACE_Thread_Manager::instance()->spawn_n
		(
		1,
		(ACE_THR_FUNC) consume
		);

	//�������߳�
	ACE_Thread_Manager::instance()->wait();

	return 0;
}