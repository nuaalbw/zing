/*************************************************************************
	> File Name: Timer.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Wed 09 Feb 2022 03:45:57 PM CST
 ************************************************************************/

#include "Timer.h"

using namespace zing;
using namespace zing::net;

AtomicInt64 Timer::s_numCreated_;

void Timer::restart(Timestamp now)
{
	if (repeat_)
	{
		expiration_ = addTime(now, interval_);
	}
	else
	{
		expiration_ = Timestamp::invalid();
	}
}