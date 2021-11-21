/*************************************************************************
	> File Name: noncopyable.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Mon 15 Nov 2021 09:11:54 PM CST
 ************************************************************************/

#ifndef ZING_BASE_NONCOPYABLE_H
#define ZING_BASE_NONCOPYABLE_H

namespace zing
{

class noncopyable
{
public:
	noncopyable(const noncopyable&) = delete;
	void operator=(const noncopyable&) = delete;

protected:
	noncopyable() = default;
	~noncopyable() = default;
};

}

#endif