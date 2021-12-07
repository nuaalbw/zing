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
using ConnectionCallback = std::function<void(TcpConnectionPtr&)>;
using DisconnectionCallback = std::function<void(TcpConnectionPtr&)>;
using MessageCallback = std::function<
		bool(TcpConnectionPtr, ReadBuffer& buffer)>;

class TcpServer: noncopyable
{
	using AcceptorPtr = std::unique_ptr<Acceptor>;

public:
	explicit TcpServer(EventLoop* loop);
	virtual ~TcpServer();

	bool start(std::string ip, uint16_t port);
	void stop();

	void setConnectionCallback(const ConnectionCallback& cb);
	void setDisconnectionCallback(const DisconnectionCallback& cb);
	void setMessageCallback(const MessageCallback& cb);

	std::string ip() const;
	uint16_t port() const;

private:
	void addConnection(int sockfd, TcpConnectionPtr conn);
	void removeConnection(int sockfd);
	void onNewConnection(int sockfd);

private:
	EventLoop* loop_;
	uint16_t port_;
	std::string ip_;
	AcceptorPtr acceptor_;
	bool started_;
	std::mutex mutex_;
	std::unordered_map<int, TcpConnectionPtr> connections_;
	ConnectionCallback connectionCallback_;
	MessageCallback messageCallback_;
	DisconnectionCallback disconnectionCallback_;
};
	
} // namespace net
} // namespace zing

#endif