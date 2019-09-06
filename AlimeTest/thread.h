#pragma once

typedef unsigned int   ThreadId;
typedef void*      ThreadHandle;

enum ThreadPriority
{
	kThreadPriorityDefault,
	kThreadPriorityLow,
	kThreadPriorityNormal,
	kThreadPriorityHigh,
	kThreadPriorityRealtime
};

void SetThreadName(unsigned long thread_id, const char* name);

class Thread
{
public:
	Thread();
	virtual ~Thread();
	virtual void run()=0;
	bool start();
	void join();
	bool joinable();
	ThreadId thread_id();
	void set_thread_id(ThreadId thread_id);
	ThreadHandle thread_handle();
	void set_thread_priority(ThreadPriority priority);

public:
	static void sleep(int duration_ms);
	static void yield();
	static ThreadId currentId();

private:
	void terminate();
	static unsigned int  __stdcall threadProcFunc(void *arg);
	ThreadId       thread_id_;
	ThreadHandle   thread_handle_;
	ThreadPriority thread_priority_;
	static int threadInitNumber;

};
