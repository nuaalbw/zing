/*************************************************************************
	> File Name: Acceptor.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Sun 21 Nov 2021 07:36:15 PM CST
 ************************************************************************/

#include "Acceptor.h"
#include "SocketUtil.h"
#include "../base/Logger.h"

using namespace zing;
using namespace zing::net;
using namespace zing::base;

Acceptor::Acceptor(EventLoop* loop)
	: loop_(loop), 
	  acceptSocket_(new TcpSocket)
{
}

Acceptor::~Acceptor()
{
}

void Acceptor::setNewConnectionCallback(NewConnectionCallback cb)
{
	newConnectionCallback_ = std::move(cb);
}

int Acceptor::listen(std::string ip, uint16_t port)
{
	std::lock_guard<std::mutex> locker(mutex_);

	if (acceptSocket_->getSocket() > 0) {
		acceptSocket_->close();
	}

	int sockfd = acceptSocket_->create();
	acceptChannel_.reset(new Channel(sockfd));
	SocketUtil::setReuseAddr(sockfd);
	SocketUtil::setReusePort(sockfd);
	SocketUtil::setNonblock(sockfd);

	if (!acceptSocket_->bind(ip, port)) {
		LOG_ERROR("bind %s %d", ip.c_str(), port);
		return -1;
	}

	if (!acceptSocket_->listen(1024)) {
		LOG_ERROR("listen error");
		return -1;
	}

	// acceptChannel_->setReadCallback( [this](){ this->onAccept(); } );
	acceptChannel_->setReadCallback(std::bind(&Acceptor::onAccept, this));
	acceptChannel_->enableReading();
	loop_->updateChannel(acceptChannel_);
	LOG_INFO("listening %s %d", ip.c_str(), port);

	return 0;
}

void Acceptor::close()
{
	std::lock_guard<std::mutex> locker(mutex_);

	if (acceptSocket_->getSocket() > 0) {
		loop_->removeChannel(acceptChannel_);
		acceptSocket_->close();
	}
}

void Acceptor::onAccept()
{
	std::lock_guard<std::mutex> locker(mutex_);

	int cfd = acceptSocket_->accept();
	if (acceptSocket_ > 0) {
		if (newConnectionCallback_) {
			LOG_INFO("new connection: %d", cfd);
			newConnectionCallback_(cfd);
		}
		else {
			SocketUtil::close(cfd);
		}
	}
}