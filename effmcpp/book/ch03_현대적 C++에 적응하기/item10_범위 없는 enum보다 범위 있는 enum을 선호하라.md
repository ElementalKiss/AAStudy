# 항목 10: 범위 없는 enum보다 범위 있는 enum을 선호하라

## enum
* C++98: enum을 포함하는 범위에 속하므로 해당 범위 내에 같은 이름이 있으면 안됨
위와 같은 enum을 공식적으로 범위 없는(unscoped) enum이라고 함.
* C++11: 범위 있는(scoped) enum 추가됨, enum class라고도 함.
* unscoped enum
```cpp
enum Color { black, white, red }; 
auto white = false;    // white는 enum에서 이미 선언하여 사용중이므로 사용 불가
// 1>c:\users\meerim\source\repos\consoleapplication1\consoleapplication1\extern.cpp(11): error C2365: 'test::white': 재정의: 이전 정의는 '열거자'입니다.
// 1>c:\users\meerim\source\repos\consoleapplication1\consoleapplication1\extern.cpp(10): note: 'test::white' 선언을 참조하십시오.
```
* scoped enum
```cpp
enum class NewColor { pink, blue, yellow }; 
auto pink = false;                    // 이 범위에 다른 pink 없으므로 문제 없음
NewColor c = blue;                    // blue라는 이름의 enum이 없으므로 에러
// 1>c:\users\meerim\source\repos\consoleapplication1\consoleapplication1\extern.cpp(15): error C2065: 'blue': 선언되지 않은 식별자입니다.
NewColor d = NewColor::yellow;
auto e = NewColor::yellow;
```

## scoped enum의 장점
* unscoped enum은 암묵적으로 정수 형식으로 변환되지만(정수 형식으로부터 암묵적으로 부동소수점 형식으로 변환됨) scoped enum은 암묵적으로 다른 형식으로 변환 되지 않음.
    * unscoped enum
    ```cpp
    enum Color { black, white, red }; // unscoped enum
    std::vector<std::size_t> // func. returning
      primeFactors(std::size_t x); // prime factors of x
    Color c = red;
    …

    if (c < 14.5) { // compare Color to double (!)
     auto factors = // compute prime factors
     primeFactors(c); // of a Color (!)
     …
    }
    ``` 
    * scoped enum
    ```cpp
    enum class Color { black, white, red }; // enum is now scoped
    Color c = Color::red;                   // as before, but
    …                                       // with scope qualifier

    if (c < 14.5) {                         // error! can't compare
                                            // Color and double
    auto factors =                          // error! can't pass Color to
      primeFactors(c);                      // function expecting std::size_t
    …
    }

    if (static_cast<double>(c) < 14.5) { // odd code, but
                                         // it's valid
    auto factors = // suspect, but
      primeFactors(static_cast<std::size_t>(c)); // it compiles
    …
    }
    ```
* 전방 선언(forward declaration, 또는 선행 선언) 가능
    * C++98: enum 정의만 지원
    * C++11: unscoped도 전방 선언이 가능하긴 함. 가능한 이유는 모든 enum의 바탕 형식이 정수 형식이기 때문.







