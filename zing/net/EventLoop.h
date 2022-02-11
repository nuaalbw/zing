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
class TimerQueue;

class EventLoop: noncopyable
{
public:
	using Functor = std::function<void()>;

	EventLoop();
	~EventLoop();

	// 永久执行EventLoop循环，必须在EventLoop对象所创建的线程调用
	void loop();

	// 退出循环
	void quit();

	// poll或Epoll系统调用返回的时间，通常意味着数据到来
	Timestamp pollReturnTime() const { return pollReturnTime_; }

	int64_t iteration() const { return iteration_; }

	// EventLoop线程中执行回调函数，此操作会唤醒线程，可以跨线程调用
	void runInLoop(Functor cb);
	void queueInLoop(Functor cb);

	size_t queueSize() const;

	// 在time时刻执行回调，可跨线程调用
	TimerId runAt(Timestamp time, TimerCallback cb);

	// 在delay秒之后执行回调，可跨线程调用
  TimerId runAfter(double delay, TimerCallback cb);

	// 每interval秒执行一次回调，可跨线程调用
  TimerId runEvery(double interval, TimerCallback cb);

  void cancel(TimerId timerId);

  void wakeup();
	void updateChannel(Channel* channel);
	void removeChannel(Channel* channel);
	bool hasChannel(Channel* channel);

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

	static EventLoop* getEventLoopOfCurrentThread();

private:
	void abortNotInLoopThread();
	void handleRead();
	void doPendingFunctors();
	void printActiveChannels() const;	// for Debug

	using ChannelList = std::vector<Channel*>;

	bool looping_;
	std::atomic<bool> quit_;
	bool eventHandling_;
	bool callingPendingFunctors_;
	int64_t iteration_;
	const pid_t threadId_;
	Timestamp pollReturnTime_;
	std::unique_ptr<Poller> poller_;
	std::unique_ptr<TimerQueue> timerQueue_;
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