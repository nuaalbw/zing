/*************************************************************************
	> File Name: TaskScheduler.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Tue 23 Nov 2021 04:34:28 PM CST
 ************************************************************************/

#ifndef ZING_NET_TASK_SCHEDULER_H
#define ZING_NET_TASK_SCHEDULER_H

#include "../base/noncopyable.h"
#include "../base/RingBuffer.h"
#include "Channel.h"
#include "TimerQueue.h"
#include "Pipe.h"
#include <functional>
#include <atomic>
#include <mutex>

namespace zing
{
namespace net
{

using EventCallback = std::function<void(void)>;

// 抽象基类，IO multiplexing的封装
class TaskScheduler: noncopyable
{
	using PipePtr = std::unique_ptr<Pipe>;
	using RingBufferPtr = std::unique_ptr<base::RingBuffer<EventCallback>>;

public:
	TaskScheduler(int id = 1);
	virtual ~TaskScheduler();

	void start();
	void stop();
	TimerId addTimer(TimerCallback cb, uint32_t msec);
	void removeTimer(TimerId timerId);
	bool addTriggerEvent(EventCallback cb);

	// interface
	virtual void updateChannel(ChannelPtr channel) = 0;
	virtual void removeChannel(ChannelPtr& channel) = 0;
	virtual bool handleEvent(int timeout) = 0;

	int id() const;

protected:
	void wake();
	void handleTriggerEvent();

protected:
	int id_;
	std::atomic_bool shutdown_;
	PipePtr pipe_;			// wakeup pipe
	ChannelPtr channel_;	// wakeup channel
	RingBufferPtr events_;
	std::mutex mutex_;
	TimerQueue timerQueue_;

	static constexpr char kTriggerEvent = 1;	// 事件标志
	static constexpr char kTimerEvent = 2;		// 定时器事件标志
	static constexpr int kMaxTriggerEvents = 50000;
};
	
} // namespace net
} // namespace zing

#endif