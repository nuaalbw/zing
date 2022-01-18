/*************************************************************************
	> File Name: CurrentThread.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Mon 17 Jan 2022 05:34:06 PM CST
 ************************************************************************/

#ifndef ZING_BASE_CURRENTTHREAD_H
#define ZING_BASE_CURRENTTHREAD_H

#include "Types.h"

// 临时
#include <unistd.h>
#include <sys/prctl.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <linux/unistd.h>

namespace zing
{
namespace CurrentThread
{

// __thread是gcc内置的线程局部存储设施，存取效率可以和全局变量相比。
// 使用__thread关键字修饰的变量每一个线程有一份独立实体，各个线程的值互不干扰。
// 可以用来修饰那些带有全局性且值可能变，但是又不值得用全局变量保护的变量。
extern __thread int t_cachedTid;		
extern __thread char t_tidString[32];
extern __thread int t_tidStringLength;
extern __thread	const char* t_threadName;

inline void cachedTid()
{
 	if (t_cachedTid == 0)
  {
    t_cachedTid = static_cast<pid_t>(::syscall(SYS_gettid));
    t_tidStringLength = snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid);
  }
}

inline int tid()
{
	if (__builtin_expect(t_cachedTid == 0, 0))
	{
		cachedTid();
	}
	return t_cachedTid;
}

inline const char* tidString()
{
	return t_tidString;
}

inline int tidStringLength()
{
	return t_tidStringLength;
}

inline const char* name()
{
	return t_threadName;
}

bool isMainThread();

void sleepUsec(int64_t usec);	// for test

string stackTrace(bool demangle);
	
} // namespace CurrentThread
} // namespace zing

#endif