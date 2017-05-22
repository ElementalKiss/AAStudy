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

	Test(int lhs) : val(lhs)
	{

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

	//1 ) 예제문에 나온 내용은 정보를 모두 클리어 하는 것이다
	test.push_back(Test(1));
	test.push_back(Test(2));
	test.push_back(Test(3));
	test.push_back(Test(4));

// 	std::cout << "test.resize(0) 시작" << std::endl;
// 	test.resize(0);
// 	std::cout << "resize 컨테이너 사이즈 " << test.capacity() << std::endl;
// 
// 	std::cout << "test.clear() 시작" << std::endl;
// 	test.clear();
// 	std::cout << "clear 컨테이너 사이즈 " << test.capacity() << std::endl;

	// clear , resize 모두 소멸자를 호출하나 컨테이너의 사이즈의 변화는 없다.

	std::cout <<  "std::vector<Test>().swap(test) 스왑 시작 !! " << std::endl;
	std::vector<Test>().swap(test);
	std::cout <<  "std::vector<Test>().swap(test) 스왑 끝 !! " << std::endl;
	// 내용이 있는 경우 소멸 자를 호출 하였고
	// 컨테이너 사이즈 또한 정리 하였다

	std::cout << "swap 컨테이너 사이즈 " << test.capacity() << std::endl;

	// 코드 힝식상 rhs 가 없기때문에 새로 생성된 내용과 swap 하는 형태. 이지 않을까?
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
	
	std::vector<Test>::pointer ref = &(*test.begin());

	std::cout << "soruce 메모리 시작 얻어옴" << std::endl;
	CheckMemory(*ref);
	std::cout << "soruce 컨테이너 사이즈 " << test.capacity() << std::endl;

	std::cout <<  "std::vector<Test>(test).swap(test) 스왑 시작 !! " << std::endl;
	std::vector<Test>(test).swap(test);
	std::cout <<  "std::vector<Test>(test).swap(test) 스왑 끝 !! " << std::endl;

	std::cout << "soruce 컨테이너 사이즈 " << test.capacity() << std::endl;

	// 과연 유효 할까? (스왑하면서 컨테이너 사이즈를 다시 잡았기 때문에 주소가 다르다)
	std::cout << "soruce 메모리 시작 확인" << std::endl;
	CheckMemory(*ref);

	// 다시 얻어옴
	ref = &(*test.begin());
	
	std::cout << "soruce 메모리 시작 얻어옴" << std::endl;
	CheckMemory(*ref);

	std::cout << "soruce 컨테이너 사이즈 " << test.capacity() << std::endl;

	std::cout <<  "std::vector<Test> (test).swap(test2) 스왑 시작 !! " << std::endl;
	std::vector<Test> test2;
	std::swap(test2, test);
	std::cout <<  "std::vector<Test> (test).swap(test2) 스왑 끝 !! " << std::endl;
	
	std::cout << "soruce 컨테이너 사이즈 " << test.capacity() << std::endl;
	std::cout << "target 컨테이너 사이즈 " << test2.capacity() << std::endl;

	std::cout << "soruce 메모리 시작 확인" << std::endl;
	CheckMemory(*ref);

	// test -> test2로 넘어왔다. iter

	std::cout << "-------------------------" << std::endl;
	std::cout << "----------  끝 ----------" << std::endl;

	return 0;
}

