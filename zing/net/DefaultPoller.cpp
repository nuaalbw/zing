/*************************************************************************
	> File Name: DefaultPoller.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Mon 07 Feb 2022 12:35:46 AM CST
 ************************************************************************/

#include "Poller.h"
#include "PollPoller.h"
#include "EPollPoller.h"

#include <stdlib.h>

using namespace zing::net;

Poller* Poller::newDefaultPoller(EventLoop* loop)
{
	if (::getenv("ZING_USE_POLL"))
	{
		return new PollPoller(loop);
	}
	else
	{
		return new EPollPoller(loop);
	}
}