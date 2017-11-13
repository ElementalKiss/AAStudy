# 항목 9: typedef보다 별칭 선언을 선호하라

* typedef는 너무 C++98스러운 유물이다.
* C++11에서도 typedef가 작동하지만, **별칭 선언(alias declaration)** 이라는 것을 제공한다.

## 별칭 선언의 강력한점은 바로 템플릿
* typedef는 템플릿화할 수 없지만 별칭 선언은 템플릿화할 수 있다.
* 템플릿화된 별칭 선언을 별칭 템플릿(alias template)이라고 부른다.

* C++98에서는 템플릿화된 struct 안에 내포된 typedef들을 활용하는 편법을 동원해야 표현할 수 있었떤 것들이 C++11에서는 직접적으로 표현이 가능해졌다.

* 예를 들어 MyAlloc이라는 커스텀 할당자를 사용하는 Linked List의 동의어를 정의한다고 하자.
```cpp
template<typename T>                            // MyAllocList<T>는
using MyAllocList = std::list<T, MyAlloc<T>>;   // std::list<T, MyAlloc<T>>와
                                                // 동의어

MyAllocList<Widget> lw;                         // 클라이언트 코드
```

* typedef를 활용한다면..
```cpp
template<typename T>
struct MyAllocList {
    typedef std::list<Y, MyAlloc<T>> type;
};

MyAllocList<Widget>::type lw;
```

* 상황이 이보다 더 나빠질 수도 있다. 템플릿 매개변수로 지정된 형식의 객체들을 담는 Linked List를 생성하려는 목적으로 템플릿 안에서 typedef를 사용하려 한다면 typede 이름 앞에 typename 을 붙여야 한다.
```cpp
template<typename T>
class Widget {
private:
    typename MyAllocList<T>::type list;
};
```


## 기억해 둘 사항들

* typedef는 템플릿화를 지원하지 않지만, 별칭 선언은 지원한다.
* 별칭 템플릿에서는 "::type" 접미어를 붙일 필요가 없다. 템플릿 안에서 typedef를 지칭할 때에는 "typename" 접두사를 붙여야 하는 경우가 많다.
* C++14는 C++11의 모든 형식 특질 변환에 대한 별칭 템플릿들을 제공한다.