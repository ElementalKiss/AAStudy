# 항목 4: 추론된 형식을 파악하는 방법을 알아두자

## IDE

```c++
const int theAnswer = 42;

auto x = theAnswer;
auto y = &theAnswer;
```

- Visual Studio 같은 IDE로 마우스 커서를 올리면...
  + `x`는 `int`
  + `y`는 `const int*`

## 컴파일러의 진단(오류) 메시지

```c++
template<typename T>
class TD;
```

- 위와 같이 클래스 템블릿을 **정의** 없이 **선언**만 했다.
- 이를 사용하면 오류가 날테지만, 이 오류를 이용할 것

```c++
TD<decltype<x>> xType;
TD<decltype<y>> yType;
```

- 이를 컴파일 해보면 다음과 같은 오류를 낸다. 따라서, `x`, `y`의 타입을 알 수 있다.

> error: aggregate 'TD\<int\> xType' has incomplete type and cannot be defined  
> error: aggregate 'TD\<const int*\>' xType' has incomplete type and cannot be defined  

## 실행시점 출력

```c++
std::cout << typeid(x).name() << '\n';
std::cout << typeid(y).name() << '\n';
```

- 이 방법은 정확한 타입을 말해주지 않을수도 있다
- 아래는 그 예이다

```c++
template<typename T>
void f(const T& param);

std::vector<Widget> createVec();

const auto vw = createVec();
/* some code */
f(&vw[0]);
```

1. `const auto vw`는 `const std::vector<Widget> vw`
2. `vw[0]`은 `const  Widget&`
3. `&vw[0]`은 `const Widget*`
4. `f(const T& param)`의 ParamType은 `const Widget* const&`

```c++
std::cout << typeid(T).name() << '\n';
std::cout << typeid(param).name() << '\n';
```

- 위의 출력 결과는 `const Widget* const&`이 아닌 `const Widget*`이다
- 하지만 이는 의도된 것! 표준에 의하면, `std::type_info::name`은 주어진 형식을 **pass-by-value**로써 취급해야 한다.
- 따라서 **pass-by-value** 에 의한 추론형식에 의하면, 참조성을 무시하고 그 뒤에 `const`또한 무시하게 된다.
- 결론적으로 `const Widget*`이 맞는 셈