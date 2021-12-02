/*************************************************************************
	> File Name: ReadBuffer.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Mon 29 Nov 2021 03:57:44 PM CST
 ************************************************************************/

#include "ReadBuffer.h"
#include "SocketFiles.h"
#include <algorithm>

using namespace zing;
using namespace zing::net;

constexpr char ReadBuffer::kCRLF[];

ReadBuffer::ReadBuffer(uint32_t size)
	: readIdx_(0), 
	  writeIdx_(0)
{
	buffer_.resize(size);
}

ReadBuffer::~ReadBuffer()
{
}
	
uint32_t ReadBuffer::readableBytes() const
{
	return (uint32_t)(writeIdx_ - readIdx_);
}

uint32_t ReadBuffer::writeableBytes() const
{
	return (uint32_t)(buffer_.size() - writeIdx_);
}

uint32_t ReadBuffer::size() const
{
	return (uint32_t)buffer_.size();
}

char* ReadBuffer::peek()
{
	return begin() + readIdx_;
}

const char* ReadBuffer::peek() const
{
	return begin() + readIdx_;
}

const char* ReadBuffer::findFirstCrlf() const
{
	const char* crlf = std::search(peek(), beginWrite(), kCRLF, kCRLF + 2);
	return crlf == beginWrite() ? nullptr : crlf;
}

const char* ReadBuffer::findLastCrlf() const
{
	const char* crlf = std::find_end(peek(), beginWrite(), kCRLF, kCRLF + 2);
	return crlf == beginWrite() ? nullptr : crlf;
}

const char* ReadBuffer::findLastCrlfCrlf() const
{
	char crlfCrlf[] = "\r\n\r\n";
	const char* crlf = std::find_end(peek(), beginWrite(), crlfCrlf, crlfCrlf + 4);
	return crlf == beginWrite() ? nullptr : crlf;
}

void ReadBuffer::retrieveAll()
{
	writeIdx_ = 0;
	readIdx_ = 0;
}

void ReadBuffer::retrieve(size_t len)
{
	if (len <= readableBytes()) {
		readIdx_ += len;
		if (readIdx_ == writeIdx_) {
			readIdx_ = 0;
			writeIdx_ = 0;
		}
	}
	else {
		retrieveAll();
	}
}

void ReadBuffer::retrieveUntil(const char* end)
{
	retrieve(end - peek());
}

int ReadBuffer::read(int sockfd)
{
	uint32_t size = writeableBytes();
	if (size < MAX_BYTES_PER_READ) {
		uint32_t readBufferSize = (uint32_t)buffer_.size();
		if (readBufferSize > MAX_BUFFER_SIZE) {
			return 0;
		}
		buffer_.resize(readBufferSize + MAX_BYTES_PER_READ);
	}

	int bytesRead = ::recv(sockfd, beginWrite(), MAX_BYTES_PER_READ, 0);
	if (bytesRead > 0) {
		writeIdx_ += bytesRead;
	}

	return bytesRead;
}

uint32_t ReadBuffer::readAll(std::string& data)
{
	uint32_t size = readableBytes();
	if (size > 0) {
		data.assign(peek(), size);
		writeIdx_ = 0;
		readIdx_ = 0;
	}

	return size;
}

uint32_t ReadBuffer::readUntilCrlf(std::string& data)
{
	const char* crlf = findLastCrlf();
	if (crlf == nullptr) {
		return 0;
	}

	uint32_t size = (uint32_t)(crlf - peek() + 2);
	data.assign(peek(), size);
	retrieve(size);

	return size;
}

char* ReadBuffer::begin()
{
	return &*buffer_.begin();
}

const char* ReadBuffer::begin() const
{
	return &*buffer_.begin();
}

char* ReadBuffer::beginWrite()
{
	return begin() + writeIdx_;
}

const char* ReadBuffer::beginWrite() const
{
	return begin() + writeIdx_;
}