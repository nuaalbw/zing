/*************************************************************************
	> File Name: copyable.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Sat 15 Jan 2022 02:28:03 PM CST
 ************************************************************************/

#ifndef ZING_BASE_COPYABLE_H
#define ZING_BASE_COPYABLE_H

namespace zing
{

class copyable
{
 protected:
  copyable() = default;
  ~copyable() = default;
};

}  // namespace zing

#endif