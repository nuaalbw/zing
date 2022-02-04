/*************************************************************************
	> File Name: PollPoller.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Fri 04 Feb 2022 08:51:45 PM CST
 ************************************************************************/

#ifndef ZING_NET_POLLPOLLER_H
#define ZING_NET_POLLPOLLER_H

#include "Poller.h"

#include <vector>

struct pollfd;

namespace zing
{
namespace net
{

class PollPoller: public Poller
{
public:
	PollPoller(EventLoop* loop);
	~PollPoller() override;

	Timestamp poll(int timeoutMs, ChannelList* activeChannels) override;
	void updateChannel(Channel* channel) override;
	void removeChannel(Channel* channel) override;

private:
	void fillActiveChannels(int numEvents, 
													ChannelList* activeChannels) const;	

	using PollFdList = std::vector<struct pollfd>;
	PollFdList pollfds_;
};
	
} // namespace net
} // namespace zing

#endif