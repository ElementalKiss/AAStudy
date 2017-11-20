# 항목 12: 재정의 함수들을 override로 선언하라

## 개요 

- c++의 객체 지향 프로그래밍의 세계 = 클래스, 상속, 가상 함수가 중심.
- 기본적이며, 근본적인 개념 가상 함수 재정의 (override)
    + derived 클래스 함수를 base 클래스의 인터페이스를 통해 호출한다.
- 그치만 잘 못 되기 너무 쉽고, 잘 못 되었어도 컴파일러가 잡아내지 못 함,
- 그래서 override를 명시적으로 사용하게 만들었고,
- 이를 사용하면 컴파일러가 미리 오류를 파악 가능하게 함,

## 예제

- 잘 못 된 곳을 찾아 보세요.
```cpp
class Base{
public:
    virtual void mf1() const;
    virtual void mf2(int x);
    virtual void mf3() &;
    void mf4() const;
};

class Derived: public Base {
public:
    virtual void mf1();
    virtual void mf2(unsigned int x);
    virtual void mf3() &&;
    void mf4() const;
};
```

- 컴파일 잘 됨,
- 하지만, 정말 작성자가 이걸 의도한 코드였을까?

- override 명시적 선언 할 경우,
```cpp
class Derived: public Base {
public:
    virtual void mf1() override;
    virtual void mf2(unsigned int x) override;
    virtual void mf3() && override;
    void mf4() const override;
};
```

- 컴파일러가 에러 호출
- ```Error C3668 : 'Derived::mf1': method with override specifier 'override' did not override any base class methods```

## 생각 해 보자,

- 모든 재정의 함수에 override를 명시적으로 선언 했다고 가정 해 보자,
- base가 없는 override 함수를 미리 파악하여 고칠 수 있다.
- base 클래스의 재정의 함수 선언부를 변경 시, 비용이 얼마인지 가늠 할 수 있다.
    + derived 클래스의 override 된 함수들이 에러를 뱉어낼테니,

# 추가 내용

- c++98 override 필수조건
    + base 클래스 함수가 virtual 함수여야 함,
    + base 함수와 derived 함수의 이름이 동일해야 함,
    + base 함수와 derived 함수의 매개변수 타입은 동일해야 함,
    + base 함수와 derived 함수의 const성이 동일해야 함,
    + base 함수와 derived 함수의 return 타입과  예외 명세가 호환되어야 함,
- c++11에 추가된 override 필수조건
    + 멤버 함수들의 참조 한정사(reference qualifier)들이 동일해야 함,

- reference qualifier란?
    + 멤버 함수 lValue, rValue로 한정시켜 사용하는 방법

```cpp
class Widget {
public:
    void doWrok() &;    // *this가 lValue일 경우,
    void doWork() &&;   // *this가 rValue일 경우,
};

// ex)
Widget MakeWidget();    // factory 함수 (rValue)
Widget w;               // 보통 객체 (lValue)

w.doWork();             // & 호출
MakeWidget().doWork();  // && 호출 
```

- erference qualifier 예제

```cpp
class Widget {
public:
    using DataType = std::vector<double>;
    DataType& data() { return values; }
private:
    DataType values;
};

// ex)
Widget w;
auto vals1 = w.data();

Widget MakeWidget();
auto vals2 = MakeWidget().data();   // 복사 비용을 줄이고 싶지 않아?
```

- factory 함수에서 rValue를 생성하였으면, copy보단 move를 이용하고 싶을 것이다,
- 감이 잡히는가?

```cpp
class Widget {
public:
    using DataType = std::vector<double>;
    DataType&  data() &  { return values; }
    DataType&& data() && { return std::move(values); }

    // reference qualifier 시, 참조 되지 않은 overloading은 사용 금지,
    // DataType&& data() { return std::move(values); }
    // ref-qualifier 함수와 일반 함수 중 둘이 경쟁 구도를 일으키게 됨,
    // 컴파일러는 누구를 사용해야되는지 판단 할 수 없음,
    // Error C2559 'void Test::mf1(void)': cannot overload a member function without ref-qualifier with a member function with ref-qualifier

private:
    DataType values;
};

auto vals1 = w.data();              // 복사 생성
auto vals2 = MakeWidget().data();   // 이동 생성
```

# 결론 요약
- 재정의 함수는 override를 명시적으로 선언하라.
- reference qualifier를 통해, 멤버 함수가 호출되는 객체(*this)의 lValue, rValue를 다르게 처리할 수 있다.