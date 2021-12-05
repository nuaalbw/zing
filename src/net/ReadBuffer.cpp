/*************************************************************************
	> File Name: ReadBuffer.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Mon 29 Nov 2021 03:57:44 PM CST
 ************************************************************************/

#include "ReadBuffer.h"
#include "SocketFiles.h"
#include "assert.h"
#include <algorithm>

using namespace zing;
using namespace zing::net;

constexpr char ReadBuffer::kCRLF[];

ReadBuffer::ReadBuffer(uint32_t size)
	: readIdx_(kCheapPrepend), 
	  writeIdx_(kCheapPrepend)
{
	buffer_.resize(kCheapPrepend + size);
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

uint32_t ReadBuffer::prependableBytes() const
{
	return (uint32_t)readIdx_;
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
	writeIdx_ = kCheapPrepend;
	readIdx_ = kCheapPrepend;
}

void ReadBuffer::retrieve(size_t len)
{
	if (len <= readableBytes()) {
		readIdx_ += len;
		if (readIdx_ == writeIdx_) {
			readIdx_ = kCheapPrepend;
			writeIdx_ = kCheapPrepend;
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

int ReadBuffer::readFd(int sockfd, int* savedErrno)
{
	char extrabuf[65536] = { 0 };
	struct iovec vec[2];
	const uint32_t writable = writeableBytes();
	vec[0].iov_base = begin() + writeIdx_;
	vec[0].iov_len = writable;
	vec[1].iov_base = extrabuf;
	vec[1].iov_len = sizeof(extrabuf);

	const int iovcnt = (writable < sizeof(extrabuf)) ? 2 : 1;
	const ssize_t n = ::readv(sockfd, vec, iovcnt);
	if (n < 0) {
		*savedErrno = errno;
	}
	else if (static_cast<uint32_t>(n) <= writable) {
		writeIdx_ += n;
	}
	else {
		writeIdx_ = buffer_.size();
		append(extrabuf, n - writable);
	}

	return n;
}

uint32_t ReadBuffer::readAll(std::string& data)
{
	uint32_t size = readableBytes();
	if (size > 0) {
		data.assign(peek(), size);
		writeIdx_ = kCheapPrepend;
		readIdx_ = kCheapPrepend;
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

void ReadBuffer::append(const char* data, size_t len)
{
	if (writeableBytes() < len) {
		makeSpace(len);
	}
	assert(writeableBytes() >= len);

	std::copy(data, data + len, beginWrite());
	assert(len <= writeableBytes());
	writeIdx_ += len;
}

void ReadBuffer::append(const void* data, size_t len)
{
	append(static_cast<const char*>(data), len);
}

void ReadBuffer::append(const std::string str)
{
	append(str.data(), str.size());
}

void ReadBuffer::makeSpace(size_t len)
{
	if (writeableBytes() + prependableBytes() < len + kCheapPrepend) {
		buffer_.resize(writeIdx_ + len);
	}
	else {
		// move readble data to the front, make space inside buffer
		assert(kCheapPrepend < readIdx_);
		uint32_t readable = readableBytes();
		std::copy(begin() + readIdx_, 
				  begin() + writeIdx_, 
				  begin() + kCheapPrepend);
		readIdx_ = kCheapPrepend;
		writeIdx_ = readIdx_ + readable;
		assert(readable == readableBytes());
	}
}
