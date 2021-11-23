/*************************************************************************
	> File Name: EpollTaskScheduler.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Tue 23 Nov 2021 08:48:35 PM CST
 ************************************************************************/

#include "EpollTaskScheduler.h"
#include <sys/epoll.h>
#include <errno.h>

using namespace zing;
using namespace zing::net;

EpollTaskScheduler::EpollTaskScheduler(int id)
	: TaskScheduler(id), 
	  epollfd_(epoll_create(1024))
{
	updateChannel(channel_);
}

EpollTaskScheduler::~EpollTaskScheduler()
{
}

void EpollTaskScheduler::updateChannel(ChannelPtr channel)
{
	std::lock_guard<std::mutex> locker(mutex_);

	int fd = channel->fd();
	if (channels_.find(fd) != channels_.end()) {
		if (channel->isNoneEvent()) {
			update(EPOLL_CTL_DEL, channel);
			channels_.erase(fd);
		}
		else {
			update(EPOLL_CTL_MOD, channel);
		}
	}
	else {
		if (!channel->isNoneEvent()) {
			channels_.emplace(fd, channel);
			update(EPOLL_CTL_ADD, channel);
		}
	}
}

void EpollTaskScheduler::removeChannel(ChannelPtr& channel)
{
	std::lock_guard<std::mutex> locker(mutex_);

	int fd = channel->fd();
	if (channels_.find(fd) != channels_.end()) {
		update(EPOLL_CTL_DEL, channel);
		channels_.erase(fd);
	}
}

bool EpollTaskScheduler::handleEvent(int timeout)
{
	struct epoll_event events[512] = { 0 };
	int num = ::epoll_wait(epollfd_, events, 512, timeout);
	if (num < 0) {
		if (errno != EINTR) {
			return false;
		}
	}

	for (int i = 0; i < num; ++i) {
		if (events[i].data.ptr) {
			((Channel*)events[i].data.ptr)->handleEvent(events[i].events);
		}
	}
	return true;
}

void EpollTaskScheduler::update(int op, ChannelPtr& channel)
{
	struct epoll_event event = { 0 };

	if (op != EPOLL_CTL_DEL) {
		event.data.ptr = channel.get();
		event.events = channel->events();
	}
	::epoll_ctl(epollfd_, op, channel->fd(), &event);
}