// contest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <algorithm>
#include "../../common/TestObject.h"
#include <vector>
#include <functional>
#include <algorithm>
#include <iostream>
#include <set>
#include <ostream>
#include  <iterator>

template<typename FPType>
FPType avarage(FPType val1, FPType val2)
{
    return (val1 + val2) / 2;
}

template<typename FPType>
struct Avarage : public std::binary_function<FPType, FPType, FPType> {
    FPType operator()(FPType val1, FPType val2) const
    {
        return avarage(val1, val2);
    }
};
template<typename inputIter1, 
         typename inputIter2>
void writeAvarages(inputIter1 begin1, 
                   inputIter1 end1, 
                   inputIter2 begin2, 
                   std::ostream& s)
{
    std::transform(
        begin1, end1, begin2, 
        std::ostream_iterator<typename std::iterator_traits<inputIter1>::value_type>(s, "\n"),
        Avarage<typename std::iterator_traits<inputIter1>::value_type>()
    );
}

inline
bool doubleGrater(double d1, double d2)
{
    return d1 > d2;
}

int _tmain(int argc, _TCHAR* argv[])
{
    std::set<std::string> s;
  
    std::transform(s.begin(), s.end(), 
        std::ostream_iterator<std::string::size_type>(std::cout, "\n"),
        mem_fun_ref(&std::string::size));

    struct StringSize : public std::unary_function<std::string, std::string::size_type> {
        std::string::size_type operator()(const std::string& s) const
        {
            return s.size();
        }
    };

    std::transform(s.begin(), s.end(), 
        std::ostream_iterator<std::string::size_type>(std::cout, "\n"), 
        StringSize());
   
    std::string ss;
    std::string temp_1 = "";
    std::ostream* temp;
    writeAvarages(ss.begin(), ss.end(), ss.begin(), *temp);
    // 컴파일만 확인해본 코드들임..
}