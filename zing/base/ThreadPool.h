/*************************************************************************
	> File Name: ThreadPool.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Wed 19 Jan 2022 09:39:22 PM CST
 ************************************************************************/

#ifndef ZING_BASE_THREADPOOL_H
#define ZING_BASE_THREADPOOL_H

#include "Condition.h"
#include "Mutex.h"
#include "Thread.h"
#include "Types.h"

#include <deque>
#include <vector>

namespace zing
{

class ThreadPool: noncopyable
{
public:
	using Task = std::function<void()>;

	explicit ThreadPool(const string& nameArg = string("ThreadPool"));
	~ThreadPool();

	void setMaxQueueSize(int maxSize)
	{ maxQueueSize_ = maxSize; }

	void setThreadInitCallback(const Task& cb)
	{ threadInitCallback_ = cb; }

	void start(int numThreads);

	void stop();

	const string& name() const
	{ return name_; }

	size_t queueSize() const;

	void run(Task f);

private:
	bool isFull() const;
	void runInThread();
	Task take();

	mutable MutexLock mutex_;
	Condition notEmpty_;
	Condition notFull_;
	const string name_;
	Task threadInitCallback_;
	std::vector<std::unique_ptr<zing::Thread>> threads_;
	std::deque<Task> queue_;
	size_t maxQueueSize_;
	bool running_;
};

	
} // namespace zing

#endif