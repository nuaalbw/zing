/*************************************************************************
	> File Name: Exception.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Mon 17 Jan 2022 06:16:17 PM CST
 ************************************************************************/

#include "Exception.h"
#include "CurrentThread.h"

namespace zing
{

Exception::Exception(string msg)
  : message_(std::move(msg)),
    stack_(CurrentThread::stackTrace(/*demangle=*/false))
{
}

}  // namespace zing
