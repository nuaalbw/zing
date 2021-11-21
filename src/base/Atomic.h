/*************************************************************************
	> File Name: Atomic.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Mon 15 Nov 2021 09:08:30 PM CST
 ************************************************************************/

#ifndef ZING_BASE_ATOMIC_H
#define ZING_BASE_ATOMIC_H

#include "noncopyable.h"
#include <stdint.h>

namespace zing
{

namespace detail	// 内部类
{
template<typename T>
class AtomicIntegerT: noncopyable
{
public:
	AtomicIntegerT(): value_(0)
	{
	}

	T get()
	{
    	// in gcc >= 4.7: __atomic_load_n(&value_, __ATOMIC_SEQ_CST)
		return __sync_val_compare_and_swap(&value_, 0, 0);
	}

	T getAndAdd(T x)
	{
    	// in gcc >= 4.7: __atomic_fetch_add(&value_, x, __ATOMIC_SEQ_CST)
		return __sync_fetch_and_add(&value_, x);
	}

	T addAndGet(T x)
	{
		return getAndAdd(x) + x;
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
		getAndAdd(x);
	}

	void increment()
	{
		incrementAndGet();
	}

	void decrement()
	{
		decrementAndGet();
	}

	T getAndSet(T newValue)
	{
    	// in gcc >= 4.7: __atomic_exchange_n(&value_, newValue, __ATOMIC_SEQ_CST)
    	return __sync_lock_test_and_set(&value_, newValue);
	}

private:
	volatile T value_;
};
}

using AtomicInt32 = detail::AtomicIntegerT<int32_t>;
using AtomicInt64 = detail::AtomicIntegerT<int64_t>;

}

#endif