# 항목 20: shared_ptr처럼 작동하되 대상을 잃을 수도 있는 포인터가 필요하면 weak_ptr를 사용하라

* shared_ptr 비슷하되 참조 횟수에 영향을 미치지 않는 포인터가 필요한 경우가 있다.
* 포인터의 인스턴스가 사라진 상황을 검출할 수 있어야 되는데 이것이 weak_ptr

## weak_ptr

* API 자체는 별로 똑똑해 보이지 않다.
* 역참조할 수 없으며, NULL인지 판정할 수 없다.
* (중요) shared_ptr를 보강하는 스마트 포인터이다.
* weak_ptr의 생성은 shared_ptr를 이용하여 생성한다.

## weak_ptr 생성, 만료

```cpp
auto spw = std::make_shared<Widget>();  // spw 생성된 후, 참조 카운트는 1
std::weak_ptr<Widget> wpw(spw);         // wpw는 spw와 같은 Widget을 가리키지만 참조 카운트는 그대로 1
spw = nullptr;                          // 횟수가 0이 되고 Widget은 파괴. wpw는 대상을 잃는다.

if(wpw.expired()) { }                   // 이런 코드는 짜면 안 된다고 처음에 적었지만
else() { } // use wpw                   // else일 때 객체에 접근하는 방식과 같은
                                        // 코드를 짜면 안된다고 한다.
```

* 대상을 잃은 weak_ptr을 가리켜 만료되었다(expired)라고 한다.
* weak_ptr을 이용해 만료 상태를 보고 인스턴스 객체에 접근하는 코드는 허용되지 않는다.
* 역참조 연산이 없으므로 애초에 작성할 수 없고 만료 점검과 참조를 분리하면 경쟁 조건이 발생할 수 있다.
* expired 호출과 역참조 사이에서 다른 스레드가 해당 객체를 가리키는 마지막 shard_ptr을 배정하거나 파괴할 수 있다.
* 그 스레드에서 객체가 파괴되는 시점에 expired를 통과 후 포인터를 역참조 하려면 미정의 행동이 나온다.

## weak_ptr 사용의 제대로된 용법

* weak_ptr의 만료 여부를 점검한다.
* 만료되지 않았다면 해당 객체에 대한 접근을 돌려주는 연산을 하나의 원자적 연산으로 수행한다.
* weak_ptr로부터 shared_ptr을 생성하면 된다.
* 두 가지 방법이 있는데 weak_ptr::lock과 shard_ptr 생성자를 이용하는 방법.

```cpp
std::shared_ptr<Widget> spw1 = wpw.lock();  // wpw가 만료이면 spw1은 널
std::shared_ptr<Widget> spw2(wpw)           // wpw가 만료이면 std::bad_weak_ptr가 발생
```

## 유용한 예제1: Widget Caching

```cpp
std::shared_ptr<const Widget> fastLoadWidget(WidgetID id)
{
    static std::unordered_map<WidgetID, std::weak_ptr<const Widget>> cache;
    auto objPtr = cache[id].lock(); // objPtr은 캐시에 있는 객체를 가리키는
                                    // std::shared_ptr. 단, 객체가 캐시에 없으면 nullptr

    if (!objPtr) {                  // 캐시에 없으면 적재하고 캐시에 저장
        objPtr = loadWidget(id);
        cache[id] = objPtr;
    }

    return objPtr;
}
```

## 유용한 예제2: Observer Pattern

* Subject는 Observer들을 가리키는 포인터들을 담는 자료 맴버가 있다.
* Subject은 Observer들의 수명을 제어하는데에 관심이 없다.
* 하지만 소멸된 Observer들에게 접근 하는 일은 절대 없어야 한다.
* 그래서 이 포인터들을 weak_ptr로 가지는 것이 유리하다. 만료 여부를 판단하고 Observer들에 접근.

## 유용한 예제3: raw 포인터의 미정의 동작과 Shared_ptr의 Cycle 문제 피하기

```
class MyClass;
class SharedClass {
public:
    // std::shared_ptr<MyClass> _ptr;
};

class MyClass {
public:
    std::shared_ptr<SharedClass> _ptr;
};

void main()
{
    auto B = std::make_shared<SharedClass>();
    MyClass A;
    MyClass C;
    A._ptr = B;
    C._ptr = B;

    // B->_ptr = std::make_shared<MyClass>(A);
}

```

* A와 C가 B의 자원을 shared_ptr로 공유하고 있는 상황.
* 공유되고 있는 SharedClass도 MyClass A와 B의 포인터를 가지고 싶다면 어떤 포인터가 적절할까?
* raw 포인터: Dangling. 미정의 행동.
* shared_ptr: Cycle로 인해 둘 다 파괴하지 못한다.

```cpp
사이클 관련 디버깅
+		std::shared_ptr<SharedClass>::operator->이(가) 반환되었습니다.	0x000001d7b47c9130 {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {_ptr=shared_ptr {...} [4 strong refs] 
```

* weak_ptr 두 문제를 모두 해결할 수 있다.

## 정리

* std::shared_ptr처럼 작동하되 대상을 잃을 수도 있는 포인터가 필요하면 std::weak_ptr를 사용하라.
* std::weak_ptr의 잠재적인 용도로는 캐싱, 관찰자 목록, 그리고 std::shared_ptr 순환 고리 방지가 있다.
