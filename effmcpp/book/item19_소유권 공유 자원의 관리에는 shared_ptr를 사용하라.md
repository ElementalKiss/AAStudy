# 항목 19: 소유권 공유 자원의 관리에는 std::shared_ptr를 사용하라

## shared_ptr란?
- 참조카운트 방식으로 자원을 관리하는 객체
- ref count가 0이 되면 자동으로 자원을 해제
## shared_ptr 특징
- raw pointer에 비해 두배의 크기를 가짐(그 이상도 가능)
- ref count를 담을 메모리는 반드시 동적으로 할당
- ref count의 연산은 원자적으로 이뤄져야 한다.
- move연산인경우 ref count 증가는 되지 않는다.
## unique_ptr처럼 custom deleter를 지원함
- 하지만 unique_ptr과는 다르게 custom deleter 형식의 일부로 구현되어 있지 않다.
```c++
auto customDeleter1 = [](Widget *pw) { ... };
auto customDeleter2 = [](Widget *pw) { ... };
std::shared_ptr<Widget> pw1(new Widget, customDeleter1);
std::shared_ptr<Widget> pw2(new Widget, customDeleter2);
std::vector<std::shared_ptr<Widget>> vpw{pw1, pw2}; // 가능
``` 
## 제어블럭
- ref count나 custom deleter를 저장하고 있는 자료구조
- 힙에 생성
- 후려쳐서 말하자면 std:shared_ptr내 멤버구조체 변수정도
```c++
auto pw = new Widget;
std::shared_ptr<Widget> spw1(pw);
std::shared_ptr<Widger> spw2(pw);
// 중복 delete로 crash!!!
```
- 이런식으로 할당하자
```c++
std::shared_ptr<Widget> spw1(new Widget);
std::shared_ptr<Widget> spw2(make_shared<Widget>());
```
## enable_shared_from_this
```c++
std::vector<std::shared_ptr<Widget>> processedWidgets;

class Widget{
public:
    void Process();
}

void Widget::Process()
{
    processedWidgets.emplace_back(this);
}
```
- 함수 바깥에 이미 그 Widget을 가리키는 다른 std::shared_ptr들이 있다면 문제가 발생
- std::enable_shared_form_this<T> 상속하자
```c++
class Widget: public std::enable_shared_from_this<Widget>{
public:
    void Process();
}
```
- std::enable_shared_from_this 현재 객체를 가리키는 std::shared_ptr를 생성하되 제어블록을 복제하지 않는 멤버함수를 정의함
```c++
void Widget::Process()
{
    processedWidgets.emplace_back(shared_from_this());
}
```
## 정리
- raw pointer를 shared_ptr로 생성하는 일은 피하자
    - 웬만하면 make_shard 사용
- custom deleter를 사용해도 shared_ptr의 형식에는 영향을 미치지 않는다.
- std::enable_shared_from_this를 적재적소에 잘 사용하자.
 
