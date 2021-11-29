/*************************************************************************
	> File Name: WriteBuffer.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Mon 29 Nov 2021 09:20:41 PM CST
 ************************************************************************/

#include "WriteBuffer.h"
#include "SocketUtil.h"
#include <cstring>

using namespace zing;
using namespace zing::net;

WriteBuffer::WriteBuffer(int capacity)
	: maxQueueLength_(capacity)
{
}

WriteBuffer::~WriteBuffer()
{
}

bool WriteBuffer::append(std::shared_ptr<char> data, uint32_t size, uint32_t idx)
{
	if (size <= idx) {
		return false;
	}

	if ((int)buffer_.size() >= maxQueueLength_) {
		return false;
	}

	buffer_.emplace(data, size, idx);
	return true;
}

bool WriteBuffer::append(const char* data, uint32_t size, uint32_t idx)
{
	if (size <= idx) {
		return false;
	}

	if ((int)buffer_.size() >= maxQueueLength_) {
		return false;
	}

	std::shared_ptr<char> ptr(new char[size + 512], 
		std::default_delete<char[]>());
	::memcpy(ptr.get(), data, size);
	buffer_.emplace(std::move(ptr), size, idx);

	return true;
}

int WriteBuffer::send(int sockfd, int timeout)
{
	if (timeout > 0) {
		SocketUtil::setBlock(sockfd, timeout);
	}

	int ret = 0;
	int count = 1;

	do
	{
		// 缓冲区中无需要发送的数据
		if (buffer_.empty()) {
			return 0;
		}

		count -= 1;
		// 拿出每一个分包
		Packet& pkt = buffer_.front();
		ret = ::send(sockfd, pkt.data_.get(), pkt.size_ - pkt.writeIdx_, 0);
		if (ret > 0) {
			pkt.writeIdx_ += ret;
			if (pkt.writeIdx_ == pkt.size_) {	// 一个分包发送完毕
				count += 1;						// 继续发送下一个
				buffer_.pop();
			}
		}
		else if (ret < 0) {
			if (errno == EINTR || errno == EAGAIN) {
				ret = 0;
			}
		}
	} while (count > 0);

	if (timeout > 0) {
		SocketUtil::setNonblock(sockfd);
	}

	return ret;	
}

bool WriteBuffer::empty() const
{
	return buffer_.empty();
}

bool WriteBuffer::full() const
{
	return (int)buffer_.size() >= maxQueueLength_;
}

uint32_t WriteBuffer::size() const
{
	return (uint32_t)buffer_.size();
}