/*************************************************************************
	> File Name: BlockingQueue.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Tue 18 Jan 2022 10:28:16 PM CST
 ************************************************************************/

#ifndef ZING_BASE_BLOCKINGQUEUE_H
#define ZING_BASE_BLOCKINGQUEUE_H

#include "Condition.h"
#include "Mutex.h"

#include <deque>
#include <assert.h>

namespace zing
{

template<typename T>
class BlockingQueue: noncopyable
{
public:
	using queue_type = std::deque<T>;

	BlockingQueue()
		: mutex_(), 
			notEmpty_(), 
			queue_()
	{
	}

	void put(const T& x)
	{
		MutexLockGuard lock(mutex_);
		queue_.push_back(x);
		notEmpty_.notify();
	}

	void put(T&& x)
	{
		MutexLockGuard lock(mutex_);
		queue_.push_back(std::move(x));
		notEmpty_.notify();
	}

	T take()
	{
		MutexLockGuard lock(mutex_);
		while (queue_.empty())
		{
			notEmpty_.wait(lock);
		}
		assert(!queue_.empty());
		T front(std::move(queue_.front()));
		queue_.pop_front();
		return front;
	}

	queue_type drain()
	{
		queue_type queue;
		{
			MutexLockGuard lock(mutex_);
			queue = std::move(queue_);
			assert(queue_.empty());
		}
		return queue;
	}

	size_t size() const
	{
		MutexLockGuard lock(mutex_);
		return queue_.size();
	}

private:
	mutable MutexLock mutex_;
	Condition notEmpty_;
	queue_type queue_;
};
	
} // namespace zing

#endif