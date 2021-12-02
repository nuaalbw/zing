/*************************************************************************
	> File Name: daytime.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Thu 02 Dec 2021 10:39:24 AM CST
 ************************************************************************/

#include "daytime.h"
#include "EventLoop.h"
#include "Timestamp.h"
#include "Logger.h"

using namespace zing;
using namespace zing::net;
using namespace zing::base;
using std::placeholders::_1;
using std::placeholders::_2;

DaytimeServer::DaytimeServer(zing::net::EventLoop* loop)
	: server_(loop)
{
	server_.setConnectionCallback(
		std::bind(&DaytimeServer::onConnection, this, _1));
	server_.setMessageCallback(
		std::bind(&DaytimeServer::onMessage, this, _1, _2));
}

void DaytimeServer::start(std::string ip, uint16_t port)
{
	server_.start(ip, port);
}

void DaytimeServer::onConnection(zing::net::TcpConnectionPtr conn)
{
	conn->send(Timestamp::now().toFormattedString() + "\n");
	conn->disconnect();
}

bool DaytimeServer::onMessage(zing::net::TcpConnectionPtr conn,
                   			  zing::net::ReadBuffer& buffer)
{
	std::string msg(buffer.peek(), buffer.readableBytes());
	buffer.retrieveAll();
	LOG_INFO("Daytime server discards %u bytes", msg.size());	
	return true;
}