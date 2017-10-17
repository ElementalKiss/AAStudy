# 형식 연역 서론

* 형식 연역은 영어로 type deduction
* C++98에서는 형식 연역에 관한 규칙이 한 종류 밖에 없었다.
* C++11에서는 그 규칙을 수정, 규칙 집합을 두 가지가 추가 되었다. auto와 decltype을 위한 규칙들.
* C++14에서는 auto와 decltype을 사용할 수 있는 문맥들이 확장.
* **형식 연역의 확장으로 C++ 소프트웨어의 적응성이 높아졌으나 프로그래머가 코드를 보고 의미를 추론하기가 어려워졌다.**
* 그래서 auto와 decltype의 동작을 제대로 이해하고 활용할 수 있어야 한다.

# 항목 1: 템플릿 형식 연역 규칙을 숙지하라.

* C++의 템플릿은 형식 연역은 성공작!
* 좋은 소식: auto는 템플릿에 대한 형식 연역을 기반으로 작동한다.
* 나쁜 소식: 한글 책이 무슨 말인지 도통 이해가 안 되지만, 덜 직관적인 경우가 있다고 한다.

## 일반적인 템플릿 선언 의사 코드

```cpp
template<class T> void function(ParamType param);
function(expr); // 어떤 표현식으로 function을 호출
// ex)
// template<class T> void function(const T& param);
//
// int x = 0;
// fcuntion(x);
```

## 세 가지 형식 연역 케이스

* 경우 1. ParamType이 포인터 또는 참조 형식이지만 보편 참조(Universal reference)는 아닌 경우.
* 경우 2. ParamType이 보편 참조인 경우.
* 경우 3. ParamType이 포인터도 아니고 참조도 아닌 경우.

## 경우 1: ParamType이 포인터 또는 참조 형식이지만 보편 참조(Universal reference)는 아닌 경우.

* 형식 연역은 다음과 같이 된다.

    1. 만일 expr이 참조 형식이면 참조 부분을 무시한다.
    2. 그런 다음 expr의 형식을 ParamType에 대한 패턴 매칭 방식으로 대응시켜 T의 형식을 결정한다.

```cpp
template<class T> void f(T& param);

    int x = 27;         // x는 int
    const int cx = x;   // cx는 const int
    const int& rx = x;  // rx는 const int인 x에 대한 참조
    f(x);               // T는 int, param의 형식은 int&
    f(cx);              // T는 const int, param의 형식은 const int&
    f(rx);              // T는 const int, param의 형식은 const int&
```

* const 성은 연역된 형식의 일부가 되어 T로 전달된다.
* rx의 참조성은 연역 과정에서 무시된다.
* const T&, T*도 우리가 알 고 있듯이 연역 된다.
* 여기까진 너무 쉬워서 하품이 나온다고 한다.

## 경우 2: ParamType이 보편 참조인 경우.

* 보편 참조 매개변수를 받은 경우에는 상황이 불투명해진다.
* 선언은 오른값 참조와 같은 모습.(T&&)
* 왼값 인수가 전달되면 오른값 참조와는 다른 방식으로 동작한다.(item24)

    1. 만일 expr이 왼값이면, T와 ParamType 둘 다 왼값 참조로 연역. 특이점: T가 참조 형식으로 연역되는 유일한 경우, 선언 구문(오른값)과 연역 형식(왼값)이 다르다.
    2. expr이 오른값이면, 경우 1처럼 규칙이 적용된다.

```cpp
template<class T> void f(T&& param);

    int x = 27;
    const int cx = x;
    const int& rx = x;

    f(x);           // x는 왼값, T는 int&, param은 int&
    f(cx);          // cx는 왼값, T는 const int&, param은 const int&
    f(rx);          // rx는 왼값, T는 const int&, param은 const int&
    f(27);          // 27은 오른값, T는 int, param은 int&&
```

* 해당 모습처럼 연역되는 이유는 item24에서 구체적으로 알려준다고 한다.
* 보편 참조가 관여하는 경우 왼값 인수와 오른값 인수에 대해 서로 다른 연역 규칙들이 적용됨을 기억하자.

## 경우 3: ParamType이 포인터도 아니고 참조도 아님.

* call by value.

    1. expr의 형식이 참조이면, 참조 부분은 무시된다.
    2. expr의 참조성을 무시한 후, 만일 expr이 const이면 그 const 역시 무시한다.
    3. 만일 volatile이면 그것도 무시한다.

```cpp
template<class T> void f(T param);

    int x = 27;
    const int cx = x;
    const int& rx = x;

    f(x);       // T와 param의 형식은 둘 다 int
    f(cx);      // 위와 동일
    f(rx);      // 위와 동일
```

* 특히 const성이 사라짐을 명심하자.

## 배열 인수

* 배열이 배열 첫 원소를 가리키는 포인터로 붕괴한다.(decay)

```cpp
const char name[] = "J. M. H. >_<"; // name의 형식은 const char[13]
const char* ptrName = name;         // 배열이 포인터로 붕괴된다.

template<class T> void f(T param);
f(name);                            // name은 배열이지만 T는 const char*로 연역된다.
```

* 교묘한 요령은 매개변수를 배열에 대한 참조로 선언하면 된다.
* T는 const char [13]으로 연역되고 매개변수는 const char (&)[13]으로 연역된다.

```cpp
template<class T> void f(T& param);
f(name);
```

## 함수 인수

* 배열과 같이 붕괴되는 녀석이니 알아두면 좋다.

```cpp
void someFunc(int, double);

template<class T> void f1(T param);
template<class T> void f2(T& param);

f1(someFunc);       // param의 함수 포인터로 연역, 형식은 void (*)(int, double)
f2(someFunc);       // param은 함수 참조로 연역, 형식은 void (&)(int, double)
```

## 정리

* 템플릿 형식 연역 도중에 참조 형식의 인수들은 참조성이 무시된다.
* 보편 참조는 왼값과 오른값에 대한 처리 방식이 다르므로 알아두자.
* call by value 방식의 매개변수에 대한 형식 연역 과정에서 const성과 volatile이 사라진다.
* 배열과 함수 이름의 인수는 포인터로 붕괴된다. 인수를 참조로 사용하면 포인터로 붕괴되지 않는다.