// contest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <iostream>

class Test
{
public:
	Test():val(0)
	{
		std::cout << this << " 생성자 호출" << std::endl;
	}
	
	~Test()
	{
		std::cout <<  this << " 소멸자 호출" << std::endl;
		val = -1;
	}

	Test(const Test& lh)
	{
		std::cout <<  this << " 복사생성자 호출" << std::endl;
		this->val = lh.val;
	}

	void Set(int val) { this->val = val; }
	int Get() { return val; }

private:
	int val;
};

int _tmain(int argc, _TCHAR* argv[])
{
	std::vector<Test> test;
	const int size_test = 10;

	// 공간을 크게 잡자
	test.reserve(size_test + 30);
	
	int i=0;
	for (; i < size_test; ++i )
	{
		Test temp;
		temp.Set(i+1);
		test.push_back(temp);
	}
	
	for (i ; i > 10; --i)
	{
		test.pop_back();
	}

	std::vector<Test>::iterator it = test.begin();
	
	std::cout <<  reinterpret_cast<void*>(&(*it)) << " iter 메모리" << std::endl;

	// 과연 살아 있을까?
	const Test& temp2 = (*it);

	std::cout <<  reinterpret_cast<const void*>((&temp2)) << " ref 메모리" << std::endl;

	std::cout <<  " 스왑 시작 !! " << std::endl;
	std::vector<Test>(test).swap(test);
	std::cout <<  " 스왑 끝 !! " << std::endl;

	// 과연 유효 할까?
	//std::cout <<  reinterpret_cast<void*>(&(*it)) << "iter 스왑후 메모리" << std::endl;

	//std::cout << (*it).Get() << std::endl;
	
	return 0;
}

