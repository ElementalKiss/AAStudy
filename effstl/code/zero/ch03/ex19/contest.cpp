// contest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <set>
#include <iostream>
#include "../../common/TestObject.h"

int _tmain(int argc, _TCHAR* argv[])
{
	std::set<TestObject> test;

	test.insert(1);
	test.insert(2);
	test.insert(3);
	test.insert(4);
	test.insert(5);

	test.find(3);

	return 0;
}

