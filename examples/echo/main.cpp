/*************************************************************************
	> File Name: main.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Tue 22 Feb 2022 03:00:28 PM CST
 ************************************************************************/

#include "examples/echo/echo.h"
#include "zing/base/Logging.h"
#include "zing/net/EventLoop.h"
#include <unistd.h>

using namespace zing;
using namespace zing::net;

int main()
{
	LOG_INFO << "pid = " << getpid();
	EventLoop loop;
	InetAddress listenAddr(8848);
	EchoServer server(&loop, listenAddr);
	server.start();
	loop.loop();
}