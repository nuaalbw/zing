/*************************************************************************
	> File Name: echo.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Tue 22 Feb 2022 02:44:24 PM CST
 ************************************************************************/

#ifndef ZING_EXAMPLES_ECHO_ECHO_H
#define ZING_EXAMPLES_ECHO_ECHO_H

#include "zing/net/TcpServer.h"

class EchoServer
{
public:
	EchoServer(zing::net::EventLoop* loop, 
						 const zing::net::InetAddress& listenAddr);
	
	void start();

private:
	void onConnection(const zing::net::TcpConnectionPtr& conn);

	void onMessage(const zing::net::TcpConnectionPtr& conn, 
								 zing::net::Buffer* buf, 
								 zing::Timestamp time);

	zing::net::TcpServer server_;
};

#endif