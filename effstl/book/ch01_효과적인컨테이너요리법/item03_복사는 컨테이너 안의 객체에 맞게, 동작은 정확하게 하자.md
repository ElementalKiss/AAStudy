# 항목 3 : 복사(Copy)는 컨테이너 안의 객체에 맞게 비용은 최소화하며, 동작은 정확하게 하자

# STL 컨테이너가 하는 일

* 컨테이너의 본분은 객체를 담아 관리하는 것
* 컨테이너에서 어떤 객체를 넣고 뺄 때 객체가 복사된다.
* "객체 복사", 이것이 바로 STL이 사는 법
* vector,string,deque 등에 객체를 insert, erase 하면 객체들은 복사를 통해 밀려나고 밀려온다. 복사 비용이 큰 객체라면..?
* next_permutation, previous_permutation, remove, unique 등의 정렬 알고리즘이나 rotate, reverse 같은 것들을 호출하면 컨테이너의 객체들이 이동한다.
* 원리는 객체의 복사용 멤버 함수를 사용하며 복사 생성자와 복사 대입 연산자가 사용된다. (정의하지 않으면 컴파일러가 자동 생성)
* STL에서 복사는 거의 모든 순간에 일어난다.

# 복사로 인한 문제

* 복사 비용이 큰 객체를 컨테이너에 넣을 땐 수행 성능의 병목현상이 일어난다.
* 복사 동작이 단순한 복사의 의미를 넘어선다면 더 큰 슬픔이..(동적할당 등?)
* **기본(base) 클래스 객체의 컨테이너에 파생(derived) 클래스 객체를 넣을 경우 복사 중에 __데이터가 잘려나갈 수 있다(Slice Problem)__** (항목 22에서 자세히)
* 정적 인스턴스를 말하는 것 일듯

# 복사에 대한 대안은 포인터

* 이러한 문제의 대안은 포인터를 컨테이너에 담는 것!
* 포인터는 복사도 빠르고, 잘려나갈 일도 없다.
* 단, 포인터의 컨테이너는 STL에 관련된 골칫거리를 안고 있다. (항목 7, 33에서 자세히)
* 위의 골칫거리를 피하는 데에는 smart pointer를 사용하는 것도 한 가지 방법
* 그렇다고 하지만 실제로 STL은 일반적으로 불필요한 복사 "생성"을 피하도록 설계
* C/C++의 배열과 비교해 보면 알 수 있다.
```
  widget w[max_num]; // num개의 기본 객체를 '생성'
  vector<widget> vw; // 추가하기 전에는 '생성'되지 않음
```

# 어찌됐던 배열보단 STL이 훨씬 좋다!