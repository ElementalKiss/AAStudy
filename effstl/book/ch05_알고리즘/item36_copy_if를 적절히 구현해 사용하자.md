# 항목 36: copy_if를 적절히 구현해 사용하자

## copy_if
* copy_if 정의 : 조건에 만족하는 데이터를 복사
* STL에 copy_if가 없다. 따라서 구현 해 쓰자
* 책을 썼을 당시엔 copy_if가 빠져 있지만, 후에 copy_if가 추가 됨.

## 예시
* 객체들 중 조건에 맞는 객체들만 복사하고 싶다.

```
bool isDefective(const Widget& w)

vector<Widget> widgets;
...
copy_if(widgets.begin(), widgets.end(),
        ostream_iterator<Widget>(cerr, "\n"),
        isDefective)
```

## copy_if 만들자 1 (썩 좋지 않음)
```
template< typename InputIterator,
          typename OutputIterator,
          typename Predicate>
OutputIterator copy_if(InputIterator begin,
                        IutputIterator end,
                        OutputIterator destBegin,
                        Predicate p)
{
    return remove_copy_if(begin, end, destBegin, not1(p));
}
```

* 조건이 참인 데이터를 모두 복사한다 == 조건이 참이 아닌 데이터를 제외하고 모두 복사한다.
* not1()은 함수 포인터에 바로 적용 불가.
* ptr_fun()로 감싸면 해결되지만, 이렇게 되면 변환이 가능한 함수 객체를 넘겨줘야하는 불편함이 생긴다 함.

## copy_if 만들자 2 (완성)
```
template< typename InputIterator,
          typename OutputIterator,
          typename Predicate>
OutputIterator copy_if(InputIterator begin,
                        IutputIterator end,
                        OutputIterator destBegin,
                        Predicate p)
{
    while (begin != end) 
    {
        if (p(*begin)) *destBegin++ = *begin;
        ++begin;
    }
    return destBegin;
}

```

## 결론
* 지금은 사용 가능 함.
