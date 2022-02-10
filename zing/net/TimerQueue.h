/*************************************************************************
	> File Name: TimerQueue.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Wed 09 Feb 2022 03:51:38 PM CST
 ************************************************************************/

#ifndef ZING_NET_TIMERQUEUE_H
#define ZING_NET_TIMERQUEUE_H

#include "../base/Mutex.h"
#include "../base/Timestamp.h"
#include "Callbacks.h"
#include "Channel.h"

#include <set>
#include <vector>

namespace zing
{
namespace net
{

class EventLoop;
class Timer;
class TimerId;

// 底层由红黑树实现的定时器队列
class TimerQueue: noncopyable
{
public:
	explicit TimerQueue(EventLoop* loop);
	~TimerQueue();

	TimerId addTimer(TimerCallback cb, 
									 Timestamp when, 
									 double interval);
	
	void cancel(TimerId timerId);

private:
	using Entry = std::pair<Timestamp, Timer*>;
	using TimerList = std::set<Entry>;
	using ActiveTimer = std::pair<Timer*, int64_t>;
	using ActiveTimerSet = std::set<ActiveTimer>;

	void addTimerInLoop(Timer* timer);		// 在EventLoop线程中添加定时器
	void cancelInLoop(TimerId timerId);		// 在EventLoop线程中删除定时器
	void handleRead();		// 当定时器到期时调用
	std::vector<Entry> getExpired(Timestamp now);		// 返回并移除所有到期的定时器
	void reset(const std::vector<Entry>& expired, Timestamp now);

	bool insert(Timer* timer);

	EventLoop* loop_;
	const int timerfd_;
	Channel timerfdChannel_;
	TimerList timers_;

	ActiveTimerSet activeTimers_;
	bool callingExpiredTimers_;
	ActiveTimerSet cancelingTimers_;
};

} // namespace net
} // namespace zing

#endif