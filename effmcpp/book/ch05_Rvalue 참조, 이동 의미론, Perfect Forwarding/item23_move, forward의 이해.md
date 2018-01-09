# 항목 19: move, forward의 이해

## 선행지식

- Move semantics (이동 의미론)
  + 이동 의미론은 컴파일러로 하여금 비싼 복사연산을 덜 비싼 이동연산으로 대체할 수 있게 해준다.
  + 이동생성자와 이동 할당 연산자가 이동 의미론에 대한 컨트롤을 제공해준다
  + 이동 의미론은 move-only 타입을 가능하게 해준다
    * `std::unique_ptr`
    * `std::future`
    * `std::thread`

- Perfect forwarding
  + Perfect forwarding은 임의의 인자를 받아서 완벽히 같은 타입으로 포워딩해주는 함수 템플릿을 작성할 수 있게 해준다. (예를 들자면, rvalue 참조를 받아서 rvalue참조로 포워딩)

## 오해와 진실

- `std::move`는 아무것도 이동시키지 않고, `std::forward`는 아무것도 포워딩하지 않는다. 이 함수들은 런타임에 아무런 코드도 만들어내지 않는다. 즉, 컴파일러에만 관여하는 코드인 것이다.
- `std::move`는 조건없이 rvalue로 캐스팅을 해준다.
- `std::forward`는 특정 조건이 만족됐을때만 rvalue로 캐스팅을 해준다.

## `std::move`의 구현

### C++11 에서의 정의

```c++
template <typename T>
typename remove_reference<T>::type&&
move(T&& param)
{
    using ReturnType = typename remove_reference<T>::type&&;
    return static_cast<ReturnType>(param);
}
```

- 구현을 보면 universal reference로 인자를 받아서 `remove_reference<T>::type&&`으로 캐스팅한 뒤 리턴하고 있다.

- 이해를 위해 `Widget w` 으로부터 `std::move(w)`를 실행하는 과정을 생각해보자. (lvalue)
  1. universal reference로 lvalue를 받았으므로 `T`는 `Widget&`이고, `param`의 타입도 `Widget&`다. (item01 참조)
  2. `remove_reference<Widget&>::type`은 `Widget`이다.
  3. `remove_reference<Widget&>::type&&`은 `Widget&&`이다.
  4. `static_cast<Widget&&>(param)`을 통해 `Widget&&` 타입으로 캐스팅한 뒤 리턴한다.

- 이번엔 `std::move(Widget())` 를 실행하는 과정을 생각해보자. (rvalue)
  1. universal reference로 rvalue를 받았으므로 `T`는 `Widget`이고, `param`의 타입은 `Widget&&`다. (item01 참조)
  2. `remove_reference<Widget>::type`은 `Widget`이다.
  3. `remove_reference<Widget>::type&&`은 `Widget&&`이다.
  4. `static_cast<Widget&&>(param)`을 통해 `Widget&&` 타입으로 캐스팅한 뒤 리턴한다.

### C++14 에서의 정의

```c++
template <typename T>
decltype(auto) move(T&& param)
{
    using ReturnType = remove_reference_t<T>&&;
    return static_cast<ReturnType>(param);
}
```

- 같은 행동을 하지만 깔끔하다.

## 이동 의미론을 사용할 때 주의할 점

### 문제 상황

- 주석을 담는 클래스 `Annotation`을 가정하자.

```c++
class Annotation
{
public:
    Annotation(const std::string text) : value(std::move(text)) {}

    /* some code */

private:
    std::string value;
}
```

- `std::string`는 내부에 할당된 메모리에 대한 포인터를 들고 있으므로 복사보다 이동이 훨씬 빠르다. 따라서 위와 같이 `value`를 `std::move(text)`를 통하여 초기화하였다.
- 이 코드는 컴파일과 링크가 문제없이 잘 되며, 실행도 문제가 없다.
- 이 코드의 문제를 찾아보자.

### 해답

- `text`의 타입은 `const std::string`이다. 따라서 `std::move(text)`의 타입은 `const std::string&&`이 된다.
- 이를 토대로 `std::string`의 생성자를 보자.

```c++
class String
{
public:
    string(const string& rhs);  // copy constructor
    string(string&& rhs);       // move constructor

    /* some code */
}
```

- `const std::string&&`은 복사생성자와 이동생성자 그 어느 곳에도 해당하지 않는다.
- 하지만 lvalue reference를 인자로 받는 함수는 자신보다 더 우선권이 있는 다른 오버로딩된 함수가 없다는 전제하에 rvalue reference 또한 받을 수 있다.
- 결국 문제 상황에서의 `std::move(text)`는 rvalue로 넘겨서 이동을 하고자 했지만 복사생성자로 넘어가면서 복사가 이루어지게 된다.

### 결론

- 이동을 지원할 객체는 `const`로 선언하지 말아야 한다. `const`객체에 대한 이동 요청은 소리 없이 복사 연산으로 변환된다.
- `std::move`는 실제로 아무것도 이동하지 않을 뿐만 아니라, 캐스팅 되는 객체가 이동할 것이라는 보장도 제공하지 않는다.

## `std::forward`에 대하여

- `std::forward`는 `std::move`와 똑같이 캐스팅을 해주지만, 캐스팅을 하는데에는 조건이 있다.

```c++
void process(const Widget& rhs);
void process(Widget&& rhs);

template <typename T>
void logAndProcess(T&& param)
{
    /*
     * leave log
     */

    // process(param);  // 무조건 lvalue process함수가 호출된다
    process(std::forward<T>(param));
}
```

- 함수가 받은 인자 `param` 또한 변수명이 있는 엄연한 lvalue 이다.
- 따라서 주석처리된 부분처럼 `process`함수에 `param`을 바로 넘기면 무조건 `void process(const Widget& rhs)` 버전이 호출된다.
- `std::forward`는 `T`가 참조 타입이 아닐 때에만 인자를 rvalue로 캐스팅 해준다.
