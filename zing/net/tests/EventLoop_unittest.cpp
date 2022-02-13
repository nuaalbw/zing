/*************************************************************************
	> File Name: EventLoop_unittest.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Sun 13 Feb 2022 01:08:34 PM CST
 ************************************************************************/

#include "../EventLoop.h"
#include "../../base/Thread.h"

#include <assert.h>
#include <stdio.h>
#include <unistd.h>

using namespace zing;
using namespace zing::net;

EventLoop* g_loop;

void callback()
{
  printf("callback(): pid = %d, tid = %d\n", getpid(), CurrentThread::tid());
  EventLoop anotherLoop;
}

void threadFunc()
{
  printf("threadFunc(): pid = %d, tid = %d\n", getpid(), CurrentThread::tid());

  assert(EventLoop::getEventLoopOfCurrentThread() == nullptr);
  EventLoop loop;
  assert(EventLoop::getEventLoopOfCurrentThread() == &loop);
  loop.runAfter(1.0, callback);
  loop.loop();
}

int main()
{
  printf("main(): pid = %d, tid = %d\n", getpid(), CurrentThread::tid());

  assert(EventLoop::getEventLoopOfCurrentThread() == nullptr);
  EventLoop loop;
  assert(EventLoop::getEventLoopOfCurrentThread() == &loop);

  Thread thread(threadFunc);
  thread.start();

  loop.loop();
}
