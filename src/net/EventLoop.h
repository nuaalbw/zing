/*************************************************************************
	> File Name: EventLoop.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Thu 25 Nov 2021 09:49:44 PM CST
 ************************************************************************/

#ifndef ZING_NET_EVENT_LOOP_H
#define ZING_NET_EVENT_LOOP_H

#include "EpollTaskScheduler.h"
#include "Pipe.h"
#include "Timer.h"
#include "../base/RingBuffer.h"
#include <mutex>
#include <functional>
#include <memory>
#include <thread>
#include <vector>

namespace zing
{
namespace net
{

using TaskSchedulerPtr = std::shared_ptr<TaskScheduler>;

class EventLoop: noncopyable
{
	using ThreadPtr = std::shared_ptr<std::thread>;
public:
	EventLoop(uint32_t threadNum = 1);
	virtual ~EventLoop();

	TaskSchedulerPtr getTaskScheduler();

	bool addTriggerEvent(EventCallback cb);
	TimerId addTimer(TimerCallback cb, uint32_t msec);
	void removeTimer(TimerId id);
	void updateChannel(ChannelPtr channel);
	void removeChannel(ChannelPtr& channel);

	void loop();
	void quit();

private:
	std::mutex mutex_;
	uint32_t threadNum_;
	uint32_t index_;
	std::vector<TaskSchedulerPtr> taskSchedulers_;	// loops
	std::vector<ThreadPtr> threads_;	// threads
};
	
} // namespace net
} // namespace zing

#endif