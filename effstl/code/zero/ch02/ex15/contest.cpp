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
	std::cout << " 데이터 생성 끝 " << std::endl;

	for (i ; i > 10; --i)
	{
		test.pop_back();
	}
	
	std::cout << " 복사 대상자 생성 " << std::endl;
	const int array_size = test.size(); 
	Test* temp = new Test[array_size] ;

	std::cout << "- 복사 -" << std::endl;
	memcpy(temp, &test[0], array_size * sizeof(Test));

	std::cout << " 출력 " << std::endl;
	for (int i = 0; i < array_size; ++i)
	{
		std::cout << reinterpret_cast<void*>(&temp[i]) << "주소 " << (Test*)(temp[i]).Get() << " 복사된 값 출력" << std::endl;
	}

	std::cout << " 복사 대상자 삭제 " << std::endl;
	delete[] temp;

	std::cout << " 다끝남... " << std::endl;

	return 0;
}

