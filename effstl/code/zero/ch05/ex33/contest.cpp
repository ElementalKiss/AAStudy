// contest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <algorithm>
#include "../../common/TestObject.h"
#include <vector>
#include <functional>
#include <algorithm>
#include <iostream>

int _tmain(int argc, _TCHAR* argv[])
{
    class Test
    {
    public:
        Test() {}

    };

    std::vector<TestObject*> testObj;
    
    for (int i = 1; i <= 5; ++i) {
        testObj.push_back(new TestObject(i+1));
    }
    
    
    TestObject* temp = new TestObject(3);
    testObj.erase(std::remove(testObj.begin(), testObj.end(), *temp), testObj.end());
    ///testObj.erase(std::remove(testObj.begin(), testObj.end(), std::not1(  )), testObj.end() );
//    mem_fun<bool, TestObject*>(&testObj::Get() == 1))

    ///std::mem_fun<bool, std::vector<TestObject*>>(&(temp->Equal));
    std::mem_fun<bool, TestObject>(&TestObject);
    int a = 0;
}

