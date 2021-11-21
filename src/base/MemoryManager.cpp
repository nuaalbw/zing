/*************************************************************************
	> File Name: MemoryManager.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Sat 20 Nov 2021 07:24:45 PM CST
 ************************************************************************/

#include "MemoryManager.h"

using namespace zing;
using namespace zing::base;

void* zing::base::alloc(uint32_t size)
{
	return MemoryManager::instance().alloc(size);
}

void zing::base::release(void* ptr)
{
	return MemoryManager::instance().free(ptr);
}

MemoryPool::MemoryPool()
	: memory_(nullptr), 
	  blockSize_(0), 
	  blockNum_(0), 
	  head_(nullptr)
{
}

MemoryPool::~MemoryPool()
{
	if (memory_ != nullptr) {
		::free(memory_);
		memory_ = nullptr;
	}
}

void MemoryPool::init(uint32_t size, uint32_t n)
{
	if (memory_ != nullptr) {
		return;
	}

	blockSize_ = size;
	blockNum_ = n;
	memory_ = (char*)malloc(blockNum_ * (blockSize_ + sizeof(MemoryBlock)));

	head_ = reinterpret_cast<MemoryBlock*>(memory_);
	head_->id_ = 1;
	head_->pool_ = this;
	head_->next_ = nullptr;

	MemoryBlock* cur = head_;
	for (uint32_t n = 1; n < blockNum_; ++n) {
		MemoryBlock* node = (MemoryBlock*)(memory_ + 
			(n * (blockSize_ + sizeof(MemoryBlock))));

		node->id_ = n + 1;
		node->pool_ = this;
		node->next_ = nullptr;

		cur->next_ = node;
		cur = node;
	}
}

void* MemoryPool::alloc(uint32_t size)
{
	std::lock_guard<std::mutex> locker(mutex_);

	if (head_ != nullptr) {
		MemoryBlock* node = head_;
		head_ = head_->next_;
		return ((char*)node + sizeof(MemoryBlock));
	}

	return nullptr;
}

void MemoryPool::free(void* ptr)
{
	MemoryBlock* node = (MemoryBlock*)((char*)ptr - sizeof(MemoryBlock));
	if (node->id_ != 0) {
		std::lock_guard<std::mutex> locker(mutex_);
		node->next_ = head_;
		head_ = node;
	}
}

size_t MemoryPool::blockSize() const
{
	return blockSize_;	
}

MemoryManager& MemoryManager::instance()
{
	static MemoryManager s_manager;
	return s_manager;
}

void* MemoryManager::alloc(uint32_t size)
{
	// 优先从内存池中拿内存
	for (int n = 0; n < kMaxPoolNums; ++n) {
		if (size <= pools_[n].blockSize()) {
			void* ptr = pools_[n].alloc(size);
			if (ptr != nullptr) {
				return ptr;
			}
			else {
				break;
			}
		}
	}
	// 否则再进行动态分配
	MemoryBlock* node = (MemoryBlock*)malloc(size + sizeof(MemoryBlock));
	node->id_ = 0;
	node->pool_ = nullptr;
	node->next_ = nullptr;

	return ((char*)node + sizeof(MemoryBlock));
}

void MemoryManager::free(void* ptr)
{
	MemoryBlock* node = (MemoryBlock*)((char*)ptr - sizeof(MemoryBlock));
	MemoryPool* pool = node->pool_;

	// 优先选择放回池中
	if (pool != nullptr && node->id_ > 0) {
		pool->free(ptr);
	}
	// 否则再动态释放内存
	else {
		::free(node);
	}
}

MemoryManager::MemoryManager()
{
	pools_[0].init(4096, 50);	// 4kb
	pools_[1].init(40690, 10);	// 40kb
	pools_[2].init(102400, 5);	// 100kb
}