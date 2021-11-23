/*************************************************************************
	> File Name: RingBuffer.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Mon 22 Nov 2021 10:40:10 PM CST
 ************************************************************************/

#ifndef ZING_BASE_RING_BUFFER_H
#define ZING_BASE_RING_BUFFER_H

#include "noncopyable.h"
#include <atomic>
#include <vector>

namespace zing
{
namespace base
{

template<typename T>
class RingBuffer: noncopyable
{
public:
	RingBuffer(int capacity = 60)
		: capacity_(capacity), 
		  size_(0), 
		  buffer_(capacity)
	{}

	virtual ~RingBuffer() {}

	bool push(const T& data)
	{
		return pushData(std::forward<T>(data));
	}

	bool push(T&& data)
	{
		return pushData(data);
	}

	bool pop(T& data)
	{
		if (size_ > 0) {
			data = std::move(buffer_[getPos_]);
			add(getPos_);
			size_--;
			return true;
		}
		return false;
	}

	bool full() const
	{
		return size_ == capacity_;
	}

	bool empty() const
	{
		return size_ == 0;
	}

	int size() const
	{
		return size_;
	}

private:

	template<typename F>
	bool pushData(F&& data)
	{
		if (size_ < capacity_) {
			buffer_[putPos_] = std::forward<F>(data);
			add(putPos_);
			size_++;
			return true;
		}
		return false;
	}

	void add(int& pos)
	{
		if (pos + 1 == capacity_) {
			pos = 0;
		}
		else {
			pos += 1;
		}
	}

private:
	int capacity_;
	int putPos_;
	int getPos_;
	std::atomic_int size_;
	std::vector<T> buffer_;	
};
	
} // namespace base
} // namespace zing

#endif