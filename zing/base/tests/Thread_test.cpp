/*************************************************************************
	> File Name: Thread_test.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Tue 18 Jan 2022 09:53:44 PM CST
 ************************************************************************/

#include "../Thread.h"
#include "../CurrentThread.h"

#include <string>
#include <stdio.h>
#include <unistd.h>

void mysleep(int seconds)
{
  timespec t = { seconds, 0 };
  nanosleep(&t, NULL);
}

void threadFunc()
{
  printf("tid = %d\n", zing::CurrentThread::tid());
}

void threadFunc2(int x)
{
  printf("tid = %d, x = %d\n", zing::CurrentThread::tid(), x);
}

void threadFunc3()
{
  printf("tid = %d\n", zing::CurrentThread::tid());
  mysleep(1);
}

class Foo
{
 public:
  explicit Foo(double x)
    : x_(x)
  {
  }

  void memberFunc()
  {
    printf("tid = %d, Foo::x_ = %f\n", zing::CurrentThread::tid(), x_);
  }

  void memberFunc2(const std::string& text)
  {
    printf("tid = %d, Foo::x_ = %f, text = %s\n", zing::CurrentThread::tid(), x_, text.c_str());
  }

 private:
  double x_;
};

int main()
{
  printf("pid = %d, tid = %d\n", ::getpid(), zing::CurrentThread::tid());

  zing::Thread t1(threadFunc);
  t1.start();
  printf("t1.tid = %d\n", t1.tid());
  t1.join();

  zing::Thread t2(std::bind(threadFunc2, 42),
                   "thread for free function with argument");
  t2.start();
  printf("t2.tid = %d\n", t2.tid());
  t2.join();

  Foo foo(87.53);
  zing::Thread t3(std::bind(&Foo::memberFunc, &foo),
                   "thread for member function without argument");
  t3.start();
  t3.join();

  zing::Thread t4(std::bind(&Foo::memberFunc2, std::ref(foo), std::string("Li Bowen")));
  t4.start();
  t4.join();

  {
    zing::Thread t5(threadFunc3);
    t5.start();
    // t5 may destruct eariler than thread creation.
  }
  mysleep(2);
  {
    zing::Thread t6(threadFunc3);
    t6.start();
    mysleep(2);
    // t6 destruct later than thread creation.
  }
  sleep(2);
  printf("number of created threads %d\n", zing::Thread::numCreated());
}
