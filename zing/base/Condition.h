/*************************************************************************
	> File Name: Condition.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Mon 17 Jan 2022 07:31:38 PM CST
 ************************************************************************/

#ifndef ZING_BASE_CONDITION_H
#define ZING_BASE_CONDITION_H

#include "Mutex.h"

#include <condition_variable>
#include <chrono>

namespace zing
{

// 使用C++11的std::condition_variable封装条件变量
class Condition: noncopyable
{
public:
	void wait(MutexLockGuard& guard)
	{
		MutexLock::UnassignGuard ug(guard.getMutexLock());
		cond_.wait(guard.getUniqueLock());	
	}

	// 如果超时时间到，则返回true，否则返回false
	bool waitForSeconds(MutexLockGuard& guard, double seconds)
	{
		MutexLock::UnassignGuard ug(guard.getMutexLock());
		return cond_.wait_for(guard.getUniqueLock(), 
					 std::chrono::duration<double>(seconds)) == std::cv_status::timeout;
	}

	void notify()
	{
		cond_.notify_one();
	}

	void notifyAll()
	{
		cond_.notify_all();
	}

private:
	std::condition_variable cond_;
};
	
} // namespace zing

#endif