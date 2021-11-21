/*************************************************************************
	> File Name: TcpSocket.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Sat 20 Nov 2021 10:44:46 PM CST
 ************************************************************************/

#include "TcpSocket.h"
#include "SocketUtil.h"
#include "../base/Logger.h"

using namespace zing;
using namespace zing::net;
using namespace zing::base;

TcpSocket::TcpSocket(int sockfd)
	: sockfd_(sockfd)
{
}

TcpSocket::~TcpSocket()
{
}

int TcpSocket::create()
{
	sockfd_ = ::socket(AF_INET, SOCK_STREAM, 0);
	return sockfd_;
}

bool TcpSocket::bind(std::string ip, uint16_t port)
{
	struct sockaddr_in addr;
	::bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = ::htons(port);
	int ret = ::inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);
	if (ret != 1) {
		LOG_WARNING("bad IP address <%s>", ip.c_str());
		return false;
	}
	ret = ::bind(sockfd_, (struct sockaddr*)&addr, sizeof(addr));	
	if (ret == -1) {
		LOG_WARNING("socket %d bind <%s:%u> failed",
			 sockfd_, ip.c_str(), port);
		return false;
	}
	return true;
}

bool TcpSocket::listen(int backlog)
{
	int ret = ::listen(sockfd_, backlog);
	if (ret == -1) {
		LOG_WARNING("socket %d listen failed", sockfd_);
		return false;
	}
	return true;
}

int TcpSocket::accept()
{
	struct sockaddr_in addr;
	::bzero(&addr, sizeof(addr));
	socklen_t addrLen = sizeof(addr);
	int connfd = ::accept(sockfd_, (struct sockaddr*)&addr, &addrLen);
	return connfd;
}

bool TcpSocket::connect(std::string ip, uint16_t port, int timeout)
{
	bool ret = SocketUtil::connect(sockfd_, ip, port, timeout);
	if (ret == false) {
		LOG_WARNING("sockfd %d connect failed", sockfd_);
		return false;
	}
	return true;
}

void TcpSocket::close()
{
	::close(sockfd_);
	sockfd_ = -1;
}

void TcpSocket::shutdownWrite()
{
	::shutdown(sockfd_, SHUT_WR);
	sockfd_ = -1;
}

int TcpSocket::getSocket() const
{
	return sockfd_;
}