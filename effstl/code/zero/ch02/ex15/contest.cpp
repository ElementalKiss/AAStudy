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
	std::cout << " ������ ���� �� " << std::endl;

	for (i ; i > 10; --i)
	{
		test.pop_back();
	}
	
	std::cout << " ���� ����� ���� " << std::endl;
	const int array_size = test.size(); 
	Test* temp = new Test[array_size] ;

	std::cout << "- ���� -" << std::endl;
	memcpy(temp, &test[0], array_size * sizeof(Test));

	std::cout << " ��� " << std::endl;
	for (int i = 0; i < array_size; ++i)
	{
		std::cout << reinterpret_cast<void*>(&temp[i]) << "�ּ� " << (Test*)(temp[i]).Get() << " ����� �� ���" << std::endl;
	}

	std::cout << " ���� ����� ���� " << std::endl;
	delete[] temp;

	std::cout << " �ٳ���... " << std::endl;

	return 0;
}

