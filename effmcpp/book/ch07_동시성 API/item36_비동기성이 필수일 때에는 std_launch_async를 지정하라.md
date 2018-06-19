# 항목 36: 비동기성이 필수일 때에는 std::launch::async를 지정하라

## std::async 의 lauch policy
* std::launch::async : 비동기적, 다른 스레드에서 호출
* std::launch::deferred : 지연, 동기적, std::future 에 대해 get, wait 호출 시 대상 스레드 에서 호출
* default는 std::launch::async | std::launch::deferred 상태
 + 이는 비동기적으로 시동될수도, 동기적으로 실행될 수도 있음을 의미
 + 이런 유연성은 표준라이브러리에 책임을 전가할 수 있게 한다. (항목 35 참고)

```c++
auto fut1 = std::async(f);
auto fut2 = std::async(std::launch::async |
                        std::launch::deferred,
                        f)
// 둘은 같은 의미
```

## default의 애매함 문제?
* 어떤게 애매한가?
+ f 가 지연 실행될 수도, 선언 동시에 실행될지도,
+ f 가 어떤 쓰레드에서 동작할지?
+ f 가 반드시 실행 될 지?

## 이 애매함이 영향을 끼치는 부분
* TLS(thread-local storage) 에 어울리지 않는다.
```c++
auto fut1 = std::async(f);
```
+ 어떤 쓰레드의 지역 변수에 접근하는지 알 수 없다.
+ TLS가 독립 쓰레드? get, wait 호출 쓰레드?

* timeout wait base loop 코드
```c++
using namespace std::literals;

void f() {
  std::this_thread::sleep_for(1s);
}

auto fut = std::async(f);  // 개념적으론 비동기 실행
while (fut.wait_for(100ms) != std::future_status::ready){  // f 의 실행 끝날 때 까지
    // ... // 동시 작업 실행
}
```
* f 가 std::launch::async 로 동작할 경우
 + fut.wait_for를 통해 std::future_status::timeout, std::future_status::ready 반환
* f 가 std::launch::deferred 로 동작할 경우
 + fut.wat_for를 통해 std::future_status::deferred 반환
 + 무한 loop 발생, 
 + 이와 같은 문제는 f의 부하 여부에 따라 드러나지 않을 수 있기에, 개발 단계에서 찾아내기 쉽지 않음.
* 해당 문제 해결 방법?
 + std:::launch::deferred 인지 확인 후 get, wait로 동기적 호출한다.
 
```c++
// 상위 동일 //
auto fut = std::async(f);

if (fut.wait_for(0s) == std::future_status::deferred){  // std::launch::deferred
    fut.wait()
    // .. //
} else {    // std::launch::async
    while (fut.wait_for(100ms) != std::future_status::ready){  // f 의 실행 끝날 때 까지
        // ... // 동시 작업 실행
    }
}
```

## 그래서 default는 언제 써?
* 애매함이 문제가 되지 않을 때,
+ f가 지연 되거나 선언 동시에 실행되거나 상관 없을 때,
+ f가 어떤 쓰레드에서 동작되는지 상관없을 때,
+ f가 반드시 실행될지, futeure의 get, wait가 호출 될 지 아무 상관 없을 때,
+ TLS가 아무 상관없을 때,
+ timeout wait base loop 코드 존재 시, f가 지연된 상태임을 고려한 코드를 작성 했을 때,
* 상위의 모든 조건이 충족되었을 때만 쓰자.

## 결론 및 정리
* 개인적 결론
+ std::async 의 사용은 비동기적으로 과제를 실행하겠다는 의도가 깔려있다.
+ default 로 사용하면 상위와 같은 애매함을 다 고려해서 사용해야 한다.
+ 그러니 될 수 있으면 std::launch::async로 선언 해 써라.
* 내용 정리
+ std::async의 기본 시동 방침은 과제의 비동기적 실행과 동기적 실행을 모두 허용한다.
+ 그러나 이러한 유연성 때문에 thread_local 접근의 불확실성이 발생하고, 과제가 절대로 실행되지 않을 수도 있고, 시간 만료 기반 wait 호출에 대한 프로그램 논리에도 영향을 미친다.
+ 과제를 반드시 비동기적으로 실행해야 한다면 std::launch::async를 지정하라.

```c++
// c++98
auto fut2 = std::async(std::launch::async, f) // 이렇게 f를 비동기적으로 사용한다 명시

// c++11 & c++14
template<typename F, typename... Ts>
inline
std::future<typename std::result_of<F(Ts...)>::type> // c++14에선 auto로
reallyAsync(F&& f, Ts&&... params)
{
    return std::async(std::launch::async,
                    std::forward<F>(f),
                    std::forward<Ts>(params)...);
}

auto fut = reallyAsync(f);
```
