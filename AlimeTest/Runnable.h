#pragma once
#include <assert.h>

class Runnable
{
public:
	Runnable():started(false)
	{
	}
	virtual ~Runnable() 
	{
	}
	virtual void run()=0;
	bool started;//bool is atomic default
	static unsigned int  __stdcall threadProcFunc(void *arg)
	{
		Runnable* task = (Runnable*)arg;
		assert(task);
		task->started = true;
		Sleep(1000);
		if (task != NULL)
			task->run();
		_endthreadex(0);
		delete task;
		return 0;
	}
};
