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
#include "SocketsOps.h"
#include "TimerQueue.h"

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
const int kPollTimeMs = 10000;
	
int createEventfd()
{
  int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
  if (evtfd < 0)
  {
    LOG_SYSERR << "Failed in eventfd";
    abort();
  }
  return evtfd;
}

#pragma GCC diagnostic ignored "-Wold-style-cast"
class IgnoreSigPipe
{
 public:
  IgnoreSigPipe()
  {
    ::signal(SIGPIPE, SIG_IGN);
    // LOG_TRACE << "Ignore SIGPIPE";
  }
};
#pragma GCC diagnostic error "-Wold-style-cast"

IgnoreSigPipe initObj;
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
		wakeupFd_(createEventfd()), 
		wakeupChannel_(new Channel(this, wakeupFd_)), 
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
						<< " destructs in thread " << CurrentThread::tid();
	t_loopInThisThread = nullptr;
}

void EventLoop::loop()
{
	assert(!looping_);
	assertInLoopThread();
	looping_ = true;
	LOG_TRACE << "EventLoop " << this << " start looping";

	while (!quit_)
	{
		activeChannels_.clear();
		pollReturnTime_ = poller_->poll(kPollTimeMs, &activeChannels_);
		++iteration_;
		if (Logger::logLevel() <= Logger::TRACE)
		{
			// printActiveChannels();
		}
		eventHandling_ = true;
		for (Channel* channel: activeChannels_)
		{
			currentActiveChannel_ = channel;
			currentActiveChannel_->handleEvent(pollReturnTime_);
		}
		currentActiveChannel_ = nullptr;
		eventHandling_ = false;
		// doPendingFunctors();
	}

	LOG_TRACE << "EventLoop " << this << " stop looping";
	looping_ = false;
}

void EventLoop::quit()
{
	quit_ = true;
	if (!isInLoopThread())
	{
		wakeup();
	}
}

void EventLoop::runInLoop(Functor cb)
{
	if (isInLoopThread())
	{
		cb();
	}
	else
	{
		queueInLoop(std::move(cb));
	}
}

void EventLoop::queueInLoop(Functor cb)
{
	{
		MutexLockGuard lock(mutex_);
		pendingFunctors_.push_back(std::move(cb));
	}

	if (!isInLoopThread() || callingPendingFunctors_)
	{
		wakeup();
	}
}

size_t EventLoop::queueSize() const
{
	MutexLockGuard lock(mutex_);
	return pendingFunctors_.size();
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

void EventLoop::abortNotInLoopThread()
{
  LOG_FATAL << "EventLoop::abortNotInLoopThread - EventLoop " << this
            << " was created in threadId_ = " << threadId_
            << ", current thread id = " <<  CurrentThread::tid();
}

void EventLoop::wakeup()
{
	uint64_t one = 1;
	ssize_t n = sockets::write(wakeupFd_, &one, sizeof(one));
	if (n != sizeof(one))
	{
    LOG_ERROR << "EventLoop::wakeup() writes " << n << " bytes instead of 8";
	}
}