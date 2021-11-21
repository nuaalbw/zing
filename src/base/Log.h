/*************************************************************************
	> File Name: Log.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Sat 20 Nov 2021 02:55:57 PM CST
 ************************************************************************/


#ifndef ZING_BASE_LOG_H
#define ZING_BASE_LOG_H

#include <cstdio>

namespace zing
{
namespace base
{

#define LOG(format, ...)	\
{							\
	fprintf(stderr, "[DEBUG] [%s:%s:%d] " format "", \
	__FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);	\
}

} // namespace base
} // namespace zing

#endif