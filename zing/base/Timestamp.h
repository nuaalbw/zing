/*************************************************************************
	> File Name: Timestamp.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Sat 15 Jan 2022 02:20:39 PM CST
 ************************************************************************/

#ifndef ZING_BASE_TIMESTAMP_H
#define ZING_BASE_TIMESTAMP_H

#include "copyable.h"
#include "Types.h"

namespace zing
{

class Timestamp : public zing::copyable
{
 public:
  Timestamp()
    : microSecondsSinceEpoch_(0)
  {
  }

  explicit Timestamp(int64_t microSecondsSinceEpochArg)
    : microSecondsSinceEpoch_(microSecondsSinceEpochArg)
  {
  }

  void swap(Timestamp& that)
  {
    std::swap(microSecondsSinceEpoch_, that.microSecondsSinceEpoch_);
  }

  string toString() const;
  string toFormattedString(bool showMicroseconds = true) const;

  bool valid() const { return microSecondsSinceEpoch_ > 0; }

  // for internal usage.
  int64_t microSecondsSinceEpoch() const { return microSecondsSinceEpoch_; }
  time_t secondsSinceEpoch() const
  { return static_cast<time_t>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond); }

  static Timestamp now();
  static Timestamp invalid()
  {
    return Timestamp();
  }

  static Timestamp fromUnixTime(time_t t)
  {
    return fromUnixTime(t, 0);
  }

  static Timestamp fromUnixTime(time_t t, int microseconds)
  {
    return Timestamp(static_cast<int64_t>(t) * kMicroSecondsPerSecond + microseconds);
  }

  static const int kMicroSecondsPerSecond = 1000 * 1000;

 private:
  int64_t microSecondsSinceEpoch_;
};

inline bool operator<(Timestamp lhs, Timestamp rhs)
{
  return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
}

inline bool operator==(Timestamp lhs, Timestamp rhs)
{
  return lhs.microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
}

inline bool operator!=(Timestamp lhs, Timestamp rhs)
{
  return lhs.microSecondsSinceEpoch() != rhs.microSecondsSinceEpoch();
}

inline bool operator<=(Timestamp lhs, Timestamp rhs)
{
  return lhs.microSecondsSinceEpoch() <= rhs.microSecondsSinceEpoch();
}

inline bool operator>(Timestamp lhs, Timestamp rhs)
{
  return lhs.microSecondsSinceEpoch() > rhs.microSecondsSinceEpoch();
}

inline bool operator>=(Timestamp lhs, Timestamp rhs)
{
  return lhs.microSecondsSinceEpoch() >= rhs.microSecondsSinceEpoch();
}

inline double timeDifference(Timestamp high, Timestamp low)
{
  int64_t diff = high.microSecondsSinceEpoch() - low.microSecondsSinceEpoch();
  return static_cast<double>(diff) / Timestamp::kMicroSecondsPerSecond;
}

inline Timestamp addTime(Timestamp timestamp, double seconds)
{
  int64_t delta = static_cast<int64_t>(seconds * Timestamp::kMicroSecondsPerSecond);
  return Timestamp(timestamp.microSecondsSinceEpoch() + delta);
}

}  // namespace zing

#endif  