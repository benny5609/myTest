#include "ace/ace.h"
#include "ace/Task.h"
#include "ace/OS.h"
#pragma comment(lib, "aced.lib")

class myTask:public ACE_Task<ACE_MT_SYNCH>
{
public:
	virtual int svc(void)
	{
		int count = 5;
		while(count--)
		{
			printf("hello\n");
			ACE_OS::sleep(2);
		}
		return -1;
	}
};

int main(int argc, char* argv[])
{
	myTask mytask;
	mytask.activate();
	//mytask.wait();
	ACE_Thread_Manager::instance ()->wait ();
	return 0;
};