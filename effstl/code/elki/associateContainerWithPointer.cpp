// ConsoleApplication1.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include <iostream>
#include <set>
#include <string>
#include <functional>
#include <algorithm>
#include <array>


// �Լ����� ���ø� : ���� Ŭ������ ��� ���ø��� ���� �� ����.
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

    // 1. �׳� insert
    std::set<std::string*> ssp;
    for (auto& data : rawData) {
        ssp.insert(new std::string(data));
    }

    // �ش� ������� ����ϸ� ���ϴ� ������ ������ �ʴ´�.
    // �׳� �Ѱ��ָ� '������ ��'���� �����ϰ� �ȴ�.
    // ���ϴ� ���� insert �� ���ĵ� ���·� ���� ��.
    std::for_each(ssp.begin(), ssp.end(), PrintFunc);

    std::cout << std::endl;

    // 2. Less �� �Լ��� Ÿ�� ����
    // �����Ͱ� ����Ű�� ������ ������ �� �ְԲ� �Ѵ�.
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

    // 3. DereferenceLess �� �Լ��� ���ø� Ÿ�� ����
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


    /* ���
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