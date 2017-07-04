// contest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <algorithm>
#include "../../common/TestObject.h"

int _tmain(int argc, _TCHAR* argv[])
{
    std::vector<TestObject> testObj;
    std::string v;
    
    testObj.reserve(5);
    v.reserve(5);

    for (int i = 1; i <= 5; ++i) {
        testObj.push_back(i);
        v.push_back(char(i));
    }

    //
    std::vector<TestObject>::reverse_iterator ri = std::find(testObj.rbegin(), testObj.rend(), 3);
    std::vector<TestObject>::iterator i(ri.base());

    testObj.erase(--ri.base());
    //testObj.erase((++ri).base());
    
    //
    std::string::reverse_iterator ri_c = std::find(v.rbegin(), v.rend(), 3);
    std::string::iterator i_c(ri_c.base());

    v.erase(--ri_c.base());
    //v.erase((++ri_c).base());
}

