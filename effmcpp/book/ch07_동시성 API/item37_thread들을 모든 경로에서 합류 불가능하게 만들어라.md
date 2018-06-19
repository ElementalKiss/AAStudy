
# 항목 37: Thread 들을 모든 경로에서 합류 불가능하게 만들어라

* 먼저 thread의 join, joinable, detach이란?

```c++
/*
Blocks the current thread until the thread identified by *this finishes its execution.

The completion of the thread identified by *this synchronizes with the corresponding successful return from join().

No synchronization is performed on *this itself. Concurrently calling join() on the same std::thread object from multiple threads constitutes a data race that results in undefined behavior.
*/
#include <iostream>
#include <thread>
#include <chrono>

void foo()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

int main()
{
    std::thread t;
    std::cout << "before starting, joinable: " << std::boolalpha << t.joinable()
              << '\n';
 
    t = std::thread(foo);
    std::cout << "after starting, joinable: " << t.joinable() 
              << '\n';
 
    t.join();
    std::cout << "after joining, joinable: " << t.joinable() 
              << '\n';
}
// - Output -
//before starting, joinable: false
//after starting, joinable: true
//after joining, joinable: false
```

```c++
/*
Separates the thread of execution from the thread object, allowing execution to continue independently. Any allocated resources will be freed once the thread exits.

After calling detach *this no longer owns any thread.
*/
#include <iostream>
#include <chrono>
#include <thread>
 
void independentThread() 
{
    std::cout << "Starting concurrent thread.\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Exiting concurrent thread.\n";
}
 
void threadCaller() 
{
    std::cout << "Starting thread caller.\n";
    std::thread t(independentThread);
    t.detach();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Exiting thread caller.\n";
}
 
int main() 
{
    threadCaller();
    std::this_thread::sleep_for(std::chrono::seconds(5));
}
// - Output -
//Starting thread caller.
//Starting concurrent thread.
//Exiting thread caller.
//Exiting concurrent thread.
```

## joinable 할수 없는 thread
* 기본 생성된 thread 
* 다른 thread 객체로 이동된 thread
* join된 thread
* detach된 thread

## joinable한 thread의 소멸자가 호출될시 프로그램이 죽는다.

```c++
constexpr auto tenMillion = 1000000;

bool doWork(std::function<bool(int)> filter, int maxVal = tenMillion)
{
  std::vector<int> goodVals;
  std::thread t([&filter, maxVal, &goodVals]
  {
    for (auto i = 0; i <= maxVal; ++i)
    {
      if (filter(i))
      {
        goodVals.push_back(i);
      }
    }
  });
  auto nh = t.native_handle();
  // blah blah

  if (conditionAreSatisfied())
  {
    t.join();
    performComputation(goodVals);
    return true;
  }

  return false;
}
```
* 왜 joinable한 thread의 소멸자가 호출될시 프로그램이 죽을까
* 암묵적join, 암묵적 detach등 이로 인해 생기는 사이드 이펙트가 너무나 크기에 표준의원회에서 정함.
* thread를 정의한 scope내에서는 joinable이 되지 않도록 해야한다.
* return, continue, break, goto등등

## joinable을 신경쓰지 않는 ThreadRAII를 만들어보자
```c++
class ThreadRAII
{
public:
  enum class DtorAction { join, detach };

  ThreadRAII(std::thread&& t, DtorAction a)
    : action(a), t(std::move(t)){}
  ~ThreadRAII()
  {
    if (t.joinable())
    {
      if (action == DtorAction::join)
      {
        t.join();
      }
      else
      {
        t.detach();
      }
    }
  }

  std::thread& get() {return t;}
private:
  DtorAction action;
  std::thread t;
};
```
* std::thread 초기화되자마자 실행될수 있으므로 항상 제일 마지막에 선언하는것이 좋다.


## ThreadRAII를 적용한 예시 코드
```c++
constexpr auto tenMillion = 1000000;

bool doWork(std::function<bool(int)> filter, int maxVal = tenMillion)
{
  std::vector<int> goodVals;
  ThreadRAII t([&filter, maxVal, &goodVals]
  {
    for (auto i = 0; i <= maxVal; ++i)
    {
      if (filter(i))
      {
        goodVals.push_back(i);
      }
    }
  }, ThreadRAII::DtorAction::join);
  auto nh = t.get().native_handle();
  // blah blah

  if (conditionAreSatisfied())
  {
    t.get().join();
    performComputation(goodVals);
    return true;
  }

  return false;
}
```

## 정리
* 모든 경로에서 std::thread를 joinable이 되지 않도록 만들어라
* 소멸시 join방식은 디버깅하기 어려운 성능이상으로 이어질수 있다.
* 소멸시 detach방식은 디버기하기 어려운 크래쉬로 이어질수 있다.
* std::thread는 멤버변수의 제일 마지막에 선언하라.
