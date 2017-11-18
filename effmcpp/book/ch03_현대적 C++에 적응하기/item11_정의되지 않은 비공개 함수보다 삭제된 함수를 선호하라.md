# 항목 11: 정의되지 않은 비공개 함수보다 삭제된 함수를 선호하라

## 개요

- 다른 개발자에게 제공할 코드를 작성할 때, 그 개발자가 코드의 특정 함수를 호출하지 못하게 하고 싶다.
- 함수를 정의하지 않으면 되겠지만, 복사생성자(copy constructor), 할당연산자(assignment operator)의 경우 컴파일러가 생성해버림.
- 이런 함수의 사용을 막고 싶으면?
    + C++98: 정의되지 않은 비공개 함수
    + C++11: 삭제된 함수

## 정의되지 않은 비공개 함수

- 다음과 같이 private으로 선언만 하고 정의는 하지 않는다.

```c++
template <class charT, class traits = char_traits<charT>>
class basic_ios : public ios_base {
public:
    /* ... */
private:
    basic_ios(const basic_ios&);              // not defined
    basic_ios& operator = (const basic_ios&); // not defined
    /* ... */
}
```

- 위는 C++98 표준에 정의되어 있는 `basic_ios` 클래스이다. (주석도 표준임)
- `basic_ios` 클래스의 복사생성자와 할당연산자는 private으로 선언되어 있어 외부에서 호출할 수 없다.
- 정의되어 있지 않기 때문에 호출하면 컴파일은 되지만 **링크 에러**가 날 것이다.

## 삭제된 함수

```c++
template <class charT, class traits = char_traits<charT>>
class basic_ios : public ios_base {
public:
    /* ... */
    basic_ios(const basic_ios&) = delete;
    basic_ios& operator = (const basic_ios&) = delete;
    /* ... */
```

- 위는 C++11 표준에 정의되어 있는 `basic_ios` 클래스이다.
- 삭제된 함수는 어떤 방법으로든 사용할 수 없다. 호출하게 되면 **컴파일 에러**가 난다.

```c++
bool isLucky(int number);
bool isLucky(char) = delete;
bool isLucky(bool) = delete;
bool isLucky(double) = delete;

if (isLucky(1)) {}
if (isLucky('a')) {}    // 컴파일 에러
if (isLucky(true)) {}   // 컴파일 에러
if (isLucky(3.5)) {}    // 컴파일 에러
```

- 위와 같이 멤버함수가 아니더라도 삭제할 수 있다.
- `char`, `bool`, `double`의 타입에 대해 오버로딩을 막지 않았다면 `'a'`, `true`, `3.5` 모두 `int` 타입으로 암묵적인 형변환이 되어 컴파일 되버렸을 것이다.

```c++
template<typename T>
void ProcessPointer(T* ptr);

template<>
void ProcessPointer<void>(void*) = delete;
```

- 위와 같이 템플릿 함수의 특정 타입으로의 인스턴스화를 방지할 수도 있다.

```c++
class Widget {
public:
    /* ... */
    template<typename T>
    void ProcessPointer(T* ptr);
    /* ... */
}

template<>
void Widget::ProcessPointer<void>(void*) = delete;
```

- 템플릿 멤버함수의 특정 타입으로의 인스턴스화를 방지할 수도 있다.

## 요약

- C++98의 정의되지 않은 비공개 함수 방식은 멤버함수만 호출을 막을 수 있다.
- C++11의 삭제된 함수는 멤버함수뿐만 아니라 비멤버 함수와 템플릿 인스턴스까지 삭제할 수 있다.
- `delete`가 완벽한 상위 호환이다. `delete`를 쓰자.