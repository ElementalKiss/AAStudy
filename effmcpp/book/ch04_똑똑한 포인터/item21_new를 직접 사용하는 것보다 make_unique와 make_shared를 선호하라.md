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
* shared_ptr과 make_shared를 unique_ptr과 make_unique로 대체해도 동일하다.

## std::make_shared의 특징 하나는 향상된 효율성이다.
* std::make_shared를 사용하면 컴파일러가 좀 더 간결한 자료구조를 사용하는 더 작고 빠른 코드를 산출할 수 있게 된다.

* new를 직접 사용하는 코드는 실제로는 두 번의 할당이 일어난다.
* Widget 객체를 위한 메모리 할당과 제어 블록을 위한 또 다른 메모리 할당이 일어난다.

* std::make_shared를 사용하면 한 번의 할당으로 충분하다.
* Widget 객체와 제어 블록 모두를 담을 메모리를 한 번에 할당하기 때문이다.

* 프로그램의 정적 크기가 줄어들며, 실행 코드의 속도도 빨라진다.
* std::make_shared의 효율성은 std::allocate_shared에도 거의 그대로 적용된다.

* 이처럼 new보다 make 함수들이 소프트웨어 공학과 예외 안정성, 효율성 면에서 유리하다.

## 이 항목의 조언은 이 함수들을 항상 사용하라는 것이 아니라 **선호하라** 는 것임을 주의하자.

* make 함수들 중에는 커스텀 소멸자를 지정할 수 있는 것이 없다. 그러나 std::unique_ptr과 std::shared_ptr은 커스텀 소멸자를 받는 생성자들을 제공한다.
* Widget을 위한 다음과 같은 커스텀 소멸자가 있다고 할 때,
```cpp
auto widgetDeleter = [](Widget* pw) {...};
```

* 이 소멸자를 사용하는 스마트 포인터를 new를 이용해서 생성하는 것은 간단하다.
```cpp
std::unique_ptr<Widget, decltype(widgetDeleter)> upw(new Widget, widgetDeleter);

std::shared_ptr<Widget> spw(new Widget, widgetDeleter);
```

* 그러나 make 함수로는 이런 일을 할 수 없다.

## make 함수들의 두 번째 한계는 그 구현들의 구문적 세부사항에서 비롯된 것이다.
* std::initializer_list를 받는 생성자와 받지 않는 생성자를 모두 가진 형식의 객체를 생성할 때, 생성자 인수들을 중괄호로 감싸면 중복 적재 해소 과정에서 std::initialize_list를 받는 버전이 선택되고, 괄호로 감싸면 std::initializer_list를 받지 않는 버전이 선택된다.
* make 함수들은 자신의 매개변수들을 객체의 생성자에 완벽하게 전달한다.

```cpp
auto upv = std::make_unique<std::vector<int>>(10, 20);
auto spv = std::make_shared<std::vector<int>>(10, 20);
```

* 스마트 포인터들은 값이 20인 요소 열 개를 담은 vector를 가리킬까?
  각각 10과 20인 두 요소를 담은 vector를 가리킬까?
* 결론은 값이 20인 요소 열 개짜리 vector를 생성한다.
* 이는 make 함수들이 내부적으로 매개변수들을 완벽 전달할 때 중괄호가 아닌 괄호를 사용함을 뜻한다.
* 중괄호 초기치로 생성하려면 반드시 new를 사용해야 한다.

* make 함수 중 하나로 그런일을 하려면 중괄호 촉기치를 완벽하게 전달할 수 있어야 하는데, 항목30에서 설명하듯이 완벽 전달은 불가능하다.
* 항목30에서 우회책이 나온다. auto 형식 연역을 이용하여 중괄호 초기치로부터 std::initializer_list 객체를 생성하고, 그것을 make 함수에 넘기면 된다.
```cpp
// std::initializer_list 객체 생성
auto initList = {10, 20};

// 그 std::initializer_list 객체를 이용해서 std::vector를 생성
auto spv = std::make_shared<std::vector<int>>(initList);
```

* std::unique_ptr의 경우에는 이상의 두 시나리오가 make 함수들이 문제가 되는 상황이 전부이다.
* std::shared_ptr과 해당 make 함수들의 경우에는 문제가 되는 시나리오가 두 개 더 있다.(극단적인 경우이지만)
* 클래스 중에 자신만의 operator new와 operator delete를 정의하는 것들이 있다.

## new의 직접 사용에 비한 std::make_shared의 크기 및 속도상의 장점은 std::shared_ptr의 제어 블록이 관리 대상 객체와 동일한 메모리 조각에 놓인다는 점에서 비롯된다.
* 그 객체의 참조 횟수가 0이 되면 객체가 파괴된다.
* 그러나 객체가 차지하고 있던 메모리는 해당 제어 블록이 파괴되기 전까지는 해제될 수 없다.
* 객체와 제어 블록이 동적으로 할당된 같은 메모리 조각에 들어 있기 때문이다.

### 제어 블록에는 참조 카운트 외에도 여러 관리용 정보가 들어 있을 수 있다.
* 참조 횟수는 제어 블록을 참조하는 std::shared_ptr들의 개수를 뜻한다.
* 제어 블록을 참조하는 std::weak_ptr들의 개수에 해당하는 weak count도 있다.
* 만료 판정 시 std::weak_ptr은 자신이 가리키는 제어 블록에 있는 참조 카운트를 점검하고, 0이면 std::weak_ptr은 만료된 것이고, 0이 아니면 만료가 아니다.

* 제어 블록을 참조하는 std::weak_ptr들이 존재하는 한(즉, weak count가 0보다 크다면), 제어 블록은 계속해서 존재해야 한다.
* 그리고 제어 블록이 존재하는 한 제어 블록을 담고 있는 메모리는 여전히 할당된 상태이어야 한다.
* 따라서, std::shared_ptr용 make 함수가 할당한 메모리 조각은 그것을 참조하는 마지막 shared_ptr과 마지막 waek_ptr 둘 다 파괴된 후에만 해제될 수 있다.
* 객체 타입이 크고 마지막 shared_ptr의 파괴와 마지막 weak_ptr의 파괴 사이의 시간 간격이 꽤 길다면, **객체가 파괴된 시점과 객체가 점유하던 메모리가 해제되는 시점 사이에 시간 지연이 생길 수 있다.**

```cpp
class ReallyBigType { ... };

auto pBigObj = std::make_shared<ReallyBigType>();   // 아주 큰 객체 생성

... // 큰 객체를 가리키는 shared_ptr들과 weak_ptr들을 생성해서 사용

... // 여기서 객체를 가리키는 마지막 shared_ptr이 파괴되나, weak_ptr들은 여전히 남아 있다.

... // 이 부분에서 큰 객체가 차지하던 메모리는 여전히 할당된 상태

... // 여기서 마지막 weak_ptr이 파괴되며, 제어 블록과 객체가 차지하던 메모리가 해제

```

* new를 직접 사용한다면 ReallyBigType 객체를 가리키던 마지막 shared_ptr이 파괴되는 즉시 메모리가 해제될 수 있다.

```cpp
class ReallyBigType { ... };

std::shared_ptr<ReallyBigType> pBigObj(new ReallyBigType);  // 아주 큰 객체를 new를 이용해서 생성

...     // 이전처럼 객체를 가리키는 shared_ptr들과 weak_ptr들을 생성해서 사용

...     // 객체를 가리키는 마지막 shared_ptr이 파괴되나, weak_ptr들은 여전히 남아있다. 객체의 메모리는 해제된다.

...     // 제어 블록을 위한 메모리만 할당된 상태

...     // 여기서 객체를 가리키는 마지막 weak_ptr이 파괴된다. 이제 제어 블록의 메모리 해제

```


* 혹시라도 make_shared를 사용할 수 없거나 부적합한 상황이라면, 예외 안정선 문제들을 세심하게 방지할 필요가 있다.
* new 를 직접 사용하면서도 예외 안정성 문제를 겪지 않는 최선의 방책은, new의 결과를 **다른 일은 전혀 하지 않는 문장에서** 스마트 포인터의 생성자에 즉시 넘겨주는 것이다.
* 그러면 new 연산자의 호출과 생성된 객체를 관리할 스마트 포인터 생성자 호출 사이에서 예외가 발생할 여지가 사라진다.

* 한 예로 예외에 안전하지 않은 processWidget 함수 호출 예제에서 커스텀 삭제자를 사용한다.
```cpp
void processWidget(std::shared_ptr<Widget> spw, int priorty);

void customDel(Widget *ptr);    // 커스텀 삭제자

processWidget(std::shared_ptr<Widget>(new Widget, customDel), computePriority());   // 자원 누수 위험이 있음
```

* 예제에서는 커스텀 삭제자 때문에 make_shared를 사용할 수 없다.
* Widget의 할당과 shared_ptr 생성을 개별적인 문장으로 두고, 그 문장에서 생성한 shared_ptr로 processWidget을 호출하자.
```cpp
std::shared_ptr<Widget> spw(new Widget, customDel);

processWidget(spw, computePriority());  // 최적은 아님, 개선이 필요
```

* 위코드는 생성자에서 예외가 발생한다 해도, shared_ptr은 생성자로 전달된 생 포인터의 소유권을 확보하기 때문에 예외에 안전하다.

* 예외에 안전하지 않은 호출에서는 processWidget에 R벨류를 넘겨준다.
* 그러나 안전한 호출에서는 L벨류를 넘겨준다.

* processWidget의 shared_ptr 매개변수는 값 전달 방식이므로, R벨류를 넘겨줄 때는 shared_ptr 객체가 move 생성에 의해 만들어진다.
* 그러나 L벨류를 넘겨주면 copy 생성이 일어난다.
* 복사하려면 참조 카운트를 원자적으로 증가해야 하지만, 이동할 때에는 참조 카운트를 조작할 필요가 없기 때문에 move와 copy의 차이가 클 수 있다.
* 예외에 안전한 코드의 성능을 끌어 올리려면, std::move를 적용해서(항목 23 참고) spw를 R벨류로 변환해야 한다.
```cpp
processWidget(std::move(spw), computePriority());   // 예외 안정성과 효율성을 모두 갖춤
```

* make 함수를 사용할 수 없는 상황은 흔치 않다.
* 사용하지 않을 특별한 이유가 없는 한, make 함수를 사용하는 것이 옳은 선택이다.

## 기억해 둘 사항들
* new의 직접 사용에 비해, make 함수를 사용하면 소스 코드 중복의 여지가 없어지고, 예외 안전성이 향상되고, std::make_shared와 std::allocate_shared의 경우 더 작고 빠른 코드가 산출된다.
* make 함수의 사용이 불가능하거나 부적합한 경우는 커스텀 삭제자를 지정해야 하는 경우와 중괄호 초기치를 전달해야 하는 경우가 있다.
* std::shared_ptr에 대해서는 make 함수가 부적합한 경우가 더 있는데, 첫 번째 예는 (1) 커스텀 메모리 관리 기능을 가진 클래스를 다루는 경우와 (2) 메모리가 넉넉하지 않은 시스템에서 큰 객체를 자주 다루어야 하고 weak_ptr들이 해당 shared_ptr들 보다 더 오래 살아남는 경우이다.
