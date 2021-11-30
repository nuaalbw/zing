/*************************************************************************
	> File Name: SelectTaskScheduler.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Tue 30 Nov 2021 02:59:32 PM CST
 ************************************************************************/

#include "SelectTaskScheduler.h"
#include <forward_list>

using namespace zing;
using namespace zing::net;

SelectTaskScheduler::SelectTaskScheduler(int id)
	: TaskScheduler(id), 
	  maxfd_(0), 
	  isReadfdsReset_(false), 
	  isWritefdsReset_(false), 
	  isErrorfdsReset_(false)
{
	FD_ZERO(&readfds_);
	FD_ZERO(&writefds_);
	FD_ZERO(&errorfds_);
	updateChannel(channel_);	// wakeup channel
}

SelectTaskScheduler::~SelectTaskScheduler()
{
}

void SelectTaskScheduler::updateChannel(ChannelPtr channel)
{
	std::lock_guard<std::mutex> locker(mutex_);

	int fd = channel->fd();
	if (channels_.find(fd) != channels_.end()) {
		if (channel->isNoneEvent()) {
			isReadfdsReset_ = true;
			isWritefdsReset_ = true;
			isErrorfdsReset_ = true;
			channels_.erase(fd);
		}
		else {
			isWritefdsReset_ = true;
		}
	}
	else {
		if (!channel->isNoneEvent()) {
			channels_.emplace(fd, channel);
			isReadfdsReset_ = true;
			isWritefdsReset_ = true;
			isErrorfdsReset_ = true;
		}
	}
}

void SelectTaskScheduler::removeChannel(ChannelPtr& channel)
{
	std::lock_guard<std::mutex> locker(mutex_);

	int fd = channel->fd();
	if (channels_.find(fd) != channels_.end()) {
		isReadfdsReset_ = true;
		isWritefdsReset_ = true;
		isErrorfdsReset_ = true;
		channels_.erase(fd);
	}
}

bool SelectTaskScheduler::handleEvent(int timeout)
{
	if (channels_.empty()) {
		if (timeout <= 0) {
			timeout = 10;
		}
		Timer::sleep(timeout);
		return true;
	}

	fd_set readfds, writefds, errorfds;
	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
	FD_ZERO(&errorfds);
	bool readfdsReset = false;
	bool writefdsReset = false;
	bool errorfdsReset = false;

	if (isReadfdsReset_ || isWritefdsReset_ || isErrorfdsReset_) {
		if (isErrorfdsReset_) {
			maxfd_ = 0;
		}

		std::lock_guard<std::mutex> locker(mutex_);
		for (auto iter: channels_) {
			int events = iter.second->events();
			int fd = iter.second->fd();

			if (isReadfdsReset_ && (events & EVENT_IN)) {
				FD_SET(fd, &readfds);
			}
			if (isWritefdsReset_ && (events & EVENT_OUT)) {
				FD_SET(fd, &writefds);
			}
			if (isErrorfdsReset_) {
				FD_SET(fd, &errorfds);
				if (fd > maxfd_) {
					maxfd_ = fd;
				}
			}
		}
		std::swap(isReadfdsReset_, readfdsReset);
		std::swap(isWritefdsReset_, writefdsReset);
		std::swap(isErrorfdsReset_, errorfdsReset);
	}

	if (readfdsReset) {
		FD_ZERO(&readfds_);
		memcpy(&readfds_, &readfds, sizeof(fd_set));
	}
	else {
		memcpy(&readfds, &readfds_, sizeof(fd_set));
	}

	if (writefdsReset) {
		FD_ZERO(&writefds_);
		memcpy(&writefds_, &writefds, sizeof(fd_set));
	}
	else {
		memcpy(&writefds, &writefds_, sizeof(fd_set));
	}

	if (errorfdsReset) {
		FD_ZERO(&errorfds_);
		memcpy(&errorfds_, &errorfds, sizeof(fd_set));
	}
	else {
		memcpy(&errorfds, &errorfds_, sizeof(fd_set));
	}

	if (timeout <= 0) {
		timeout = 10;
	}

	struct timeval tv = { timeout / 1000, timeout % 1000 * 1000 };
	int ret = ::select(maxfd_ + 1, &readfds, &writefds, &errorfds, &tv);
	if (ret < 0) {
		if (errno == EINTR) {
			return true;
		}
		return false;
	}

	std::forward_list<std::pair<ChannelPtr, int>> event_list;
	if (ret > 0) {
		std::lock_guard<std::mutex> locker(mutex_);

		for (auto& iter: channels_) {
			int events = 0;
			int fd = iter.second->fd();

			if (FD_ISSET(fd, &readfds)) {
				events |= EVENT_IN;
			}
			if (FD_ISSET(fd, &writefds)) {
				events |= EVENT_OUT;
			}
			if (FD_ISSET(fd, &errorfds)) {
				events |= (EVENT_HUP);	// close
			}
			if (events != 0) {
				event_list.emplace_front(iter.second, events);
			}
		}
	}

	for (auto& iter: event_list) {
		iter.first->handleEvent(iter.second);
	}

	return true;
}