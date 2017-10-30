# 항목 2: auto의 타입 추론 규칙을 숙지하라

* 한 가지 예외를 빼면, auto 타입 추론이 곧 템플릿 타입 추론이다.
* 템플릿 타입 추론과 auto 타입 추론 사이에는 직접적인 대응 관계가 존재한다.
* 그 둘을 문자 그대로 알고리즘적으로 상호 변환할 수 있다.

* 항목 1에서 다음 처럼 템플릿 타입 추론을 설명했다.
```cpp
template<typename T>
void f(ParamType param);    // 일반적인 함수 템플릿

f(expr);                    // f 호출에서 expr로 T와 ParamType의 타입을 추론
```

* auto를 이용해서 변수를 선언할 때 auto는 템플릿의 T와 동일한 역할을 하며, 변수의 타입 지정자(type specifier)는 ParamType과 동일한 역할을 한다.
```cpp
auto x= 27;             // x의 타입 지정자는 그냥 auto 자체

const auto cx = x;      // 타입 지정자는 const auto

const auto& rx = x;     // 타입 지정자는 const auto&
```

* 위의 예들에서 x와 cx, rx의 타입들을 추론할 때, 컴파일러는 마치 선언마다 템플릿 함수 하나와 해당 초기화 표현식을 그 템플릿 함수를 호출하는 구문이 존재하는 것처럼 행동한다.

즉:
```cpp
template<typename T>                // x의 타입을 추론하기 위한
void func_for_x(T param);           // 개념적인 템플릿

func_for_x(27);                     // 개념적인 호출 : param에 대해
                                    // 추론된 타입이 바로 x의 타입

template<typename T>                // cx의 타입을 추론하기 위한
void func_for_cx(const T param);    // 개념적인 템플릿

func_for_cx(x);                     // 개념적인 호출 : param에 대해
                                    // 추론된 타입이 곧 cx의 타입

template<typename T>                // rx의 타입을 추론하기 위한
void func_for_rx(const T& param);   // 개념적인 템플릿

func_for_rx(x);                     // 개념적인 호출 : param에 대해
                                    // 추론된 타입이 바로 rx의 타입

```

* 앞에서 말했듯이, auto에 대한 타입 추론은 예외 하나를 빼면 템플릿 타입 추론과 동일하다.

* 항목 1 에서는 템플릿 타입 추론을 일반적인 함수 템플릿의 param의 타입 지정자인 ParamType의 특성에 따라 세 가지 경우로 나누어서 이야기했다.
* auto를 이용한 변수 선언에서는 변수의 타입 지정자가 ParamType의 역할을 하므로, auto 타입 추론 역시 세 가지 경우로 나뉜다.

- 경우 1: 타입 지정자가 포인터나 참조 타입이지만 보편 참조는 아닌 경우.
- 경우 2: 타입 지정자가 보편 참조인 경우.
- 경우 3: 타입 지정자가 포인터도 아니고 참조도 아닌 경우.

* 경우 1과 3은 앞에서 본 예와 같다.
```cpp
auto x = 27;            // 경우 3 (x는 포인터도 아니고 참조도 아님)
const auto cx = x;      // 경우 3 (cx 역시 둘 다 아님)
const auto& rx = x;     // 경우 1 (rx는 보편참조가 아닌 참조)
```

* 경우 2도 기대한 대로 작동한다.
```cpp
auto&& uref1 = x;       // x는 int 이자 L-value이므로
                        // uref1의 타입은 int&

auto&& uref2 = cx;      // cx는 const int이자 L-value이므로
                        // uref2의 타입은 const int&

auto&& uref3 = 27;      // 27은 int이자 R-value이므로
                        // uref3의 타입은 int&&
```

* auto 타입 추론에 대해서도 붕괴가 일어난다.
```cpp
const char name[] = "R. N.  Briggs";        // name의 타입은 const char[13]
auto arr1 = name;                           // arr1의 타입은 const char*
auto& arr2 = name;                          // arr2의 타입은 const char (&)[13]

void someFunc(int, double);                 // someFunc의 타입은 void(int, double)
auto func1 = someFunc;                      // func1의 타입은 void (*)(int, double)
auto& func2 = someFunc;                     // func2의 타입은 void (&)(int, double)
```
* 위의 예들에서 보듯이, auto의 타입 추론은 템플릿 타입 추론과 같이 작동하지만, 다른 점이 하나 있다.

* C++98 에서는 다음 두 가지 구문이 가능했다.
```cpp
int x1 = 27;
int x2(27);
```

* 균일 초기화(uniform initialization)를 지원하는 C++11에서는 위의 두 구문과 더불어 다음과 같은 구문들을 사용할 수도 있다.
```cpp
int x3 = { 27 };
int x4{ 27 };
```

* 총 네 가지 구문이 존재하나, 결과적으로 값이 27인 int가 생긴다는 점은 모두 동일하다.

* int를 auto로 치환하면
```cpp
auto x1 = 27;       // 타입은 int, 값은 27
auto x2(27);        // 타입은 int, 값은 27

auto x3 = { 27 };   // 타입은 std::initialize_list<int>, 값은 { 27 }
auto x4{ 27 };      // 타입은 std::initialize_list<int>, 값은 { 27 }
```
* 이 선언들은 모두 문제없이 컴파일되나, 이전과는 의미가 좀 달라진 것들이 생겼다.
* auto에 대한 특별한 타입 추론 규칙 때문에 auto로 선언된 변수의 초기치(initializer)가 중괄호 쌍으로 감싸인 형태이면, 추론된 타입은 std::initializer_list이다.
```cpp
auto x5 = { 1, 2, 3.0 };    // 타입을 추론할 수 없다면 
                            // std::initializer_list<T>의
                            // T를 추론할 수 없음
```
* 위 코드에서 두 종류의 타입 추론이 진행된다.
- 첫 번째는 변수 선언에 auto를 사용했기 때문에 x5의 타입을 추론해야 한다는 사실에서 비롯된 타입 추론, x5의 초기치가 중괄호 형태이므로 x5의 타입은 반드시 std::initializer_list로 추론된다.
- std::initializer_list는 하나의 템플릿이기 때문에 std::initializer_list<T>의 T에 대한 타입을 추론해야 한다.
* **이 처럼 auto 타입 추론에서 중괄호 초기치가 관여할 때 차이가 난다.**
  
* auto로 선언된 변수를 중괄호 초기치로 초기화하는 경우, 추론된 타입은 std::initializer_list의 한 인스턴스이다. 그러나 해당 템플릿 함수에 동일한 중괄호 초기치를 전달하면 타입 추론이 실패해서 컴파일이 거부된다.
```cpp
auto x = { 11, 23, 9 };     // x의 타입은 std::initializer_list<int>

template<typename T>
void f(T param);

f({ 11, 23, 9});            // 오류! T에 대한 타입은 추론할 수 없음
```
* 하지만 param의 타입이 어떤 알려지지 않은 T에 대한 std::initializer_list<T>인 템플릿에 그 중괄호 초기치를 전달하면 템플릿 타입 추론 규칙들에 의해 T의 타입이 제대로 추론된다.
```cpp
template<typename T>
void f(std::initializer_list<T> initList);

f({ 11, 23, 9});        // T는 int로 추론,
                        // initList의 타입은 std::initializer_list<int>로 추론
```

* C++14 에서는 함수의 반환 형식을 auto로 지정해서 컴파일러가 추론하게 만들 수 있다. 람다의 매개 변수 선언에 auto를 사용하는 것도 가능하다.
* 그러나 그러한 용법들에는 auto 형식이 아니라 **템플릿 타입 추론** 규칙들이 적용된다.
* 그래서 중괄호 초기치를 돌려주는 함수의 반환형식을 auto로 지정하면 컴파일이 실패한다.
```cpp
auto createInitList()
{
    return {1, 2, 3};   Error! {1, 2, 3}의 타입을 추론할 수 없음
}
```

* C++14 람다의 매개변수 형식 명세에 auto를 사용하는 경우에도 마찬가지 이유로 컴파일이 실패한다.
```cpp
std::vector<int> v;
...
auto resetV = [&v](const auto& newValue) { v = newValue; };     // C++14
...
resetV({1, 2, 3});  // Error! {1, 2, 3}의 타입을 추론할 수 없음
```

## 정리
* auto 타입 추론과 템플릿 타입 추론의 실질적인 차이는, **auto는 중괄호 초기치가 std::initializer_list를 나타낸다(이 점을 명심하자)** 고 가정하지만 템플릿 타입은 그렇지 않다는 것 뿐
* auto 타입 추론에는 중괄호 초기치에 대한 특별한 규칙이 있지만 템플릿은 그렇지 않는 이유가 궁금하지만 저자도 설득력 있는 원인을 찾지 못했다..

## 기억해 둘 사항들
* auto 와 템플릿 타입 추론은 대체로 같지만, auto 타입 추론은 중괄호 초기치가 std::initializer_list를 나타낸다고 가정하는 반면 템플릿 타입 추론은 그렇지 않다.
* 함수의 반환 타입이나 람다 매개변수에 쓰인 auto에 대해서는 auto 타입 추론이 아니라 템플릿 타입 추론이 적용된다.
