/*************************************************************************
	> File Name: logger_test.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Sat 20 Nov 2021 04:32:06 PM CST
 ************************************************************************/

#include <iostream>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>
#include "../src/base/Logger.h"

using namespace zing;
using namespace zing::base;

void* foo(void*)
{
	while (1) {
		int num = rand() % 10;
		LOG_ERROR("child thread: %d", num);
		LOG_DEBUG("child thread: %d", num);
		LOG_STATE("child thread: %d", num);
		LOG_WARNING("child thread: %d", num);
		LOG_ERROR("child thread: %d", num);
		int s = rand() % 3;
		sleep(s);
	}	

	pthread_exit(nullptr);
}

int main()
{
	Logger::instance().init("test.log");
	Logger::instance().setLogLevel(LOG_ERROR);
	for (int i = 0; i < 3; ++i) {
		pthread_t tid;
		int ret = pthread_create(&tid, nullptr, foo, nullptr);
		assert(ret != -1);
		pthread_detach(tid);
	}
	while (1) {
		int num = rand() % 100;
		LOG_INFO("main thread: %d", num);
		int s = rand() % 3;
		sleep(s);
	}

	return 0;
}

