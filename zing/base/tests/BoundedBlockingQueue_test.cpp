/*************************************************************************
	> File Name: BoundedBlockingQueue_test.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Wed 19 Jan 2022 03:06:38 PM CST
 ************************************************************************/

#include "../BoundedBlockingQueue.h"
#include "../CountDownLatch.h"
#include "../Thread.h"

#include <string>
#include <vector>

#include <stdio.h>
#include <unistd.h>

class Test
{
 public:
  Test(int numThreads)
    : queue_(20),
      latch_(numThreads)
  {
    threads_.reserve(numThreads);
    for (int i = 0; i < numThreads; ++i)
    {
      char name[32];
      snprintf(name, sizeof name, "work thread %d", i);
      threads_.emplace_back(new zing::Thread(
            std::bind(&Test::threadFunc, this), zing::string(name)));
    }
    for (auto& thr : threads_)
    {
      thr->start();
    }
  }

  void run(int times)
  {
    printf("waiting for count down latch\n");
    latch_.wait();
    printf("all threads started\n");
    for (int i = 0; i < times; ++i)
    {
      char buf[32];
      snprintf(buf, sizeof buf, "hello %d", i);
      queue_.put(buf);
      printf("tid=%d, put data = %s, size = %zd\n", zing::CurrentThread::tid(), buf, queue_.size());
    }
  }

  void joinAll()
  {
    for (size_t i = 0; i < threads_.size(); ++i)
    {
      queue_.put("stop");
    }

    for (auto& thr : threads_)
    {
      thr->join();
    }
  }

 private:

  void threadFunc()
  {
    printf("tid=%d, %s started\n",
           zing::CurrentThread::tid(),
           zing::CurrentThread::name());

    latch_.countDown();
    bool running = true;
    while (running)
    {
      std::string d(queue_.take());
      printf("tid=%d, get data = %s, size = %zd\n", zing::CurrentThread::tid(), d.c_str(), queue_.size());
      running = (d != "stop");
    }

    printf("tid=%d, %s stopped\n",
           zing::CurrentThread::tid(),
           zing::CurrentThread::name());
  }

  zing::BoundedBlockingQueue<std::string> queue_;
  zing::CountDownLatch latch_;
  std::vector<std::unique_ptr<zing::Thread>> threads_;
};

void testMove()
{
#if BOOST_VERSION >= 105500L
  muduo::BoundedBlockingQueue<std::unique_ptr<int>> queue(10);
  queue.put(std::unique_ptr<int>(new int(42)));
  std::unique_ptr<int> x = queue.take();
  printf("took %d\n", *x);
  *x = 123;
  queue.put(std::move(x));
  std::unique_ptr<int> y;
  y = queue.take();
  printf("took %d\n", *y);
#endif
}

int main()
{
  printf("pid=%d, tid=%d\n", ::getpid(), zing::CurrentThread::tid());
  testMove();
  Test t(5);
  t.run(100);
  t.joinAll();

  printf("number of created threads %d\n", zing::Thread::numCreated());
}