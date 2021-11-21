/*************************************************************************
	> File Name: TimerQueue.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Fri 19 Nov 2021 08:48:43 PM CST
 ************************************************************************/

#ifndef ZING_NET_TIMERQUEUE_H
#define ZING_NET_TIMERQUEUE_H

#include <set>
#include <vector>

#include "../base/noncopyable.h"
#include "Timer.h"
#include <mutex>
#include <memory>
#include <unordered_map>
#include <map>

namespace zing
{
namespace net
{
using TimerId = uint32_t;

class TimerQueue: noncopyable
{
public:
	TimerQueue();
	// 通过指定一个回调函数和毫秒数来添加定时器
	TimerId addTimer(TimerCallback cb, uint32_t msec);
	// 根据TimerId删除指定定时器
	void removeTimer(TimerId timerId);

	int64_t getTimeRemaining();
	void handleTimerEvent();

private:
	int64_t getTimeNow();

private:
	using TimerPtr = std::shared_ptr<Timer>;

	std::mutex mutex_;
	std::unordered_map<TimerId, TimerPtr> timers_;
	std::map<std::pair<int64_t, TimerId>, TimerPtr> events_;
	uint32_t lastTimerId_;
};
	
} // namespace net
} // namespace zing

#endif