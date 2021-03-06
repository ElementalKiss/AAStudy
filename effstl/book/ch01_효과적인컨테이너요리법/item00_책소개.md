# 책소개

* 너희는 C++ STL 봉지를 뜯어 까먹을 줄은 알지만, 효과적으로 지져 먹고 데쳐 먹고 볶아 먹을 줄은 모른다. 이런 독자들을 위해 이 책을 썼다.
* 어이없는 상황을 피하고 우아하게 STL 프로그래밍을 하자.
* 자원 누수, 이식되지 않는 코드, 불확실하게 동작하는 코드 등의 고난을 피하는데 큰 도움이 될 것이다.

# STL의 정의, STL의 사용, STL의 확장에 대해 한마디

* 필자의 STL의 정의: 반복자를 가지고 동작하는 C++ 표준 라이브러리의 일부분
* 정의에는 표준 컨테이너(string 포함), iostream 라이브러리의 일부, 함수 객체, 알고리즘 모두 포함된다.
* 반복자 지원이 미약한 것을 제외: 표준 컨테이너 어뎁터(stack, queue, priority_queue), bitset, valarray, C++ Array
* C++ 라이브러리를 확장한 것들 또한 제외: 해쉬 포인터(hashed pointer), 싱글 링크드 리스트, 표준이 아닌 변이형 함수 객체들.
* STL에 꼭 알아두어야 할 필요가 있는 것들은 포함: 해쉬 포인터는 다음 버전의 C++ 표준에 들어갈 가능성이 높다.
* STL의 using에 초점을 두었다. 자체 알고리즘을 만들거나 반복자를 설계하는 방법에 대해선 기술하지 않음.
* STL 확장은 함수 객체에 대해서만 기술했다.

# 자료 인용에 대하여

* 인용 예시: 조스티스의 "The C++ Standard Library"[3], 오스턴 "Generic Programming and the STL"[4]
* 너무 자주 사용하기에 인용 번호를 빼는 항목: 표준안("International Standard for C++"[5]), Effective C++, More Effective C++.

# STL과 표준

* 컴파일러와 STL은 다양한 종류가 있다.
* STL 구현 코드는 다양한 플랫폼에서 동작하도록 코딩하는 습관을 들이면 C++ 코드의 품질(특히 이식성)을 향상시킬 수 있다.
* STL platform: 특정 컴파일러와 특정 STL 구현코드를 묶어 놓은 것
* 표준안을 잘 따르자.

# 참조 카운팅

* STL을 이야기하면 참조 카운팅은 필연적으로 따라온다.
* 포인터의 컨테이너를 기반으로 한 설계엔 참조 카운팅을 무시할 수 없다.(항목7, 항목 33) 그리고 string 클래스.
* More Effective C++ 항목 28, 29 를 참조하면 도움이 된다.

# string과 wstring

* string에 대해 설명한 모든 내용은 유니코드 문자열 클래스 wstring에도 적용된다.
* string과 char, char*의 관계는 wstring과 wchar_t, wchar_t*의 관계와 동일하다.
* 필자는 유니코드에 대해 직접적으로 언급하지 않지만 STL은 이를 지원하므로 알아두라는 뜻.

# 용어

## 컨테이너

* vector, string, deque, list는 표준 시퀀스 컨테이너(standard sequence container)
* set, multiset, map, multimap은 표준 연관 컨테이너(standard associative container)

## iterator의 종류

* 입력 반복자(input iterator): 현재 위치에 있는 데이터에 대해 읽기만 가능한 반복자.
* 출력 반복자(output iterator): 이와 반대로 쓰기만 가능한 반복자
* 입력 반복자와 출력 반복자는 입출력 스트림에 대한 읽기와 쓰기를 바탕으로 모델링. 흔히 사용되는 것은 istream_iterator, ostream_iterator.
* 순반향 반복자(forward iterator): 입출력 모두 할 수 있지만, 한 위치에서만 되풀이해서 읽고 쓸 수 있다. 뒤로 이동할 수 없다.
* 양방향 반복자(bidirectional iterator): 순반향 반복자의 기능을 모두 가지면 뒤로도 이동이 가능한 반복자. 표준 연관 컨테이너는 모두 이 반복자를 제공.

## 함수자, 함수 객체(functor and function object)

* 함수 호출 연산자(operator())를 오버로딩한 클래스를 함수자(functor) 클래스라고 한다.
* 이 클래스를 이용해 생성한 객체는 함수 객체(function object) 또는 함수자(functor)라고 부른다.

## 바인더(binder)

* bind1st, bind2nd 함수.

## STL의 특징

* 복잡도(complexity)를 보장하고 있다. (어떤 동작을 수행할 때 걸리는 작업량의 한도를 정해 두었다는 뜻)
* 이는 같은 문제에 대해 취할 수 있는 여러 접근 방법에 대한 정성적인 비교 도구.

## 시간 복잡도

* 상수 시간(constant time): n이 변하더라도 영향을 전혀 받지 않고 수행. 아주 빠른 시간 복잡도.
* 로그 시간(logarithmic time): 로그값에 비례하는 시간 복잡도. 간단한 EX) 백 만개의 요소 수행, 백 개의 요소 수행은 log(n^3) = 3log(n)이므로 3배 정도이다. STL의 연관 컨테이너가 지원하는 탐색 함수의 시간 복잡도는 대부분 로그 시간.
* 선형 시간(linear time): n의 증가에 따라 동일한 비율로 증가한다. (표준 알고리즘 하나인 count 해당)
* 큰 n의 값에 대해선 위의 복잡도 순서가 항상 맞지만 n이 상대적으로 작을 때는 이론상 복잡도가 높은 동작이라도 나은 성능을 보일 때가 있다.

# 예제 코드에 대하여

* inlucde는 넣지 않음.
* template에 들어가는 키워드는 class 대신 typename.

```
template<typename T> class Widget {...};
```

* 매개 변수 이름은 lhs, rhs를 선호.

```
class Widget {...};
bool operator==(const Widget& lhs, const Widget&* rhs);

...

if(x == y) {...}
```

# 수행 효율에 대한 항목은 여기에

* 수행 효율에 대해 빡세게 보고 싶다면 여기로.
* 항목 4,5,14,15,23,24,25,29,31,44,46

# Effective STL에 수록된 가이드라인에 대하여

* 가이드 라인은 가이드 라인일 뿐 짜고 싶은 방식으로 짜라.
* 하지만 확실히 알고 가라.