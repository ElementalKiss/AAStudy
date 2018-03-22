# 항목 30: Perfect forwarding이 실패하는 경우들을 잘 알아두라
## 개요

- Perfect forwarding은 말 그대로 완벽하지 않다.
- 완벽하지 않은 경우를 알고 사용하자.
- 그치만 대부분 정상 동작한다.

---

## Perfect forwarding이란?

- forwarding ?
  + 한 함수가 자신의 인수들을 다른 함수에 전달함을 의미

- perfect ?
  + 둘째 함수가 첫 함수가 받았던 것과 동일한 객체들을 받게 함을 의미
  + (전달받는) (전달하는)

- perfect forwarding ?
  + 전달하는 객체의 주요 특징까지 전달해야 함.
  + 즉, type, lValue, rValue, const, volatile 특징 모두를 전달해야 함을 의미

```C++
template<typename T>
void fwd(T&& param)		// 임의의 인수를 받는다
{
  f(std::forward<T>(param)); 	// 그 인수를 f에 전달한다.
}

// 가변인수(variadic) 템플릿
template<typename... Ts>
void fwd(Ts&&... params)
{
  f(std::forward<Ts>(params)...);
}
```

- 가변인수 (variadic)이 Perfect forwarding의 기본 공통적인 형태
  + 표준 컨테이너의 emplace 함수들
  + Smart Pointer의 Factory function (`std::make_shared`, `std::make_unique`)

---

## Perfect forwarding의 실패

- 동일한 인자를 f()와 fwd()로 호출 했을 때의 행위가 다르다면 실패이다.
```C++
f ( exp );	// 이 호출이 하는 일과
fwd ( exp );	// 이 호출이 하는 일이 다르다면 
		// Perfect forwarding 하지 못함을 의미한다.
```

### 1. 중괄호 초기치 (Braced initializers)

```C++
f의 선언
void f(const std::vector<int>& v);

f({ 1, 2, 3 });   // OK; "{1, 2, 3}"는 암묵적 
		  // std::vector<int>로 변환
fwd({ 1, 2, 3 }); // 오류! 컴파일 되지 않음
```

- 중괄호 초기치의 Perfect forwarding의 실패 원인을 알자
  + f() 호출 시, 컴파일러는 f의 선언 const std::vector<int>&를 확인할 수 있다.
  + fwd() 호출 시에는 상황이 다르다.
    fwd()에 전달받은 인수들의 타입추론이 일어나고,
    추론된 인수의 타입을 f의 선언 const std::vector<int>&와 비교한다.
  + 위 과정에서 다음을 하나라도 만족되면 Perfect forwarding은 실패한다.
  + 1 fwd의 매개변수들 중 하나 이상에 대해 컴파일러가 타입 추론하지 못한다.
  + 2 컴파일러가 타입추론을 잘 못 추론한다.
    결과는 잘 못 추론의 결과로 컴파일 실패,
    또는 컴파일 성공 시, 예기치 못한 행동 ( f의 overload )

```C++
fwd({ 1, 2, 3 }); // 컴파일러 오류
// void fwd(T &&): could not deduce template argument for 'T'
// fwd: no matching overloaded function found
```

- 코드 흐름
  + void fwd(Ts&&... params)
  + non-deduced context (비연역 문맥, 비추론 문맥)
  + fwd 매개 변수가 std::initializer_list가 될 수 없는 타입으로 선언 됨,
  + fwd 호출에 쓰인 표현식 { 1, 2, 3 }의 타입을 컴파일러가 추론하는 것은 금지.
  + 컴파일러 오류

```C++
// 우회 방법
auto il = { 1, 2, 3 };  // li의 타입 추론 결과는
			// std::initializer_list<int>이다.
fwd(il);		// OK; il이 f로 완벽하게 전달 된다.
```

---

### 2. 널 포인터를 뜻하는 0 or NULL
- 0, NULL은 int로 인식
  + nullptr을 사용하자.
  + item 8을 보자

---

### 3. 선언만 된 정수 static const 및 constexpr 자료 멤버

- 위는 컴파일러가 const propagation (const 전파)를 하기에 따로 메모리를 두지 않는다.

```C++
class Widget {
publid:
  static constexpr std::size_t MinVals = 28; // MinVals의 선언
};
				 	     // MinVals의 정의는 없음

std::vector<int> widgetData;
widgetData.reserve(Widget::MinVals);         // MinVals의 사용
```

- MinVals 부분을 28이라는 값으로 치환 배치 함. (const propagation)

```C++
void f(std::size_t val);

f(Widget::MinVals); 	// OK; f(28)
fwd(Widget::MinVals);   // Error; 링크 실패
```

- 컴파일 되지만 링크 실패.
- 참조할 메모리가 존재하지 않음.
- 우회 방법은 메모리 참조 가능하게 정의를 해라.

```C++
constexpr std::size_t Widget::MinVals;	// Widget.cpp
```

---

### 4. Overload된 함수 이름과 템플릿 이름

```C++
// void f(int (*pf)(int));
void f(int pf(int));	// 비 포인터 구문 (*1 참고)

int processVal(int value);
int processVal(int value, int priuority);

f(processVal);  // ok;
fwd(processVal); // error; 어떤 processVal인지?
```

- f(processVal)의 경우는 컴파일러가 일치하는 함수를 찾아냄,
  + fwd는 못 찾아냄,
  + processVal은 함수 포인터도 아니며, 공유하는 하나의 이름일 뿐이다.
  + 따라서 processVal은 타입이 없으며, 타입이 없으면 타입추론도 없다.

```C++
template<typename T>
T workOnVal(T param)	// 값들을 처리하는 템플릿 함수
{ ... }

fwd(workOnVal);		// 오류! workOnVal의 어떤 인스턴스인지?
```
- 템플릿으로 인한 workOnVal은 다수의 함수로 구성될 것이다.
  + 그 중 어느 것인지 타입추론할 수 없기에 오류를 나타낸다.

- 우회 방법
  + 명시적 지정하자.

```C++
using ProcessFuncType = int (*)(int);	// typedef
ProcessFuncType processValPtr = processVal; // processVal에 필요한 서명을 명시

fwd(processValPtr);	// OK
fwd(static_cast<ProcessFuncType>(workOnVal)); // OK
```

---

### 5. BitField (비트 필드)
```C++
struct IPv4Header {
  std::uint32_t version:4,
		IHL:4,
		DSCP:6,
		ECN:2,
		totalLength:16;
// ...
};

void f(std::size_t sz);

IPv4Header h;
// ... init
f(h.totalLength); // OK;

fwd(h.totalLength); // Error;
```
- h.totalLength는 비const 비트 필드이다.
  + 비const 참조는 절대로 비트필드에 묶이지 않아야 한다.
  + 이유는 비트필드들은 컴퓨터 워드의 임의의 일부분으로 구성 될 수 있는데,
  그런 일부 비트를 직접적으로 지칭하는 방법은 없다.
  c++에서 직접 가리킬 수 있는 가장 작은 단위는 char이다. 
  따라서 참조를 임의의 비트들에 묶는 방법은 없다.
  + 정리하면 IPv4Header는 uint32_t로 비트 필드 구조체이며,
  비트필드 구조체 안을 직접적으로 참조할 방법은 없다.

- 우회 방법
  + 비트필드의 값에 접근 시, 복사본을 받게 된다.
  + 이 복사본을 참조하게 하자.
```C++
// 비트필드 값을 복사한다. 이런 초기화 구문에 대해서는 item 6을 보라.
auto length = static_cast<std::uint16_t>(h.totalLength); // (*2 참고)
fwd(length);	// 복사본 전달
```

---

## 결론

- 대부분의 경우 Perfect forwarding은 정상 동작한다.
  + 그러나 정상동작 하지 않을 때에는 상위의 5가지 경우를 기억하자.
  Braced initializers, 0 or NULL as nullptr, static const로 선언된 자료타입,
  중복적재된 함수 이름과 템플릿 이름, 비트 필드
- 그에 대한 우회책은 생각보다 간단하다.

---

## 각주
### (*1) 비 포인터 
```C++
void TakeArr(int arr[]);  // 1
void TakePtr(int* arr);  // 2
void TakeFunc(int f(int));  // 3
void TakeFuncPtr(int (*f)(int));  // 4
```
 - 배열 포인터 붕괴 현상과 동일하게 함수 포인터 붕괴로 보시면 .
   + 1 > 2 로 되는 것 처럼
   + 3 > 4 로 변환 동작합니다.

### (*2) 비트 필트
```C++
fwd(static_cast<uint16_t>(h.totalLength)); // 정상 동작
```
 - 간단하게, 전달 받은 데이터를 어떤 타입으로 추론할 줄 몰라 발생한다고 보임
   + 명시적으로 uint16_t로 캐스팅하니 정상 동작함

# END
