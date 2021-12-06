/*************************************************************************
	> File Name: SocketUtil.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Sun 21 Nov 2021 01:18:01 PM CST
 ************************************************************************/

#include "SocketUtil.h"

using namespace zing;
using namespace zing::net;

bool SocketUtil::bind(int sockfd, std::string ip, uint16_t port)
{
	struct sockaddr_in addr;
	::bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = ::htons(port);
	int ret = ::inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);
	if (ret != 1) {
		return false;
	}
	ret = ::bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));	
	if (ret == -1) {
		return false;
	}
	return true;	
}

void SocketUtil::setNonblock(int sockfd)
{
	int oldOption = ::fcntl(sockfd, F_GETFL, 0);
	::fcntl(sockfd, F_SETFL, oldOption | O_NONBLOCK);
}

void SocketUtil::setBlock(int sockfd, int writeTimeout)
{
	int oldOption = ::fcntl(sockfd, F_GETFL, 0);
	::fcntl(sockfd, F_SETFL, oldOption & (~O_NONBLOCK));

	if (writeTimeout > 0) {
		struct timeval tv = { 
			writeTimeout / 1000, (writeTimeout % 1000) * 1000 };
		::setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (char*)&tv, sizeof(tv));
	}
}

void SocketUtil::setReuseAddr(int sockfd)
{
	int on = 1;
	::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&on, sizeof(on));
}

void SocketUtil::setReusePort(int sockfd)
{
	int on = 1;
	::setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, (const char*)&on, sizeof(on));
}

void SocketUtil::setKeepAlive(int sockfd)
{
	int on = 1;
	::setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, (char*)&on, sizeof(on));
}

void SocketUtil::setSendBufSize(int sockfd, int size)
{
	::setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, (char*)&size, sizeof(size));
}

void SocketUtil::setRecvBufSize(int sockfd, int size)
{
	::setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, (char*)&size, sizeof(size));
}

std::string SocketUtil::getPeerIp(int sockfd)
{
	struct sockaddr_in addr;
	::bzero(&addr, sizeof(addr));
	socklen_t addrLen = sizeof(addr);
	int ret = ::getpeername(sockfd, (struct sockaddr*)&addr, &addrLen);
	if (ret == 0) {
		char ip[48] = { 0 };
		::inet_ntop(AF_INET, &addr.sin_addr, ip, addrLen);
		return ip;
	}
	return "0.0.0.0";
}

std::string SocketUtil::getSocketIp(int sockfd)
{
	struct sockaddr_in addr;
	::bzero(&addr, sizeof(addr));
	char str[INET_ADDRSTRLEN] = "127.0.0.1";
	int ret = getSocketAddr(sockfd, &addr);
	if (ret == 0) {
		::inet_ntop(AF_INET, &addr.sin_addr, str, sizeof(str));
	}
	return str;
}

int SocketUtil::getSocketAddr(int sockfd, struct sockaddr_in* addr)
{
	socklen_t addrLen = sizeof(struct sockaddr_in);
	return ::getsockname(sockfd, (struct sockaddr*)addr, &addrLen);
}

uint16_t SocketUtil::getPeerPort(int sockfd)
{
	struct sockaddr_in addr;
	::bzero(&addr, sizeof(addr));
	socklen_t addrLen = sizeof(struct sockaddr_in);
	int ret = ::getpeername(sockfd, (struct sockaddr*)&addr, &addrLen);
	if (ret == 0) {
		return ::ntohs(addr.sin_port);
	}
	return -1;
}

int SocketUtil::getPeerAddr(int sockfd, struct sockaddr_in* addr)
{
	socklen_t addrLen = sizeof(struct sockaddr_in);
	return getpeername(sockfd, (struct sockaddr*)addr, &addrLen);
}

void SocketUtil::close(int sockfd)
{
	::close(sockfd);
}

bool SocketUtil::connect(int sockfd, std::string ip, uint16_t port, int timeout)
{
	bool connected = true;

	if (timeout > 0) {
		SocketUtil::setNonblock(sockfd);	// 非阻塞connect
	}

	struct sockaddr_in addr;
	::bzero(&addr, sizeof(addr));
	socklen_t addrLen = sizeof(addr);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip.c_str());

	int ret = ::connect(sockfd, (struct sockaddr*)&addr, addrLen);
	if (ret == -1) {
		if (timeout > 0) {
			connected = false;
			fd_set fd_write;
			FD_ZERO(&fd_write);
			FD_SET(sockfd, &fd_write);
			struct timeval tv = { timeout / 1000, timeout % 1000 * 1000 };
			select(sockfd + 1, nullptr, &fd_write, nullptr, &tv);
			if (FD_ISSET(sockfd, &fd_write)) {
				connected = true;
			}
			SocketUtil::setBlock(sockfd);
		}
		else {
			connected = false;
		}
	}

	return connected;
}

uint64_t SocketUtil::hostToNetwork64(uint64_t host64)
{
	return htobe64(host64);
}

uint32_t SocketUtil::hostToNetwork32(uint32_t host32)
{
	return htole32(host32);
}

uint16_t SocketUtil::hostToNetwork16(uint16_t host16)
{
	return htobe16(host16);
}

uint64_t SocketUtil::networkToHost64(uint64_t net64)
{
	return be64toh(net64);
}

uint32_t SocketUtil::networkToHost32(uint32_t net32)
{
	return be32toh(net32);
}

uint16_t SocketUtil::networkToHost16(uint16_t net16)
{
	return be16toh(net16);
}