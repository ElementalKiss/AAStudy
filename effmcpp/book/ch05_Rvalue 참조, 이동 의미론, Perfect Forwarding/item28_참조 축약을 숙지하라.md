# 항목 28: 참고 축약을 숙지하라

## 부호화 메커니즘
```cpp
template<typename T>
void func(T&& param);

Widget widgetFactory(); // 오른값을 돌려주는 함수
Widget w; // 왼값(변수)

func(w) // func 왼값으로 호출, T는 Widget&으로 연역
func(widgetFactory()) // func 오른값으로 호출, T는 Widget으로 연역
```
* 템플릿 매개변수에 왼값 인수가 전달되면 T는 왼값 참초로 오른값이 전달되면 T는 비참조 형식으로 연역됨. (비대칭성)
* 해당 메커니즘은 std::forward의 바탕 메커니즘이기도 함.

## C++에서 참조에 대한 참조는 위법
```cpp
int x;
...
auto& & rx = x; // error
```
* 하지만 보편 참조(Forwarding Reference)를 받는 함수 템플릿에 왼값을 넘겨주면 참초 축약 덕분에 문제가 없음
* 컴파일러가 알아서 참초에 대한 참조를 산출하게 됨.

```cpp
func(w) // 함수를 왼값으로 호출, T는 Widget& 연역
void func(Widget& && param); // 메커니즘에 따라 왼값으로 연역.
void func(Widget& parma); // 최종 함수 서명 
```

## 참조에 대한 참조 조합과 규칙
* 왼값에 대한 왼값
* 오른값에 대한 왼값
* 왼값에 대한 오른값
* 오른값에 대한 오른값
* 두 참조 중 하나라도 왼값 참조이면 결과는 왼값 참조, 그렇지 않으면 오른값 참조.

## std::foward
```cpp
template<typename T>
void f(T&& fParam)
{
	...
	someFunc(std::forward<T>(fParam));
}
```
```cpp
// 저자가 생각하는 foward 구현
template<typename T>
T&& foward(typename remove_reference<T>::type& param) // C++14에서는 T&& foward(remove_reference_t<T>& param)
{
	return static_cast<T&&>(param);
}
```
```cpp
// 실제 구현
	// FUNCTION TEMPLATE forward
template<class _Ty>
	constexpr _Ty&& forward(remove_reference_t<_Ty>& _Arg) _NOEXCEPT
	{	// forward an lvalue as either an lvalue or an rvalue
	return (static_cast<_Ty&&>(_Arg));
	}

template<class _Ty>
	constexpr _Ty&& forward(remove_reference_t<_Ty>&& _Arg) _NOEXCEPT
	{	// forward an rvalue as an rvalue
	static_assert(!is_lvalue_reference_v<_Ty>, "bad forward call");
	return (static_cast<_Ty&&>(_Arg));
	}

```
* 왼값 대입
```cpp
Widget& && foward(typename remove_reference<Widget&>::type& param)
{
	return static_cast<Widget& &&>(param);
}

Widget& && foward(Widget& param) // remove_reference<Widget&>::type이 Widget 산출
{
	return static_cast<Widget& &&>(param);
}

Widget& foward(Widget& param) // 참조 축약 발생
{
	return static_cast<Widget&>(param);
}
```
* 오른값 대입
```cpp
Widget&& foward(typename remove_reference<Widget>::type& param)
{
	return static_cast<Widget&&>(param);
}

Widget&& foward(Widget& param) // remove_reference<Widget>::type이 Widget 산출
{
	return static_cast<Widget&&>(param);
}
```

## 참초 축약이 일어나는 문맥
* 템플릿 인스턴스화
* auto 변수에 대한 형식 연역
```cpp
auto&& w1 = w;
Widget& && w1 = w;
Widget& w1 = w; // 참조 축약

auto&& w2 = widgeFactory();  
Widget&& w2 = widgeFactory(); // 참조에 대한 참조가 없으므로 끝!
```
* typedef와 별칭 선언
```cpp
template<typename T>
class Widget {
public:
	typedef T&& RValueRefToT;
	...
};

// Widget을 왼값 참조 형식으로 인스턴스화
Widget<int&> w;
typedef int& && RvalueRefToT;
typedef int& RvalueRefToT;
```
* decltype

## 기억해 둘 사항들
* 참조 축약은 템플릿 인스턴스화, auto 형식 연역, typedef 별칭 선언의 지정 및 사용, decltype의 지정 및 사용이라는 네 가지 문맥에서 일어난다.
* 컴파일러가 참조 축약 문맥에서 참조에 대한 참조를 만들어내면, 그 결과는 하나의 참조가 된다. 원래의 두 참조 중 하나라도 왼값 참조이면 결과는 왼값 참조이고, 그렇지 않으면 오른값 참조이다.
* 형식 연역이 왼값과 오른값을 구분하는 문맥과 참조 축약이 일어나는 문맥에서 보편 참조는 오른값 참조이다.
