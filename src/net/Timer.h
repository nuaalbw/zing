/*************************************************************************
	> File Name: Timer.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Thu 18 Nov 2021 11:14:42 PM CST
 ************************************************************************/

#ifndef ZING_NET_TIMER_H
#define ZING_NET_TIMER_H

#include "../base/noncopyable.h"
#include <functional>

namespace zing
{
namespace net
{
using TimerCallback = std::function<bool(void)>;

class Timer: noncopyable	// 定时器类，对象语义，不可拷贝
{
public:
	Timer(TimerCallback cb, uint32_t msec);	// 指定毫秒数以初始化定时器
	static void sleep(uint32_t msec);
	void setTimerCallback(TimerCallback cb);
	void start(int64_t microseconds, bool repeat = false);
	void stop();

private:
	friend class TimerQueue;

	void setNextTimeout(int64_t timePoint);
	int64_t getNextTimeout() const;

private:
	bool repeat_;
	TimerCallback callback_;
	const uint32_t interval_;
	int64_t nextTimeout_;
};

}
}

#endif