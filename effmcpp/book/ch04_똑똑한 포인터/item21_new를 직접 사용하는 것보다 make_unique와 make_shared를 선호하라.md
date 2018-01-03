# 항목 21: new를 직접 사용하는 것보다 std::make_unique와 std::make_shared를 선호하라

* std::make_shared는 C++11의 일부이지만 std::make_unique는 C++14에서 표준 라이브러리에 포함되었다.

* c++11을 사용하는 독자는 std::make_unique의 기본적인 버전을 직접 작성하는 것이 어렵지 않다.
```cpp
template<typename T, typename... Ts>
std::unique_ptr<T> make_unique(TS&&... params)
{
    return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
}
```

* 단, 직접 만든 구현을 std 네임스페이스에 집어넣지는 말아야한다.
  나중에 C++14 표준 라이브러리 구현으로 업그레이드했을 때 표준 라이브러리 버전과 충돌할 것이기 때문이다.

* std::make_unique와 std::make_shared는 임의의 개수와 형식의 인수들을 받아서 해당 생성자로 완벽 전달해서 객체를 동적으로 생성하고, 그 객체를 가리키는 스마트 포인터를 돌려주는 세 가지 make 함수 중 둘이다.

* 나머지 하나는 std::allocate_shared이다.
* 이 함수는 std::make_shared처럼 작동하되, 첫 인수가 동적 메모리 할당에 쓰일 할당자 객체라는 점이 다르다.

## make 함수를 선호해야 하는 이유

* 스마트 포인터를 make 함수로 생성하는 것과 그냥 생성하는 것을 단순히 비교해 보기만 해도 make 를 선호할 첫 번째 이유가 명백해 진다.
```cpp
auto upw1(std::make_unique<Widget>());      // make 사용
std::unique_ptr<Widget> upw2(new Widget);   // 사용 안함

auto spw1(std::make_shared<Widget>());      // make 사용
std::shared_ptr<Widget> spw2(new Widget);   // 사용 안함
```

* **Widget** 이 쓰여있는 부분들이 두 방식의 본질적인 차이점
* new를 사용하는 버전은 생성할 객체의 형식이 되풀이되어 나오지만, make 버전은 그렇지 않다. (코드가 중복된다..)

* 두 번째는 예외 안전성과 관련이 있다.
* 어떤 Widget 객체를 그 객체의 우선순위에 따라 적절히 처리하는 함수가 있다고 하자
```cpp
void processWidget(std::shared_ptr<Widget> spw, int priority);
```

* std::shared_ptr을 값으로 전달하는 것이 꺼림직할 수 있지만 항목 41에서 설명하듯이 만일 processWidget이 항상 std::shared_ptr의 복사본을 만든다면, 이러한 값 전달 방식은 합당한 설계상의 결정이다.

```cpp
int computePriority();      // 우선 순위를 계산하는 함수
```

* std::make_shared 대신 new를 사용한 호출
```cpp
processWidget(std::shared_ptr<Widget>(new Widget), computePriority());   // 누수의 위험이 있음
```

* 컴파일러가 소스코드를 목적 코드(object code)로 번역하는 방식과 관련이 있다.
* 실행시점에서 함수가 호출될 때, 함수의 코드가 실행되기 전에 함수의 인수들이 먼저 평가된다.
* processWidget 자체가 실행되기 전에 다음과 같은 일들이 일어난다.
    - 표현식 "new Widget"이 평가된다. 즉, Widget이 힙에 생성된다.
    - new가 산출한 포인터를 관리하는 std::shared_ptr<Widget>의 생성자가 실행된다.
    - computePriority가 실행된다.
* 이 세가지 일을 순서대로 실행하는 것은 아니다.
* std::shared_ptr 생성자가 호출되려면 그 인수가 먼저 평가되어야 하므로 "new Widget"이 먼저 평가되는 것을 확실하다.
* 그러나 computePriority는 먼저, 나중에 혹은 그 **사이에서** 실행될 수도 있다.
    1. "new Widget" 실행
    2. computePriority 실행
    3. std::shared_ptr 실행
* 이런 코드가 만들어 졌다면, 그리고 실행 시점에서 computePriority가 예외를 던졌다면, 1단계에서 동적으로 할당된 Widget 객체가 새게 된다.
* std::shared_ptr은 3단계에서야 생성되며, 예외가 던져진 시점에서 Widget 객체는 그냥 생 포인터가 가리키는 동적 할당 객체일 뿐이다.

* std::make_shared를 사용하면 이런 문제가 발생하지 않는다.
```cpp
processWidget(std::make_shared<Widget>(), computePriority());   // 누수의 위험이 없음
```
