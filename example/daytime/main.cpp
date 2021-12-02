/*************************************************************************
	> File Name: main.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Thu 02 Dec 2021 11:04:36 AM CST
 ************************************************************************/

#include "daytime.h"

using namespace zing;
using namespace zing::net;

int main()
{
	EventLoop loop;
	DaytimeServer server(&loop);
	server.start("127.0.0.1", 12580);
	loop.loop();
}