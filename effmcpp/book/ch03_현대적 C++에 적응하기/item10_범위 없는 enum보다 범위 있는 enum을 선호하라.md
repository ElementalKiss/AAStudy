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
* scoped enum은 암묵적으로 다른 형식으로 변환 되지 않음. unscoped enum은 암묵적으로 정수 형식으로 변환 됨.(정수 형식으로부터 암묵적으로 부동소수점 형식으로 변환됨) 
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
    * C++11: unscoped도 전방 선언이 가능하긴 함. 가능한 이유는 모든 enum의 바탕 형식(underlying type)이 정수 형식이기 때문.
    * 전방 선언이 가능해짐으로써 새로운 열거자가 추가 되어도 전체 컴파일을 하지 않아도 됨.
    * C++98과 달리 C++11에서 scoped enum이 전방 선언이 가능한 이유는 바탕 형식의 크기를 컴파일러가 언제라도 알 수 있으며, 바탕 형식을 프로그래머가 직접 설정 가능하기 때문.
    * unscoped enum
    ```cpp
      enum Status; // underlying type is int
      enum Status: std::uint32_t; // underlying type for
                                        // Status is std::uint32_t
                                        // (from <cstdint>)
    ```
    * scoped enum
    ```cpp
      enum class Status; // fwd decl for unscoped enum;
      enum class Status: std::uint32_t; // underlying type is std::uint8_t
    ```

## unscoped enum의 장점
```cpp
  using UserInfo =
  std::tuple<std::string,  // 사용자 이름
             std::string,  // 이메일 주소
             std::size_t>; // 평판치

  UserInfo uInfo; // object of tuple type
  …
  auto val = std::get<1>(uInfo); // 1번 필드가 무엇을 뜻하는지 바로 알기 힘듦.

  enum UserInfoFields { uiName, uiEmail, uiReputation };
  UserInfo uInfo; // as before
  …
  auto val = std::get<uiEmail>(uInfo); //unscoped enum을 사용하여 이메일 필드라는 것을 바로 알 수 있음.

  enum class UserInfoFields { uiName, uiEmail, uiReputation };
  UserInfo uInfo; // as before
  …
  auto val =
  std::get<static_cast<std::size_t>(UserInfoFields::uiEmail)>(uInfo);
  // 작성은 가능하지만 까다로움.
```
* 번외: scoped enum의 사용 시 열거자를 받아서 std::size_t를 반환하는 함수 작성하기
```cpp
  template<typename E>
  constexpr typename std::underlying_type<E>::type
  toUType(E enumerator) noexcept
  {
    return static_cast<typename std::underlying_type<E>::type>(enumerator);

  //C++14에서 std::underlying_type_t를 이용하기
  template<typename E> // 
  constexpr std::underlying_type_t<E>
  toUType(E enumerator) noexcept
  {
    return static_cast<std::underlying_type_t<E>>(enumerator);
  }

  //C++14에서 auto를 이용하기
  template<typename E> // C++14
  constexpr auto
  toUType(E enumerator) noexcept
  {
    return static_cast<std::underlying_type_t<E>>(enumerator);
  }

  auto val = std::get<toUType(UserInfoFields::uiEmail)>(uInfo);
}
```


## 기억해 둘 사항들
* C++98 스타일의 enum을 이제는 범위 없는 enum이라고 부른다.
* 범위 있는 enum의 열거자들은 그 안에서만 보인다. 이 열거자들은 오직 캐스팅을 통해서만 다른 형식으로 변환한다.
* 범위 있는 enum과 범위 없는 enum 모두 바탕 형식 지정을 지원한다. 범위 있는 enum의 기본 바탕 형식은 int이다. 범위 없는 enum에는 기본 바탕 형식이 없다.
* 범위 있는 enum은 항상 전방 선언이 가능하다. 범위 없는 enum은 해당 선언에 바탕 형식을 지정하는 경우에만 전방 선언이 가능하다.







