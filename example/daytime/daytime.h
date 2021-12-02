/*************************************************************************
	> File Name: daytime.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Thu 02 Dec 2021 10:31:25 AM CST
 ************************************************************************/

#ifndef ZING_EXAMPLES_DAYTIME_H
#define ZING_EXAMPLES_DAYTIME_H

#include "TcpServer.h"

class DaytimeServer
{
public:
	DaytimeServer(zing::net::EventLoop* loop);

	void start(std::string ip, uint16_t port);

private:
	void onConnection(zing::net::TcpConnectionPtr conn);
	bool onMessage(zing::net::TcpConnectionPtr conn,
                   zing::net::ReadBuffer& buffer);

private:
	zing::net::TcpServer server_;
};

#endif