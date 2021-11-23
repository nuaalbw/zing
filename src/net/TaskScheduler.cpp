/*************************************************************************
	> File Name: TaskScheduler.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Tue 23 Nov 2021 06:56:00 PM CST
 ************************************************************************/

#include "TaskScheduler.h"
#include <signal.h>

using namespace zing;
using namespace zing::net;

TaskScheduler::TaskScheduler(int id)
	: id_(id), 
	  shutdown_(false), 
	  pipe_(new Pipe()), 
	  events_(new base::RingBuffer<EventCallback>(kMaxTriggerEvents))
{
	if (pipe_->create()) {	// 监视管道的读端
		channel_.reset(new Channel(pipe_->readfd()));
		channel_->enableReading();
		channel_->setReadCallback(std::bind(&TaskScheduler::wake, this));
	}
}

TaskScheduler::~TaskScheduler()
{
}

void TaskScheduler::start()
{
	signal(SIGPIPE, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGUSR1, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
	signal(SIGKILL, SIG_IGN);

	shutdown_ = false;
	while (!shutdown_) {				// IO复用主循环
		handleTriggerEvent();			// 处理管道触发的事件
		timerQueue_.handleTimerEvent();	// 处理定时器事件
		int64_t timeout = timerQueue_.getTimeRemaining();
		handleEvent((int)timeout);		// 处理普通IO事件
	}
}

void TaskScheduler::stop()
{
	shutdown_ = true;
	char event = kTriggerEvent;
	pipe_->write(&event, 1);	// 向管道写1字节数据
}

TimerId TaskScheduler::addTimer(TimerCallback cb, uint32_t msec)
{
	TimerId id = timerQueue_.addTimer(cb, msec);
	return id;
}

void TaskScheduler::removeTimer(TimerId timerId)
{
	timerQueue_.removeTimer(timerId);
}

bool TaskScheduler::addTriggerEvent(EventCallback cb)
{
	std::lock_guard<std::mutex> lock(mutex_);

	if (events_->size() < kMaxTriggerEvents) {
		char event = kTriggerEvent;
		events_->push(std::move(cb));
		pipe_->write(&event, 1);
		return true;
	}
	return false;
}

int TaskScheduler::id() const
{
	return id_;
}

void TaskScheduler::wake()
{
	char event[10] = { 0 };
	while (pipe_->read(event, 10) > 0);
}

void TaskScheduler::handleTriggerEvent()
{
	do
	{
		EventCallback callback;
		if (events_->pop(callback)) {
			callback();
		}
	} while (events_->size() > 0);
}