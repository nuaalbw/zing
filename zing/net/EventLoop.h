/*************************************************************************
	> File Name: EventLoop.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Fri 28 Jan 2022 09:14:51 PM CST
 ************************************************************************/

#ifndef	ZING_NET_EVENTLOOP_H
#define ZING_NET_EVENTLOOP_H

#include "../base/noncopyable.h"
#include "../base/CurrentThread.h"
#include "../base/Mutex.h"
#include "../base/Timestamp.h"
#include "Callbacks.h"
#include "TimerId.h"

#include <atomic>
#include <memory>
#include <vector>
#include <functional>

namespace zing
{
namespace net
{

class Channel;
class Poller;

class EventLoop: noncopyable
{
public:
	using Functor = std::function<void()>;

	EventLoop();
	~EventLoop();

	void loop();

	void quit();

	void runInLoop(Functor cb);	// 在EventLoop线程中执行回调函数
	void queueInLoop(Functor cb);

	size_t queueSize() const;

	// TimerId runAt(Timestamp time, TimerCallback cb);

  // TimerId runAfter(double delay, TimerCallback cb);

  // TimerId runEvery(double interval, TimerCallback cb);

  // void cancel(TimerId timerId);

  void wakeup();

	void assertInLoopThread()
	{
		if (!isInLoopThread())	
		{
			abortNotInLoopThread();
		}
	}

	bool isInLoopThread() const
	{
		return threadId_ == CurrentThread::tid();
	}

	void updateChannel(Channel* channel);
	void removeChannel(Channel* channel);
	bool hasChannel(Channel* channel);

	static EventLoop* getEventLoopOfCurrentThread();

private:
	void abortNotInLoopThread();

	using ChannelList = std::vector<Channel*>;

	bool looping_;
	std::atomic<bool> quit_;
	bool eventHandling_;
	bool callingPendingFunctors_;
	int64_t iteration_;
	const pid_t threadId_;
	Timestamp pollReturnTime_;
	std::unique_ptr<Poller> poller_;
	// std::unique_ptr<TimerQueue> timerQueue_;
	int wakeupFd_;
	std::unique_ptr<Channel> wakeupChannel_;
	// boost::any context_;

	ChannelList activeChannels_;
	Channel* currentActiveChannel_;

	mutable MutexLock mutex_;
	std::vector<Functor> pendingFunctors_;
};
	
} // namespace net
} // namespace zing

#endif