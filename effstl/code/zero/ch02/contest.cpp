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
	
	for (i ; i > 10; --i)
	{
		test.pop_back();
	}

	std::vector<Test>::iterator it = test.begin();
	
	std::cout <<  reinterpret_cast<void*>(&(*it)) << " iter �޸�" << std::endl;

	// ���� ��� ������?
	const Test& temp2 = (*it);

	std::cout <<  reinterpret_cast<const void*>((&temp2)) << " ref �޸�" << std::endl;

	std::cout <<  " ���� ���� !! " << std::endl;
	std::vector<Test>(test).swap(test);
	std::cout <<  " ���� �� !! " << std::endl;

	// ���� ��ȿ �ұ�?
	//std::cout <<  reinterpret_cast<void*>(&(*it)) << "iter ������ �޸�" << std::endl;

	//std::cout << (*it).Get() << std::endl;
	
	return 0;
}

