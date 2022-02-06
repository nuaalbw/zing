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
	

	void assertInLoopThread()
	{
		if (!isInLoopThread())	
		{
			abortInLoopThread();
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
	void abortInLoopThread();

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
	// int wakeupFd_;
	// std::unique_ptr<Channel> wakeupChannel_;
	// boost::any context_;

	ChannelList activeChannels_;
	Channel* currentActiveChannel_;

	mutable MutexLock mutex_;
	std::vector<Functor> pendingFunctors_;
};
	
} // namespace net
} // namespace zing

#endif