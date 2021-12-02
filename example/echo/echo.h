/*************************************************************************
	> File Name: echo.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Wed 01 Dec 2021 10:17:38 PM CST
 ************************************************************************/

#ifndef ECHO_H
#define ECHO_H

#include "EventLoop.h"
#include "TcpServer.h"

class EchoServer
{
public:
	EchoServer(zing::net::EventLoop* loop);
	void start(std::string ip, uint16_t port);

private:
	void onConnection(zing::net::TcpConnectionPtr conn);

	bool onMessage(zing::net::TcpConnectionPtr conn,
                   zing::net::ReadBuffer& buffer);

private:
	zing::net::TcpServer server_;
};

#endif
