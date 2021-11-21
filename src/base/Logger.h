/*************************************************************************
	> File Name: Logger.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Sat 20 Nov 2021 03:04:29 PM CST
 ************************************************************************/

#ifndef ZING_BASE_LOGGER_H
#define ZING_BASE_LOGGER_H

#include "noncopyable.h"
#include <mutex>
#include <fstream>
#include <string>

namespace zing
{
namespace base
{

enum Priority
{
	LOG_DEBUG, 
	LOG_STATE, 
	LOG_INFO, 
	LOG_WARNING, 
	LOG_ERROR
};

class Logger: noncopyable	// 单例类，全局只有一个实例
{
public:
	// 访问实例
	static Logger& instance();
	// 设置日志级别
	void setLogLevel(Priority level);
	// 获取当前日志级别
	Priority logLevel();

	// 将日志输出至指定文件
	void init(const char* pathname = nullptr);
	// 停止输出至文件
	void exit();
	void log(Priority level, const char* file, const char* func, 
			 int line, const char* fmt, ...);
	void log(Priority level, const char* fmt, ...);

private:
	Logger();
	void write(std::string buf);

private:
	std::mutex mutex_;
	std::ofstream ofs_;	
	static Priority s_level_;
};

#define LOG_DEBUG(fmt, ...) if (zing::base::Logger::instance().logLevel() <= zing::base::LOG_DEBUG) \
	zing::base::Logger::instance().log(LOG_DEBUG, __FILE__, \
	__FUNCTION__, __LINE__, fmt, ##__VA_ARGS__);

#define LOG_STATE(fmt, ...) if (zing::base::Logger::instance().logLevel() <= zing::base::LOG_STATE) \
	zing::base::Logger::instance().log(LOG_STATE, __FILE__, \
	__FUNCTION__, __LINE__, fmt, ##__VA_ARGS__);

#define LOG_INFO(fmt, ...) if (zing::base::Logger::instance().logLevel() <= zing::base::LOG_INFO) \
	zing::base::Logger::instance().log(LOG_INFO, __FILE__, \
	__FUNCTION__, __LINE__, fmt, ##__VA_ARGS__);

#define LOG_WARNING(fmt, ...) if (zing::base::Logger::instance().logLevel() <= zing::base::LOG_WARNING) \
	zing::base::Logger::instance().log(LOG_WARNING, __FILE__, \
	__FUNCTION__, __LINE__, fmt, ##__VA_ARGS__);

#define LOG_ERROR(fmt, ...) if (zing::base::Logger::instance().logLevel() <= zing::base::LOG_ERROR) \
	zing::base::Logger::instance().log(LOG_ERROR, __FILE__, \
	__FUNCTION__, __LINE__, fmt, ##__VA_ARGS__);

} // namespace base
} // namespace zing

#endif