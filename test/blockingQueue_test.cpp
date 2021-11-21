/*************************************************************************
	> File Name: blockingQueue_test.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Sat 20 Nov 2021 11:33:10 AM CST
 ************************************************************************/

#include "BlockingQueue.h"
#include <pthread.h>
#include <unistd.h>
#include <iostream>

using namespace zing;
using namespace zing::base;
using std::cout;
using std::endl;

BlockingQueue<int> queue;

void* producter(void* arg)
{
	while (1) {
		int x = rand() % 20;
		cout << "generate num: " << x << endl;
		queue.put(x);
		sleep(3);
	}

	pthread_exit(nullptr);
}

int main()
{
	for (int i = 0; i < 3; ++i) {
		pthread_t tid;
		int ret = pthread_create(&tid, nullptr, producter, nullptr);
		assert(ret != -1);
		pthread_detach(tid);
	}

	while (1) {
		int x = queue.take();
		cout << "consume num: " << x << endl;
		sleep(1);
	}

	exit(0);
}