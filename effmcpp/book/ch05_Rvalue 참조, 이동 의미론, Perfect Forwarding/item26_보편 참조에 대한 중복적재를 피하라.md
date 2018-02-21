# 항목 26: forwarding reference에 대한 overloading을 피하라

## 작성하려는 함수

* 매개변수 : const std::string& name
* 함수 내용 : log(now), names.emplace(name)

```cpp
std::multiset<std::string> names;

void LogAndAdd(const std::string& name)
{
    auto now = std::chrono::system_clock::now();
    log(now);
    names.emplace(name);
}
```

---
## LogAndAdd 함수의 비효율성

```cpp
std::string myPetName("SerKey");
LogAndAdd(myPetName); // case 1: lvalue 전달
LogAndAdd(std::string("SerKey")); // case 2: rvalue 전달
LogAndAdd("SerKey"); // case 3: literal value 전달
```

### case 1

* lvalue 전달.
* 매개변수 name은 변수 myPetName에 binding.
* names.emplace로 전달되는데 lvalue이므로 복사는 불가피.

### case 2

* rvalue 전달.
* 매개변수 name은 std::string("SerKey")로 생성된 rvalue로 binding 된다.
* 하지만 name 자체는 lvalue라서 names에 복사를 하게 된다.
* **중요) 이동을 한다면 복사 비용을 줄일 수 있다.**

### case 3

* rvalue 전달.
* 매개변수 name은 literal value로 생성된 임시 std::string 객체에 binding.
* case 2처럼 name은 names에 복사.
* **중요) 애초에 전달 된 인수가 문자열 literal이므로 std::string 객체 생성 없이 바로 names.emplace에 전달하면 복사는 커녕 이동 비용조차 필요 없다.**

---
## 개선된 LogAndAdd

```cpp
template<class T>
void LogAndAdd(T&& name)
{
    auto now = std::chrono::system_clock::now();
    log(now);
    names.emplace(std::forward<T>(name));
}

LogAndAdd(myPetName); // case 1: lvalue 전달
LogAndAdd(std::string("SerKey")); // case 2: rvalue 전달
LogAndAdd("SerKey"); // case 3: literal value 전달
```

### 개선된 case 1

* 이전과 동일하게 lvalue가 multiset으로 복사된다.

### 개선된 case 2

* rvalue가 이동한다.

### 개선된 case 3

* 이전 case 3에 비해 매개변수가 std::string을 복사하지 않는다. T를 통해 literal의 타입으로 전달받는다.
* std::forward<T>(name)에서 이동이나 복사 대신 literal을 바로 전달받아 multiset 내에 std::string를 생성한다.

---

## overloading을 구현해보자

* std::string NameFromIdx(int idx)라는 idx에 해당하는 이름을 돌려주는 함수가 있다.
* 일부 클라이언트가 name을 직접 들고 있지 못하고 ***이름을 가리키는 색인만*** 안다는 가정.
* idx 값을 넣으면 

```cpp
std::string NameFromIdx(int idx); // 색인에 대응하는 string을 리턴

void LogAndAdd(int idx) // new overloading function
{
    // 동일
    names.emplace(NameFromIdx(idx));
}
```

* 대부분 잘 동작한다.

### 문제되는 상황(중요)

* 클라이언트가 색인 값을 담은 short 값을 전달했다면?
* 'forwarding reference를 받는 버전의 함수에서 T를 short& 추론' VS 'int를 받는 버전에서 short를 int로 승격'
* 두 가지 중 인간이 보기엔 int로 승격이 맞아보인다.
* 하지만 일반 overloading 규칙에 따라, '정확한 부합'과 '승격'은 정확한 부합이 우선시된다.
* forwarding reference를 받는 템플릿 함수는 C++에서 가장 욕심이 많은 함수이므로 중복적재는 피하는 것이 좋다.

---

## 위의 overloading 문제를 해결하기 위해 perfect forwarding 생성자 구현 예제

```cpp
std::string NameFromIdx(int idx) {
    return std::string("test");
}

class Person {
public:
    template<class T> explicit Person(T&& n)
        : name(forward<T>(n)) {
        cout << "Call template<class T> explicit Person(T&& n)" << "\n";
    }

    explicit Person(int idx)
        : name(NameFromIdx(idx)) {
        cout << "Call explicit Person(int idx)" << "\n";
    }

private:
    std::string name;
};
```

* 앞선 LogAndAdd에서는 에러를 발생시키지 않았지만 perfect forwarding 생성자 호출 중에 에러 발생.

```cpp
    short n = 1;
    Person shortSerkey(n);
    /*
    심각도	코드	설명	프로젝트	파일	줄	비표시 오류(Suppression) 상태
    오류	C2664	'std::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(
    const std::basic_string<char,std::char_traits<char>,std::allocator<char>> &)': 
    인수 1을(를) 'short'에서 'std::initializer_list<_Elem>'(으)로 변환할 수 없습니다.	
    ConsoleApplication2	c:\work\test\consoleapplication2\consoleapplication2\소스.cpp	27	
    */
```

---

## LogAndAdd의 overloading 문제는 해결했으나 더욱더 심각한 문제가 많다.

### 복사 생성자와 이동 생성자를 구현 코드

```cpp
    // 복사 생성자와 이동 생성자를 구현한다.
    Person(const Person& rhs) { 
        cout << "Call Person(const Person& rhs)" << "\n";
    }

    Person(Person&& rhs) {
        cout << "Call Person(Person&& rhs)" << "\n";
    }
```

### 복사 생성자와 이동 생성자가 무시되는 경우

```cpp
    auto cloneOfSerkey(serkey);

    /*
    심각도	코드	설명	프로젝트	파일	줄	비표시 오류(Suppression) 상태
    오류	C2664	'std::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(
    const std::basic_string<char,std::char_traits<char>, std::allocator<char>> &)': 인수 1을(를)
    'Person'에서 'std::initializer_list<_Elem>'(으)로 변환할 수 없습니다.
    ConsoleApplication2	c:\work\test\consoleapplication2\consoleapplication2\소스.cpp	17
    */
```

* perfect forwarding이 이를 가로챈다.
* 가로챈 상황에서 string에는 Person을 받는 생성자가 없으므로 에러가 난다.

### 정확히 맞춰주어 복사 생성자를 부르는 경우

```cpp
    const Person constJo("min");
    auto cloneOfJo(constJo);

    /* CONSOL
        Call Person(const Person& rhs)
    */
```

* const 객체를 넘기는 경우 완전히 일치하여 복사 생성자를 부른다.
* 템플릿 함수와 비템플릿 함수가 똑같이 부합하는 상황이 생기면 비템플릿 함수가 우선시한다는 규칙이 있다.
* 그래서 결론은 어떤게 불릴지 모른고 프로그래머는 혼란이 올 수 있다.

### 상속했을 때 문제

```cpp
class DerivedPerson : public Person {
public:
    DerivedPerson(const DerivedPerson& rhs)
        : Person(rhs)
    {
        cout << "Call DerivedPerson(const DerivedPerson& rhs) : Person(rhs)" << "\n";
    }

    DerivedPerson(DerivedPerson&& rhs)
        : Person(std::move(rhs))
    {
        cout << "Call DerivedPerson(DerivedPerson&& rhs) : Person(std::move(rhs))" << "\n";
    }
};

/*
심각도	코드	설명	프로젝트	파일	줄	비표시 오류(Suppression) 상태
오류	C2664	'std::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(const std::basic_string<char,std::char_traits<char>,std::allocator<char>> &)': 인수 1을(를) 'const DerivedPerson'에서 'std::initializer_list<_Elem>'(으)로 변환할 수 없습니다.	ConsoleApplication2	c:\work\test\consoleapplication2\consoleapplication2\소스.cpp	27	
오류	C2664	'std::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(const std::basic_string<char,std::char_traits<char>,std::allocator<char>> &)': 인수 1을(를) 'DerivedPerson'에서 'std::initializer_list<_Elem>'(으)로 변환할 수 없습니다.	ConsoleApplication2	c:\work\test\consoleapplication2\consoleapplication2\소스.cpp	27
*/

```

* 파생 클래스의 복사, 이동 생성자에서 부모 클래스의 복사, 이동 생성자를 사용하고 싶다.
* 하지만 여기서 호출되는 것은 perfect forwarding 생성자를 호출하려 하고 IDE에선 위와 같은 에러를 리턴한다.

---

## 정리

* forwarding reference에 대한 overloading으로 다른 함수를 만들어도 거의 forwarding reference 버전이 불리는 경우가 많다.
* perfect forwarding 생성자 구현에 문제가 많다. 비 const lvalue에 대한 복사 생성자, 복사 및 이동 생성자의 호출을 가로챌 수 있다.

## 소스

```cpp
#include <iostream>
#include <algorithm>
#include <functional>
#include <utility>
using namespace std;

std::string NameFromIdx(int idx) {
    return std::string("test");
}

template<class T> void LogAndAdd(T&& name)
{
    cout << "Call template<class T> void LogAndAdd(T&& name)" << "\n";
}

void LogAndAdd(int idx)
{
    cout << "Call void LogAndAdd(int idx)" << "\n";
}

class Person {
public:
    template<class T> explicit Person(T&& n)
        : name(forward<T>(n)) {
        cout << "Call template<class T> explicit Person(T&& n)" << "\n";
    }

    explicit Person(int idx)
        : name(NameFromIdx(idx)) {
        cout << "Call explicit Person(int idx)" << "\n";
    }

    Person(const Person& rhs) { 
        cout << "Call Person(const Person& rhs)" << "\n";
    }

    Person(Person&& rhs) {
        cout << "Call Person(Person&& rhs)" << "\n";
    }

protected:
    string name;
};

/*
class DerivedPerson : public Person {
public:
    DerivedPerson(const DerivedPerson& rhs)
        : Person(rhs)
    {
        cout << "Call DerivedPerson(const DerivedPerson& rhs) : Person(rhs)" << "\n";
    }

    DerivedPerson(DerivedPerson&& rhs)
        : Person(std::move(rhs))
    {
        cout << "Call DerivedPerson(DerivedPerson&& rhs) : Person(std::move(rhs))" << "\n";
    }
};

심각도	코드	설명	프로젝트	파일	줄	비표시 오류(Suppression) 상태
오류	C2664	'std::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(const std::basic_string<char,std::char_traits<char>,std::allocator<char>> &)': 인수 1을(를) 'const DerivedPerson'에서 'std::initializer_list<_Elem>'(으)로 변환할 수 없습니다.	ConsoleApplication2	c:\work\test\consoleapplication2\consoleapplication2\소스.cpp	27
오류	C2664	'std::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(const std::basic_string<char,std::char_traits<char>,std::allocator<char>> &)': 인수 1을(를) 'DerivedPerson'에서 'std::initializer_list<_Elem>'(으)로 변환할 수 없습니다.	ConsoleApplication2	c:\work\test\consoleapplication2\consoleapplication2\소스.cpp	27
*/

int main()
{
    //////////////////////////////////////////////////////////////////////////////////////////////////
    LogAndAdd(1);
    short n = 1;
    LogAndAdd(n);

    /* CONSOL
        Call void LogAndAdd(int idx)
        Call template<class T> void LogAndAdd(T&& name)
    */

    /////////////////////////////////////////////////////////////////////////////////////////////////   
    Person serkey("SerKey");
    Person intSerkey(1);
    /* CONSOL
        Call template<class T> explicit Person(T&& n)
        Call explicit Person(int idx)
    */

    /*
    short n = 1;
    Person shortSerkey(n);
    
    심각도	코드	설명	프로젝트	파일	줄	비표시 오류(Suppression) 상태
    오류	C2664	'std::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(
    const std::basic_string<char,std::char_traits<char>,std::allocator<char>> &)': 
    인수 1을(를) 'short'에서 'std::initializer_list<_Elem>'(으)로 변환할 수 없습니다.	
    ConsoleApplication2	c:\work\test\consoleapplication2\consoleapplication2\소스.cpp	27	
    */

    /*
    auto cloneOfSerkey(serkey);

    복사 생성자가 불릴 것 같지만 perfect forwarding에서 에러가 나는 것.

    심각도	코드	설명	프로젝트	파일	줄	비표시 오류(Suppression) 상태
    오류	C2664	'std::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(
    const std::basic_string<char,std::char_traits<char>, std::allocator<char>> &)': 인수 1을(를)
    'Person'에서 'std::initializer_list<_Elem>'(으)로 변환할 수 없습니다.
    ConsoleApplication2	c:\work\test\consoleapplication2\consoleapplication2\소스.cpp	17
    */

    const Person constJo("min");
    auto cloneOfJo(constJo);
    /* CONSOL
        Call Person(const Person& rhs)
    */
    return 0;
}
```