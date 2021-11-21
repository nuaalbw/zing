/*************************************************************************
	> File Name: TimerQueue.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Fri 19 Nov 2021 10:42:50 PM CST
 ************************************************************************/

#include "TimerQueue.h"
#include <chrono>

using namespace zing;
using namespace zing::net;
using namespace std::chrono;

TimerQueue::TimerQueue()
	: lastTimerId_(0)
{
}

TimerId TimerQueue::addTimer(TimerCallback cb, uint32_t msec)
{
	std::lock_guard<std::mutex> locker(mutex_);

	int64_t timeout = getTimeNow();
	TimerId timerId = ++lastTimerId_;

	TimerPtr timer = std::make_shared<Timer>(cb, msec);
	timer->setNextTimeout(timeout);
	timers_.emplace(timerId, timer);
	events_.emplace(std::make_pair(timeout + msec, timerId), std::move(timer));
	return timerId;
}

void TimerQueue::removeTimer(TimerId timerId)
{
	std::lock_guard<std::mutex> locker(mutex_);

	auto iter = timers_.find(timerId);	// 根据timerId找到timer
	if (iter != timers_.end()) {
		int64_t timeout = iter->second->getNextTimeout();
		events_.erase(std::make_pair(timeout, timerId));
		timers_.erase(timerId);
	}
}

int64_t TimerQueue::getTimeRemaining()
{
	std::lock_guard<std::mutex> locker(mutex_);

	if (timers_.empty()) {
		return -1;
	}
	int64_t msec = events_.begin()->first.first - getTimeNow();

	return msec < 0 ? 0 : msec;
}

void TimerQueue::handleTimerEvent()
{
	std::lock_guard<std::mutex> locker(mutex_);

	if (timers_.empty()) {
		return;
	}
	int64_t timePoint = getTimeNow();
	while (!timers_.empty() && events_.begin()->first.first <= timePoint) {
		TimerId timerId = events_.begin()->first.second;
		bool flag = events_.begin()->second->callback_();
		if (flag == true) {
			events_.begin()->second->setNextTimeout(timePoint);
			TimerPtr timer = std::move(events_.begin()->second);
			events_.erase(events_.begin());
			events_.emplace(std::make_pair(
				timer->getNextTimeout(), timerId), timer);
		}
		else {
			events_.erase(events_.begin());
			timers_.erase(timerId);
		}
	}
}

int64_t TimerQueue::getTimeNow()
{
	auto timePoint = steady_clock::now();
	return duration_cast<milliseconds>(
			timePoint.time_since_epoch()).count();
}