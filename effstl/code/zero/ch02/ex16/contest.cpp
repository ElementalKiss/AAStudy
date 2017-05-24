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

	Test(int lhs) : val(lhs)
	{

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

	//1 ) �������� ���� ������ ������ ��� Ŭ���� �ϴ� ���̴�
	test.push_back(Test(1));
	test.push_back(Test(2));
	test.push_back(Test(3));
	test.push_back(Test(4));

// 	std::cout << "test.resize(0) ����" << std::endl;
// 	test.resize(0);
// 	std::cout << "resize �����̳� ������ " << test.capacity() << std::endl;
// 
// 	std::cout << "test.clear() ����" << std::endl;
// 	test.clear();
// 	std::cout << "clear �����̳� ������ " << test.capacity() << std::endl;

	// clear , resize ��� �Ҹ��ڸ� ȣ���ϳ� �����̳��� �������� ��ȭ�� ����.

	std::cout <<  "std::vector<Test>().swap(test) ���� ���� !! " << std::endl;
	std::vector<Test>().swap(test);
	std::cout <<  "std::vector<Test>().swap(test) ���� �� !! " << std::endl;
	// ������ �ִ� ��� �Ҹ� �ڸ� ȣ�� �Ͽ���
	// �����̳� ������ ���� ���� �Ͽ���

	std::cout << "swap �����̳� ������ " << test.capacity() << std::endl;

	// �ڵ� ���Ļ� rhs �� ���⶧���� ���� ������ ����� swap �ϴ� ����. ���� ������?
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
	
	std::vector<Test>::pointer ref = &(*test.begin());

	std::cout << "soruce �޸� ���� ����" << std::endl;
	CheckMemory(*ref);
	std::cout << "soruce �����̳� ������ " << test.capacity() << std::endl;

	std::cout <<  "std::vector<Test>(test).swap(test) ���� ���� !! " << std::endl;
	std::vector<Test>(test).swap(test);
	std::cout <<  "std::vector<Test>(test).swap(test) ���� �� !! " << std::endl;

	std::cout << "soruce �����̳� ������ " << test.capacity() << std::endl;

	// ���� ��ȿ �ұ�? (�����ϸ鼭 �����̳� ����� �ٽ� ��ұ� ������ �ּҰ� �ٸ���)
	std::cout << "soruce �޸� ���� Ȯ��" << std::endl;
	CheckMemory(*ref);

	// �ٽ� ����
	ref = &(*test.begin());
	
	std::cout << "soruce �޸� ���� ����" << std::endl;
	CheckMemory(*ref);

	std::cout << "soruce �����̳� ������ " << test.capacity() << std::endl;

	std::cout <<  "std::vector<Test> (test).swap(test2) ���� ���� !! " << std::endl;
	std::vector<Test> test2;
	std::swap(test2, test);
	std::cout <<  "std::vector<Test> (test).swap(test2) ���� �� !! " << std::endl;
	
	std::cout << "soruce �����̳� ������ " << test.capacity() << std::endl;
	std::cout << "target �����̳� ������ " << test2.capacity() << std::endl;

	std::cout << "soruce �޸� ���� Ȯ��" << std::endl;
	CheckMemory(*ref);

	// test -> test2�� �Ѿ�Դ�. iter

	std::cout << "-------------------------" << std::endl;
	std::cout << "----------  �� ----------" << std::endl;

	return 0;
}

