# 항목 17: 특수 멤버 함수들의 자동 작성 조건을 숙지하라.

## 특수 멤버 함수?
- 특수 멤버 함수 (special member function) : c++ 스스로 기꺼이 작성되는 멤버 함수들
- 이렇게 작성된 함수들은 public이며, inline이다.
- 상속 받았을 시엔, 파생 클래스의 소멸자를 제외하곤 비가상 함수이다. (소멸자는 가상 함수(virtual))

C++98 : 기본 생성자, 소멸자, Copy 생성자, Copy 할당 연산자(operator=)
C++11 : Move 생성자, Move 할당 연산자(operator=)

```cpp
class Widget {
public:
    Widget(Widget& rhs); // Copy 생성자
    Widget& operator=(Widget& rhs) // Copy 할당 연산자
    Widget(Widget&& rhs); // Move 생성자
    Widget& operator=(Widget&& rhs) // Move 할당 연산자
};
```

## Copy vs Move 연산들의 차이점?
- Copy : 생성자와 할당 연산자 서로 독립적, : 둘 중 하나라도 선언 시, 나머지 하나도 자동 작성 (필요 시)
- Move : 생성자와 할당 연산자 서로 비독립적, : 둘 중 하나라도 선언 시, 다른 하나 자동 작성하지 않음,
> 이유 : 사용자가 Move 생성자를 작성하였다면, 기본 Move 생성이 적합하지 않았기 때문,
> 그렇다면 Move 할당 연산자 또한 적합하지 않을 것 임.

```cpp
class Widget {
public:
    Widget(Widget& rhs);
    /* Widget& operator=(Widget& rhs); */ // 자동 생성

    //-----------------------------------------------------------//
    Widget(Widget&& rhs);
    /* Widget& operator=(Widget&& rhs) = delete; */ // 삭제 되어 있음.
};
```

## Copy vs Move 연산들의 서로의 관계는? 
- Copy 선언 시, Move 연산들은 작성되지 않음,
- Move 선언 시, Copy 연산들은 작성되지 않음,
> 이유 : 기본 Copy에 적합하지 않기에 새로 작성한 Copy라면, Move 연산 또한 적합하지 않을 가능성 큼, 
> Move 연산자 또한 동일.

```cpp
class Widget {
public:
    Widget(Widget&& rhs);
    Widget& operator=(Widget&& rhs)

    /*
    Widget(Widget& rhs) = delete;
    Widget& operator=(Widget& rhs) = delete;
    */ // 자동 삭제
};
```

- 위의 코드에서 Move 연산자를 호출했다면, 기존 copy로 동작한다. (C++98 레거시 코드)

## 3의 법칙 (Rule of Three) 
- Copy 생성자, Copy 할당 연산자, 소멸자 어느 한개라도 사용했다면 모두 선언해줘야 한다.
> 이유 : (1)Copy 연산을 선언했다면, 자원 관리를 수행해야 함을 의미, (2) 소멸자도 자원 관리에 참여해야 함
> 따라서 Copy 연산이 선언 되어 있으면, 소멸자는 자동 작성될 수 없다.
> 이는 역으로도 동일한 개념으로 통함,
> (1)소멸자가 선언 되어있으면, 
> (2)Copy 연산자들은 자동 작성 될 수 없다, 하지만 됨 (C++98 레거시 코드) 
- 더 나아가, Copy 연산 선언은 Move 연산들은 자동 작성 될 수 없다를 이용,
- 소멸자가 존재하면 Move 연산자들은 자동 작성 되지 않음,

## 정리 : Move 연산자가 자동 작성되는 조건
 1. Copy 연산들이 선언되어 있지 않다.
 2. Move 연산들이 선언되어 있지 않다.
 3. 소멸자가 선언되어 있지 않다.

## 기본 행동을 사용하겠다면 "=default"를 선언하라
> 이유 : 기본 행동으로 사용하겠다고 명시적 표현,
> 컴파일러가 자동으로 생성할거란 의존성 제거,
> 미묘한 버그 회피(!)

```cpp
class Widget {
public:
  ~Widget();
  Widget(const Widget&) = default; // 기본 Copy 생성자 사용하겠다.
  Widget& operator=(const Widget&) = default;  // 기본 할당 연산자를 사용하겠다.
};
```

## 미묘한 버그 코드
```cpp
// 기본 코드
class StringTable {
public:
    StringTable() {};

private:
    std::map<int, std::string> values;
};
```

- 위의 기본 코드는 Copy/Move/소멸자가 자동 작성되어 문제 없이 동작한다.
- 하지만 해당 코드에 생성자와 소멸자에 로그를 필요로 한다면?

```cpp
// 기본 코드
class StringTable {
public:
    StringTable() { makeLog("Creating StringTable object"); };    // 수정 됨
    ~StringTable() { makeLog("Destroying StringTable object"); }; // 추가 됨

private:
    std::map<int, std::string> values;
};
```

- 상위 코드는 성능 상의 문제 발생,
> 이유 : 소멸자의 선언은 Move 연산자들이 자동 작성되지 않는다.
> StringTable을 move하는 곳에서 copy가 발생하게 되고,
> 어청난 성능 이슈가 발생한다.
- 만약 "=default"로 기본 기능을 넣어줬다면 해당 이슈는 생기지 않았을 것,

## 총 정리
- 특수 멤버 함수란 : 컴파일러 스스로 작성할 수 있는 기본 생성자, 소멸자, Copy 연산들, Move 연산들을 의미
- 기본 생성자 : 없는 경우 자동,
- 소멸자 : 없는 경우 자동, 상속 시 virtual로,
- Copy 생성자와 Copy 할당 연산자 : 없을 경우 자동, Move 연산 존재 시 비활성화, 소멸자 또는 다른 Copy 연산이 선언 되었을 경우 꼭 같이 선언 해 주자,
- Move 생성자와 Move 할당 연산자 : Copy 연산들과 다른 Move 연산, 그리고 소멸자가 모두 없을 떄 자동 생성, 
- Move 연산자들이 선언되지 않았는데 사용 되었다면, Copy 연산자들로 동작
- 기본 행동을 하는 특수 멤버 함수엔 "=default"를 꼭 명시하자

## 추가 내용
- 템플릿
- 멤버 함수 템플릿 사용으로 인한 자동 작성이 비활성화 되는 규칙은 없다.

```cpp
class Widget {
  template<typename T>
  Widget(const T& rhs);  // 그 어떤 것으로도 Widget을 생성
  template<typename T>
  Widget& operator=(const T& rhs);  // 그 어떤 것으로도 Widget을 할당
};
```

- 이 템플릿으로 인한, Copy 생성자나 Copy 할당 연산자의 서명이 일치한다고 해도,
- 컴파일러는 Widget Copy 연산들과, Move 연산들을 작성한다.
- 왜 중요한지는 항목 26에서...