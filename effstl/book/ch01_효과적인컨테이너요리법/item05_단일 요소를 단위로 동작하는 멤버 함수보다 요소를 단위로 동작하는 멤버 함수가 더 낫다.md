# 항목 5 : 단일 요소를 단위로 동작하는 멤버 함수보다 요소의 범위를 단위로 동작하는 멤버 함수가 더 낫다

# 돌발 퀴즈

## 문제

* 두 개의 백터 v1과 v2가 있을 때, v1의 내용을 v2의 뒤의 반과 같게 만드는 가장 빠른 방법.

## 정답

```
    v1.assign(v2.begin() + v2.size()/2, v2.end());
```

## 퀴즈의 의미

* assign이라는 멤버 함수가 있다는 사실을 알려주기 위해
* 제목의 실 예를 보여주기 위해.

# 범위 단위 멤버 함수를 사용했을 때 장점

* 불필요한 함수 호출을 줄인다. 복사 작업 시 루프를 돌 필요 없이 std::copy 한번만 호출하면 된다.
* 당김처리로 인한 부하를 막을 수 있다. 객체를 담았다고 가정할 시 대입 연산자와 복사 생성자의 호출을 줄일 수 있다.
* 메모리 할당면에서 효율적이다. 루프와 다르게 인자를 통해 크기를 예상할 수 있다.

# 모든 시퀀스 컨테이너들이 이러한가?

* vector와 string은 동일한 양상을 보인다.
* deque의 경우 비슷하긴 하지만 메모리 관리 방식이 조금 달라 '반복적인 메모리 재할당'에 관해선 조금 다르다.
* list도 범위 버전이 성능이 우수하다. 단일 버전은 next와 prev가 새 노드가 추가 될 때마다 다음과 이전 노드를 업데이트 하는 비용이 든다. 하지만 범위 버전은 해당 노드가 몇 개인지 알 수 있으므로 이 비용이 들지 않는다.

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