// contest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include "../../common/TestObject.h"

int _tmain(int argc, _TCHAR* argv[])
{
	TestObjectSupportBySetContainer test;

	test.insert(1);
	test.insert(2);
	test.insert(3);
	test.insert(4);
	test.insert(5);
	
	TestObjectSupportBySetContainer::iterator itor = test.find(3);
	
	// 안바뀌어요
	static_cast<TestObjectSupportBySet>(*itor).SetKey(5);
	((TestObjectSupportBySet)(*itor)).SetKey(6);
	TestObjectSupportBySet test2(*itor); test2.SetKey(7);

	// key 값을 바꿀 수있음
	const_cast<TestObjectSupportBySet&>(*itor).SetKey(1); // 기존key 3이.. 1로
	const_cast<TestObjectSupportBySet&>(*itor).SetValue(3);
	test.insert(3);
}

