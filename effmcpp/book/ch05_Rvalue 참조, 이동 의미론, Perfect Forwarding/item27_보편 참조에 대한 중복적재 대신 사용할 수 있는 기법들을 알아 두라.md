# 항목 27: forwarding reference에 대한 overloading 대신 사용할 수 있는 기법들을 알아두라

* 포워딩 레퍼런스에 대한 오버로딩이 부합할 수 있는 형식을 제한함으로써 그런 바람직한 행동을 달성하는 방법들을 살펴보자

## 오버로딩을 포기한다

* 함수 이름을 각각 다르게 붙이면 포워딩 레퍼런스에 대한 오버로딩의 단점을 피할 수 있다.
* 그러나 이 방식은 두번째 예제인 Person의 생성자에는 통하지 않는다. 생성자의 이름은 언어에 의해 고정되기 때문
* 게다가 오버로딩을 왜 포기..?

## const T& 매개변수를 사용한다

* C++98로 돌아가서 포워딩 레퍼런스 매개변수 대신 const에 대한 l-value 참조 매개변수를 사용하는 것
* 26장에서 logAndAddName의 첫버전이 사용한 접근방식
* 효율적이지는 못하다. 문제가 있으니 효율성을 포기하더라도 예상치 않은 문제를 피하는 것이 괜찮은 절충안


## 값 전달 방식의 매개변수를 사용한다

* 종종 복잡도를 높이지 않고 성능을 높일 수 있는 한 가지 접근방식은, call by reference 대신 call by value를 사용하는 것

```cpp
class Person {
public:
    explicit Person(std::string n)  // T&& 생성자를 대체
    : name(std::move(n)) {}

    explicit Person(int inx)
    :name(nameFromIdx(idx)) {}

private:
    std::string name;
}
```

* int 타입의 인수로 Person 생성자를 호출하면 int를 받는 오버로딩이 선택된다.
* 마찬가지로, std::string 을 생성할 수 있는 인수에 대해서는 std::string을 받는 생성자가 선택된다.


## 꼬리표 배분을 사용한다

* const l-value 참조 전달이나 값 전달은 완벽 전달을 지원하지 않는다.
* 포워딩 레퍼런스를 사용하는 이유가 완벽 전달이라면, 포워딩 레퍼런스 말고는 다른 대안이 없다..
* 포워딩 레퍼런스와 오버로딩 둘 다 사용하되 포워딩 레퍼런스에 대한 오버로딩은 피하려면 어떡해야 할까?

* 오버로딩된 함수들의 모든 가능한 조합을 고려해서, 가장 잘 부합하는 것을 선택한다.
* 일반적으로 포워딩 레퍼런스 매개변수는 전달된 인수에 대해 정확히 부합하지만, 매개변수 목록에 포워딩 레퍼런스 매개변수뿐만 아니라 포워딩 레퍼런스가 아닌 매개변수들도 포함되어 있으면, 포워딩 레퍼런스가 아닌 매개변수들에 대해 충분히 나쁜 부합이 포워딩 레퍼런스 매개변수가 있는 오버로딩을 제치고 선택될 가능성이 있다.
* 이것이 꼬리표 배분(tag dispatch) 접근 방식에 깔린 기본 착안

```cpp
// 앞장의 LogAndAdd
std::multiset<std::string> names;   // 전역 자료 구조

template<class T>
void LogAndAdd(T&& name)
{
    auto now = std::chrono::system_clock::now();
    log(now, "LogAndAdd");
    names.emplace(std::forward<T>(name));
}
```

```cpp
//앞의 LogAndAdd 함수에 꼬리표 배분 기법을 적용
template<typename T>
void LogAndAdd(T&& name)
{
    LogAndAddImpl(std::forward<T>(name), std::is_integral<T>());    // 아주 정확하지는 않음

    LogAndAddImpl(std::forward<T>(name), std::is_integral<typename std::remove_reference<T>::type>());
}
```

```cpp
template<typename T>
void LogAndAdd(T&& name, std::false_type)
{
    LogAndAddImpl(std::forward<T>(name), std::is_integral<T>());    // 아주 정확하지는 않음
}
```

```cpp
template<typename T>
void LogAndAddImpl(T&& name, std::false_type)
{
    auto now = std::chrono::system_clock::now();
    log(now, "LogAndAdd");
    names.emplace(std::forward<T>(name));
}

std::string nameFromIdx(int idx);

void LogAndAddImpl(int idx, std::true_type)
{
    LogAndAdd(nameFromIdx());
}
```

* 이러한 설계에서 std::true_type 과 std::false_type 은 오버로딩 해소가 우리가 원하는 방식으로 일어나게 하는 데에만 쓰이는 일종의 '꼬리표(tag)'이다.
* LogAndAdd 안에서 오버로딩된 구현 함수들을 호출하는 것은 꼬리표에서 기초해서 '배분(dispatch)'하는 것에 해당한다.
* 이는 템플릿 메타프로그래밍의 표준적인 구축 요소로 C++ 라이브러리들의 내부에 이런 설계가 많다.

## 포워딩 레퍼런스를 받는 템플릿을 제한한다

* 꼬리표 배분에는 Person의 완벽 전달 생성자에 관련된 문제점이 있다.
* 컴파일러가 자동으로 복사, 이동 생성자를 작성할 수 있으므로, 만일 생성자를 하나만 작성해서 그 안에서 꼬리표 배분을 사용한다면, 일부 생성자 호출은 컴파일러가 작성한 함수들로 처리되어 결과적으로 꼬리표 배분이 적용되지 않을 위험이 있다.
* std::enable_if를 이용하면 컴파일러가 특정 템플릿이 존재하지 않는것처럼 만들 수 있다. 그런 템플릿을 비활성화된(disabled) 템플릿이라 한다.
* 모든 템플릿은 활성화된(enabled) 상태이나, std::enable_if를 사용하면 지정된 조건이 만족될 경우에만 활성화된다.
* 지금의 예에서는 Person이 아닌 형식의 인수가 전달된 경우에만 Person의 완벽 전달 생성자가 활성화되게 해야 한다.

```cpp

class Person {
public:
    template<typename T, typename = typename std::enable_if<조건>::type>
    explicit Person(T&& n);
}
```
* 일어나는 일을 정확히 이해하고 싶다면 다른책을 참고하란다..(내용이 길기 때문에..)
* 조건에는 !std::is_same<Person, T>::value 를 지정

* 단, 참조 여부. Person, Person&, Person&&은 모두 Person으로 간주되어야 한다.
* const성과 volatile성도 마찬가지 모두 Person과 같은 형식으로 간주되어야 한다.

* std::decay<T> 는 T 에 붙은 모든 참조와 cv-한정사(const, volatile)를 제거한 타입에 해당된다.

* 결론적으로 조건은 !std::is_same<Person, std::decay<T>::type>::value

```cpp
class Person {
public:
    template<typename T, typename = std::enable_if_t<!std::is_base_of<Person, std::decay_t<T>>::value
        && !std::is_integral<std::remove_reference_t<T>>::value>>
    explicit Person(T&& n) : name(std::forward<T>(n))
    {...}

    explicit Person(int idx) : name(nameFromIdx(idx))
    {...}

private:
    std::string name;
}
```
* 이 설계는 완벽 전달을 사용하므로 효율성이 최대이며, 보편 참조에 대한 오버로딩을 금지하는 대신 둘의 조합을 제어하므로 오버로딩을 피할 수 없는 상황에도 적용할 수 있다.


## 기억해 둘 사항들
* std::enable_if를 사용해서 템플릿의 인스턴스화를 제한함으로써 포워딩 레퍼런스와 오버로딩을 함께 사용할 수 있다.
  std::enable_if는 컴파일러가 포워딩 레퍼런스 중 중복적재를 사용하는 조건을 프로그래머가 직접 제어하는 용도로 쓰인다.
* 포워딩 레퍼런스 매개변수는 효율성 면에서 장점인 경우가 많지만, 대체로 사용성 면에서는 단점이 된다.