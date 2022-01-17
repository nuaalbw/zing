/*************************************************************************
	> File Name: Exception.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Mon 17 Jan 2022 06:08:56 PM CST
 ************************************************************************/

#ifndef ZING_BASE_EXCEPTION_H
#define ZING_BASE_EXCEPTION_H

#include "Types.h"
#include <exception>

namespace zing
{

class Exception : public std::exception
{
 public:
  Exception(string what);
  ~Exception() noexcept override = default;

  // default copy-ctor and operator= are okay.

  const char* what() const noexcept override
  {
    return message_.c_str();
  }

  const char* stackTrace() const noexcept
  {
    return stack_.c_str();
  }

 private:
  string message_;
  string stack_;
};

}  // namespace zing

#endif