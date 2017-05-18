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

void CheckMemory(Test& rhs)
{
	std::cout <<  "체크 메모리 " << reinterpret_cast<void*>(&rhs) << std::endl;

	try
	{
		std::cout <<  " 저장된 값은 ? [" << rhs.Get() << "]" << std::endl;
	}
	catch (...)
	{
		std::cout <<  "메모리 참조 에러" << std::endl;
	}
}

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
	
	std::vector<Test>::pointer ref = &(*test.begin());
	std::cout << "1 메모리 시작 얻어옴" << std::endl;
	CheckMemory(*ref);

	std::cout <<  "1 스왑 시작 !! " << std::endl;
	std::vector<Test>(test).swap(test);
	std::cout <<  "1 스왑 끝 !! " << std::endl;

	// 과연 유효 할까?
	std::cout << "1 메모리 시작 확인" << std::endl;
	CheckMemory(*ref);

	// 다시 얻어옴
	ref = &(*test.begin());
	
	std::cout << "2 메모리 시작 얻어옴" << std::endl;
	CheckMemory(*ref);

	std::cout <<  "2 스왑 시작 !! " << std::endl;
	std::vector<Test> test2;
	std::swap(test2 , test);
	std::cout <<  "2 스왑 끝 !! " << std::endl;
	
	std::cout << "2 메모리 시작 확인" << std::endl;
	CheckMemory(*ref);

	std::cout << " 다끝남... " << std::endl;

	return 0;
}

