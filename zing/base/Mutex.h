/*************************************************************************
	> File Name: Mutex.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Mon 17 Jan 2022 06:33:24 PM CST
 ************************************************************************/

#ifndef ZING_BASE_MUTEX_H
#define ZING_BASE_MUTEX_H

#include "CurrentThread.h"
#include "noncopyable.h"

#include <mutex>
#include <assert.h>

namespace zing
{

// 使用C++11中std::mutex封装的互斥量
class MutexLock: noncopyable
{
public:
	MutexLock()
		: holder_(0)
	{
	}

	~MutexLock()
	{
		assert(holder_ == 0);
	}

	bool isLockedByThisThread() const
	{
		return holder_ == CurrentThread::tid();
	}

	void assertLocked() const
	{
		assert(isLockedByThisThread());
	}

	// 仅限内部使用
	void lock()
	{
		mutex_.lock();
		assignHolder();	
	}

	void unlock()
	{
		unassignHolder();
		mutex_.unlock();
	}

	std::mutex& getStdMutex()
	{
		return mutex_;
	}

private:
	friend class Condition;
	friend class MutexLockGuard;

	class UnassignGuard: noncopyable
	{
	public:
		explicit UnassignGuard(MutexLock& owner)
			: owner_(owner)
		{
			owner_.unassignHolder();
		}

		~UnassignGuard()
		{
			owner_.assignHolder();
		}
	private:
		MutexLock& owner_;
	};

	void assignHolder()
	{
		holder_ = CurrentThread::tid();
	}

	void unassignHolder()
	{
		holder_ = 0;
	}

private:
	std::mutex mutex_;
	pid_t holder_;	// 记录当前互斥量的所属线程
};

class MutexLockGuard: noncopyable
{
public:
	explicit MutexLockGuard(MutexLock& mutex)
		: mutex_(mutex),
			lock_(mutex_.getStdMutex())
	{
		mutex_.assignHolder();
	}

	~MutexLockGuard()
	{
		mutex_.unassignHolder();
	}

	std::unique_lock<std::mutex>& getUniqueLock()
	{
		return lock_;
	}

	MutexLock& getMutexLock()
	{
		return mutex_;
	}

private:
	MutexLock& mutex_;
	std::unique_lock<std::mutex> lock_;
};
	
} // namespace zing

#define MutexLockGuard(x) error "Missing guard object name"

#endif