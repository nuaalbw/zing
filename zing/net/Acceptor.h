/*************************************************************************
	> File Name: Acceptor.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Tue 15 Feb 2022 02:01:01 PM CST
 ************************************************************************/

#ifndef ZING_NET_ACCEPTOR_H
#define ZING_NET_ACCEPTOR_H

#include "Channel.h"
#include "Socket.h"

#include <functional>

namespace zing
{
namespace net
{

class EventLoop;
class InetAddress;

// 用于接受一个新的TCP链接，并通过回调通知使用者
class Acceptor: noncopyable
{
public:
	using NewConnectionCallback = std::function<void(int, const InetAddress&)>;

	Acceptor(EventLoop* loop, const InetAddress& listenAddr, bool reuseport);
	~Acceptor();

	void setNewConnectionCallback(const NewConnectionCallback& cb)
	{ newConnectionCallback_ = cb; }

	void listen();

	bool listening() const { return listening_; }

private:
	void handleRead();

	EventLoop* loop_;
	Socket acceptSocket_;
	Channel acceptChannel_;
	NewConnectionCallback newConnectionCallback_;
	bool listening_;
	int idleFd_;
};
	
} // namespace net
} // namespace zing

#endif