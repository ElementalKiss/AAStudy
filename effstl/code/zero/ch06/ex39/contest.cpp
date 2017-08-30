// contest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <algorithm>
#include "../../common/TestObject.h"
#include <vector>
#include <functional>
#include <algorithm>
#include <iostream>

// if
template <typename FItor, typename Pre>
FItor remove_if_my(FItor itor_begin, FItor itor_end, Pre p)
{
    itor_begin = find_if(itor_begin, itor_end, p);
    if (itor_begin == itor_end) return itor_begin;
    else {
        FItor next = itor_begin;
        return remove_copy_if(++next, itor_end, itor_begin, p);
    }
}

// stl
template<class _FwdIt, class _Pr> 
inline _FwdIt remove_if(_FwdIt _First, _FwdIt _Last, _Pr _Pred)
{	// remove each satisfying _Pred
    _First = _STD find_if(_First, _Last, _Pred);
    if (_First == _Last)
        return (_First);	// empty sequence, all done
    else
        return (_Rechecked(_First,_Remove_if(_Unchecked(_First), _Unchecked(_Last), _Pred)));
}


int _tmain(int argc, _TCHAR* argv[])
{

    class Test : std::unary_function<int, bool> {
    public:
        Test() : _count(0) {
        }

        bool operator()(int& type) {
            return ++_count == 3;
        }

    private:
        int _count;
    };

    std::vector<int> vw;
    vw.resize(10);

    for (int i = 0; i < 10; ++i) {
        vw[i] = i + 1;
    }

    vw.erase(remove_if_my(vw.begin(), vw.end(), Test()), vw.end());
}