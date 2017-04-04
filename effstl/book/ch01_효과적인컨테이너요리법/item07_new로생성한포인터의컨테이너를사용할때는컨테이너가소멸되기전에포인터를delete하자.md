# 항목 7 : new로 생성한 포인터의 컨테이너를 사용할 때에는 컨테이너가 소멸되기 전에 포인터를 delete하는 일을 잊지 말자.

# 문제점

* 포인터의 컨테이너는 자신이 소멸될 때 각 요소 자체를 없애준다.
* 그러나 포인터의 소멸자가 아무 일을 하지 않는다.
* 포인터에 대한 delete를 하지 않는다.

# 예제

```
void doSomthing()
{
    std::vector<Widget*> vwp;

    for (int i = 0; i < 10; i++)
    {
        vwp.push_back(new Widget);
    }

    // use vwp
} // memory leak

```

* vwp에서 Widget에 대한 포인터를 담아 관리한다.
* 결국 memory leak이 발생하고 이런 경우는 프로그래머가 직접 포인터에 대한 해제를 관리 해줘야 한다.

# 해결1: 간단한 해결

# 해결2: 함수객체를 이용한 방법

# 해결3: 스마트 포인터