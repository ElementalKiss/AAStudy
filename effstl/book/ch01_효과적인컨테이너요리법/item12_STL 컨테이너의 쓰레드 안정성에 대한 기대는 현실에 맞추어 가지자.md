# 항목 12 : STL 컨테이너의 쓰레드 안정성에 대한 기대는 현실에 맞추어 가지자

# STL의 다중 쓰레딩 지원
- 여러 쓰레드에서 읽는 것은 안전하다
- 여러 쓰레드에서 다른 컨테이너에 쓰는 것은 안전하다

위 두 항목에 대해서는 STL을 믿어도 좋다! 하지만 위 두 항목 이외의 상황에 대해서는 직접 concurrency를 고려하여 데이터를 보호해야 한다.

# 다중 쓰레드 코드를 작성하기 위해 당신이 알아야 할 것
- 컨테이너의 멤버 함수를 호출하는 시간 동안에 lock 걸기
- 컨테이너가 만들어 내어 주는 iterator의 유효 기간 동안에 lock 걸기
- 컨테어너에 대해 실행된 알고리즘의 수행 시간 동안에 lock 걸기

# 문제 상황
```
vector<int> v;
/* ... */
vector<int>::iterator first5( find(v.begin(), v.end(), 5) ); // 첫째줄
if (first5 != v.end()) {                                     // 둘째줄
  *first5 = 0;                                               // 셋째줄
}
```
1. 첫째줄에서 iterator를 가져와서
2. 둘째줄에서 해당 값이 있는지 확인을 한 뒤에
3. 셋째줄에서 이 값을 5로 초기화하고 있다.

1, 2, 3 단계 사이에서 다른 쓰레드가 얼마든지 값을 수정할 수 있기 때문에 이는 **Thread Unsafety...**
따라서 thread dependent 한 부분인 부분을 lock 으로 보호하자

# 문제 해결

## 첫번째 시도
```
vector<int> v;
/* ... */
getMutexFor(v);
vector<int>::iterator first5( find(v.begin(), v.end(), 5) );
if (first5 != v.end()) {
  *first5 = 0;
}
releaseMutexFor(v);
```
이제는 멀티쓰레드로부터 안전하게 되었다... 하지만 `getMutexFor`과 `releaseMutexFor`이 서로 맞지 않게 코드를 짤 가능성이 존재한다.

## 두번째 시도
```
template<typename Container>
class Lock
{
public:
  Lock(const Container& container)
    : c(continaer)
  {
    getMutexFor(c);
  }
  ~Lock()
  {
    releaseMutexFor(c);
  }
private:
  const Container& c;
}
```
[RAII-style](https://en.wikipedia.org/wiki/Resource_acquisition_is_initialization)의 class를 정의했다. 여기서 클래스의 수명은 곧 lock이 걸리는 기간이 된다.
```
vector<int> v;
/* ... */
Lock<vector<int>> lock(v);
vector<int>::iterator first5( find(v.begin(), v.end(), 5) );
if (first5 != v.end()) {
  *first5 = 0;
}
```
lock 객체가 정의된 스코프 내에서는 해당 vector에 대한 락이 유지가 된다.
