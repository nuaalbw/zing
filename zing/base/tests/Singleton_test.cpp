/*************************************************************************
	> File Name: Singleton_test.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Thu 20 Jan 2022 06:49:31 PM CST
 ************************************************************************/

#include "../Singleton.h"
#include "../CurrentThread.h"
#include "../Thread.h"

#include <stdio.h>

class Test : zing::noncopyable
{
 public:
  Test()
  {
    printf("tid=%d, constructing %p\n", zing::CurrentThread::tid(), this);
  }

  ~Test()
  {
    printf("tid=%d, destructing %p %s\n", zing::CurrentThread::tid(), this, name_.c_str());
  }

  const zing::string& name() const { return name_; }
  void setName(const zing::string& n) { name_ = n; }

 private:
  zing::string name_;
};

class TestNoDestroy : zing::noncopyable
{
 public:
  // Tag member for Singleton<T>
  void no_destroy();

  TestNoDestroy()
  {
    printf("tid=%d, constructing TestNoDestroy %p\n", zing::CurrentThread::tid(), this);
  }

  ~TestNoDestroy()
  {
    printf("tid=%d, destructing TestNoDestroy %p\n", zing::CurrentThread::tid(), this);
  }
};

void threadFunc()
{
  printf("tid=%d, %p name=%s\n",
         zing::CurrentThread::tid(),
         &zing::Singleton<Test>::instance(),
         zing::Singleton<Test>::instance().name().c_str());
  zing::Singleton<Test>::instance().setName("only one, changed");
}

int main()
{
  zing::Singleton<Test>::instance().setName("only one");
  zing::Thread t1(threadFunc);
  t1.start();
  t1.join();
  printf("tid=%d, %p name=%s\n",
         zing::CurrentThread::tid(),
         &zing::Singleton<Test>::instance(),
         zing::Singleton<Test>::instance().name().c_str());
  zing::Singleton<TestNoDestroy>::instance();
  printf("with valgrind, you should see %zd-byte memory leak.\n", sizeof(TestNoDestroy));
}