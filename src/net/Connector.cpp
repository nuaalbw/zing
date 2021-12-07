/*************************************************************************
	> File Name: Connector.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Tue 07 Dec 2021 11:05:44 AM CST
 ************************************************************************/

#include "Connector.h"
#include "../base/Logger.h"
#include "SocketUtil.h"
#include "assert.h"
#include <errno.h>

using namespace zing;
using namespace zing::net;
using namespace zing::base;

const int Connector::kMaxRetryDelayMs;

Connector::Connector(EventLoop* loop, std::string ip, uint16_t port)
	: loop_(loop), 
	  ip_(ip), 
	  port_(port), 
	  connect_(false), 
	  state_(kDisconnected), 
	  retryDelayMs_(kInitRetryDelayMs)
{
	LOG_DEBUG("ctor[%p]", this);	
}

Connector::~Connector()
{
	LOG_DEBUG("dtor[%p]", this);
	assert(!channel_);
}

void Connector::start()
{
	std::lock_guard<std::mutex> locker(mutex_);

	assert(state_ == kDisconnected);
	connect();
	connect_ = true;
}

void Connector::restart()
{
	std::lock_guard<std::mutex> locker(mutex_);

	setState(kDisconnected);
	retryDelayMs_ = kInitRetryDelayMs;
	connect_ = true;
	connect();
}

void Connector::stop()
{
	std::lock_guard<std::mutex> locker(mutex_);

	if (state_ == kConnecting) {
		setState(kDisconnected);
		int sockfd = removeAndResetChannel();
		retry(sockfd);
	}
}

bool Connector::connect()
{
	int sockfd = SocketUtil::createNonblockingOrDie();
	bool ret = SocketUtil::connect(sockfd, ip_, port_);
	int savedErrno = ret ? 0 : errno;
	switch (savedErrno) 
	{
		case 0:
		case EINPROGRESS:
		case EINTR:
		case EISCONN:
			connecting(sockfd);
			break;
		
		case EAGAIN:
		case EADDRINUSE:
		case EADDRNOTAVAIL:
		case ENETUNREACH:
			retry(sockfd);
			break;
		case ENOTSOCK:
			LOG_ERROR("connect error %d", savedErrno);
			SocketUtil::close(sockfd);
			break;
		default:
			LOG_ERROR("unexpected error %d", savedErrno);
			SocketUtil::close(sockfd);
			break;
	}
	return true;
}

void Connector::connecting(int sockfd)
{
	setState(kConnecting);
	assert(!channel_);
	channel_.reset(new Channel(sockfd));
	channel_->setWriteCallback(
		std::bind(&Connector::handleWrite, this));
	channel_->setErrorCallback(
		std::bind(&Connector::handleError, this));
	channel_->enableWriting();
	loop_->updateChannel(channel_);
}

void Connector::handleWrite()
{
	if (state_ == kConnecting) {
		int sockfd = removeAndResetChannel();
		int err = SocketUtil::getSocketError(sockfd);
		if (err) {
			LOG_WARNING("SO_ERROR = %d %s", err, strerror(err));
			retry(sockfd);
		}
		else if (SocketUtil::isSelfConnect(sockfd)) {
			LOG_WARNING("Self connect");
			retry(sockfd);
		}
		else {
			setState(kConnected);
			if (connect_) {
				newConnectionCallback_(sockfd);
			}
			else {
				SocketUtil::close(sockfd);
			}
		}
	}
	else {
		assert(state_ == kDisconnected);
	}
}

void Connector::handleError()
{
	LOG_ERROR("state = %d", state_);
	if (state_ == kConnecting) {
		int sockfd = removeAndResetChannel();
		int err = SocketUtil::getSocketError(sockfd);
		LOG_ERROR("SO_ERROR = %d %s", err, strerror(err));
		retry(sockfd);
	}
}

void Connector::retry(int sockfd)
{
	SocketUtil::close(sockfd);
	setState(kDisconnected);
	if (connect_) {
		LOG_INFO("Retry connecting to %s %hd in %d milliseconds.", ip_, port_, retryDelayMs_);
		loop_->addTimer(std::bind(&Connector::connect, shared_from_this()),
						retryDelayMs_);
		retryDelayMs_ = std::min(retryDelayMs_ * 2, kMaxRetryDelayMs);
	}
	else {
		LOG_DEBUG("do not connect");
	}
}

int Connector::removeAndResetChannel()
{
	channel_->disableWriting();
	channel_->disableReading();
	loop_->updateChannel(channel_);
	int sockfd = channel_->fd();
	channel_.reset();
	return sockfd;
}
