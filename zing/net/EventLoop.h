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

#include <memory>

namespace zing
{
namespace net
{

class Channel;
class Poller;

class EventLoop: noncopyable
{
public:
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

	bool looping_;
	const pid_t threadId_;

	std::unique_ptr<Poller> poller_;
	
};

	
} // namespace net
} // namespace zing


#endif