#include "pch.h"
#include "thread.h"
#include "windows.h"
#include "assert.h"
#include <process.h>
#include <exception>


Thread::Thread()
{
	thread_id_ = 0;
	thread_handle_ = nullptr;
	thread_priority_ = kThreadPriorityDefault;
}

Thread::~Thread()
{
	terminate();
}

ThreadId Thread::thread_id()
{
	return thread_id_; 
}

void Thread::set_thread_id(ThreadId thread_id) 
{ 
	thread_id_ = thread_id; 
}

ThreadHandle Thread::thread_handle()
{ 
	return thread_handle_; 
}

void Thread::set_thread_priority(ThreadPriority priority) 
{ 
	thread_priority_ = priority;
}


bool Thread::joinable()
{
	return thread_handle_;
}

void Thread::join()
{
	if (thread_handle_)
	{
		if (WAIT_OBJECT_0 == WaitForSingleObject(thread_handle_, INFINITE))
		{
			::CloseHandle(thread_handle_);
			thread_handle_ = NULL;
			thread_id_ = 0;
		}
		else
		{
			terminate();
		}
	}
}





void Thread::terminate()
{
	if (thread_handle_)
	{
		::TerminateThread(thread_handle_, 0);
		::CloseHandle(thread_handle_);
		thread_handle_ = NULL;
		thread_id_ = 0;
		std::terminate();
	}
}

unsigned int  __stdcall Thread::threadProcFunc(void *arg)
{
	Thread* the_thread = (Thread*)arg;
	assert(the_thread);
	if (the_thread != NULL)
	{
		the_thread->run();
	}
	_endthreadex(0);
	return 0;
}

bool Thread::start()
{
	// create thread first
	thread_handle_ = (HANDLE)_beginthreadex(NULL,
		0, threadProcFunc, this, 0, (unsigned*)&thread_id_);

	if (thread_handle_ < (HANDLE)2)
	{
		thread_handle_ = NULL;
		thread_id_ = 0;
	}

	// if thread need set priority
	if (thread_priority_ > kThreadPriorityDefault)
	{
		// set the thread priority
		int pri = THREAD_PRIORITY_BELOW_NORMAL;
		if (thread_priority_ == kThreadPriorityLow)
			pri = THREAD_PRIORITY_LOWEST;
		else if (thread_priority_ == kThreadPriorityNormal)
			pri = THREAD_PRIORITY_BELOW_NORMAL;
		else if (thread_priority_ == kThreadPriorityHigh)
			pri = THREAD_PRIORITY_HIGHEST;
		else if (thread_priority_ == kThreadPriorityRealtime)
			pri = THREAD_PRIORITY_TIME_CRITICAL;
		SetThreadPriority(thread_handle_, pri);
	}

	return thread_handle_ > (HANDLE)1;
}
