/*************************************************************************
	> File Name: EpollTaskScheduler.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Tue 23 Nov 2021 08:39:13 PM CST
 ************************************************************************/

#ifndef ZING_NET_EPOLL_TASK_SCHEDULER_H
#define ZING_NET_EPOLL_TASK_SCHEDULER_H

#include "TaskScheduler.h"
#include <unordered_map>

namespace zing
{
namespace net
{

class EpollTaskScheduler: public TaskScheduler
{
public:
	explicit EpollTaskScheduler(int id = 0);
	virtual ~EpollTaskScheduler();

	void updateChannel(ChannelPtr channel) override;
	void removeChannel(ChannelPtr& channel) override;
	bool handleEvent(int timeout) override;	// timeout: ms

private:
	void update(int op, ChannelPtr& channel);
	int epollfd_;
	std::mutex mutex_;
	std::unordered_map<int, ChannelPtr> channels_;	// fd -> Channel
};
	
} // namespace net
} // namespace zing

#endif