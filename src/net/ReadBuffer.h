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
	explicit ReadBuffer(uint32_t size = 2048);	
	virtual ~ReadBuffer();
	
	uint32_t readableBytes() const;		// 可读字节数
	uint32_t writeableBytes() const;	// 可写字节数
	uint32_t size() const;

	char* peek();
	const char* peek() const;

	const char* findFirstCrlf() const;
	const char* findLastCrlf() const;
	const char* findLastCrlfCrlf() const;

	void retrieveAll();
	void retrieve(size_t len);
	void retrieveUntil(const char* end);

	int read(int sockfd);
	uint32_t readAll(std::string& data);
	uint32_t readUntilCrlf(std::string& data);

private:
	char* begin();
	const char* begin() const;
	char* beginWrite();
	const char* beginWrite() const;

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