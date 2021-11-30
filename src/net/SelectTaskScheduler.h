/*************************************************************************
	> File Name: SelectTaskScheduler.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Tue 30 Nov 2021 02:41:20 PM CST
 ************************************************************************/

#ifndef ZING_NET_SELECT_TASK_SCHEDULER_H
#define ZING_NET_SELECT_TASK_SCHEDULER_H

#include "../base/noncopyable.h"
#include "TaskScheduler.h"
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>

namespace zing
{
namespace net
{

class SelectTaskScheduler: public TaskScheduler
{
public:
	explicit SelectTaskScheduler(int id = 0);
	virtual ~SelectTaskScheduler();

	void updateChannel(ChannelPtr channel) override;
	void removeChannel(ChannelPtr& channel) override;
	bool handleEvent(int timeout) override;

private:
	fd_set readfds_;
	fd_set writefds_;
	fd_set errorfds_;
	int maxfd_;

	bool isReadfdsReset_;
	bool isWritefdsReset_;
	bool isErrorfdsReset_;

	std::mutex mutex_;
	std::unordered_map<int, ChannelPtr> channels_;
};
	
} // namespace net
} // namespace zing

#endif