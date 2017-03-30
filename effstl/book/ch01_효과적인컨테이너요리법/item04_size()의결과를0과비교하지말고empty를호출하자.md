# 항목 4: size()의 결과를 0과 비교할 생각이라면 차라리 empty를 호출하자.

# 왜 empty인가?

* empty 함수는 size가 0인지 여부를 반환하는 인라인 함수로 구현되어 있다.
* 상수 시간으로 수행된다.
* 일부 list 클래스에서 size는 선형 시간에 수행된다.

# list의 size는 왜 선형이 되는가?

* 리스트의 유용한 두 함수: 크기를 알아내는 size(), 중간을 잘라 붙이는 splice()
* size()와 splice() 둘 중 하나는 선형 시간으로 구현해야 한다.
* size를 상수 시간으로 구현하기 위해선 모든 작업에서 size를 업데이트 해야된다.
* splice 중 두 리스트에서 잘려나간 요소와 남은 요소를 세기 위해선 선형 시간으로 구현할 수 밖에 없다.
* splice를 세지 않고 선형 시간으로 붙여주면 splice는 상수 시간이 되지만 size()는 모두 세어야 하기 때문에 선형시간이 된다.
* 결론은 list가 어떻게 구현되었는지 모르니 empty를 선호하라는 뜻.

# VS2015 STL list의 size
```
    size_type& _Mysize() _NOEXCEPT
        {   // return reference to _Mysize
        return (_Get_data()._Mysize);
        }

    ...

    size_type size() const _NOEXCEPT
        {   // return length of sequence
        return (this->_Mysize());
        }
```

