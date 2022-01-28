/*************************************************************************
	> File Name: LogFile.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Sun 23 Jan 2022 07:10:09 PM CST
 ************************************************************************/

#ifndef ZING_BASE_LOGFILE_H
#define ZING_BASE_LOGFILE_H

#include "Mutex.h"
#include "Types.h"

#include <memory>

namespace zing
{

namespace FileUtil
{
class AppendFile;	
} // namespace FileUtil

class LogFile: noncopyable
{
public:
	LogFile(const string& basename, 
					off_t rollSize, 
					bool threadSafe = true, 
					int flushInterval = 3, 
					int checkEvenyN = 1024);
	~LogFile();

	void append(const char* logline, int len);
	void flush();
	bool rollFile();

private:
	void append_unlocked(const char* logline, int len);

	static string getLogFileName(const string& basename, time_t* now);

	const string basename_;
	const off_t rollSize_;
	const int flushInterval_;
	const int checkEveryN_;

	int count_;

	std::unique_ptr<MutexLock> mutex_;
	time_t startOfPeriod_;
	time_t lastRoll_;
	time_t lastFlush_;
	std::unique_ptr<FileUtil::AppendFile> file_;

	const static int kRollPerSeconds_ = 60 * 60 * 24;
};
	
} // namespace zing

#endif