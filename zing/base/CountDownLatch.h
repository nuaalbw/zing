/*************************************************************************
	> File Name: CountDownLatch.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Mon 17 Jan 2022 10:45:03 PM CST
 ************************************************************************/

#ifndef ZING_BASE_COUNTDOWNLATCH_H
#define ZING_BASE_COUNTDOWNLATCH_H

#include "Condition.h"
#include "Mutex.h"

namespace zing
{

class CountDownLatch: noncopyable
{
public:
	explicit CountDownLatch(int count);

	void wait();

	void countDown();

	int getCount() const;

private:
	mutable MutexLock mutex_;
	Condition condition_;
	int count_;
};
	
} // namespace zing

#endif