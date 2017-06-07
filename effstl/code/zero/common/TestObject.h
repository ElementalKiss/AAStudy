#pragma once
#include "stdarg.h"
#include <set>


void WriteString(const char *format, ...);

class TestObject
{
public:
	TestObject():val(0)
	{
		WriteString("������ ȣ�� %x" , this );
	}

	TestObject(int lhs) : val(lhs)
	{
		WriteString("������ ȣ�� [%d]", val);
	}

	~TestObject()
	{
		WriteString("�Ҹ��� ȣ�� %x" , this );
		val = -1;
	}

	TestObject(const TestObject& lh)
	{
		WriteString("��������� ȣ�� %x" , this );
		this->val = lh.val;
	}

	bool operator() (const TestObject& lhs, const TestObject& rhs)
    {
        if(lhs.val < rhs.val)  return true;
        if(lhs.val > rhs.val)  return false;
        return (lhs.val < rhs.val);
    }

	bool operator == (const TestObject& rhs)
	{
		WriteString("�� ���� == ");
		return this->val == rhs.val;
	}

	bool operator < (const TestObject& rhs) const
	{
		bool result = (this->val < rhs.val);

		WriteString("�� ���� <  [%d < %d]" ,this->val, rhs.val);
		
		return result;
	}

	bool operator > (const TestObject& rhs) const
	{
		WriteString("�� ���� > ");
		
		bool result = (this->val < rhs.val);
		
		return result;
	}

	void Set(int val) { this->val = val; }
	int Get() { return val; }

private:
	int val;
};

class TestObjectSupportBySet :public TestObject
{
public:
	TestObjectSupportBySet(int value2) : val2(value2), value(0) { WriteString("������ ȣ�� v2 %x" , this ); }

	void SetKey(int val) { this->val2 = val; WriteString("v2 setv2 üũ �޸� %x", reinterpret_cast<void*>(this) );  }
	int GetV2() const { return val2; } 

	void SetValue(int val) { this->value = val; WriteString("v2 set üũ �޸� %x", reinterpret_cast<void*>(this) ); }
	int GetValue() { return value; } 

private:
	int val2;
	int value;
};

// less�� ��������
struct TestObjectSupportBySetLess : 
	public std::binary_function<TestObjectSupportBySet,TestObjectSupportBySet, bool>{
		bool operator() (const TestObjectSupportBySet& lhs, const TestObjectSupportBySet& rhs) const {
			WriteString("�� ���� v2 less ");
			return lhs.GetV2() < rhs.GetV2();
		}
	}; 
	
typedef std::set<TestObjectSupportBySet, TestObjectSupportBySetLess> TestObjectSupportBySetContainer;

void CheckMemory(TestObject& rhs)
{
	WriteString("üũ �޸� %x", reinterpret_cast<void*>(&rhs) );

	try
	{
		WriteString("����� ���� ? [%x]", rhs.Get());
	}
	catch (...)
	{
		WriteString("�޸� ���� ����");
	}
}

void WriteString(const char *format, ...)
{
	char buffer[1024];
	va_list args;
	va_start(args, format);
	vsprintf_s(buffer, format, args);
	va_end(args);

	std::cout << buffer << std::endl;
}