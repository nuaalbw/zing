/*************************************************************************
	> File Name: WriteBuffer.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Mon 29 Nov 2021 09:04:23 PM CST
 ************************************************************************/

#ifndef ZING_NET_WRITE_BUFFER_H
#define ZING_NET_WRITE_BUFFER_H

#include "../base/noncopyable.h"
#include <memory>
#include <queue>

namespace zing
{
namespace net
{

class WriteBuffer: noncopyable
{
public:
	explicit WriteBuffer(int capacity = kMaxQueueLength);
	~WriteBuffer();

	// 向缓冲区中添加需要发送的数据
	bool append(std::shared_ptr<char> data, uint32_t size, uint32_t idx = 0);
	bool append(const char* data, uint32_t size, uint32_t idx = 0);
	// 发送缓冲区中的数据
	int send(int sockfd, int timeout = 0);

	bool empty() const;
	bool full() const;
	uint32_t size() const;

private:
	// 对每一个需要发送的数据做分包处理
	struct Packet
	{
		Packet(std::shared_ptr<char> data, uint32_t size, uint32_t writeIdx)
			: data_(data), 
			  size_(size), 
			  writeIdx_(writeIdx)
		{
		}
		std::shared_ptr<char> data_;	// 使用智能指针维护缓冲区
		uint32_t size_;					// 缓冲区大小
		uint32_t writeIdx_;				// 当前已发送字节数大小
	};

	std::queue<Packet> buffer_;	
	int maxQueueLength_;				// 最大队列长度

	static constexpr int kMaxQueueLength = 10000;
};
	
} // namespace net
} // namespace zing

#endif