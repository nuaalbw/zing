/*************************************************************************
	> File Name: TcpSocket.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Sat 20 Nov 2021 10:30:29 PM CST
 ************************************************************************/

#ifndef ZING_NET_TCP_SOCKET_H
#define ZING_NET_TCP_SOCKET_H

#include "../base/noncopyable.h"
#include "SocketFiles.h"
#include <string>

namespace zing
{
namespace net
{

class TcpSocket: noncopyable
{
public:
	explicit TcpSocket(int sockfd = -1);
	virtual ~TcpSocket();

	// 创建一个Socket文件描述符
	int  create();
	// 绑定地址结构
	bool bind(std::string ip, uint16_t port);
	// 设置监听
	bool listen(int backlog);
	// 接受连接
	int  accept();
	// 发起连接
	bool connect(std::string ip, uint16_t port, int timeout = 0);
	// 关闭连接
	void close();
	// 关闭写端（半关闭）
	void shutdownWrite();
	// 获取socket fd
	int  getSocket() const;

private:
	int sockfd_;
};
	
} // namespace net
} // namespace zing

#endif
