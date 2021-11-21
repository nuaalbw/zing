/*************************************************************************
	> File Name: SocketUtil.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Sun 21 Nov 2021 01:17:58 PM CST
 ************************************************************************/

#ifndef ZING_NET_SOCKET_UTIL_H
#define ZING_NET_SOCKET_UTIL_H

#include "SocketFiles.h"
#include "../base/noncopyable.h"
#include <string>

namespace zing
{
namespace net
{

class SocketUtil: noncopyable
{
public:
	// 绑定地址结构
	static bool bind(int sockfd, std::string ip, uint16_t port);
	// 设置非阻塞
	static void setNonblock(int sockfd);
	// 设置阻塞和写超时时间(毫秒)
	static void setBlock(int sockfd, int writeTimeout = 0);
	// 设置地址复用
	static void setReuseAddr(int sockfd);
	static void setReusePort(int sockfd);
	// 设置长连接
	static void setKeepAlive(int sockfd);
	// 设置TCP发送缓冲区大小
	static void setSendBufSize(int sockfd, int size);
	// 设置TCP接收缓冲区大小
	static void setRecvBufSize(int sockfd, int size);
	// 获取对端连接的IP地址
	static std::string getPeerIp(int sockfd);
	// 获取己端的IP地址
	static std::string getSocketIp(int sockfd);
	// 获取sockfd绑定的地址结构
	static int getSocketAddr(int sockfd, struct sockaddr_in* addr);
	// 获取对端连接的端口号
	static uint16_t getPeerPort(int sockfd);
	// 获取对端连接的地址结构
	static int getPeerAddr(int sockfd, struct sockaddr_in* addr);
	// 关闭连接
	static void close(int sockfd);
	// 发起连接
	static bool connect(int sockfd, std::string ip, uint16_t port, int timeout = 0);
};

} // namespace net
} // namespace zing

#endif