/*************************************************************************
	> File Name: Date.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Sat 22 Jan 2022 06:25:09 PM CST
 ************************************************************************/

#ifndef ZING_BASE_DATE_H
#define ZING_BASE_DATE_H

#include "copyable.h"
#include "Types.h"

struct tm;

namespace zing
{

class Date : public copyable
{
public:

  struct YearMonthDay
  {
    int year; // [1900..2500]
    int month;  // [1..12]
    int day;  // [1..31]
  };

  static const int kDaysPerWeek = 7;
  static const int kJulianDayOf1970_01_01;

  Date()
    : julianDayNumber_(0)
  {}

  Date(int year, int month, int day);

  explicit Date(int julianDayNum)
    : julianDayNumber_(julianDayNum)
  {}

  explicit Date(const struct tm&);

  void swap(Date& that)
  {
    std::swap(julianDayNumber_, that.julianDayNumber_);
  }

  bool valid() const { return julianDayNumber_ > 0; }

  string toIsoString() const;

  struct YearMonthDay yearMonthDay() const;

  int year() const
  {
    return yearMonthDay().year;
  }

  int month() const
  {
    return yearMonthDay().month;
  }

  int day() const
  {
    return yearMonthDay().day;
  }

  int weekDay() const
  {
    return (julianDayNumber_+1) % kDaysPerWeek;
  }

  int julianDayNumber() const { return julianDayNumber_; }

private:
  int julianDayNumber_;
};

inline bool operator<(Date x, Date y)
{
  return x.julianDayNumber() < y.julianDayNumber();
}

inline bool operator==(Date x, Date y)
{
  return x.julianDayNumber() == y.julianDayNumber();
}

}  // namespace zing

#endif