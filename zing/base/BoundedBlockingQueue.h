/*************************************************************************
	> File Name: BoundedBlockingQueue.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Tue 18 Jan 2022 10:56:12 PM CST
 ************************************************************************/

#ifndef ZING_BASE_BOUNDEDBLOCKINGQUEUE_H
#define ZING_BASE_BOUNDEDBLOCKINGQUEUE_H

#include "Condition.h"
#include "Mutex.h"

#include <queue>
#include <assert.h>

namespace zing
{

template<typename T>
class BoundedBlockingQueue: noncopyable
{
public:
	explicit BoundedBlockingQueue(int maxSize)
		: mutex_(), 
			notEmpty_(), 
			notFull_(), 
			queue_(), 
			maxSize_(maxSize)
	{
		assert(maxSize_ > 0);
	}

	void put(const T& x)
	{
		MutexLockGuard lock(mutex_);
		while (isFull())
		{
			notFull_.wait(lock);
		}
		assert(!isFull());
		queue_.push(x);
		notEmpty_.notify();	
	}

	void put(T&& x)
	{
		MutexLockGuard lock(mutex_);
		while (isFull())
		{
			notFull_.wait(lock);
		}
		assert(!isFull());
		queue_.push(std::move(x));
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
		queue_.pop();
		notFull_.notify();
		return front;
	}

	bool empty() const
	{
		MutexLockGuard lock(mutex_);
		return queue_.empty();
	}

	bool full() const
	{
		MutexLockGuard lock(mutex_);
		return isFull();
	}

	size_t size() const
	{
		MutexLockGuard lock(mutex_);
		return queue_.size();
	}

	size_t capacity() const
	{
		MutexLockGuard lock(mutex_);
		return maxSize_;
	}

private:
	bool isFull() const
	{
		return static_cast<int>(queue_.size()) == maxSize_;
	}

	mutable MutexLock mutex_;
	Condition notEmpty_;
	Condition notFull_;
	std::queue<T> queue_;
	int maxSize_;
};

} // namespace zing

#endif