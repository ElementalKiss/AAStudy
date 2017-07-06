# 항목 26: const_iterator나 reverse_iterator, const_reverse_iterator도 좋지만 역시 쓸 만한 것은 iterator이다

# STL 표준 컨테이너는 네 종류의 반복자 (iterator)를 제공함 
* iterator
* const_iterator
* reverse_iterator
* const_reverse_iterator

# container<T>라는 컨테이너가  있다면 iterator는 T*와 같은 개념, const_iterator는 const T*와 같은 개념이다.

# iterator와 const_iterator는 증가 시키면 다음 요소를 가리키지만  reverse_iterator와 const_reverse_iterator는 증가 시키면 이전 요소를 가리킴.

# vector<T>에서 제공 하는 insert, erase 함수들은 모두 매개 변수 타입으로 iterator를 받음.
* iterator insert(iterator position, const T& x);
* iterator erase(iterator position);
* iterator insert(iterator rangeBegin, iterator rangeEnd);

# iterator의 상호 변환 관계
![alt text](https://github.com/ElementalKiss/AAStudy/blob/master/effstl/book/image/item26.png "iterator relationship")
* 이미지 중 화살표만 그려진 관게는 컴파일러의 추론에 의해서 바로 변환이 가능
* 하지만 화살표에 함수 이름이 쓰여진 관계는 별도의 함수를 통해서만 변환 가능

# 상수 반복자와 역방향 반복자보다 비상수 반복자 타입인 iterator를 쓰는 것이 좋은 이유
* 어떤 형태의 insert, erase 멤버 함수는 무조건 iterator만을 넘겨야 함.
* const_iterator를 iterator로 암묵적으로 변환하는 방법은 없으며 방법이 있긴 하나 일반성도 떨어지며 효율에 대한 보장 불가
* reverse_iterator를 iterator로 변환할 수 있으나 변환한 후 약간의 조정이 필요.

# 신기한 iterator와 const_iterator의 관계
```
typedef deque<int> IntDeque;
typedef IntDeque::iterator iter;
typedef IntDeque::const_iterator ConstIter;

iter I;
ConstIter ci;

if (i == ci) ....
```

* 비상수 반복자와 상수 반복자 비교시 재수 없는 경우 컴파일이 되지 않을 수 있음
* 이유는 const_itertator에 대해 operator==를 멤버 함수로 선언해두었기 때문(비멤버 함수로 선언하여 두개의 매개 변수를 받게 하면 해결)
* 급하다면 피연산자의 순서를 바꾸면 해결됨. (if ci == i)
* 위 문제는 반복자를 이용하는 뺄셈에서도 발생!
  if (i-ci >= 3)은 불가 if (ci+3 <=i)는 가능

# 결론은 iterator를 사랑합시다!
