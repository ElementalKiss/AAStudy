# 항목 39 : 술어 구문은 순수 함수로 만들자

## 용어 설명
술어 구문 : predicate 주어에 대해 주장되는(긍정 또는 부정하는)는 의미 , bool IsHuman('cat')

순수 함수 : 함수의 매개 변수에 종속된 함수 f(x,y) 라 한다면 return의 값은 언제나 x,y의 변화에 의해서만 변경 될수 있는 

술어 구문 클래스 : operator()가 술어 구문인 함수자 클래스

## 주 내용
stl에서 사용되는 algorithm 등에서 술어구문 함수를 요구 하는 경우에 stl의 구현에 따라 비정상 동작 하는 경우를 상정한 예시

// 
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

// my 
template <typename FItor, typename Pre>
FItor remove_if_my(FItor itor_begin, FItor itor_end, Pre p)
{
    itor_begin = find_if(itor_begin, itor_end, p);
    if (itor_begin == itor_end) return itor_begin;
    else {
        FItor next = itor_begin; // 여기서 복사가 일어나 면서 내용이 초기화 된다.
        return remove_copy_if(++next, itor_end, itor_begin, p);
    }
}

// stl
template<class _FwdIt, class _Pr> 
inline _FwdIt remove_if(_FwdIt _First, _FwdIt _Last, _Pr _Pred)
{   // remove each satisfying _Pred
    _First = _STD find_if(_First, _Last, _Pred);
    if (_First == _Last)
        return (_First);    // empty sequence, all done
    else
        return (_Rechecked(_First,_Remove_if(_Unchecked(_First), _Unchecked(_Last), _Pred)));
}

## 결론
remove_if 의 구현에 따라 술어구문 클레스 작성에 유의하자