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
    void PrintMember() const { std::cout << "num = " << _oddNum << std::endl; }
    int GetOddNum() { return _oddNum; }
    void AddNum(int num) { _oddNum += num; }
private:
    int _oddNum;
};

using MyClassMap = std::map<int, std::shared_ptr<MyClass>>;

void main()
{
    // 1. using lambda function
    MyClassMap myclassMap;

    for (size_t i = 0; i < 10; i++) {
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

    unsigned int count = 0;

    enum Mark {
        EAST = 0,
        WEST,
        NORTH,
        SOUTH,
    };

    // [capture] (parameter) -> return { statement; }
    auto MyFunc = [&](MyClass* myClass) -> Mark {
        count++;
        std::cout << "Lambda Call. count is " << count << std::endl;
        myClass->AddNum(count);
        auto num = myClass->GetOddNum() % 4;
        return static_cast<Mark>(num);
    };

    for (auto& it : myclassMap) {
        std::cout << "Mark is = " << MyFunc(it.second.get()) << std::endl;
    }
    /*Lambda Call. count is 1
    Mark is = 2
    Lambda Call. count is 2
    Mark is = 3
    Lambda Call. count is 3
    Mark is = 2
    Lambda Call. count is 4
    Mark is = 3
    Lambda Call. count is 5
    Mark is = 2
    Lambda Call. count is 6
    Mark is = 3
    Lambda Call. count is 7
    Mark is = 2
    Lambda Call. count is 8
    Mark is = 3
    Lambda Call. count is 9
    Mark is = 2
    Lambda Call. count is 10
    */
    return;
}