/*************************************************************************
	> File Name: Pipe.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Mon 22 Nov 2021 09:51:42 PM CST
 ************************************************************************/

#include "Pipe.h"
#include "SocketUtil.h"

using namespace zing;
using namespace zing::net;

bool Pipe::create()
{
	int ret = pipe2(pipefd_, O_NONBLOCK | O_CLOEXEC);
	if (ret < 0) {
		return false;
	}
	return true;
}

int Pipe::write(void* buf, int len)
{
	return ::write(pipefd_[1], buf, len);
}

int Pipe::read(void* buf, int len)
{
	return ::read(pipefd_[0], buf, len);
}

void Pipe::close()
{
	::close(pipefd_[0]);
	::close(pipefd_[1]);
}

int Pipe::readfd() const
{
	return pipefd_[0];
}

int Pipe::writefd() const
{
	return pipefd_[1];
}