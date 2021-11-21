/*************************************************************************
	> File Name: timestamp_test.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Mon 15 Nov 2021 08:48:51 PM CST
 ************************************************************************/

#include "Timestamp.h"
#include <iostream>

using namespace zing;
using std::cout;
using std::endl;

void test()
{
	Timestamp time1;
	Timestamp time2(37129312810371230);

	cout << time2.toString() << endl;
	cout << time2.toFormattedString() << endl;

	time1.swap(time2);
	cout << "交换之后：" << endl;
	cout << time2.toString() << endl;
	cout << time2.toFormattedString() << endl;

	Timestamp cur = Timestamp::now();
	cout << cur.toString() << endl;
	cout << cur.toFormattedString(true) << endl;

}

int main()
{
	test();
	return 0;
}