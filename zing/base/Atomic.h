/*************************************************************************
	> File Name: Atomic.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Sat 15 Jan 2022 07:24:00 PM CST
 ************************************************************************/

#ifndef ZING_BASE_ATOMIC_H
#define ZING_BASE_ATOMIC_H

#include "noncopyable.h"

#include <atomic>
#include <stdint.h>

namespace zing
{

namespace detail
{

// 使用C++11中的std::atomic对原子型数据结构进行封装
template<typename T>
class AtomicIntegerT : noncopyable
{
public:
	AtomicIntegerT()
		: value_(0)
	{
	}

	T get()
	{
		return value_.load();
	}

	T getAndAdd(T x)
	{
		return value_.fetch_add(x);
	}

	T addAndGet(T x)
	{
		return value_.fetch_add(x) + x;
	}

	T incrementAndGet()
	{
		return addAndGet(1);
	}

	T decrementAndGet()
	{
		return addAndGet(-1);
	}

	void add(T x)
	{
		value_ += x;
	}

	void increment()
	{
		incrementAndGet();
	}

	void decrement()
	{
		decrementAndGet();
	}

	void set(T newValue)
	{
		value_.store(newValue);
	}

private:
	std::atomic<T> value_;
};

} // namespace detail

using AtomicInt32 = detail::AtomicIntegerT<int32_t>;
using AtomicInt64 = detail::AtomicIntegerT<int64_t>;
	
} // namespace zing

#endif