/*************************************************************************
	> File Name: echo.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Wed 01 Dec 2021 10:33:31 PM CST
 ************************************************************************/

#include "echo.h"
#include "Logger.h"

using namespace zing;
using namespace zing::net;
using namespace zing::base;

using std::placeholders::_1;
using std::placeholders::_2;

EchoServer::EchoServer(zing::net::EventLoop* loop)
	: server_(loop)
{
	server_.setConnectionCallback(
		std::bind(&EchoServer::onConnection, this, _1));

	server_.setMessageCallback(
		std::bind(&EchoServer::onMessage, this, _1, _2));
}

void EchoServer::start(std::string ip, uint16_t port)
{
	server_.start(ip, port);
}

void EchoServer::onConnection(zing::net::TcpConnectionPtr conn)
{
	LOG_INFO("EchoServer - ip<%s> port<%d>", conn->ip().c_str(), conn->port());
}

bool EchoServer::onMessage(zing::net::TcpConnectionPtr conn,
                   zing::net::ReadBuffer& buffer)
{
	std::string res(buffer.peek(), buffer.readableBytes());
	buffer.retrieveAll();
	conn->send(res.c_str(), res.size());

	return true;
}

