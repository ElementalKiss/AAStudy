#pragma once
#include "common.h"
#include "stdarg.h"

void WriteString(const char *format, ...);

class TestObject
{
public:
	TestObject():val(0)
	{
		WriteString("생성자 호출 %x" , this );
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
		WriteString("간접 지정 연산자 %x , %x", (&lhs), (&rhs));

        if(lhs.val < rhs.val)  return true;
        if(lhs.val > rhs.val)  return false;
        return (lhs.val < rhs.val);
    }

	TestObject& operator[] (int idx)
	{
		return *this;
	}
	//( ) [ ] -> *
	//	함수, 배열, 구조체, 포인터


	bool operator == (const TestObject& rhs)
	{
		WriteString("비교 연산 == ");
		return this->val == rhs.val;
	}

	bool operator < (const TestObject& rhs) const
	{
		bool result = (this->val < rhs.val);

		static std::string result_text[] = {"false","true"};

		WriteString("비교 연산 <  [%d < %d] result [%s]" ,this->val, rhs.val, result_text[result].c_str());
		
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

class TestObjectSupportBySet :public TestObject
{
public:
	TestObjectSupportBySet(int value2) : val2(value2), value(0) { WriteString("생성자 호출 v2 %x" , this ); }

	void SetKey(int val) { this->val2 = val; WriteString("v2 setv2 체크 메모리 %x", reinterpret_cast<void*>(this) );  }
	int GetV2() const { return val2; } 

	void SetValue(int val) { this->value = val; WriteString("v2 set 체크 메모리 %x", reinterpret_cast<void*>(this) ); }
	int GetValue() { return value; } 

private:
	int val2;
	int value;
};

// less의 정의해줌
struct TestObjectSupportBySetLess : 
	public std::binary_function<TestObjectSupportBySet,TestObjectSupportBySet, bool>{
		bool operator() (const TestObjectSupportBySet& lhs, const TestObjectSupportBySet& rhs) const {
			
			static std::string result_text[] = {"false","true"};

			bool result = (lhs.GetV2() < rhs.GetV2());

			WriteString("binary_function [%d < %d] result [%s]", lhs.GetV2(), rhs.GetV2(), result_text[result].c_str());

			return result;
		}
	}; 
	
typedef std::set<TestObjectSupportBySet, TestObjectSupportBySetLess> TestObjectSupportBySetContainer;

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