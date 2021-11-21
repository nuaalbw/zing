/*************************************************************************
	> File Name: Timestamp.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Sun 14 Nov 2021 10:13:26 PM CST
 ************************************************************************/

#ifndef ZING_BASE_TIMESTAMP_H
#define ZING_BASE_TIMESTAMP_H

#include <stdint.h>
#include <string>

namespace zing
{

class Timestamp		// 值语义，可以拷贝
{
public:
	/* 默认构造函数，创造一个无效的Timestamp */
	Timestamp();
	/* 根据给定的时间创造一个Timestamp */
	explicit Timestamp(int64_t microSecondsSinceEpoch);

	/* 与其他Timestamp交换 */
	void swap(Timestamp& that);

	/* 格式化为字符串 */
	std::string toString() const;
	std::string toFormattedString(bool showMicroSeconds = true) const;

	/* 判定是否有效 */
	bool valid() const;

	/* 获取时间 */
	int64_t microSecondsSinceEpoch() const;
	time_t secondsSinceEpoch() const;

	/* 获取时间戳与当前时间的差值 */
	double elapsed() const;

	/* 获取现在的时间 */
	static Timestamp now();
	static Timestamp invalid();
	static Timestamp fromUnixTime(time_t t);
	static Timestamp fromUnixTime(time_t t, int microSeconds);

	/* 操作符重载 */
	bool operator<(Timestamp that);
	bool operator==(Timestamp that);
	bool operator>(Timestamp that);

	static constexpr int kMicroSecondsPerSecond = 1000 * 1000;
	
private:
	int64_t microSecondsSinceEpoch_;
};

/* 获取两个时间戳的差值，返回秒数 */
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

};

#endif