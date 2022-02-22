/*************************************************************************
	> File Name: echo.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Tue 22 Feb 2022 02:50:11 PM CST
 ************************************************************************/

#include "examples/echo/echo.h"
#include "zing/base/Logging.h"

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

using namespace zing;
using namespace zing::net;

EchoServer::EchoServer(EventLoop* loop, 
											 const InetAddress& listenAddr)
	: server_(loop, listenAddr, "EchoServer")
{
	server_.setConnectionCallback(
		std::bind(&EchoServer::onConnection, this, _1));
	server_.setMessageCallback(
		std::bind(&EchoServer::onMessage, this, _1, _2, _3));
}

void EchoServer::start()
{
	server_.start();
}

void EchoServer::onConnection(const TcpConnectionPtr& conn)
{
	LOG_INFO << "EchoServer - " << conn->peerAddress().toIpPort() << " -> "
					 << conn->localAddress().toIpPort() << " is "
					 << (conn->connected() ? "UP" : "DOWN");
}

void EchoServer::onMessage(const TcpConnectionPtr& conn, 
													 Buffer* buf, 
													 Timestamp time)
{
	string msg(buf->retrieveAllAsString());
	LOG_INFO << conn->name() << " echo " << msg.size() << " bytes, "
					 << "data received at " << time.toString();
	conn->send(msg);
}