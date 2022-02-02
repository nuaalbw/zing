/*************************************************************************
	> File Name: Poller.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Wed 02 Feb 2022 08:53:34 PM CST
 ************************************************************************/

#ifndef ZING_NET_POLLER_H
#define ZING_NET_POLLER_H

#include "../base/Timestamp.h"
#include "EventLoop.h"

#include <map>
#include <vector>

namespace zing
{
namespace net
{

class Channel;

// IO复用的封装，可以支持POLL和EPOLL两种机制
class Poller: noncopyable
{
public:
	using ChannelList = std::vector<Channel*>;

	Poller(EventLoop* loop);
	virtual ~Poller();

	// 获取当前正在活动的事件，必须在EventLoop线程中调用
	virtual Timestamp poll(int timeoutMs, ChannelList* activeChannels) = 0;

	// 更新所关注的IO事件，必须在EventLoop线程中调用
	virtual void updateChannel(Channel* channel) = 0;

	// 移除所关注的IO事件，必须在EventLoop线程中调用
	virtual void removeChannel(Channel* channel) = 0;

	virtual bool hasChannel(Channel* channel) const;

	static Poller* newDefaultPoller(EventLoop* loop);

	void assertInLoopThread() const
	{
		ownerLoop_->assertInLoopThread();
	}

protected:
	using ChannelMap = std::map<int, Channel*>;
	ChannelMap channels_;

private:
	EventLoop* ownerLoop_;
};

} // namespace net
} // namespace zing

#endif