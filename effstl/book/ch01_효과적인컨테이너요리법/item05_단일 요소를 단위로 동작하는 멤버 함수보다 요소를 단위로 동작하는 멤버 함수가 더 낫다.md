# 항목 5 : 단일 요소를 단위로 동작하는 멤버 함수보다 요소의 범위를 단위로 동작하는 멤버 함수가 더 낫다

# 범위 생성(Range Construction)
```
    contanier::container(InputIterator begin, // 범위의 시작
                         InputIterator end);  // 범위의 끝
```

# 범위 삽입(Range Insertion)
```
    void contanier::insert(iterator position,    // 범위를 삽입할 위치
                           InputIterator begin,) // 삽입할 범위의 시작
                           InputIterator end);   // 삽입할 범위의 끝

    // 연관 컨테이너
    void contanier::insert(InputIterator begin, InputIterator end);
```


# 범위 삭제(Range Erasure)
```
    // 시퀀스 컨테이너
    iterator contanier::erase(iterator begin, iterator end)

    // 연관 컨테이너
    void contanier::erase(iterator begin, iterator end)
```

# 범위 대입(Range Assignment)
```
    // 시퀀스 컨테이너
    iterator contanier::assign(InputIterator begin, InputIterator end)
```