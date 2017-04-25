#include <iostream>
#include <map>
#include <algorithm>
#include <memory>

class MyClass
{
public:
    MyClass() : _oddNum(0) { std::cout << "Call MyClass()" << std::endl; }
    virtual ~MyClass() { std::cout << "Call ~MyClass()" << std::endl; }

    MyClass(int oddNum) : _oddNum(oddNum) { std::cout << "Call MyClass(int oddNum)" << std::endl; }
    MyClass(const MyClass& myClass) = delete;

public:
    void MakeOdd() { if (_oddNum % 2 == 0) _oddNum++; }
    void PrintMember() { std::cout << "num = " << _oddNum << std::endl; }
private:
    int _oddNum;
};

using MyClassMap = std::map<int, std::shared_ptr<MyClass>>;

void main()
{
    // 1. using lambda function
    MyClassMap myclassMap;

    for (size_t i = 0; i < 10; i++)
    {
        myclassMap[i] = std::shared_ptr<MyClass>(new MyClass(i));
    }

    std::for_each(myclassMap.begin(), myclassMap.end(), [](MyClassMap::value_type& value) {
        MyClass* myClass = value.second.get();
        myClass->MakeOdd();
        myClass->PrintMember();
    }
    );

    /*
        num = 1
        num = 1
        num = 3
        num = 3
        num = 5
        num = 5
        num = 7
        num = 7
        num = 9
        num = 9
    */

    // 2. capture



    return;
}