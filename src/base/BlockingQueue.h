/*************************************************************************
	> File Name: BlockingQueue.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Sat 20 Nov 2021 10:47:13 AM CST
 ************************************************************************/

#ifndef ZING_BASE_BLOCKINGQUEUE_H
#define ZING_BASE_BLOCKINGQUEUE_H

#include "noncopyable.h"
#include <assert.h>
#include <mutex>
#include <queue>
#include <condition_variable>

namespace zing
{
namespace base
{

template<typename T>
class BlockingQueue: noncopyable
{
public:
	using QueueType = std::queue<T>;

	void put(const T& x);
	void put(T&& x);
	T take();
	QueueType drain();
	size_t size() const;
	bool empty() const;

private:
	mutable std::mutex mutex_;
	QueueType queue_;
	std::condition_variable notEmpty_;
};

template<typename T>
void BlockingQueue<T>::put(const T& x)
{
	std::lock_guard<std::mutex> locker(mutex_);

	queue_.push(x);
	notEmpty_.notify_one();
}

template<typename T>
void BlockingQueue<T>::put(T&& x)
{
	std::lock_guard<std::mutex> locker(mutex_);

	queue_.push(std::move(x));
	notEmpty_.notify_one();
}

template<typename T>
T BlockingQueue<T>::take()
{
	std::unique_lock<std::mutex> locker(mutex_);
	
	while (queue_.empty()) {
		notEmpty_.wait(locker);
	}
	assert(!queue_.empty());
	T front(std::move(queue_.front()));
	queue_.pop();
	return front;
}

template<typename T>
std::queue<T> BlockingQueue<T>::drain()
{
	std::queue<T> qu;
	{
		std::lock_guard<std::mutex> locker(mutex_);
		qu = std::move(queue_);
		assert(queue_.empty());
	}
	return qu;
}

template<typename T>
size_t BlockingQueue<T>::size() const
{
	std::lock_guard<std::mutex> locker(mutex_);
	return queue_.size();
}

template<typename T>
bool BlockingQueue<T>::empty() const
{
	std::lock_guard<std::mutex> locker(mutex_);
	return queue_.empty();
}
	
} // namespace base
} // namespace zing

#endif