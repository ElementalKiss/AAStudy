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

if(wpw.expired()) { }                   // 이런 코드는 짜면 안 된다!
```

* 대상을 잃은 weak_ptr을 가리켜 만료되었다(expired)라고 한다.
* weak_ptr을 이용해 만료 상태를 보고 인스턴스 객체에 접근하는 코드는 허용되지 않는다.
* 역참조 연산이 없으므로 애초에 작성할 수 없고 만료 점검과 참조를 분리하면 경쟁 조건이 발생할 수 있다.
* expired 호출과 역참조 사이에서 다른 스레드가 해당 객체를 가리키는 마지막 shard_ptr을 배정하거나 파괴할 수 있다.
* 그 스레드에서 객체가 파괴되는 시점에 expired를 통과 후 포인터를 역참조 하려면 미정의 행동이 나온다.

## 정리

* std::shared_ptr처럼 작동하되 대상을 잃을 수도 있는 포인터가 필요하면 std::weak_ptr를 사용하라.
* std::weak_ptr의 잠재적인 용도로는 캐싱, 관찰자 목록, 그리고 std::shared_ptr 순환 고리 방지가 있다.