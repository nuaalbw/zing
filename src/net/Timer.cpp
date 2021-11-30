/*************************************************************************
	> File Name: Timer.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Fri 19 Nov 2021 09:46:20 PM CST
 ************************************************************************/

#include "Timer.h"
#include <chrono>
#include <cstdint>
#include <thread>

using namespace zing;
using namespace zing::net;
using namespace std::chrono;
using std::this_thread::sleep_for;

Timer::Timer(TimerCallback cb, uint32_t msec)
			: repeat_(false), 
			  callback_(std::move(cb)), 
			  interval_(msec == 0 ? 1 : msec), 
			  nextTimeout_(0)
{
}

void Timer::sleep(uint32_t msec)
{
	sleep_for(milliseconds(msec));
}

void Timer::setTimerCallback(TimerCallback cb)
{
	callback_ = std::move(cb);
}

void Timer::start(int64_t microsecs, bool repeat)
{
	repeat_ = repeat;
	auto timeBegin = high_resolution_clock::now();
	int64_t elapsed = 0;

	do
	{
		sleep_for(microseconds(microsecs - elapsed));
		timeBegin = high_resolution_clock::now();
		if (callback_) {
			callback_();
		}
		elapsed = duration_cast<microseconds>(
			high_resolution_clock::now() - timeBegin).count();

		if (elapsed < 0) {
			elapsed = 0;
		}

	} while (repeat_);
}

void Timer::stop()
{
	repeat_ = false;
}

void Timer::setNextTimeout(int64_t timePoint)
{
	nextTimeout_ = timePoint + interval_;
}

int64_t Timer::getNextTimeout() const
{
	return nextTimeout_;
}