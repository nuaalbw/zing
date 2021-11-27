/*************************************************************************
	> File Name: Acceptor.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Sun 21 Nov 2021 07:24:20 PM CST
 ************************************************************************/

#ifndef ZING_NET_ACCEPTOR_H
#define ZING_NET_ACCEPTOR_H

#include "../base/noncopyable.h"
#include "TcpSocket.h"
#include "Channel.h"
#include "EventLoop.h"
#include <functional>
#include <mutex>
#include <memory>

namespace zing
{
namespace net
{

using NewConnectionCallback = std::function<void(int)>;
using TcpSocketPtr = std::unique_ptr<TcpSocket>;

// 用于accept新TCP连接，并通过回调通知使用者
class Acceptor: noncopyable
{
public:
	Acceptor(EventLoop* loop);
	virtual ~Acceptor();

	void setNewConnectionCallback(NewConnectionCallback cb);
	int listen(std::string ip, uint16_t port);
	void close();

private:
	void onAccept();

private:
	EventLoop* loop_;
	std::mutex mutex_;
	TcpSocketPtr acceptSocket_;
	ChannelPtr acceptChannel_;	// 用于观察acceptSocket上的readable事件
	NewConnectionCallback newConnectionCallback_;
};
	
} // namespace net
} // namespace zing

#endif