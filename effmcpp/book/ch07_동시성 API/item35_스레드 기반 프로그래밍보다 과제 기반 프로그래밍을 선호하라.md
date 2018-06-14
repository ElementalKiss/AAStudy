# 7장 동시성 API 서론

* C++11의 커다란 성과 중 하나는 동시성(concurrency)을 언어와 표준 라이브러리에 도입한 것.
* C++의 동시성 지원의 상당 부분은 컴파일러 작성자에 대한 제약 형태.
* 이런 제약들에 의한 언어 차원의 보장 덕분에 C++은 다중 thread 프로그램을 작성할 수 있게 됨.
* 동시성 구성 요소들(Task, 미래, thread, 뮤텍스, 조건 변수, 원자적 객체 등) 같이 점점 더 풍부해지는 도구 모음이 되고 있음.
* 이번 장에서 알아둘 점: 표준 라이브러리에는 future 객체를 위한 템플릿이 std::future와 std::shared_future 두 개가 있지만 둘 다 '미래 객체'라고 통칭하는 경우가 많다.

---
# 항목 35: Thread 기반 프로그래밍보다 Task 기반 프로그래밍을 선호하라

* doAsyncWork라는 함수를 비동기로 실행해야 되는 상황.

## thread-based 예시

* std::thread 객체를 생성해서 그 객체에서 doAsyncWork를 실행.


```
int doAsyncWork();
std::thread t(doAsyncWork);
```

## task-based 예시

* doAsyncwork를 std::async에 넘겨주는 것.

```
auto future = std::async(doAsyncWork);
```

* 이런 호출에서 std::async에 전달된 함수 객체(doAsyncWork)는 하나의 Task로 간주된다.

## thread-based와 task-based 비교

* 대체적으로 task-based가 우월하다.
* doAsyncWork는 반환값을 돌려주는데 호출하는 코드가 그 값에 관심이 있을거라 가정할 수 있다.
* thread_based에선 그 반환값에 접근할 방법이 없다.
* task-based에선 auto future로 받은 std::async 미래 객체에 예를들면 get이라는 함수로 예외를 방출했는지 확인할 수 있다.
* task-based에선 get으로 예외에 접근할 수 있지만 thread-based는 프로그램이 죽는다.

## 두 방식의 근본적인 차이

* task-based가 더 높은 수준의 추상을 구현할 수 있다.
* 프로그래머는 세부적인 thread 관리에 벗어날 수 있다.

## C++ 소프트웨어에서 'thread'라는 용어의 의미

* 하드웨어 thread: 실제 계산을 수행하는 thread를 뜻하는 하드웨어 thread. 현세대의 컴퓨터 아키텍처는 CPU 코어당 하나 이상 제공한다.
* 소프트웨어 thread
  + 운영체제가 하드웨어 thread에서 실행되는 모든 프로세서와 일정을 관리하는 데 사용하는 소프트웨어 thread.
  + OS thread 또는 System thread라고 한다. 대체로 하드웨어 thread보다 많이 생성할 수 있다.
* C++ 표준 라이브러리의 std::thread
  + C++ 프로세스 안에서 std:: 객체는 background 소프트웨어 thread에 대한 핸들로 작용.
  + std::thread가 null 핸들인 경우: 기본 생성자로 생성, std::thread 객체로 move, join을 통해 다른 thread들과 합류한 후, detach를 통해 탈착된 후.

## 소프트웨어 thread는 제한된 자원이다.

* 시스템이 제공할 수 있는 것보다 많은 소프트웨어 thread를 생성하려고 하면 std::system_error 예외가 발생한다.
* thread-based에서 std::thread t(doAsyncWork);에서 예외를 던질 수 있다. 사용 가능한 thread가 없으면 예외 발생.
* thread가 부족한 상황에서 한 가지 접근 방식은 doAsyncWork를 현재 thread에서 실행하는 것이지만 그러면 thread에 부하가 심하게 걸린다.
* 또 다른 접근은 일부 소프트웨어 thread가 완료되길 기다렸다가 std::thread를 새로 생성하는 것.
* 그리고 과다구독(oversubscription) 문제가 발생할 수 있다.
  + 과다구독이란 실행 준비가 된 소프트웨어 thread가 하드웨어 thread보다 많은 상황을 가리킨다.
  + 설명이 매우 복잡하다. 대충 context switch가 많이 발생하고 이를 thread가 처리하며 매우 힘들다는 것.
  + 과다구독을 피하긴 어렵고, 여러 방법을 써도 프로그램을 잘 만들기 어렵다는 이야기.

## 답은 task-based

* 위와 같은 문제를 std::async에게 떠넘기면 삶이 편해진다.
* 이 호출은 thread 관리의 책임을 독자에서 C++ 표준 라이브러리 구현자로 옮기는 것....(?)
* std::thread는 thread를 생성하는 것이지만 std::async는 새 소프트웨어 thread를 생성하지 않을 수도 있다.
* 대신 std::async는 지정된 함수를 doAsyncWork의 결과가 필요한 thread에서 실행하라고 스케줄러에게 요청할 수 있다.
* oversubscription, thread 부족 상황에서도 자유로운 장점을 취한다.

* std::async에서도 GUI thread 반응성이 문제가 될 수 있는데 스케줄러로서는 반응성이 좋아야 하는 thread가
  어떤 것인지알 수 없기 때문이다. 이런 경우 std::launch::async라는 시동 방침(launch policy)을 넘겨주면 된다.
  그러면 실행하고자 하는 함수가 실제로 현재 thread와는 다른 thread에서 실행된다.
* 최신의 thread 스케줄러는 시스템 전역 thread pool을 이용해 oversubscription을 피한다.
* 작업 훔치기(work-stealing) 알고리즘으로 부하를 하드웨어 코어들에게 좀 더 균형 있게 분산한다.

## thread-based를 사용하는 것이 적합한 경우

* background thread 적용 라이브러리의 API에 접근해야 하는 경우
  + C++ 동시성 API는 저수준 플랫폼 고유의 API를 이용해 구현된다.(pthreads 라이브러리나 Windows thread 라이브러리)
  + std::thread 객체는 흔히 native_handle이라는 맴버 함수를 제공한다. std::future에는 이에 해당하는 기능이 없다.
* 응용 프로그램의 thread 사용량을 최적화해야 하는, 그리고 할 수 있어야 하는 경우
  + 하드뒈어 특성들이 미리 정해진 컴퓨터에 유일하게 의미있는 프로세스로 실행될 서버 소프트웨어 개발.
* C++ 동시성 API가 제공하는 것 이상의 thread 적용 기술을 구현해야 하는 경우
  + C++ 구현이 thread pool을 제공하지 않는 특정 플랫폼을 위해 직접 구성해야 하는 경우

## 정리

* std::thread API에서는 비동기적으로 실행된 함수의 반환값을 직접 얻을 수 있으며, 만일 그런 함수가 예외를 던지면 프로그램이 종료된다.
* thread-based programming에서는 thread의 고갈, oversubscription, load-balancing, 새 플랫폼으로 적응을 독자가 직접 처리해야 한다.
* std::async와 기본 시동 방침을 이용한 task-based programming은 그런 대부분의 문제를 알아서 처리한다.
