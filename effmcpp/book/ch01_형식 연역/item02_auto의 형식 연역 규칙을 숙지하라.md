# 항목 2: auto의 형식 연역 규칙을 숙지하라

* 한 가지 기이한 예외를 빼면, auto 형식 연역이 곧 템플릿 형식 연역이다.
* 템플릿 형식 연역에는 템플릿과 함수 및 매개 변수가 관여하지만, auto는 그런 것들을 전혀 다루지 않는데 말이다.

* 템플릿 형식 연역과 auto 형식 연역 사이에는 직접적인 대응 관계가 존재한다.
* 그 둘을 문자 그대로 알고리즘적으로 상호 변환할 수 있따.

* auto를 이용해서 변수를 선언할 때 auto는 템플릿의 T와 동일한 역할을 하며, 변수의 형식 지정자(type specifier)는 ParamType과 동일한 역할을 한다.

```cpp
auto x= 27;             // x의 형식 지정자는 그냥 auto 자체

const auto cx = x;      // 형식 지정자는 const auto

const auto& rx = x;     // 형식 지정자는 const auto&
```

* 위의 예들에서 x와 cx, rx의 형식들을 연역할 때, 컴파일러는 마치 선언마다 템플릿 함수 하나와 해당 초기화 표현식을 그 템플릿 함수를 호출하는 구문이 존재하는 것처럼 행동한다.

즉:
```cpp
template<typename T>                // x의 형식을 연역하기 위한
void func_for_x(T param);           // 개념적인 템플릿

func_for_x(27);                     // 개념적인 호출 : param에 대해
                                    // 연역된 형식이 바로 x의 형식

template<typename T>                // cx의 형식을 연역하기 위한
void func_for_cx(const T param);    // 개념적인 템플릿

func_for_cx(x);                     // 개념적인 호출 : param에 대해
                                    // 연역된 형식이 곧 cx의 형식

template<typename T>                // rx의 형식을 연역하기 위한
void func_for_rx(const T& param);   // 개념적인 템플릿

func_for_rx(x);                     // 개념적인 호출 : param에 대해
                                    // 연역된 형식이 바로 rx의 형식

```

* 앞에서 말했듯이, auto에 대한 형식 연역은 예외 하나를 빼면 템플릿 형식 연역과 동일하다.

* 항목 1 에서는 템플릿 형식 연역을 일반적인 함수 템플릿의 param의 형식 지정자인 ParamType의 특성에 따라 세 가지 경우로 나누어서 이야기했다.
* auto를 이용한 변수 선언에서는 변수의 형식 지정자가 ParamType의 역할을 하므로, auto 형식 연역 역시 세 가지 경우로 나뉜다.
- 경우 1: 형식 지정자가 포인터나 참조 형식이지만 보편 참조는 아닌 경우.
- 경우 2: 형식 지정자가 보편 참조인 경우.
- 경우 3: 형식 지정자가 포인터도 아니고 참조도 아닌 경우.

* 경우 1과 3은 앞에서 본 예와 같다.
```cpp
auto x = 27;            // 경우 3 (x는 포인터도 아니고 참조도 아님)
const auto cx = x;      // 경우 3 (cx 역시 둘 다 아님)
const auto& rx = x;     // 경우 1 (rx는 보편참조가 아닌 참조)
```

* 경우 2도 기대한 대로 작동한다.
```cpp
auto&& uref1 = x;       // x는 int 이자 왼값이므로
                        // uref1의 형식은 int&

auto&& uref2 = cx;      // cx는 const int이자 왼값이므로
                        // uref2의 형식은 const int&

auto&& uref3 = 27;      // 27은 int이자 오른값이므로
                        // uref3의 형식은 int&&
```

* auto 형식 연역에 대해서도 붕괴가 일어난다.
```cpp
const char name[] = "R. N.  Briggs";        // name의 형식은 const char[13]
auto arr1 = name;                           // arr1의 형식은 const char*
auto& arr2 = name;                          // arr2의 형식은 const char (&)[13]

void someFunc(int, double);                 // someFunc의 형식은 void(int, double)
auto func1 = someFunc;                      // func1의 형식은 void (*)(int, double)
auto& func2 = someFunc;                     // func2의 형식은 void (&)(int, double)
```
* 위의 예들에서 보듯이, auto의 형식 연역은 템플릿 형식 연역과 같이 작동하지만, 다른 점이 하나 있다.

* C++98 에서는 다음 두 가지 구문이 가능했다.
```cpp
int x1 = 27;
int x2(27);
```

* 균일 초기화(uniform initializetion)를 지원하는 C++11에서는 위의 두 구문과 더불어 다음과 같은 구문들을 사용할 수도 있다.
```cpp
int x3 = { 27 };
int x4{ 27 };
```

* 총 네 가지 구문이 존재하나, 결과적으로 값이 27인 int가 생긴다는 점은 모두 동일하다.

* int를 auto로 치환하면
```cpp
auto x1 = 27;       // 형식은 int, 값은 27
auto x2(27);        // 형식은 int, 값은 27

auto x3 = { 27 };   // 형식은 std::initialize_list<int>, 값은 { 27 }
auto x4{ 27 };      // 형식은 std::initialize_list<int>, 값은 { 27 }
```
* 이 선언들은 모두 문제없이 컴파일되나, 이전과는 의미가 좀 달라진 것들이 생겼다.
* auto에 대한 특별한 형식 연역 규칙 때문에 auto로 선언된 변수의 초기치(initializer)가 중괄호 쌍으로 감싸인 형태이면, 연역된 형식은 std::initializer_list이다.
```cpp
auto x5 = { 1, 2, ***3.0*** };      // 형식을 연역할 수 없다면 
                                    // std::initializer_list<T>의
                                    // T를 연역할 수 없음
```
* 위 코드에서 두 종류의 형식 연역이 진행된다.
- 첫 번째는 변수 선언에 auto를 사용했기 때문에 x5의 형식을 연역해야 한다는 사실에서 비롯된 형식 연역, x5의 초기치가 중괄호 형태이므로 x5의 형식은 반드시 std::initializer_list로 연역된다.
- std::initializer_list는 하나의 템플릿이기 때문에 std::initializer_list<T>의 T에 대한 형식을 연역해야 한다.
* *** 이 처럼 auto 형식 연역에서 중괄호 초기치가 관여할 때 차이가 난다. ***

* auto로 선언된 변수를 중괄호 초기치로 초기화하는 경우, 연역된 형식은 std::initializer_list의 한 인스턴스이다. 그러나 해당 템플릿 함수에 동일한 중괄호 초기치를 전달하면 형식 연역이 실패해서 컴파일이 거부된다.
```cpp
auto x = { 11, 23, 9 };     // x의 형식은 std::initializer_list<int>

template<typename T>
void f(T param);

f({ 11, 23, 9});            // 오류! T에 대한 형식은 역역할 수 없음
```
* 하지만 param의 형식이 어떤 알려지지 않은 T에 대한 std::initializer_list<T>인 템플릿에 그 중괄호 초기치를 전달하면 템플릿 형식 연역 규칙들에 의해 T의 형식이 제대로 연역된다.
```cpp
template<typename T>
void f(std::initializer_list<T> initList);

f({ 11, 23, 9});        // T는 int로 연역,
                        // initList의 형식은 std::initializer_list<int>로 연역
```

## 정리
* auto 형식 연역과 템플릿 형식 연역의 실질적인 차이는, ***auto는 중괄호 초기치가 std::initializer_list를 나타낸다(이 점을 명심하자)***고 가정하지만 템플릿 형식은 그렇지 않다는 것 뿐
* auto 형식 연역에는 중괄호 초기치에 대한 특별한 규칙이 있지만 템플릿은 그렇지 않는 이유가 궁금하지만 저자도 설득력 있는 원인을 찾지 못했다..

## 기억해 둘 사항들
* auto 와 템플릿 형식 연역은 대체로 같지만, auto 형식 연역은 중괄호 초기치가 std::initializer_list를 나타낸다고 가정하는 반면 템플릿 형식 연역은 그렇지 않다.
* 함수의 반환 형식이나 람다 매개변수에 쓰인 auto에 대해서는 auto 형식 연역이 아니라 템플릿 형식 연역이 적용된다.