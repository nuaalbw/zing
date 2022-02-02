/*************************************************************************
	> File Name: Poller.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Wed 02 Feb 2022 09:30:32 PM CST
 ************************************************************************/

#include "Poller.h"
#include "Channel.h"

using namespace zing;
using namespace zing::net;

Poller::Poller(EventLoop* loop)
	: ownerLoop_(loop)
{
}

Poller::~Poller() = default;

bool Poller::hasChannel(Channel* channel) const
{
	assertInLoopThread();
	ChannelMap::const_iterator it = channels_.find(channel->fd());
	return it != channels_.end() && it->second == channel;
}