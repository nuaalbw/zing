/*************************************************************************
	> File Name: noncopyable.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Sat 15 Jan 2022 07:25:24 PM CST
 ************************************************************************/

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
