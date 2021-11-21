/*************************************************************************
	> File Name: Logger.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Sat 20 Nov 2021 03:15:47 PM CST
 ************************************************************************/

#include "Logger.h"
#include "Timestamp.h"
#include <stdarg.h>
#include <iostream>

using namespace zing;
using namespace zing::base;

Priority Logger::s_level_ = LOG_INFO;

static const char* priorityToString[] = 
{
	"DEBUG", 
	"CONFIG", 
	"INFO", 
	"WARN", 
	"ERROR"
};

Logger& Logger::instance()
{
	static Logger s_logger;
	return s_logger;
}

void Logger::setLogLevel(Priority level)
{
	std::lock_guard<std::mutex> locker(mutex_);

	Logger::s_level_ = level;
}

Priority Logger::logLevel()
{
	std::lock_guard<std::mutex> locker(mutex_);

	return Logger::s_level_;
}

void Logger::init(const char* pathname)
{
	std::unique_lock<std::mutex> locker(mutex_);

	if (pathname != nullptr) {
		ofs_.open(pathname, std::ios::out | std::ios::binary);
		if (ofs_.fail()) {
			std::cerr << "Failed to open logfile." << std::endl;
		}
	}
}

void Logger::exit()
{
	std::unique_lock<std::mutex> locker(mutex_);
	if (ofs_.is_open()) {
		ofs_.close();
	}
}

void Logger::log(Priority level, const char* file, const char* func, 
	 int line, const char* fmt, ...)
{
	std::unique_lock<std::mutex> locker(mutex_);

	char buf[2048] = { 0 };
	int n = snprintf(buf, sizeof(buf), "[%s] %s:%s:%d ",
					 priorityToString[level], file, func, line);
	va_list args;
	va_start(args, fmt);
	vsnprintf(buf + n, sizeof(buf) - n, fmt, args);
	va_end(args);
	write(buf);
}

void Logger::log(Priority level, const char* fmt, ...)
{
	std::unique_lock<std::mutex> locker(mutex_);

	char buf[4096] = { 0 };
	int n = snprintf(buf, sizeof(buf), priorityToString[level]);
	va_list args;
	va_start(args, fmt);
	vsnprintf(buf + n, sizeof(buf) - n, fmt, args);
	va_end(args);
	write(buf);
}

Logger::Logger()
{
}

void Logger::write(std::string buf)
{
	if (ofs_.is_open()) {
		ofs_ << Timestamp::now().toFormattedString() 
			 << " " << buf << std::endl;
	}
	std::cout << Timestamp::now().toFormattedString() 
		 	  << " " << buf << std::endl;
}