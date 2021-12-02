/*************************************************************************
	> File Name: EventLoop.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Thu 25 Nov 2021 10:34:30 PM CST
 ************************************************************************/

#include "EventLoop.h"
#include "assert.h"

using namespace zing;
using namespace zing::net;

EventLoop::EventLoop(uint32_t threadNum)
	: threadNum_(threadNum), 
	  index_(1), 
	  running_(false)
{
	assert(threadNum_ > 0);
	for (uint32_t i = 0; i < threadNum_; ++i) {
		TaskSchedulerPtr taskSchedulerPtr(new EpollTaskScheduler(i));
		taskSchedulers_.push_back(taskSchedulerPtr);
	}
}

EventLoop::~EventLoop()
{
	quit();
}

TaskSchedulerPtr EventLoop::getTaskScheduler()
{
	std::lock_guard<std::mutex> locker(mutex_);

	if (taskSchedulers_.size() == 1) {
		return taskSchedulers_.at(0);
	}
	else {
		auto taskScheduler = taskSchedulers_.at(index_);
		if (++index_ >= taskSchedulers_.size()) {
			index_ = 1;
		}
		return taskScheduler;
	}
}

bool EventLoop::addTriggerEvent(EventCallback cb)
{
	std::lock_guard<std::mutex> locker(mutex_);

	if (taskSchedulers_.size() > 0) {
		return taskSchedulers_[0]->addTriggerEvent(cb);
	}
	return false;
}

TimerId EventLoop::addTimer(TimerCallback cb, uint32_t msec)
{
	std::lock_guard<std::mutex> locker(mutex_);

	if (taskSchedulers_.size() > 0) {
		return taskSchedulers_[0]->addTimer(cb, msec);
	}
	return 0;
}

void EventLoop::removeTimer(TimerId id)
{
	std::lock_guard<std::mutex> locker(mutex_);

	if (taskSchedulers_.size() > 0) {
		taskSchedulers_[0]->removeTimer(id);
	}
}

void EventLoop::updateChannel(ChannelPtr channel)
{
	std::lock_guard<std::mutex> locker(mutex_);

	if (taskSchedulers_.size() > 0) {
		taskSchedulers_[0]->updateChannel(channel);
	}
}

void EventLoop::removeChannel(ChannelPtr& channel)
{
	std::lock_guard<std::mutex> locker(mutex_);

	if (taskSchedulers_.size() > 0) {
		taskSchedulers_[0]->removeChannel(channel);
	}
}

void EventLoop::loop()
{
	// std::lock_guard<std::mutex> locker(mutex_);

	// 防止重复调用
	if (running_) {
		return;
	}
	running_ = true;

	// 创建threadNum个线程，实现one loop per thread
	for (uint32_t i = 1; i < threadNum_; ++i) {
		TaskSchedulerPtr taskSchedulerPtr = taskSchedulers_[i];
		ThreadPtr thread(new std::thread(&TaskScheduler::start, taskSchedulerPtr.get()));
		thread->native_handle();
		threads_.push_back(thread);
	}
	// 主线程也启动一个loop
	taskSchedulers_[0]->start();
}

void EventLoop::quit()
{
	std::lock_guard<std::mutex> locker(mutex_);

	for (auto iter: taskSchedulers_) {
		iter->stop();
	}
	for (auto iter: threads_) {
		iter->join();
	}

	taskSchedulers_.clear();
	threads_.clear();
}