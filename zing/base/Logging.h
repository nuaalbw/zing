/*************************************************************************
	> File Name: Logging.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Sat 22 Jan 2022 06:07:50 PM CST
 ************************************************************************/

#ifndef ZING_BASE_LOGGING_H
#define ZING_BASE_LOGGING_H

#include "LogStream.h"
#include "Timestamp.h"

#include <functional>

namespace zing
{

class TimeZone;

class Logger
{
public:
	enum LogLevel
	{
		TRACE, 
		DEBUG, 
		INFO, 
		WARN, 
		ERROR, 
		FATAL, 
		NUM_LOG_LEVELS
	};

	// 编译时期计算源文件的名称
	class SourceFile
	{
	public:
		template<int N>
		SourceFile(const char (&arr)[N])
			: data_(arr), 
				size_(N - 1)
		{
			const char* slash = strrchr(data_, '/');
			if (slash)
			{
				data_ = slash + 1;
				size_ -= static_cast<int>(data_ - arr);
			}
		}

		explicit SourceFile(const char* filename)
			: data_(filename)
		{
			const char* slash = strrchr(filename, '/');
			if (slash)
			{
				data_ = slash + 1;
			}
			size_ = static_cast<int>(strlen(data_));
		}

		const char* data_;
		int size_;
	};

	Logger(SourceFile file, int line);
	Logger(SourceFile file, int line, LogLevel level);
	Logger(SourceFile file, int line, LogLevel level, const char* func);
	Logger(SourceFile file, int line, bool toAbort);
	~Logger();

	LogStream& stream() { return impl_.stream_; }

	static LogLevel loglevel();
	static void setLogLevel(LogLevel level);

	using OutputFunc = std::function<void(const char*, int)>;
	using FlushFunc = std::function<void(void)>;
	static void setOutput(OutputFunc);
	static void setFlush(FlushFunc);
	static void setTimeZone(const TimeZone& tz);

private:
	class Impl
	{
	public:
		using LogLevel = Logger::LogLevel;
		Impl(LogLevel level, int old_errno, const SourceFile& file, int line);
		void formatTime();
		void finish();

		Timestamp time_;
		LogStream stream_;
		LogLevel level_;
		int line_;
		SourceFile basename_;
	};

	Impl impl_;
};

extern Logger::LogLevel g_logLevel;

inline Logger::LogLevel Logger::loglevel()
{
	return g_logLevel;
}

#define LOG_TRACE if (zing::Logger::logLevel() <= zing::Logger::TRACE) \
  zing::Logger(__FILE__, __LINE__, zing::Logger::TRACE, __func__).stream()
#define LOG_DEBUG if (zing::Logger::logLevel() <= zing::Logger::DEBUG) \
  zing::Logger(__FILE__, __LINE__, zing::Logger::DEBUG, __func__).stream()
#define LOG_INFO if (zing::Logger::logLevel() <= zing::Logger::INFO) \
  zing::Logger(__FILE__, __LINE__).stream()
#define LOG_WARN zing::Logger(__FILE__, __LINE__, zing::Logger::WARN).stream()
#define LOG_ERROR zing::Logger(__FILE__, __LINE__, zing::Logger::ERROR).stream()
#define LOG_FATAL zing::Logger(__FILE__, __LINE__, zing::Logger::FATAL).stream()
#define LOG_SYSERR zing::Logger(__FILE__, __LINE__, false).stream()
#define LOG_SYSFATAL zing::Logger(__FILE__, __LINE__, true).stream()

const char* strerror_tl(int savedErrno);

// Check that the input is non NULL.  This very useful in constructor
// initializer lists.
#define CHECK_NOTNULL(val) \
  ::zing::CheckNotNull(__FILE__, __LINE__, "'" #val "' Must be non NULL", (val))

// A small helper for CHECK_NOTNULL().
template <typename T>
T* CheckNotNull(Logger::SourceFile file, int line, const char *names, T* ptr)
{
  if (ptr == NULL)
  {
   Logger(file, line, Logger::FATAL).stream() << names;
  }
  return ptr;
}

} // namespace zing

#endif