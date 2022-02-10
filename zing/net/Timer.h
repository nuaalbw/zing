/*************************************************************************
	> File Name: Timer.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Mon 07 Feb 2022 12:59:31 PM CST
 ************************************************************************/

#ifndef ZING_NET_TIMER_H
#define ZING_NET_TIMER_H

#include "../base/Atomic.h"
#include "../base/Timestamp.h"
#include "Callbacks.h"

namespace zing
{
namespace net
{

class Timer: noncopyable
{
public:
	Timer(TimerCallback cb, Timestamp when, double interval)
		: callback_(std::move(cb)), 
			expiration_(when), 
			interval_(interval), 
			repeat_(interval > 0.0), 
			sequence_(s_numCreated_.incrementAndGet())
	{
	}

	void run() const
	{
		callback_();
	}

	Timestamp expiration() const { return expiration_; }
	bool repeat() const { return repeat_; }
	int64_t sequence() const { return sequence_; }

	void restart(Timestamp now);

	static int64_t numCreated() { return s_numCreated_.get(); }

private:
	const TimerCallback callback_;			// 定时器回调函数
	Timestamp expiration_;							// 超时时间
	const double interval_;							// 时间间隔	
	const bool repeat_;									// 是否重复触发
	const int64_t sequence_;

	static AtomicInt64 s_numCreated_;		// 创建定时器的个数
};

} // namespace net
} // namespace zing

#endif