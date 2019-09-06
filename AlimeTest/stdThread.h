#pragma once
#include <thread>
std::thread t;

typedef unsigned int _Thrd_id_t;
typedef struct
{	/* thread identifier for Win32 */
	void *_Hnd;	/* Win32 HANDLE */
	_Thrd_id_t _Id;
} _Thrd_imp_t;

#define _Thr_val(thr) thr._Id
#define _Thr_set_null(thr) (thr._Hnd = nullptr, thr._Id = 0)
#define _Thr_is_null(thr) (thr._Id == 0)

typedef unsigned int(__stdcall *_Thrd_callback_t)(void *);

class StdThread
{	// class for observing and managing threads
public:
	class id;

	typedef void *native_handle_type;

	StdThread() noexcept
	{	// construct with no thread
		_Thr_set_null(_Thr);
	}

	~StdThread() noexcept
	{	// clean up
		if (joinable())
			_STD terminate();
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

	void join();

	void detach()
	{	// detach thread
		if (!joinable())
			throw std::exception("joinable");
		_Thrd_detachX(_Thr);
		_Thr_set_null(_Thr);
	}

	 id get_id() const noexcept;

	 static unsigned int hardware_concurrency() noexcept
	{	// return number of hardware thread contexts
		return (_Thrd_hardware_concurrency());
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

