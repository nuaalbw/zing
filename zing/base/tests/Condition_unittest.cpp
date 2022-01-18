/*************************************************************************
	> File Name: Condition_unittest.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Tue 18 Jan 2022 04:08:22 PM CST
 ************************************************************************/

#include "../Condition.h"
#include <thread>
#include <stdio.h>
#include <time.h>

using namespace zing;

// 生产者消费者模型的实现
struct Node
{
	Node(int val): val_(val), next_(nullptr) {}
	int val_;
	Node* next_;
};

MutexLock g_mutex;
Condition g_cond;
Node* g_head;

void printList(Node* head)
{
	while (head)
	{
		printf("%d->", head->val_);
		head = head->next_;
	}
	printf("\n");
}

void producer()
{
	srand(static_cast<int>(time(nullptr)));
	while (true)
	{
		int val = rand() % 10;
		Node* cur = new Node(val);
		// 生产者开始生产节点，进入临界区
		{
			MutexLockGuard guard(g_mutex);
			g_mutex.assertLocked();
			cur->next_ = g_head;
			g_head = cur;
			printList(g_head);
			printf("producer products num %d into the list, tid = %d\n", 
					 		g_head->val_, CurrentThread::tid());
		}
		// 生产完毕后唤醒消费者
		g_cond.notify();
		sleep(1);
	}
}

void consumer()
{
	while (true)
	{
		{
			MutexLockGuard guard(g_mutex);
			// 等待生产者线程生产节点
			while (g_head == nullptr)
			{
				printf("consumer wait, tid = %d\n", CurrentThread::tid());
				g_cond.wait(guard);
			}
			// 消费者线程开始消费节点
			Node* tmp = g_head;
			printf("consumer get %d from the list, tid = %d\n", 
						 tmp->val_, CurrentThread::tid());
			g_head = g_head->next_;
			delete tmp;
		}
		sleep(2);
	}
}

int main()
{
	std::thread t1(producer);
	std::thread t2(consumer);
	t1.join();
	t2.join();
	return 0;
}