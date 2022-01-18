/*************************************************************************
	> File Name: Thread.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Tue 18 Jan 2022 08:49:25 PM CST
 ************************************************************************/

#ifndef ZING_BASE_THREAD_H
#define ZING_BASE_THREAD_H

#include "Atomic.h"
#include "CountDownLatch.h"
#include "Types.h"

#include <functional>
#include <memory>
#include <pthread.h>

namespace zing
{

class Thread: noncopyable
{
public:
	using ThreadFunc = std::function<void()>;

	explicit Thread(ThreadFunc, const string& name = string());

	~Thread();

	void start();
	int join();

	bool started() const { return started_; }

	pid_t tid() const { return tid_; }

	const string& name() const { return name_; }

	static int numCreated() { return numCreated_.get(); }

private:
	void setDefaultName();

	bool started_;
	bool joined_;
	pthread_t pthreadId_;
	pid_t tid_;
	ThreadFunc func_;
	string name_;
	CountDownLatch latch_;

	static AtomicInt32 numCreated_;
};
	
} // namespace zing

#endif