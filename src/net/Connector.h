/*************************************************************************
	> File Name: Connector.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Tue 07 Dec 2021 10:47:51 AM CST
 ************************************************************************/

#ifndef ZING_NET_CONNECTOR_H
#define ZING_NET_CONNECTOR_H

#include "../base/noncopyable.h"
#include "Channel.h"
#include "EventLoop.h"
#include <functional>
#include <memory>
#include <mutex>

namespace zing
{
namespace net
{

class Connector: noncopyable, 
				 public std::enable_shared_from_this<Connector>
{
public:
	using NewConnectionCallback = std::function<void(int)>;	// sockfd

	Connector(EventLoop* loop, std::string ip, uint16_t port);
	~Connector();

	void setNewConnectionCallback(const NewConnectionCallback& cb)
	{ newConnectionCallback_ = cb; }

	void start();	// can be called in any thread
	void restart();
	void stop();

	std::string ip() const { return ip_; }
	uint16_t port() const { return port_; }

private:
	enum States { kDisconnected, kConnecting, kConnected };

	void setState(States s) { state_ = s; }
	bool connect();
	void connecting(int sockfd);
	void handleWrite();
	void handleError();
	void retry(int sockfd);
	int removeAndResetChannel();

private:
	static const int kMaxRetryDelayMs = 30 * 1000;	// 最大重连时长
	static const int kInitRetryDelayMs = 500;		// 初始重连时长

	EventLoop* loop_;
	std::string ip_;
	uint16_t port_;
	bool connect_;
	States state_;
	ChannelPtr channel_;
	NewConnectionCallback newConnectionCallback_;
	int retryDelayMs_;
	std::mutex mutex_;
};
	
} // namespace net
} // namespace zing

#endif