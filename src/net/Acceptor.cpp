/*************************************************************************
	> File Name: Acceptor.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Sun 21 Nov 2021 07:36:15 PM CST
 ************************************************************************/

#include "Acceptor.h"
#include "SocketUtil.h"

using namespace zing;
using namespace zing::net;

Acceptor::Acceptor(EventLoop* loop)
	: loop_(loop), 
	  socket_(new TcpSocket)
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

	if (socket_->getSocket() > 0) {
		socket_->close();
	}

	int sockfd = socket_->create();
	channel_.reset(new Channel(sockfd));
	SocketUtil::setReuseAddr(sockfd);
	SocketUtil::setReusePort(sockfd);
	SocketUtil::setNonblock(sockfd);

	if (!socket_->bind(ip, port)) {
		return -1;
	}

	if (!socket_->listen(1024)) {
		return -1;
	}

	channel_->setReadCallback( [this](){ this->onAccept(); } );
	channel_->enableReading();
	loop_->updateChannel(channel_);

	return 0;
}

void Acceptor::close()
{
	std::lock_guard<std::mutex> locker(mutex_);

	if (socket_->getSocket() > 0) {
		loop_->removeChannel(channel_);
		socket_->close();
	}
}

void Acceptor::onAccept()
{
	std::lock_guard<std::mutex> locker(mutex_);

	int socket = socket_->getSocket();
	if (socket_ > 0) {
		if (newConnectionCallback_) {
			newConnectionCallback_(socket);
		}
		else {
			SocketUtil::close(socket);
		}
	}
}