/*************************************************************************
	> File Name: TimerId.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Mon 07 Feb 2022 11:37:25 AM CST
 ************************************************************************/

#ifndef ZING_NET_TIMERID_H
#define ZING_NET_TIMERID_H

#include "../base/copyable.h"
#include "../base/Types.h"

namespace zing
{
namespace net
{

class Timer;

class TimerId: public copyable
{
public:
	TimerId()
		: timer_(nullptr), 
			sequence_(0)
	{
	}

	TimerId(Timer* timer, int64_t seq)
		: timer_(timer), 
			sequence_(seq)
	{
	}

	friend class TimerQueue;

private:
	Timer* timer_;
	int64_t sequence_;
};
	
} // namespace net
} // namespace zing

#endif