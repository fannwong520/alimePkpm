#include "pch.h"
#include "thread.h"
#include "windows.h"
#include <iosfwd>
#include <stdexcept>
#include "process.h"
#include <string>
#include <assert.h>

namespace Alime
{
	struct launcher
	{
		typedef thread::ThreadFunc ThreadFunc;
		ThreadFunc func_;
		Thrd_ty* thr_;
		//CountDownLatch* latch_;
		launcher(ThreadFunc func ,Thrd_ty* tid/*, CountDownLatch* latch*/)
			: func_(std::move(func)), thr_(tid)//,latch_(latch)
		{
		}

		void runInThread()
		{
			(*thr_).tid= GetCurrentThreadId();
			(*thr_).handle=GetCurrentThread();
			thr_ = NULL;
			//latch_->countDown();
			//latch_ = NULL;
			try
			{
				func_();
			}
			catch (const std::exception& ex)
			{
				//LOG_ERR<<ex
				abort();
			}
		}
	};

	void thread::start()
	{
		assert(!started);
		started = true;
		launcher* task = new launcher(func_, &_Thr);
		_Thr.handle = (HANDLE)_beginthreadex(0, 0, ThreadProcFunc, task, 0, &_Thr.tid);
		auto ret = (_Thr.handle == 0 ? _Thrd_error : _Thrd_success);
		if (_Thrd_success == ret)
		{
			//latch_.wait();
			assert(_Thr.tid > 0);
		}
		else
		{// error happened
			started = false;
			delete task; 
		}

	}

	uint32_t __stdcall thread::ThreadProcFunc(void *obj)
	{
		launcher* data = static_cast<launcher*>(obj);
		assert(data);
		data->runInThread();
		delete data;
		_endthreadex(0);
		return 0;
	}


	thread::thread()
	{
		started = false;
		_Thr_set_null(_Thr);
	}

	thread::thread(ThreadFunc func, bool startImmediately)
		:func_(std::move(func))
	{
		started = false;
		if (startImmediately)
		{
			start();
			assert(started = true);
		}
	}




	thread::~thread()
	{
		if (joinable())
			std::terminate();
	}

	thread::thread(thread&& _Other)
		: _Thr(_Other._Thr)
	{	// move from _Other
		Thr_set_null(_Other._Thr);
	}

	thread& thread::operator=(thread&& _Other) 
	{	// move from _Other
		return (MoveThread(_Other));
	}

	thread& thread::MoveThread(thread& _Other)
	{	// move from _Other
		if (joinable())
			std::terminate();
		_Thr = _Other._Thr;
		Thr_set_null(_Other._Thr);
		return (*this);
	}

	void thread::swap(thread& _Other)
	{	// swap with _Other
		std::swap(_Thr, _Other._Thr);
	}

	unsigned int thread::get_id() const
	{
		return _Thr.tid;
	}

	bool thread::joinable() const
	{	// return true if this thread can be joined
		return (!Thr_is_null(_Thr));
	}


	int _Thrd_join(Thrd_ty thr, int *code)
	{	/* return exit code when thread terminates */
		unsigned long res;
		if (WaitForSingleObjectEx(thr.handle, INFINITE, FALSE) == WAIT_FAILED
			|| GetExitCodeThread(thr.handle, &res) == 0)
			return (_Thrd_error);
		if (code)
			*code = (int)res;
		return (CloseHandle(thr.handle) == 0 ? _Thrd_error : _Thrd_success);
	}
	void thread::join()
	{	// join thread
		if (!joinable())
			throw std::exception("joinable");
		const bool _Is_null = Thr_is_null(_Thr);	// Avoid Clang -Wparentheses-equality
		if (_Is_null)
			throw std::exception("in valid argument");
		if (get_id() == ::GetCurrentThreadId())
			throw std::exception("dead lock will occur");
		if (_Thrd_join(_Thr, nullptr) != _Thrd_success)
			throw std::exception("no such process");
		Thr_set_null(_Thr);
	}

	int _Thrd_detach(Thrd_ty thr)
	{	/* tell OS to release thread's resources when it terminates */
		return (CloseHandle(thr.handle) == 0 ? _Thrd_error : _Thrd_success);
	}
	void thread::detach()
	{	// detach thread
		if (!started)
			throw std::exception("thread should started first");
		if (!joinable())
			throw std::exception("in valid argument");
		if(_Thrd_success!=_Thrd_detach(_Thr))
			throw std::exception("detach failed");
		Thr_set_null(_Thr);
	}

	unsigned int hardware_concurrency() noexcept
	{	// return number of hardware thread contexts
		 SYSTEM_INFO info;
		 GetNativeSystemInfo(&info);
		 return (info.dwNumberOfProcessors);
	}

	thread::native_handle_type thread::native_handle()
	{	// return Win32 HANDLE as void *
		return (_Thr.handle);
	}
}
