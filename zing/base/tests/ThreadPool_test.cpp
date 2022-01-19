/*************************************************************************
	> File Name: ThreadPool_test.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Wed 19 Jan 2022 10:29:13 PM CST
 ************************************************************************/

#include "../ThreadPool.h"
#include "../CountDownLatch.h"
#include "../CurrentThread.h"

#include <stdio.h>
#include <unistd.h>  // usleep

void print()
{
  printf("tid=%d\n", zing::CurrentThread::tid());
}

void printString(const std::string& str)
{
	printf("%s\n", str.c_str());
  usleep(100*1000);
}

void test(int maxSize)
{
	printf("Test ThreadPool with max queue size = %d\n", maxSize);
  zing::ThreadPool pool("MainThreadPool");
  pool.setMaxQueueSize(maxSize);
  pool.start(5);

	printf("Adding\n");
  pool.run(print);
  pool.run(print);
  for (int i = 0; i < 100; ++i)
  {
    char buf[32];
    snprintf(buf, sizeof buf, "task %d", i);
    pool.run(std::bind(printString, std::string(buf)));
  }
	printf("Done\n");

  zing::CountDownLatch latch(1);
  pool.run(std::bind(&zing::CountDownLatch::countDown, &latch));
  latch.wait();
  pool.stop();
}

void longTask(int num)
{
	printf("longTask %d\n", num);
  zing::CurrentThread::sleepUsec(3000000);
}

void test2()
{
	printf("Test ThreadPool by stoping early.\n");
  zing::ThreadPool pool("ThreadPool");
  pool.setMaxQueueSize(5);
  pool.start(3);

  zing::Thread thread1([&pool]()
  {
    for (int i = 0; i < 20; ++i)
    {
      pool.run(std::bind(longTask, i));
    }
  }, "thread1");
  thread1.start();

  zing::CurrentThread::sleepUsec(5000000);
	printf("stop pool\n");
  pool.stop();  // early stop

  thread1.join();
  // run() after stop()
  pool.run(print);
	printf("test2 Done\n");
}

int main()
{
  test(0);
  test(1);
  test(5);
  test(10);
  test(50);
  test2();
}