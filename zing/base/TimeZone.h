/*************************************************************************
	> File Name: TimeZone.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Sat 22 Jan 2022 06:10:21 PM CST
 ************************************************************************/

#ifndef ZING_BASE_TIMEZONE_H
#define ZING_BASE_TIMEZONE_H

#include "copyable.h"
#include <memory>
#include <time.h>

namespace zing
{

// TimeZone for 1970 ~ 2030
class TimeZone: public copyable
{
public:
	explicit TimeZone(const char* zonefile);
	TimeZone(int eastOfUtc, const char* tzname);	// a fixed timezone
	TimeZone() = default;	// an invalid timezone

	bool valid() const;

	struct tm toLocalTime(time_t secondsSinceEpoch) const;
	time_t fromLocalTime(const struct tm&) const;

	// gmtime(3)
	static struct tm toUtcTime(time_t secondsSinceEpoch, bool yday = false);

	// timegm(3)
	static time_t fromUtcTime(const struct tm&);

	static time_t fromUtcTime(int year, int month, int day, 
														int hour, int minute, int seconds);
	struct Data;

private:
	std::shared_ptr<Data> data_;
};
	
} // namespace zing

#endif