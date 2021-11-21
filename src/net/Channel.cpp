/*************************************************************************
	> File Name: Channel.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Sun 21 Nov 2021 05:31:58 PM CST
 ************************************************************************/

#include "Channel.h"

using namespace zing;
using namespace zing::net;

Channel::Channel(int sockfd)
	: sockfd_(sockfd), 
	  events_(0)
{
}

Channel::~Channel()
{
}

void Channel::setReadCallback(EventCallback cb)
{
	readCallback_ = std::move(cb);
}

void Channel::setWriteCallback(EventCallback cb)
{
	writeCallback_ = std::move(cb);
}

void Channel::setCloseCallback(EventCallback cb)
{
	closeCallback_ = std::move(cb);
}

void Channel::setErrorCallback(EventCallback cb)
{
	errorCallback_ = std::move(cb);
}

int Channel::fd() const
{
	return sockfd_;
}

int Channel::events() const
{
	return events_;
}

void Channel::setEvents(int events)
{
	events_ = events;
}

void Channel::enableReading()
{
	events_ |= EVENT_IN;
}

void Channel::enableWriting()
{
	events_ |= EVENT_OUT;
}

void Channel::disableReading()
{
	events_ &= ~EVENT_IN;
}

void Channel::disableWriting()
{
	events_ &= ~EVENT_OUT;
}

bool Channel::isNoneEvent() const
{
	return events_ == EVENT_NONE;
}

bool Channel::isWriting() const
{
	return (events_ & EVENT_OUT) != 0;
}

bool Channel::isReading() const
{
	return (events_ & EVENT_IN) != 0;
}

void Channel::handleEvent(int events)
{
	if (events_ & (EVENT_PRI | EVENT_IN)) {
		readCallback_();
	}

	if (events_ & (EVENT_OUT)) {
		writeCallback_();
	}

	if (events_ & EVENT_HUP) {
		closeCallback_();
		return;
	}

	if (events_ & EVENT_ERR) {
		errorCallback_();
	}
}