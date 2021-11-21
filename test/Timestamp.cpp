/*************************************************************************
	> File Name: Timestamp.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Mon 15 Nov 2021 03:23:08 PM CST
 ************************************************************************/

#include "Timestamp.h"

#include <sys/time.h>
#include <stdio.h>
#include <inttypes.h>

using namespace zing;


Timestamp::Timestamp(): microSecondsSinceEpoch_(0)
{
}

Timestamp::Timestamp(int64_t microSecondsSinceEpoch)
: microSecondsSinceEpoch_(microSecondsSinceEpoch)
{
}

void Timestamp::swap(Timestamp& that)
{
	std::swap(microSecondsSinceEpoch_, that.microSecondsSinceEpoch_);
}

std::string Timestamp::toString() const
{
	char buf[32] = { 0 };
	int64_t seconds = microSecondsSinceEpoch_ / kMicroSecondsPerSecond;
	int64_t microSeconds = microSecondsSinceEpoch_ % kMicroSecondsPerSecond;
	snprintf(buf, sizeof(buf), "%" PRId64 ".%06" PRId64 "", seconds, microSeconds);
	return buf;
}

std::string Timestamp::toFormattedString(bool showMicroSeconds) const
{
	char buf[64] = { 0 };
	time_t seconds = static_cast<time_t>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond);
	struct tm tm_time;
	gmtime_r(&seconds, &tm_time);
	
	if (showMicroSeconds) {
		int microSeconds = static_cast<int>(microSecondsSinceEpoch_ % kMicroSecondsPerSecond);
		snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%06d", 
				 tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday, 
				 tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec, 
				 microSeconds);
	}
	else {
    	snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d",
             	 tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
             	 tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
	}
	return buf;
}

bool Timestamp::valid() const
{
	return microSecondsSinceEpoch_ > 0;
}

int64_t Timestamp::microSecondsSinceEpoch() const
{
	return microSecondsSinceEpoch_;
}

time_t Timestamp::secondsSinceEpoch() const
{
	return static_cast<time_t>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond);
}

double Timestamp::elapsed() const
{
	int64_t diff = now().microSecondsSinceEpoch_ - microSecondsSinceEpoch_;
	return static_cast<double>(diff) / Timestamp::kMicroSecondsPerSecond;
}

Timestamp Timestamp::now()
{
	struct timeval tv;
	gettimeofday(&tv, nullptr);
	int64_t seconds = tv.tv_sec;
	return Timestamp(seconds * kMicroSecondsPerSecond + tv.tv_usec);
}

Timestamp Timestamp::invalid()
{
	return Timestamp();
}

Timestamp Timestamp::fromUnixTime(time_t t)
{
	return Timestamp::fromUnixTime(t, 0);
}

Timestamp Timestamp::fromUnixTime(time_t t, int microSeconds)
{
	return Timestamp(static_cast<int64_t>(t) * kMicroSecondsPerSecond + microSeconds);
}

bool Timestamp::operator<(Timestamp that)
{
	return microSecondsSinceEpoch_ < that.microSecondsSinceEpoch_;
}

bool Timestamp::operator==(Timestamp that)
{
	return microSecondsSinceEpoch_ == that.microSecondsSinceEpoch_;
}

bool Timestamp::operator>(Timestamp that)
{
	return microSecondsSinceEpoch_ > that.microSecondsSinceEpoch_;
}