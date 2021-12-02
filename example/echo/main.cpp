/*************************************************************************
	> File Name: main.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Wed 01 Dec 2021 10:57:18 PM CST
 ************************************************************************/

#include "echo.h"

using namespace zing;
using namespace zing::net;

int main()
{
	EventLoop loop;
	EchoServer server(&loop);
	server.start("127.0.0.1", 12345);
	loop.loop();
	return 0;
}