/*************************************************************************
	> File Name: EventLoopThread.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Sun 13 Feb 2022 01:27:34 PM CST
 ************************************************************************/

#ifndef ZING_NET_EVENTLOOPTHREAD_H
#define ZING_NET_EVENTLOOPTHREAD_H

#include "../base/Condition.h"
#include "../base/Mutex.h"
#include "../base/Thread.h"

namespace zing
{
namespace net
{

class EventLoop;

class EventLoopThread: noncopyable
{
public:
	using ThreadInitCallback = std::function<void(EventLoop*)>;

	EventLoopThread(const ThreadInitCallback& cb = ThreadInitCallback(), 
									const string& name = string());
	~EventLoopThread();
	EventLoop* startLoop();

private:
	void threadFunc();

	EventLoop* loop_;
	bool exiting_;
	Thread thread_;
	MutexLock mutex_;
	Condition cond_;
	ThreadInitCallback callback_;
};

} // namespace net
} // namespace zing

#endif