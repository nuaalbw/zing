/*************************************************************************
	> File Name: Channel.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Fri 28 Jan 2022 09:00:51 PM CST
 ************************************************************************/

#include "../base/Logging.h"
#include "Channel.h"
#include "EventLoop.h"

#include <sstream>

#include <poll.h>

using namespace zing;
using namespace zing::net;

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel(EventLoop* loop, int fd)
	: loop_(loop), 
		fd_(fd), 
		events_(0), 
		revents_(0), 
		index_(-1), 
		logHup_(true), 
		tied_(false), 
		eventHandling_(false), 
		addedToLoop_(false)
{
}

Channel::~Channel()
{
	assert(!eventHandling_);
	assert(!addedToLoop_);
	if (loop_->isInLoopThread())
	{
		assert(!loop_->hasChannel(this));
	}
}

void Channel::tie(const std::shared_ptr<void>& obj)
{
	tie_ = obj;
	tied_ = true;
}

void Channel::update()
{
	addedToLoop_ = true;
	loop_->updateChannel(this);
}

void Channel::remove()
{
	assert(isNoneEvent());
	addedToLoop_ = false;
	loop_->removeChannel(this);
}

void Channel::handleEvent(Timestamp receiveTime)
{
	std::shared_ptr<void> guard;
	if (tied_)
	{
		guard = tie_.lock();
		if (guard)
		{
			handleEventWithGuard(receiveTime);
		}
	}
	else
	{
		handleEventWithGuard(receiveTime);
	}
}

void Channel::handleEventWithGuard(Timestamp receiveTime)
{
	eventHandling_ = true;
	LOG_TRACE << reventsToString();
	if ((revents_ & POLLHUP) && !(revents_ & POLLIN))		// 对端关闭（挂起）
	{
		if (logHup_)
		{
			LOG_WARN << "fd = " << fd_ << " Channel::handle_event() POLLHUP";
		}
		if (closeCallback_)
		{
			closeCallback_();
		}
	}

	if (revents_ & POLLNVAL)		// 非法的fd，文件描述符不是一个打开的文件
	{
		LOG_WARN << "fd = " << fd_ << " Channel::handle_event() POLLNVAL";
	}

	if (revents_ & (POLLERR | POLLNVAL))		// 发生错误
	{
		if (errorCallback_)
		{
			errorCallback_();
		}
	}

	if (revents_ & (POLLIN | POLLPRI | POLLRDHUP))	// 普通或高优先级数据可读
	{
		if (readCallback_)
		{
			readCallback_(receiveTime);
		}
	}

	if (revents_ & POLLOUT)			// 普通数据可写
	{
		if (writeCallback_)
		{
			writeCallback_();
		}
	}

	eventHandling_ = false;
}

string Channel::reventsToString() const
{
	return eventsToString(fd_, revents_);
}

string Channel::eventsToString() const
{
	return eventsToString(fd_, events_);
}

string Channel::eventsToString(int fd, int ev)
{
  std::ostringstream oss;
  oss << fd << ": ";
  if (ev & POLLIN)
    oss << "IN ";
  if (ev & POLLPRI)
    oss << "PRI ";
  if (ev & POLLOUT)
    oss << "OUT ";
  if (ev & POLLHUP)
    oss << "HUP ";
  if (ev & POLLRDHUP)
    oss << "RDHUP ";
  if (ev & POLLERR)
    oss << "ERR ";
  if (ev & POLLNVAL)
    oss << "NVAL ";

  return oss.str();
}