#include "windows.h"
#include <exception>
class stl_critical_section_win7
{
public:
	stl_critical_section_win7()
	{
		InitializeSRWLock(&m_srw_lock);
	}

	~stl_critical_section_win7() = delete;
	stl_critical_section_win7(const stl_critical_section_win7&) = delete;
	stl_critical_section_win7& operator=(const stl_critical_section_win7&) = delete;

	virtual void destroy() {}

	virtual void lock()
	{
		AcquireSRWLockExclusive(&m_srw_lock);
	}

	virtual bool try_lock()
	{
		return TryAcquireSRWLockExclusive(&m_srw_lock) != 0;
	}

	virtual bool try_lock_for(unsigned int)
	{
		// STL will call try_lock_for once again if this call will not succeed
		return stl_critical_section_win7::try_lock();
	}

	virtual void unlock()
	{
		ReleaseSRWLockExclusive(&m_srw_lock);
	}

	PSRWLOCK native_handle()
	{
		return &m_srw_lock;
	}

private:
	SRWLOCK m_srw_lock;
};

class stl_condition_variable_win7
{
public:
	stl_condition_variable_win7()
	{
		InitializeConditionVariable(&m_condition_variable);
	}

	~stl_condition_variable_win7() = delete;
	stl_condition_variable_win7(const stl_condition_variable_win7&) = delete;
	stl_condition_variable_win7& operator=(const stl_condition_variable_win7&) = delete;

	virtual void destroy() {}

	virtual void wait(stl_critical_section_win7 *lock)
	{
		if (!stl_condition_variable_win7::wait_for(lock, INFINITE))
			std::terminate();
	}

	virtual bool wait_for(stl_critical_section_win7 *lock, unsigned int timeout)
	{
		return SleepConditionVariableSRW(&m_condition_variable, static_cast<stl_critical_section_win7 *>(lock)->native_handle(), timeout, 0) != 0;
	}

	virtual void notify_one()
	{
		WakeConditionVariable(&m_condition_variable);
	}

	virtual void notify_all()
	{
		WakeAllConditionVariable(&m_condition_variable);
	}

private:
	CONDITION_VARIABLE m_condition_variable;
};