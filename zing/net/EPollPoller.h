/*************************************************************************
	> File Name: EPollPoller.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Sat 05 Feb 2022 08:10:24 PM CST
 ************************************************************************/

#ifndef ZING_NET_EPOLLPOLLER_H
#define ZING_NET_EPOLLPOLLER_H

#include "Poller.h"
#include <vector>

struct epoll_event;

namespace zing
{
namespace net
{

class EPollPoller: public Poller
{
public:
	EPollPoller(EventLoop* loop);
	~EPollPoller() override;

	Timestamp poll(int timeoutMs, ChannelList* activeChannels) override;
	void updateChannel(Channel* channel) override;
	void removeChannel(Channel* channel) override;

private:
	static const int kInitEventListSize = 16;
	static const char* operationToString(int op);
	void fillActiveChannels(int numEvents, 
													ChannelList* activeChannels) const;
	void update(int operation, Channel* channel);

	using EventList = std::vector<struct epoll_event>;
	int epollfd_;
	EventList events_;	
};
	
} // namespace net
} // namespace zing

#endif