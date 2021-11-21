/*************************************************************************
	> File Name: memory_test.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Sat 20 Nov 2021 08:47:44 PM CST
 ************************************************************************/

#include "../src/base/MemoryManager.h"
#include <iostream>

using namespace zing;
using namespace zing::base;
using std::cout;
using std::endl;

int main()
{
	int* nums = (int*)alloc(100 * sizeof(int));
	for (int i = 0; i < 100; ++i) {
		nums[i] = rand() % 200;
		cout << i << " " << nums[i] << endl;	
	}
	release(nums);

	return 0;
}