/*************************************************************************
	> File Name: ThreadLocal.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Thu 20 Jan 2022 07:52:26 PM CST
 ************************************************************************/

#ifndef ZING_BASE_THREADLOCAL_H
#define ZING_BASE_THREADLOCAL_H

#include "Mutex.h"
#include "noncopyable.h"

#include <pthread.h>

namespace zing
{

template<typename T>
class ThreadLocal: noncopyable
{
public:
	ThreadLocal()
	{
		int ret = pthread_key_create(&pkey_, &ThreadLocal::destructor);
		assert(ret == 0);
	}

	~ThreadLocal()
	{
		int ret = pthread_key_delete(pkey_);
		assert(ret == 0);
	}

	T& value()
	{
		T* perThreadValue = static_cast<T*>(pthread_getspecific(pkey_));
		if (!perThreadValue)
		{
			T* newObj = new T();
			int ret = pthread_setspecific(pkey_, newObj);
			assert(ret == 0);
			perThreadValue = newObj;
		}
		return *perThreadValue;
	}

private:
	static void destructor(void* x)
	{
		T* obj = static_cast<T*>(x);
		typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];
    T_must_be_complete_type dummy; (void) dummy;
    delete obj;
	}

private:
	pthread_key_t pkey_;
};
	
} // namespace zing

#endif