#pragma once
#include "stdarg.h"

void WriteString(const char *format, ...);

class TestObject
{
public:
	TestObject():val(0)
	{
		WriteString("생성자 호출");
	}

	TestObject(int lhs) : val(lhs)
	{
		WriteString("생성자 호출 [%d]", val);
	}

	~TestObject()
	{
		WriteString("소멸자 호출 %x" , this );
		val = -1;
	}

	TestObject(const TestObject& lh)
	{
		WriteString("복사생성자 호출 %x" , this );
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
		WriteString("비교 연산 == ");
		return this->val == rhs.val;
	}

	bool operator < (const TestObject& rhs) const
	{
		bool result = (this->val < rhs.val);

		WriteString("비교 연산 <  [%d < %d]" ,this->val, rhs.val);
		
		return result;
	}

	bool operator > (const TestObject& rhs) const
	{
		WriteString("비교 연산 > ");
		
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
	WriteString("체크 메모리 %x", reinterpret_cast<void*>(&rhs) );

	try
	{
		WriteString("저장된 값은 ? [%x]", rhs.Get());
	}
	catch (...)
	{
		WriteString("메모리 참조 에러");
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