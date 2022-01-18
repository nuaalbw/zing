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

// CountDownLatch是一个同步工具类，用来协调多个线程之间的同步
// 或者说起到线程之间的通信（而不是用作互斥的作用）
// CountDownLatch能够使一个线程在等待另外一些线程完成各自工作之后，
// 再继续执行。使用一个计数器进行实现。计数器初始值为线程的数量。
// 当每一个线程完成自己任务后，计数器的值就会减一
// 当计数器的值为0时，表示所有的线程都已经完成一些任务
// 然后在CountDownLatch上等待的线程就可以恢复执行接下来的任务。
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