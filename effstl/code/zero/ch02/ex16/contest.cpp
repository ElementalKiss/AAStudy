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
		std::cout << this << " ������ ȣ��" << std::endl;
	}
	
	~Test()
	{
		std::cout <<  this << " �Ҹ��� ȣ��" << std::endl;
		val = -1;
	}

	Test(const Test& lh)
	{
		std::cout <<  this << " ��������� ȣ��" << std::endl;
		this->val = lh.val;
	}

	void Set(int val) { this->val = val; }
	int Get() { return val; }

private:
	int val;
};

void CheckMemory(Test& rhs)
{
	std::cout <<  "üũ �޸� " << reinterpret_cast<void*>(&rhs) << std::endl;

	try
	{
		std::cout <<  " ����� ���� ? [" << rhs.Get() << "]" << std::endl;
	}
	catch (...)
	{
		std::cout <<  "�޸� ���� ����" << std::endl;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	std::vector<Test> test;
	const int size_test = 10;

	// ������ ũ�� ����
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
	std::cout << "1 �޸� ���� ����" << std::endl;
	CheckMemory(*ref);

	std::cout <<  "1 ���� ���� !! " << std::endl;
	std::vector<Test>(test).swap(test);
	std::cout <<  "1 ���� �� !! " << std::endl;

	// ���� ��ȿ �ұ�?
	std::cout << "1 �޸� ���� Ȯ��" << std::endl;
	CheckMemory(*ref);

	// �ٽ� ����
	ref = &(*test.begin());
	
	std::cout << "2 �޸� ���� ����" << std::endl;
	CheckMemory(*ref);

	std::cout <<  "2 ���� ���� !! " << std::endl;
	std::vector<Test> test2;
	std::swap(test2 , test);
	std::cout <<  "2 ���� �� !! " << std::endl;
	
	std::cout << "2 �޸� ���� Ȯ��" << std::endl;
	CheckMemory(*ref);

	std::cout << " �ٳ���... " << std::endl;

	return 0;
}

