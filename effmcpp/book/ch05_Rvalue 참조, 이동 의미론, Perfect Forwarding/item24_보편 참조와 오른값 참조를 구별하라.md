# 항목 24: Forwarding Reference와 rValue Reference를 구별하라

## Forwarding Reference ?
* 보편참조, Universal Reference, Forwarding Reference, 모두 같은 의미
* Universal Reference : 스콧마이어스 제안
* Forwarding Reference : C++17 제안
* (https://stackoverflow.com/questions/39552272/is-there-a-difference-between-universal-references-and-forwarding-references)

## T&& 두가지 의미
* rValue Reference
* rValue만을 참조하기 위한 타입
* Forwarding Reference
* rValue, lValue 둘 중 하나를 참조하기 위한 타입

```c++
void f(Widget&& param)		// rValue Reference
Widget&& var1 = Widget();	// rValue Reference
auto&& var2 = var1;		// Forwarding Reference

template<typename T>
void f(std::vector<T>&& param);	// rValue Reference

template<typename T>
void f(T&& param);		// Forwarding Reference
```

## Forwarding Reference 구분법
* type deduction(타입 추론)이 일어난다.

```c++
template<typename T>
void f(T&& param);	// Forwarding Reference

Widget w;		// f에 왼값이 전달, param 타입은
f(w);			// Widget&

f(std::move(w));	// f에 오른값이 전달, param 타입은
			// Widget&&
```

## Forwarding Reference 오해할 수 있는 상황
* std::vector의 push_back과
* std::vector의 emplace_back

```c++
template<class T, class Allocator = allocator<T>>
class vector {
public:
  void push_back(T&& x);		// rValue Reference
  template <class... Args>
  void emplace_back(Args&&... x);	// Forwarding Reference
  ...
}
```
* cf) push_back
* push_back의 T&&는 rValue Reference다,
* 템플릿이 인스턴스화 될 때, T의 타입이 미리 정해지며,
* void push_back(Widget&& x); 로 고정된다.
* 이후에 해당 멤버 함수는 어떠한 타입추론도 발생되지 않으며,
* 따라서 해당 멤버함수는 rValue Reference이다.

* cf) emplace_back
* emplace_back의 Args&&는 Forwarding Reference다.
* 템플릿이 인스턴스화 되어도, T와는 독립적으로,
* Args에 의해 타입추론이 발생한다.

## 정리
* 보편참조, Universal Reference, Forwarding Reference는 같은 의미이다.
* T&&은 두가지 해석이 가능한데, rValue Reference와 Forwarding Reference이다.
* T&&가 Forwarding Reference인 경우는, 타입추론이 일어난 경우이다.
* Forwarding Reference는 rValue Refence도, lValue Reference도 될 수 있다.

## 개인적으로 한번에 와 닿지 않았던 부분
* const 한정사가 붙으면 Forwarding Reference가 되지 못한다?
```c++
template<typename T>
void f(const T&& param);	// param은 rValue Reference
```
