/*************************************************************************
	> File Name: TcpServer.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Tue 30 Nov 2021 07:56:58 PM CST
 ************************************************************************/

#ifndef ZING_NET_TCP_SERVER_H
#define ZING_NET_TCP_SERVER_H

#include "../base/noncopyable.h"
#include "EventLoop.h"
#include "TcpConnection.h"
#include "Acceptor.h"
#include <string>

namespace zing
{
namespace net
{

using TcpConnectionPtr = std::shared_ptr<TcpConnection>;

class TcpServer: noncopyable
{
	using AcceptorPtr = std::unique_ptr<Acceptor>;

public:
	explicit TcpServer(EventLoop* loop);
	virtual ~TcpServer();

	virtual bool start(std::string ip, uint16_t port);
	virtual void stop();

	std::string ip() const;
	uint16_t port() const;

protected:
	virtual TcpConnectionPtr onConnection(int sockfd);
	virtual void addConnection(int sockfd, TcpConnectionPtr conn);
	virtual void removeConnection(int sockfd);

protected:
	EventLoop* loop_;
	uint16_t port_;
	std::string ip_;
	AcceptorPtr acceptor_;
	bool started_;
	std::mutex mutex_;
	std::unordered_map<int, TcpConnectionPtr> connections_;
};
	
} // namespace net
} // namespace zing

#endif