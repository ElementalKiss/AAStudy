# 항목 17: 특수 멤버 함수들의 자동 작성 조건을 숙지하라

## 특수 멤버 함수(special member function)

- C+++이 스스로 작성하는 멤버 함수들을 말한다
- C++98
  + 기본 생성자
  + 소멸자
  + 복사 생성자
  + 복사 할당 연산자 (copy assignment operator)
- C++11
  + 위의 네 개
  + 이동 생성자 (move constructor)
  + 이동 할당 연산자 (move assignment operator)

## 특수 멤버 함수들의 서명(signature)

```C++
class Widget
{
public:
    Widget();                               // 기본 생성자
    ~Widget();                              // 소멸자

    Widget(const Widget& rhs);              // 복사 생성자
    Widget& operator=(const Widget& rhs);   // 복사 할당 연산자

    Widget(Widget&& rhs);                   // 이동 생성자
    Widget& operator=(Widget&& rhs);        // 이동 할당 연산자
}
```

## 3의 법칙 (Rule of Three)

- 복사 생성자, 복사 할당 연산자, 소멸자 중 하나라도 선언했다면 나머지 둘도 선언해야 한다.
- 어떤 클래스의 복사 할당 연산을 프로그래머가 직접 구현하는 것은 그 클래스가 자원 관리가 필요하기 때문이다. 그리고 그런 클래서에서는 한 복사 연산이 수행하는 자원 관리를 다른 복사 연산에서도 수행해야 한다. 또한, 소멸자 역시 그 자원 관리에 참여한다.

## C++11에서의의 강화된(엄격해진) 규칙

### 기본 생성자

- C++98의 규칙들과 같다.
- 클래스에 사용자 선언 생성자가 없는 경우에만 자동으로 작성된다.

### 소멸자

- C++98의 규칙들과 본질적으로 같다.
- 유일한 차이는 소멸자가 기본적으로 noexcept라는 점이다.
- C++98처럼 기본적으로 작성되는 소멸자는 오직 기반 클래스 소멸자가 가상일 때에만 가상이다.

### 복사 생성자

- 클래스에 사용자 선언 복사 생성자가 없을 때에만 자동으로 작성된다.
- 클래스에 이동 연산이 하나라도 선언되어 있으면 삭제된다.
- 사용자 선언 복사 할당 연산자나 소멸자가 있는 클래스에서 이 함수가 자동 작성되는 기능은 비권장이다. (3의 법칙)

### 복사 할당 연산자

- 클래스에 사용자 선언 복사 할당 연산자가 없을 때에만 자동으로 작성된다.
- 클래스에 이동 연산이 하나라도 선언되어 있으면 삭제된다.
- 사용자 선언 복사 생성자나 소멸자가 있는 클래스에서 이 함수가 자동 작성되는 기능은 비권장이다. (3의 법칙)

### 이동 생성자, 이동 할당 연산자

- 클래스에 사용자 선언 복사 연산들과 이동 연산들, 소멸자가 없을 때에만 자동으로 작성된다.

## 삭제(비활성화)된 특수 멤버 함수를 사용하는 방법

- C++11에서 특수 멤버 함수가 자동 작성되는 조건이 꽤 엄격하다.

### 문제 상황

```C++
class StringTable
{
public:
    StringTable() {
        MakeLogEntry("Creating StringTable object");
    }
    ~StringTable() {
        MakeLogEntry("Destroying StringTable object");
    }

    /* some code */

private:
    std::map<int, std::string> values;
}
```

- `StringTable`클래스에서 생성과 소멸을 기록할 필요가 생겼다고 하자. 그리하여 위와같이 새로 생성자와 소멸자를 정의했다.
- `StringTable`클래스는 사용자 선언 소멸자가 정의되었기 때문에 **이동 연산**들은 자동으로 작성되지 않는다. (`StringTable` 클래스의 이전 버전에서 쓰이던 이동 연산들이 사라져버린 상황)
- 하지만, **복사 연산** 자동 작성에는 아무런 영향도 미치지 않는다.
- `StringTable`에 소멸자만 추가한 상황인데 이동 연산(이동 생성자, 이동 할당 연산자)들이 사라져버렸다.
- 즉, `StringTable` 객체를 **이동**하는 코드는 실제로는 `std::map<int, std::string>` 객체의 복사본을 생성해버린다....
- `std::map`의 복사에 걸리는 시간은 이동에 걸리는 시간에 비해 수백, 수천 배 차이가 날 가능성이 있다!!

### 해결

```C++
class StringTable
{
public:
    StringTable() {
        MakeLogEntry("Creating StringTable object");
    }
    ~StringTable() {
        MakeLogEntry("Destroying StringTable object");
    }

    StringTable(StringTable&&) = default;               // 이동 생성자와
    StringTable& operator=(StringTable&&) = default;    // 이동 할당 연산자를 default로 선언

    /* some code */

private:
    std::map<int, std::string> values;
}
```

- C++11 에서는 특수 멤버 함수를 "= default"를 사용하여 컴파일러로 하여금 해당 함수를 생성하도록 할 수 있다.

## 기타

- 멤버 함수 템플릿은 특수 멤버 함수의 자동 작성에 영향을 미치지 않는다

```C++
class Widget
{
    /* some code */

    template <typename T>
    Widget(const T& rhs);

    template <typename T>
    Widget& operator=(const T& rhs);

    /* some code */
}
```

- 위 `Widget` 클래스의 템플릿 멤버 함수는 `T`가 `Widget`으로 인스턴스화 되면 각각 복사 생성자와 복사 할당 연산자가 되지만 컴파일러는 이를 상관하지 않고 여전히 `Widget`의 복사 연산들과 이동 연산들을 활성화 한다.