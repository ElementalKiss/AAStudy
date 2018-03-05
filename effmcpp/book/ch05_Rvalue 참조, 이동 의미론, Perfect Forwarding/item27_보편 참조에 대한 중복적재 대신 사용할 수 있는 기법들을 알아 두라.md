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
* 

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
* 포워딩 레퍼런스를 사용하는 이유가 완벽 전달이라면, 보편 참조 말고는 다른 대안이 없다..
* 포워딩 레퍼런스와 오버로딩 둘 다 사용하되 포워딩 레퍼런스에 대한 오버로딩은 피하려면 어떡해야 할까?

* 오버로딩된 함수들의 모든 가능한 조합을 고려해서, 가장 잘 부합하는 것을 선택한다.
* 일반적으로 포워딩 레퍼런스 매개변수는 전달된 인수에 대해 정확히 부합하지만, 매개변수 목록에 보편 참조 매개변수뿐만 아니라 보편 참조가 아닌 매개변수들도 포함되어 있으면, 포워딩 레퍼런스가 아닌 매개변수들에 대해 충분히 나쁜 부합이 포워딩 레퍼런스 매개변수가 있는 오버로딩을 제치고 선택될 가능성이 있다.
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
}
```


## 포워딩 레퍼런스를 받는 템플릿을 제한한다

## 절충점들
