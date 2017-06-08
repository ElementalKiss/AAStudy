// contest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <set>
#include <iostream>
#include "../../common/TestObject.h"

int _tmain(int argc, _TCHAR* argv[])
{
// 	std::set<TestObject> test;
// 
// 	test.insert(1);
// 	test.insert(2);
// 	test.insert(3);
// 	test.insert(4);
// 	test.insert(5);
// 
// 	//
// 	test.insert(4);
// 
// 
// 	test.find(3);

	WriteString("===============");
	TestObjectSupportBySetContainer test_set;

	test_set.insert(1);
	test_set.insert(2);
	test_set.insert(3);
	test_set.insert(4);
	test_set.insert(5);

	test_set.insert(4);


	TestObjectSupportBySetContainer test2(test_set);

	return 0;
}

