/*************************************************************************
	> File Name: Pipe.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Sun 21 Nov 2021 08:05:23 PM CST
 ************************************************************************/

#ifndef ZING_NET_PIPE_H
#define ZING_NET_PIPE_H

namespace zing
{
namespace net
{

class Pipe
{
public:
	bool create();
	int write(void* buf, int len);
	int read(void* buf, int len);
	void close();

	int readfd() const;
	int writefd() const;

private:
	int pipefd_[2];
};
	
} // namespace net
} // namespace zing

#endif