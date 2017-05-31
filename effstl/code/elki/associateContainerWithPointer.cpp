// ConsoleApplication1.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include <iostream>
#include <set>
#include <string>
#include <functional>
#include <algorithm>
#include <array>


// 함수자의 템플릿 : 지역 클래스는 멤버 템플릿을 가질 수 없음.
struct DereferenceLess
{
    template <typename PtrT>
    bool operator()(PtrT pT1, PtrT pT2) const {
        return *pT1 < *pT2;
    }
};

int main(int argc, const char* argv[])
{
    auto PrintFunc = [](const std::string* ps) {
        std::cout << ps->c_str() << std::endl;
    };

    std::array<char *, 5> rawData = { "elki", "ralf", "pjel", "reds", "zero" };

    // 1. 그냥 insert
    std::set<std::string*> ssp;
    for (auto& data : rawData) {
        ssp.insert(new std::string(data));
    }

    // 해당 방법으로 출력하며 원하는 순서가 나오지 않는다.
    // 그냥 넘겨주면 '포인터 값'으로 정렬하게 된다.
    // 원하는 것은 insert 시 정렬된 상태로 들어가는 것.
    std::for_each(ssp.begin(), ssp.end(), PrintFunc);

    std::cout << std::endl;

    // 2. Less 비교 함수자 타입 정의
    // 포인터가 가리키는 값으로 정렬할 수 있게끔 한다.
    struct StringPtrLess :
        public std::binary_function<const std::string*, const std::string*, bool>
    {
        bool operator()(const std::string* ps1, const std::string* ps2) const {
            return *ps1 < *ps2;
        }
    };

    typedef std::set<std::string*, StringPtrLess> StringPtrSet;
    StringPtrSet lessFunctorSsp;

    for (auto& data : rawData) {
        lessFunctorSsp.insert(new std::string(data));
    }
    
    std::for_each(lessFunctorSsp.begin(), lessFunctorSsp.end(), PrintFunc);

    std::cout << std::endl;

    // 3. DereferenceLess 비교 함수자 템플릿 타입 정의
    typedef std::set<std::string*, DereferenceLess> DereferencePtrSet;
    DereferencePtrSet dereferenceLessFunctorSsp;

    for (auto& data : rawData) {
        dereferenceLessFunctorSsp.insert(new std::string(data));
    }

    std::for_each(dereferenceLessFunctorSsp.begin(), dereferenceLessFunctorSsp.end(), PrintFunc);

    // Delete heap alloc
    auto DelStringPtrFunc = [](const std::string* delStr) {
        if (delStr) {
            delete delStr;
            delStr = nullptr;
        }
    };

    std::for_each(ssp.begin(), ssp.end(), DelStringPtrFunc);
    ssp.clear();

    std::for_each(lessFunctorSsp.begin(), lessFunctorSsp.end(), DelStringPtrFunc);
    lessFunctorSsp.clear();

    std::for_each(dereferenceLessFunctorSsp.begin(), dereferenceLessFunctorSsp.end(), DelStringPtrFunc);
    dereferenceLessFunctorSsp.clear();


    /* 출력
        koko
        love
        elki
        momo

        elki
        koko
        love
        momo

        elki
        koko
        love
        momo
    */

    return 0;
}