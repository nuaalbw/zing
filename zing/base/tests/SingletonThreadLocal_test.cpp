/*************************************************************************
	> File Name: SingletonThreadLocal_test.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Thu 20 Jan 2022 08:44:30 PM CST
 ************************************************************************/

#include "../Singleton.h"
#include "../CurrentThread.h"
#include "../ThreadLocal.h"
#include "../Thread.h"

#include <stdio.h>
#include <unistd.h>

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

#define STL zing::Singleton<zing::ThreadLocal<Test> >::instance().value()

void print()
{
  printf("tid=%d, %p name=%s\n",
         zing::CurrentThread::tid(),
         &STL,
         STL.name().c_str());
}

void threadFunc(const char* changeTo)
{
  print();
  STL.setName(changeTo);
  sleep(1);
  print();
}

int main()
{
  STL.setName("main one");
  zing::Thread t1(std::bind(threadFunc, "thread1"));
  zing::Thread t2(std::bind(threadFunc, "thread2"));
  t1.start();
  t2.start();
  t1.join();
  print();
  t2.join();
  pthread_exit(0);
}
