/*************************************************************************
	> File Name: MemoryManager.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Sat 20 Nov 2021 07:08:14 PM CST
 ************************************************************************/

#ifndef ZING_BASE_MEMORY_MANAGER_H
#define ZING_BASE_MEMORY_MANAGER_H

#include <stdint.h>
#include <mutex>
#include "noncopyable.h"

namespace zing
{
namespace base
{

// 非线程安全
void* alloc(uint32_t size);
void release(void* ptr);

class MemoryPool;

struct MemoryBlock: noncopyable	// 内存块的头部信息
{
	MemoryBlock()
		: id_(0), 
		  pool_(nullptr), 
	  	  next_(nullptr)
	{}

	uint32_t id_;
	MemoryPool* pool_;	// 所属内存池
	MemoryBlock* next_;	// 下一块地址
};

class MemoryPool: noncopyable
{
public:
	MemoryPool();
	virtual ~MemoryPool();

	// 初始化n个大小为size的内存块
	void init(uint32_t size, uint32_t n);
	// 拿出一个内存块
	void* alloc(uint32_t size);
	// 放回一个内存块
	void free(void* ptr);
	// 获取内存块的大小
	size_t blockSize() const;

private:
	char* memory_;			// 内存池起始地址
	uint32_t blockSize_;	// 内存块大小，不包含MemoryBlock的大小
	uint32_t blockNum_;		// 内存块数目
	MemoryBlock* head_;		// 内存块以链表的方式组织在内存池中
	std::mutex mutex_;
};

class MemoryManager: noncopyable
{
public:
	static MemoryManager& instance();
	void* alloc(uint32_t size);
	void free(void* ptr);

private:
	MemoryManager();

private:
	static constexpr int kMaxPoolNums = 3;
	MemoryPool pools_[kMaxPoolNums];
};
	
} // namespace base
} // namespace zing

#endif