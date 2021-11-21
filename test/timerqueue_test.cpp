/*************************************************************************
	> File Name: timerqueue_test.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Sat 20 Nov 2021 10:36:13 AM CST
 ************************************************************************/

#include "TimerQueue.h"
#include <iostream>

using namespace zing;
using namespace zing::net;
using std::cout;
using std::endl;

bool foo1()
{
	cout << "foo1 timeout" << endl;
	return true;
}

bool foo2()
{
	cout << "foo2 timeout" << endl;
	return true;
}

bool foo3()
{
	cout << "foo3 timeout" << endl;
	return true;
}

int main()
{
	TimerQueue qu;
	qu.addTimer(foo1, 2000);
	qu.addTimer(foo2, 3000);
	qu.addTimer(foo3, 5000);
	while (1) {
		qu.handleTimerEvent();
	}

	return 0;
}