/*************************************************************************
	> File Name: EventLoop.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Fri 28 Jan 2022 09:37:15 PM CST
 ************************************************************************/

#include "EventLoop.h"

#include "../base/Logging.h"
#include "../base/Mutex.h"
#include "Channel.h"
#include "Poller.h"

#include <algorithm>
#include <signal.h>
#include <sys/eventfd.h>
#include <unistd.h>

#include <poll.h>

using namespace zing;
using namespace zing::net;

namespace
{
__thread EventLoop* t_loopInThisThread = nullptr;	// one loop per thread
	
} // namespace

EventLoop* EventLoop::getEventLoopOfCurrentThread()
{
	return t_loopInThisThread;
}

EventLoop::EventLoop()
	: looping_(false), 
		quit_(false), 
		eventHandling_(false), 
		callingPendingFunctors_(false), 
		iteration_(0), 
		threadId_(CurrentThread::tid()),
		poller_(Poller::newDefaultPoller(this)), 
		// timerQueue_(new TimerQueue(this)), 
		// wakeupFd_(createEventfd()), 
		// wakeupChannel_(new Channel(this, wakeupFd_)), 
		currentActiveChannel_(nullptr)
{
	LOG_DEBUG << "EventLoop created " << this << " in thread " << threadId_;
	if (t_loopInThisThread)
	{
		LOG_FATAL << "Another EventLoop " << t_loopInThisThread
							<< " exists in this thread " << threadId_;
	}
	else
	{
		t_loopInThisThread = this;
	}
}

EventLoop::~EventLoop()
{
	LOG_DEBUG << "EventLoop " << this << " of thread " << threadId_
						<< "destructss in thread " << CurrentThread::tid();
	t_loopInThisThread = nullptr;
}

void EventLoop::loop()
{
	assert(!looping_);
	assertInLoopThread();
	looping_ = true;
	LOG_TRACE << "EventLoop " << this << " start looping";
	::poll(nullptr, 0, 5 * 1000);

	LOG_TRACE << "EventLoop " << this << " stop looping";
	looping_ = false;
}

void EventLoop::updateChannel(Channel* channel)
{
	assert(channel->ownerLoop() == this);
	assertInLoopThread();
	poller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel* channel)
{
	assert(channel->ownerLoop() == this);
	assertInLoopThread();
	// if (eventHandling_)
  // {
  //   assert(currentActiveChannel_ == channel ||
  //       std::find(activeChannels_.begin(), activeChannels_.end(), channel) == activeChannels_.end());
  // }
  poller_->removeChannel(channel);
}

bool EventLoop::hasChannel(Channel* channel)
{
	assert(channel->ownerLoop() == this);
	assertInLoopThread();
	return poller_->hasChannel(channel);
}
