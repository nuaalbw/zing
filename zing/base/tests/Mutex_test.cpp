/*************************************************************************
	> File Name: Mutex_test.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Mon 17 Jan 2022 11:20:03 PM CST
 ************************************************************************/

#include "../CountDownLatch.h"
#include "../Mutex.h"
#include "../Timestamp.h"

#include <vector>
#include <thread>
#include <stdio.h>

using namespace zing;
using namespace std;

MutexLock g_mutex;
vector<int> g_vec;
const int kCount = 10 * 1000 * 1000;

void threadFunc()
{
	for (int i = 0; i < kCount; ++i) {
		MutexLockGuard lock(g_mutex);
		g_vec.push_back(i);
	}
}

int foo() __attribute__((noinline));

int g_count = 0;

int foo()
{
	MutexLockGuard lock(g_mutex);
	if (!g_mutex.isLockedByThisThread()) {
		printf("FAIL\n");
		return -1;
	}

	++g_count;
	return 0;
}

int main()
{
	printf("sizeof pthread_mutex_t: %zd\n", sizeof(pthread_mutex_t));
  printf("sizeof Mutex: %zd\n", sizeof(MutexLock));
  printf("sizeof pthread_cond_t: %zd\n", sizeof(pthread_cond_t));
  printf("sizeof Condition: %zd\n", sizeof(Condition));

	int ret = foo();
	assert(ret == 0);

	const int kMaxThreads = 8;
	g_vec.reserve(kMaxThreads * kCount);
	Timestamp start(Timestamp::now());
	for (int i = 0; i < kCount; ++i) {
		g_vec.push_back(i);
	}

	printf("single thread without lock %f\n", timeDifference(Timestamp::now(), start));

	start = Timestamp::now();
	threadFunc();
	printf("single thread with lock %f\n", timeDifference(Timestamp::now(), start));

	for (int nthreads = 1; nthreads < kMaxThreads; ++nthreads)
	{
		vector<unique_ptr<thread>> threads;
		g_vec.clear();
		start = Timestamp::now();
		for (int i = 0; i < nthreads; ++i)
		{
			threads.emplace_back(new thread(threadFunc));
		}
		for (int i = 0; i < nthreads; ++i)
		{
			threads[i]->join();
		}
		printf("%d thread(s) with lock %f\n", nthreads, timeDifference(Timestamp::now(), start));
	}
}