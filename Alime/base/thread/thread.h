#pragma once
#include <functional>
#include <assert.h>
#include "base/Timestamp.h"
#include "base/duration.h"
/*
按标准库接口实现
方便以后过度到std::thread。
thread 不可复制构造/赋值
可移动构造/赋值.移动的本质就是交换id/handle
*/

namespace Alime
{
	enum {	/* return codes */
		_Thrd_success,
		_Thrd_nomem,
		_Thrd_timedout,
		_Thrd_busy,
		_Thrd_error
	};

	typedef struct
	{	/* thread identifier for Win32 */
		void * handle;	/* Win32 HANDLE */
		unsigned int tid;
	} Thrd_ty;

	typedef int(*_Thrd_start_t)(void *);

#define Thr_val(thr) thr.tid
#define Thr_set_null(thr) (thr.handle = nullptr, thr.tid = 0)
#define Thr_is_null(thr) (thr.tid == 0)

	typedef unsigned int(__stdcall *_Thrd_callback_t)(void *);

	class thread
	{
	public:
		typedef void *native_handle_type;
		typedef std::function<void()> ThreadFunc;
		typedef unsigned int id;
	public:
		thread();
		thread(ThreadFunc ,bool startImmediately=true);
		thread(thread&&);
		thread& operator=( thread&&);
		~thread();
		void start();
		bool joinable() const;
		unsigned int get_id() const;
		native_handle_type native_handle();
		unsigned int hardware_concurrency();
		void join();
		void detach();
		void swap(thread&);
		static uint32_t __stdcall ThreadProcFunc(void*);
	private:
		thread& MoveThread(thread& );
		Thrd_ty _Thr;
		ThreadFunc func_;
		bool started;
	};



	namespace this_thread 
	{
		inline thread::id get_id() noexcept
		{
			return GetCurrentThreadId();
		}

		inline void yield() noexcept
		{	// give up balance of time slice
			::Sleep(0);
		}

		inline void sleep_for(Duration t)
		{	// sleep for duration
			assert(t.Valid());
			Sleep(t.kMillisecond);
		}

		inline void sleep_until(Timestamp t)
		{	// sleep until _Abs_time
			sleep_for(t - Timestamp::Now());
		}



	}	// namespace this_thread
}

