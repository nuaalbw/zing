/*************************************************************************
	> File Name: TcpServer.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Tue 30 Nov 2021 08:15:08 PM CST
 ************************************************************************/

#include "TcpServer.h"
#include "../base/Logger.h"

using namespace zing;
using namespace zing::net;
using namespace zing::base;

TcpServer::TcpServer(EventLoop* loop)
	: loop_(loop), 
	  port_(0), 
	  acceptor_(new Acceptor(loop)), 
	  started_(false)
{
	acceptor_->setNewConnectionCallback(
		std::bind(&TcpServer::onNewConnection, this, std::placeholders::_1));
}

TcpServer::~TcpServer()
{
	stop();
}

bool TcpServer::start(std::string ip, uint16_t port)
{
	stop();

	if (!started_) {
		if (acceptor_->listen(ip, port) < 0) {
			return false;
		}
		port_ = port;
		ip_ = ip;
		started_ = true;
		return true;
	}

	return false;
}

void TcpServer::stop()
{
	if (!started_) {
		return;
	}

	mutex_.lock();
	for (auto iter: connections_) {
		iter.second->disconnect();
	}
	mutex_.unlock();
	
	acceptor_->close();
	started_ = false;
	
	while (1) {
		Timer::sleep(10);
		if (connections_.empty()) {
			break;
		}
	}
}

void TcpServer::setConnectionCallback(const ConnectionCallback& cb)
{
	connectionCallback_ = cb;
}

void TcpServer::setMessageCallback(const MessageCallback& cb)
{
	messageCallback_ = cb;
}

std::string TcpServer::ip() const
{
	return ip_;
}

uint16_t TcpServer::port() const
{
	return port_;
}

void TcpServer::addConnection(int sockfd, TcpConnectionPtr conn)
{
	std::lock_guard<std::mutex> locker(mutex_);

	connections_.emplace(sockfd, conn);
}

void TcpServer::removeConnection(int sockfd)
{
	std::lock_guard<std::mutex> locker(mutex_);

	connections_.erase(sockfd);
}

void TcpServer::onNewConnection(int sockfd)
{
	auto conn = std::make_shared<TcpConnection>(
		loop_->getTaskScheduler().get(), sockfd);

	LOG_INFO("new client - IP: %s port: %d",
		conn->ip().c_str(), conn->port());

	if (connectionCallback_) {
		connectionCallback_(conn);
	}

	if (messageCallback_) {
		conn->setReadCallback(messageCallback_);
	}

	if (conn) {
		addConnection(sockfd, conn);
		conn->setDisconnectionCallback([this](TcpConnectionPtr conn) {
			TaskScheduler* scheduler = conn->taskScheduler();
			int sockfd = conn->fd();
			bool ret = scheduler->addTriggerEvent(
				[this, sockfd]() { removeConnection(sockfd); });
			if (!ret) {
				scheduler->addTimer(
					[this, sockfd]() { removeConnection(sockfd); return false; }, 100);
			}
		});
	}
}