/*************************************************************************
	> File Name: Channel_test.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Mon 07 Feb 2022 10:54:04 AM CST
 ************************************************************************/

#include <sys/timerfd.h>
#include "../EventLoop.h"
#include "../Channel.h"

using namespace zing;
using namespace zing::net;

EventLoop* g_loop;

void timeout(Timestamp stamp)
{
	printf("Timeout! time = %s\n", stamp.toFormattedString().c_str());
	g_loop->quit();
}

int main()
{
	EventLoop loop;
	g_loop = &loop;

	int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
	Channel channel(&loop, timerfd);
	channel.setReadCallback(timeout);
	channel.enableReading();

	struct itimerspec howlong;
	bzero(&howlong, sizeof(howlong));
	howlong.it_value.tv_sec = 5;
	::timerfd_settime(timerfd, 0, &howlong, nullptr);

	loop.loop();

	channel.disableAll();
	channel.remove();
	::close(timerfd);
}