# 항목 32: 클로저 내부로 객체를 옮기기 위해서는 init capture 를 사용하자

## 배경 지식

```C++
auto func = [](Widget& w) { /* ... */ };
```

- **람다 표현식**(`[](Widget& w) {...}`)으로부터 **클로저 객체**가 생성되어 `func` 변수에 할당 되었다.

---

## 개요

- 람다 표현식을 사용하다 보면, by-value 캡처나 by-reference 캡처로는 부족하고 객체를 클로저 내부로 이동시켜야 할 때가 있다.
  + move-only 객체 (e.g., `std::unique_ptr`, `std::future`, ...)
  + 복사는 비싸지만 이동은 값싼 경우 (e.g., 대부분의 STL 컨테이너)

- C++11 에서는 클로저 내부로 객체를 move capture 할 방법이 없다.

- C++14 에서는 클로저 내부로 객체를 move capture 하는 것에 그치지 않고, 더욱 우아하고 유연한 init capture 라는 기능이 도입 되었다.

---

## Init Capture

- init capture는 다음과 같은 것이 가능하다.  
  1. 클로저 클래스 내에 캡처를 할 때 data member의 이름을 정할 수 있다.
  2. data member를 초기화할 방법을 지정할 수 있다.

```C++
class Widget
{
public:
    /* ... */

    bool isValidated() const;
    bool isArchived() const;

private:
    /* ... */
};

auto widgetPtr = std::make_unique<Widget>();

/* do something with widgetPtr */

auto func = [wp = std::move(widgetPtr)] {
    return wp->isValidated() && wp->isArchived();
};
```

- 람다 표현식 내부는 클로저 클래스의 스코프라고 할 수 있다.

- 따라서 `wp`는 클로저 클래스의 data member이다.

- data member `wp`는 `std::move(widgetPtr)`의 리턴값에 의해 초기화되고 있다.

```C++
auto func = [wp = std::make_unique<Widget>()] {
    return wp->isValidated() && wp->isArchived();
};
```

- 클로저를 생성하기 이전에 `Widget`에 대해서 설정할 것이 없다면, 클로저 클래스의 data member인 `wp`를 `std::make_unique<Widget>()`를 통해 직접 초기화 할 수도 있다.

- 이와 같이 C++14에 와서 "캡쳐"라는 개념은 C++11에 비해서 더욱 일반화 되었다. 따라서 init capture를 *generalized lambda capture*(일반화된 람다 캡처)라고 부르기도 한다.

---

## C++11 에서의 by-move capture

### 1. 클래스를 이용한 방법

```C++
class IsValAndArch
{
public:
    explicit IsValAndArch(std::unique_ptr&& ptr) : wp(std::move(ptr)) {}

    bool operator () () const {
        return wp->isValidated() && wp->isArchived();
    }

private:
    std::unique_ptr<Widget> wp;
};

auto func = IsValAndArch(std::make_unique<Widget>());
```

- 람다 표현식을 이용한 방법보다는 코드가 길어졌지만 하는 역할은 init capture 와 같음을 알 수 있다.

### 2. `std::bind`를 이용한 방법

```C++
std::vector<double> data;

/* do something with data */

/**
 * C++14 code
 */
auto func = [data = std::move(data)] { /* use of data */ };

/**
 * C++11 code
 */
auto func = std::bind(
    [](const std::vector<double>& data) { /* use of data */ },
    std::move(data)
);
```

- 람다 표현식처럼 `std::bind` 또한 함수 객체(bind 객체)를 만들어낸다.

- `std::bind`는 첫 번째 인자로 호출가능한 객체를 받고, 이후의 인자들은 첫 번째 인자로 받은 호출 가능한 객체의 인자에 묶는다.

- `std::bind`는 lvalue/rvalue에 따라 적절하게 bind 객체 내부에 복사 생성/이동 생성 해서 값들을 저장하게 된다.

- 나중에 bind 객체가 호출되면, 갖고 있는 함수 인자들을 호출 가능한 객체에게 넘겨준다.

- 결국 C++11 버전 코드의 `func`은 C++14 버전 코드에 쓰인 init capture를 이용한 `func`과 동일한 기능을 하게 된다.

---

## 요약

- 클로저 내부로 객체를 이동시키기 위해서는 C++14의 init capture를 사용하자

- C++11 에서는 init capture가 없으므로 클래스를 이용하거나 `std::bind`를 사용하자