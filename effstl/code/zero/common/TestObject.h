#pragma once
#include "stdarg.h"

void WriteString(const char *format, ...);

class TestObject
{
public:
	TestObject():val(0)
	{
		WriteString("������ ȣ��");
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