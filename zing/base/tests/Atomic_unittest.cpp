#include "../Atomic.h"

#include <thread>
#include <assert.h>
#include <stdio.h>

zing::AtomicInt64 g_num;

void test_1()
{
  {
    zing::AtomicInt64 a0;
    assert(a0.get() == 0);
    assert(a0.getAndAdd(1) == 0);
    assert(a0.get() == 1);
    assert(a0.addAndGet(2) == 3);
    assert(a0.get() == 3);
    assert(a0.incrementAndGet() == 4);
    assert(a0.get() == 4);
    a0.increment();
    assert(a0.get() == 5);
    assert(a0.addAndGet(-3) == 2);
    a0.set(100);
    assert(a0.get() == 100);
  }

  {
    zing::AtomicInt32 a1;
    assert(a1.get() == 0);
    assert(a1.getAndAdd(1) == 0);
    assert(a1.get() == 1);
    assert(a1.addAndGet(2) == 3);
    assert(a1.get() == 3);
    assert(a1.incrementAndGet() == 4);
    assert(a1.get() == 4);
    a1.increment();
    assert(a1.get() == 5);
    assert(a1.addAndGet(-3) == 2);
    a1.set(100);
    assert(a1.get() == 100);
  }
  printf("test_1 done\n"); 
}

void thread_a()
{
  for (int i = 0; i < 1000000; ++i) {
    g_num.increment();
    g_num.add(-3);
    g_num.incrementAndGet();
    g_num.getAndAdd(3);
  }
}

void thread_b()
{
  for (int i = 0; i < 1000000; ++i) {
    g_num.decrement();
    g_num.add(3);
    g_num.decrementAndGet();
    g_num.getAndAdd(-3);
  }
}

// 多线程测试
void test_2()
{
  assert(g_num.get() == 0);
  std::thread t1(thread_a);
  std::thread t2(thread_b);
  t1.join();
  t2.join();
  assert(g_num.get() == 0);
  printf("test_2 done, g_num = %ld\n", g_num.get());
}

int main()
{
  test_1();
  test_2();
}