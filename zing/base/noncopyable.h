/*************************************************************************
	> File Name: noncopyable.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Sat 15 Jan 2022 07:25:24 PM CST
 ************************************************************************/

#ifndef ZING_BASE_NONCOPYABLE_H
#define ZING_BASE_NONCOPYABLE_H

namespace zing
{

class noncopyable
{
 public:
  noncopyable(const noncopyable&) = delete;
  noncopyable(noncopyable&&) = delete;
  void operator=(const noncopyable&) = delete;

 protected:
  noncopyable() = default;
  ~noncopyable() = default;
};
	
} // namespace zing

#endif