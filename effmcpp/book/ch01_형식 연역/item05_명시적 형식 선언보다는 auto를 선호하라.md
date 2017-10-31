# 항목 5: 명시적 형식 선언보다는 auto를 선호하자

- 변수 선언을 할때, 명시적 선언보다는 auto를 써서 선언을 하는 것이 더 유리할 수 있다는 저자의 의견

## 초기화를 강제하는 상황

```c++
int x1;       // 초기화 안 될수도 있음
auto x2;      // error!!
auto x3 = 0;  // 반드시 초기화
```

- `auto`는 초기화를 하지 않으면 오류를 냄

## 형식을 지정할 수 없는 상황

```c++
auto derefUPLess = [](const std::unique_ptr<Widget>& p1,
                      const std::unique_ptr<Widget>& p2) {
    return *p1 < *p2;
}
```

- 람다는 하나하나가 유일한 형식이다. 따라서 프로그래머 입장에서는 람다가 있어도 어떤 형식인지 알 방법이 없다.
  + 람다에서 컴파일 오류났을 경우 에러목록을 보면 보이는 끔찍한 타입이 컴파일러만의 람다 형식
- `std::fuction`으로 아래와 같이 받을 수 있긴 하지만, `std::function`은 람다 이외에도 모든 호출 가능한 값을 저장하기 위한 자료형이기 때문에 최적화 측면에서 `auto`보다 좋지 않다.

```c++
std::function<bool(const std::unique_ptr<Widget>&,
    const std::unique_ptr<Widget>&)>
derefUPLess = [](const std::unique_ptr<Widget>& p1,
                      const std::unique_ptr<Widget>& p2) {
    return *p1 < *p2;
```

- `auto`보다 최적화가 안되는 주제에 타이핑도 무시무시하다...

```c++
auto derefUPLess = [](const auto& p1,
                      const auto& p2) {
    return *p1 < *p2;
}
```

- C++14 에서는 람다가 받는 인자조차도 `auto`로 받을 수 있다

## 실수를 방지해주는 상황

```c++
std::vector<int> v;
/* do something with v */
auto size = v.size();
```

- `v.size()` 가 반환하는 타입은 `std::vector<int>::size_type`으로 C의 `size_t`이다.
- `size_t`의 경우 운영체제에 따라 크기가 다르다.
  + 64bit OS: `sizeof(size_t) == 8`
  + 32bit OS: `sizeof(size_t) == 4`
- 만약 위의 문장을 다음과 같이 적었다면 OS에 의존적인 코드가 되고 만다.
  + `uint32_t size = v.size()`
  + `uint64_t size = v.size()`

```c++
std::map<std::string, int> m;
/* do something with m */
for (const std::pair<std::string, int>& p : m) {
    // do something
}
```

- 위 코드는 컴파일은 되지만 생각해보면 굉장히 문제가 있는 코드이다. 찾아보자!

```c++
std::map<std::string, int> m;
/* do something with m */
for (const auto& p : m) {
    // do something
}
```

- `std::map`의 key부분이 `const`임을 떠올려보자.
- 위의 range-based for loop 에서 `m`이 넘겨주는 값은 `std::pair<std::string, int>`가 아니라 `std::pair<const std::string, int>`이다!!!
- 따라서 `auto`를 쓰지 않은 코드의 경우 for loop 를 돌때마다 `m`이 넘겨준 `std::pair<const std::string, int>`로부터 `std::pair<std::string, int>`로의 복사가 일어난다.