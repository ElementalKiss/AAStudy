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

    for (int i = 0; i < 10; i++) {
        vwp.push_back(new Widget);
    }

    // use vwp
} // memory leak
```

* vwp에서 Widget에 대한 포인터를 담아 관리한다.
* 결국 memory leak이 발생하고 이런 경우는 프로그래머가 직접 포인터에 대한 해제를 관리 해줘야 한다.

# 해결1: 간단한 해결

```
void doSomthingDelete()
{
    std::vector<Widget*> vwp;
    for (size_t i = 0; i < 10; i++) {
        vwp.push_back(new Widget);
    }
    
    // use
    
    for (auto i : vwp) {
        delete i;
    }
}
```

* 해결은 되었지만 예외 안전성에 취약하다.
* vwp에 포인터가 들어 있는데 delete가 수행되기 전에 예외가 발생하면 누수.

# 해결2: 함수객체를 이용한 방법

```
template<typename T>
struct DeleteObject : public std::unary_function<const T*, void> {
    void operator()(const T* ptr) const {
        delete ptr;
    }
};

void doSomthingDelete() {
    std::vector<Widget*> vwp;
    for (size_t i = 0; i < 10; i++) {
        vwp.push_back(new Widget);
    }

    // use
    std::for_each(vwp.begin(), vwp.end(), DeleteObject<Widget>());
}
```

* 해결은 되지만 DeleteObject가 삭제하려고 하는 객체의 타입을 직접 지정해주어야 한다. 예제처럼 Widget*를 삭제하는 것이 당연한데도 Widget을 지정해주어야한다.
* 추적하기 힘든 버그를 일으킬 수 있다. 예시: string을 상속한 클래스 선언후 DeleteObject\<string>를 하게 되면. 가상 소멸자가 없는 기본 클래스의 포인터를 가지고 파생 객체를 삭제하려고 하는 문제가 생긴다.

```
struct DeleteObject {
    template<typename T>
    void operator()(const T* ptr) const {
        delete ptr;
    }
};


void doSomthingDelete() {
    std::vector<Widget*> vwp;
    for (size_t i = 0; i < 10; i++) {
        vwp.push_back(new Widget);
    }

    // use
    std::for_each(vwp.begin(), vwp.end(), DeleteObject());
}
```

* 템플릿을 함수에 넘겨지는 매개 변수의 타입을 템플릿 매개 변수로 사용한다.
* 해당 포인터의 타입을 받는 operator()를 인스턴스화 할 수 있다.
* 이 구현은 DeleteObject의 응용도는 떨어뜨리나 위의 기본 클래스 상속에 대한 문제가 해결되며 타입 안정성도 갖추고 있다.
* for_each가 호출되기 전 예외 발생에 대한 누수는 막을 수 없다.



# 해결3: 스마트 포인터

```
void doSomthingSharedPTR()
{
    typedef std::shared_ptr<Widget> SPW;
 
    std::vector<SPW> vwp;
    for(size_t i = 0; i < 10; i++) {
        vwp.push_back(SPW(new Widget));
    }
    ...
}
```