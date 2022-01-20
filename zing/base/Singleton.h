/*************************************************************************
	> File Name: Singleton.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Wed 19 Jan 2022 11:07:07 PM CST
 ************************************************************************/

#ifndef ZING_BASE_SINGLETON_H
#define ZING_BASE_SINGLETON_H

#include "noncopyable.h"

#include <assert.h>
#include <pthread.h>
#include <stdlib.h>

namespace zing
{

namespace detail
{
template<typename T>
struct has_no_destroy
{
	template<typename C>
	static char test(decltype(&C::no_destroy));

	template<typename C>
	static int32_t test(...);

	const static bool value = sizeof(test<T>(0)) == 1;

};
} // namespace detail

template<typename T>
class Singleton: noncopyable
{
public:
	Singleton() = delete;
	~Singleton() = delete;

	static T& instance()
	{
		pthread_once(&ponce_, &Singleton::init);
		assert(value_ != nullptr);
		return *value_;
	}

private:
	static void init()
	{
		value_ = new T();
		if (!detail::has_no_destroy<T>::value)
		{
			::atexit(destroy);
		}
	}

	static void destroy()
	{
    typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];
		T_must_be_complete_type dummy;
		(void)dummy;
		delete value_;
		value_ = nullptr;
	}

private:
	static pthread_once_t ponce_;
	static T* value_;
};

template<typename T>
pthread_once_t Singleton<T>::ponce_ = PTHREAD_ONCE_INIT;

template<typename T>
T* Singleton<T>::value_ = nullptr;

} // namespace zing

#endif