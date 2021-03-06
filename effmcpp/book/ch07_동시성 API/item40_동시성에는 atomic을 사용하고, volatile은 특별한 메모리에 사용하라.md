
# 항목 40: 동시성에는 std::atomic을 사용하고, volatile은 특별한 메모리에 사용하라

## volatile의 오해

* volatile은 동시적 프로그래밍과는 무관하므로 이번 장은 다루지 말아야 한다.(C++)
* 그러나 다른 프로그래밍 언어(JAVA나 C#)에서는 volatile은 동시적 프로그램에 유용하다.
* C++에서도 동시성을 제공하긴 하지만 가능한 컴파일러가 있지 모두 가능하진 않다.

## std::atomic은 뮤택스처럼 원자적으로 객체를 보호한다. 다음 두 가지가 중요.

```c++
std::atomic<int> ai(0);
ai = 10;
std::cout << ai;
++ai;
--ai;
```

* std::cout << ai 문장을 볼때, 문장 전체가 원자정 행동이 아닌 ai를 읽는 시점만 원자적 부분이라는 것을 이해하고 있어야한다.
* ++, --처럼 증가 연산이나 감소 연산과 같은 읽기-수정-쓰기(RMW), std::atomic으로 생성된 모든 맴버 함수들은 다른 스레들에서 반드시 원자적으로 수행된다.

## volatile

* volatile의 경우 위의 예제 코드에서 다중스레드 환경일 때, 거의 아무것도 보장하지 않는다.
* 메모리에 기록자(writer), 판독자(reader)들이 동시에 접근하는 자료 경쟁(data race) 상황이 생긴다.

```c++
auto y = x;
y = x;
x = 10;
x = 20;

// 위 코드가 최적화 된다면
auto y = x;
x = 20;
```

## 최적화와 volatile

* 메모리에 불필요하게 저장하거나 기록하는 문제는 컴파일러가 이를 최적화 한다.(죽은 저장(dead writes), 남아도는 적재(redundant loads))
* volatile은 volatile이 적용된 변수가 보통의 방식으로 행동하지 않음을 컴파일러에게 알려주는 역할이다.
* "메모리에 대한 연산들에는 어떤 최적하도 수행하지 말라"라는 지시와도 같다.

## 정리

* std::atomic은 뮤텍스 보호 없이 여러 스레드가 접근하는 자료를 위한 것으로, 동시적 소프트웨어에 작성을 위한 도구이다.
* volatile은 읽기와 기록을 최적화도 제거하지 말아야 하는 메모리를 위한 것으로, 특별한 메모리를 다룰 때 필요한 도구이다.