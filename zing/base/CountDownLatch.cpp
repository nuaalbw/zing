/*************************************************************************
	> File Name: CountDownLatch.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Mon 17 Jan 2022 10:54:33 PM CST
 ************************************************************************/

#include "CountDownLatch.h"

using namespace zing;

CountDownLatch::CountDownLatch(int count)
	: mutex_(), 
		condition_(), 
		count_(count)
{
}

void CountDownLatch::wait()
{
	MutexLockGuard lock(mutex_);
	while (count_ > 0)
	{
		condition_.wait(lock);
	}
}

void CountDownLatch::countDown()
{
	MutexLockGuard lock(mutex_);
	--count_;
	if (count_ == 0) {
		condition_.notifyAll();
	}
}

int CountDownLatch::getCount() const
{
	MutexLockGuard lock(mutex_);
	return count_;
}