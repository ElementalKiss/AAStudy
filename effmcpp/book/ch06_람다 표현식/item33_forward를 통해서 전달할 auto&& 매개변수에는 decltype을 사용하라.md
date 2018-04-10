# 항목 33: std::forward를 통해서 전달할 auto&& 매개변수에는 decltype을 사용하라


* C++14에서 가장 고무적인 기능은 일반적 람다(generic lambdas), 즉 인수에 auto를 사용하는 람다이다.
* 이 기능의 구현은 람다의 클로저 클래스의 operator()를 템플릿 함수로 만들면 된다.

* 아래와 같은 람다를 생각해보자
```cpp
auto f = [](auto x) { return normalize(x); };
```

* 이 람다가 산출하는 클로저 클래스의 함수 호출 연산자는 다음과 같은 모습이다.
```cpp
class 컴파일러가_만든_어떤_클래스_이름 {
public:
  template<typename T>
  auto operator()(T x) const
  { return nomalize(x); }
}
```
* 이 람다는 항상 인수의 l-value를 전달한다.
* 제대로 하려면 람다가 x를 normalize에 완벽하게 전달해야 한다.
* 바꿀것은 첫째로 x가 보편 참조여야 하고, 둘째로 x를 std::forward를 통해서 normalize에 전달해야 한다.

```cpp
auto f = [](auto&& x) {
  return nomalize(std::forward<???>(x));
};
```

* ??? 대신 무엇을 써넣어야 할까
* 형식 매개변수 T를 받는 템플릿 함수 안에서 사용하는 경우는 그냥 std::forward<T>라 쓰면 된다.
* 그러나 일반적 람다에는 그런식으로 사용할 형식 매개변수 T가 없으며 operator()에는 T가 있지만 람다에서 그 T를 지칭할 수 없다.

* 이 람다에 주어진 인수가 l-value인지 r-value인지를 decltype으로 조사해보면 알 수 있다.
* 항목 28에서 std::forward 호출 시 전달할 인수가 l-value임을 나타내기 위해서는 l-value참조 형식 인수를 사용하고 r-value임을 나타내기 위해서는 비참조 형식 인수를 사용하는 것이 관례라는 점을 설명한다.
* 그러나 이 예제에서 x가 r-value에 묶였다면 묶였다면 decltype(x)는 오른값 참조를 산출한다.
* 항목 28에 나온 std::forward의 C++14 구현을 다시보면
```cpp
template<typename T>
T&& forward(remove_reference_t<T>& param)
{
  return static_cast<T&&>(param);
}
```

* 일반적으로 클라이언트 코드가 Widget 형식의 오른값을 완벽하게 전달할 때에는 그 Widget 형식으로 std::forward를 인스턴스화 할 것이다.
```cpp
Widget&& forward(Widget& param)
{
  return static_cast<Widget&&>(param);
}
```

* 그런데 클라이언트 코드가 Widget 형식의 동일한 r-value를 전달하되, T를 비참조 형식으로 지정하는 관례를 따르지 않고 r-value 참조 형식으로 지정하면 어떻게 될까? 즉, T를 Widget&&로 지정한다면?
* std::forward가 인스턴스화되고 std::remove_reference_t가 적용된 후의, 그러나 참조 축약이 적용되기 전의 모습
```cpp
Widget&& && forward(Widget& param)
{
  return static_cast<Widget&& &&>(param);
}
```

* 여기에 r-value 참조에 대한 r-value 참조는 단일한 r-value참조가 된다는 축약 규칙을 적용하면
```cpp
Widget&& forward(Widget& param)
{
  return static_cast<Widget&&>(param);
}
```

* 이 인스턴스를 std::forward<Widget> 인스턴스, 즉 T를 Widget으로 지정해서 forward를 호출했을 때의 인스턴스와 동일하다.
* 즉 r-value 참조 형식으로 std::forward를 인스턴스화한 결과는 비참조 형식으로 인스턴스화한 결과와 같다.
* 람다에 r-value가 전달되었을 때 decltype(x)가 산출하는 형식이 관례와 맞지 않아도 어짜피 결과는 같다.
* 즉, l-value든 r-value든 decltype(x)를 std::forward로 넘겨주면 우리가 원하는 결과가 된다.

* 결론적으로, 완벽 전달 람다는 다음과 같다.
```cpp
auto f = [](auto&& x)
  {
    return normalize(std::forward<decltype(x)>(x));
  };
```

* C++14의 람다가 가변 인수를 지원하기 때문에 마침표를 추가하면 가변인수를 받아 완벽 전달하는 람다가 된다.
```cpp
auto f = [](auto&&... x)
  {
    return normalize(std::forward<decltype(x)>(x)...);
  };
```