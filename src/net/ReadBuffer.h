/*************************************************************************
	> File Name: ReadBuffer.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Sat 27 Nov 2021 10:48:48 AM CST
 ************************************************************************/

#ifndef ZING_NET_READ_BUFFER_H
#define ZING_NET_READ_BUFFER_H

#include "../base/noncopyable.h"
#include <stdint.h>
#include <vector>
#include <string>

namespace zing
{
namespace net
{

class ReadBuffer: noncopyable
{
public:
	static constexpr int kCheapPrepend = 8;	// 预留8字节首部
	explicit ReadBuffer(uint32_t size = 2048);	
	virtual ~ReadBuffer();
	
	uint32_t readableBytes() const;		// 可读字节数
	uint32_t writeableBytes() const;	// 可写字节数
	uint32_t prependableBytes() const;	
	uint32_t size() const;

	char* peek();
	const char* peek() const;

	const char* findFirstCrlf() const;
	const char* findLastCrlf() const;
	const char* findLastCrlfCrlf() const;

	void retrieveAll();
	void retrieve(size_t len);
	void retrieveUntil(const char* end);

	void retrieveInt64();
	void retrieveInt32();
	void retrieveInt16();
	void retrieveInt8();

	int readFd(int sockfd, int* savedErrno);
	uint32_t readAll(std::string& data);
	uint32_t readUntilCrlf(std::string& data);

	void append(const char* data, size_t len);
	void append(const void* data, size_t len);
	void append(const std::string str);

	// append intxx_t using network endian
	void appendInt64(int64_t x);
	void appendInt32(int32_t x);
	void appendInt16(int16_t x);
	void appendInt8(int8_t x);

	// read intxx_t from network endian
	int64_t readInt64();
	int32_t readInt32();
	int16_t readInt16();
	int8_t readInt8();

	// peek int64_t from network endian
	int64_t peekInt64();
	int32_t peekInt32();
	int16_t peekInt16();
	int8_t peekInt8();

	// prepend intxx_t using network endian
	void prependInt64(int64_t x);
	void prependInt32(int32_t x);
	void prependInt16(int16_t x);
	void prependInt8(int8_t x);
	void prepend(const void* data, size_t len);

private:
	char* begin();
	const char* begin() const;
	char* beginWrite();
	const char* beginWrite() const;
	void makeSpace(size_t len);

private:
	std::vector<char> buffer_;
	size_t readIdx_;
	size_t writeIdx_;

	static constexpr char kCRLF[] = "\r\n";
	static constexpr uint32_t MAX_BYTES_PER_READ = 4096;
	static constexpr uint32_t MAX_BUFFER_SIZE = 1024 * 100000;
};
	
} // namespace net
} // namespace zing

#endif