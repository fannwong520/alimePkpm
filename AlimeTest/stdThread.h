#pragma once
#include "windows.h"
#include <iosfwd>
#include <stdexcept>
#include "Runnable.h"
#include "process.h"

using  std::basic_ostream;
namespace alime {

	typedef unsigned int _Thrd_id_t;
	typedef struct
	{	/* thread identifier for Win32 */
		void *_Hnd;	/* Win32 HANDLE */
		_Thrd_id_t _Id;
	} _Thrd_imp_t;

#define _Thr_val(thr) thr._Id
#define _Thr_set_null(thr) (thr._Hnd = nullptr, thr._Id = 0)
#define _Thr_is_null(thr) (thr._Id == 0)

enum {	/* return codes */
	_Thrd_success,
	_Thrd_nomem,
	_Thrd_timedout,
	_Thrd_busy,
	_Thrd_error
};


/* threads */
typedef _Thrd_imp_t _Thrd_t;
typedef int(*_Thrd_start_t)(void *);


int _Thrd_detach(_Thrd_t thr)
{	/* tell OS to release thread's resources when it terminates */
	return (CloseHandle(thr._Hnd) == 0 ? _Thrd_error : _Thrd_success);
}

int _Thrd_join(_Thrd_t thr, int *code)
{	/* return exit code when thread terminates */
	unsigned long res;
	if (WaitForSingleObjectEx(thr._Hnd, INFINITE, FALSE) == WAIT_FAILED
		|| GetExitCodeThread(thr._Hnd, &res) == 0)
		return (_Thrd_error);
	if (code)
		*code = (int)res;
	return (CloseHandle(thr._Hnd) == 0 ? _Thrd_error : _Thrd_success);
}


typedef unsigned int(__stdcall *_Thrd_callback_t)(void *);

class StdThread
{	// class for observing and managing threads
public:

	typedef void *native_handle_type;


	StdThread() noexcept
	{	// construct with no thread
		_Thr_set_null(_Thr);
	}

	~StdThread() noexcept
	{	// clean up
		if (joinable())
			std::terminate();
		//if (joinable())
		//{
		//	if (WAIT_OBJECT_0 == WaitForSingleObject(_Thr._Hnd, INFINITE))
		//	{
		//		::CloseHandle(_Thr._Hnd);
		//		_Thr._Hnd = NULL;
		//		_Thr._Id = 0;
		//	}
		//	else
		//		std::terminate();
		//}
	}
	typedef std::function<void()> Task;

	struct Runnable
	{
		typedef alime::StdThread::Task ThreadFunc;
		ThreadFunc func_;
		CountDownLatch* latch_;

		Runnable(ThreadFunc func,
			CountDownLatch* latch)
			: func_(std::move(func)),
			latch_(latch)
		{ }

		void runInThread()
		{
			*tid_ = muduo::CurrentThread::tid();
			tid_ = NULL;
			latch_->countDown();
			latch_ = NULL;

			muduo::CurrentThread::t_threadName = name_.empty() ? "muduoThread" : name_.c_str();
			::prctl(PR_SET_NAME, muduo::CurrentThread::t_threadName);
			try
			{
				func_();
				muduo::CurrentThread::t_threadName = "finished";
			}
			catch (const Exception& ex)
			{
				muduo::CurrentThread::t_threadName = "crashed";
				fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
				fprintf(stderr, "reason: %s\n", ex.what());
				fprintf(stderr, "stack trace: %s\n", ex.stackTrace());
				abort();
			}
			catch (const std::exception& ex)
			{
				muduo::CurrentThread::t_threadName = "crashed";
				fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
				fprintf(stderr, "reason: %s\n", ex.what());
				abort();
			}
			catch (...)
			{
				muduo::CurrentThread::t_threadName = "crashed";
				fprintf(stderr, "unknown exception caught in Thread %s\n", name_.c_str());
				throw; // rethrow
			}
		}
	};


	explicit StdThread(Task task)
	{	
		_Thr._Hnd=
			(HANDLE)_beginthreadex(0, 0, &Runnable::threadProcFunc, task, 0, &_Thr._Id); 
		auto ret=(_Thr._Hnd == 0? _Thrd_error : _Thrd_success);
		while (task->started== false)
			Sleep(1000);
	}

	StdThread(StdThread&& _Other) noexcept
		: _Thr(_Other._Thr)
	{	// move from _Other
		_Thr_set_null(_Other._Thr);
	}

	StdThread& operator=(StdThread&& _Other) noexcept
	{	// move from _Other
		return (_Move_thread(_Other));
	}

	StdThread(const StdThread&) = delete;
	StdThread& operator=(const StdThread&) = delete;

	void swap(StdThread& _Other) noexcept
	{	// swap with _Other
		std::swap(_Thr, _Other._Thr);
	}

	_NODISCARD bool joinable() const noexcept
	{	// return true if this thread can be joined
		return (!_Thr_is_null(_Thr));
	}

	inline void join()
	{	// join thread
		if (!joinable())
			throw std::exception("joinable");
		const bool _Is_null = _Thr_is_null(_Thr);	// Avoid Clang -Wparentheses-equality
		if (_Is_null)
			throw std::exception("joinable");
		if (get_id() == id(GetCurrentThreadId()))
			throw std::exception("deadlock");
		if (_Thrd_join(_Thr, nullptr) != _Thrd_success)
			throw std::exception("no such proces");
		_Thr_set_null(_Thr);
	}

	void detach()
	{	// detach thread
		if (!joinable())
			throw std::exception("joinable");
		_Thrd_detach(_Thr);
		_Thr_set_null(_Thr);
	}

	 id get_id() const noexcept
	{	// return id for this thread
		return (_Thr_val(_Thr));
	}

	native_handle_type native_handle()
	{	// return Win32 HANDLE as void *
		return (_Thr._Hnd);
	}

private:
	StdThread& _Move_thread(StdThread& _Other)
	{	// move from _Other
		if (joinable())
			_STD terminate();
		_Thr = _Other._Thr;
		_Thr_set_null(_Other._Thr);
		return (*this);
	}

	_Thrd_t _Thr;
};
}

