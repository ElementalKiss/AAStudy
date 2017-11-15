# 항목 9: typedef보다 별칭 선언을 선호하라

* typedef는 너무 C++98스러운 유물이다.
* C++11에서도 typedef가 작동하지만, **별칭 선언(alias declaration)** 이라는 것을 제공한다.
```cpp
using UPtrMapSS = std::unique_ptr<std::unordered_map<std::string, std::string>>;
```

## typedef와 별칭 선언이 하는 일은 동일하다.
* 함수 포인터가 관여하는 형식을 다룰 때는 별칭 선언 쪽을 더 쉽게 이해하는 사람들이 많다.
```cpp
// FP는 int 하나와 const std::string& 하나를 받고
// 아무것도 돌려주지 않는 함수와 동의어(synonym)이다.
typedef void (*FP)(int, const std::string&);

// 위와 같은 의미
using FP = void (*)(int, ocnst std::string&);
```

## 별칭 선언의 강력한점은 바로 템플릿
* typedef는 템플릿화할 수 없지만 별칭 선언은 템플릿화할 수 있다.
* 템플릿화된 별칭 선언을 **별칭 템플릿(alias template)** 이라고 부른다.

* C++98에서는 템플릿화된 struct 안에 내포된 typedef들을 활용하는 편법을 동원해야 표현할 수 있었던 것들이 C++11에서는 직접적으로 표현이 가능해졌다.

* 예를 들어 MyAlloc이라는 커스텀 할당자를 사용하는 Linked List의 동의어를 정의한다고 하자.
```cpp
template<typename T>
using MyAllocList = std::list<T, MyAlloc<T>>;

MyAllocList<Widget> lw;
```

* typedef는 수고를 들여야 한다.
```cpp
template<typename T>
struct MyAllocList {
    typedef std::list<Y, MyAlloc<T>> type;
};

MyAllocList<Widget>::type lw;
```

* 템플릿 매개변수로 지정된 형식의 객체들을 담는 Linked List를 생성하려는 목적으로 템플릿 안에서 typedef를 사용하려 한다면 typede 이름 앞에 typename 을 붙여야 한다.
```cpp
template<typename T>
class Widget {
private:
    typename MyAllocList<T>::type list;
};
```
* MyAllocList<T>::type은 템플릿 형식 매개변수(T)에 의존적 형식(dependent type)이며, C++ 규칙 중 하나로 의존적 형식의 이름 앞에 반드시 typename을 붙여야 한다.

## MyAllocList를 별칭 템플릿으로 정의하면 typename을 붙일 필요가 없다.
* 또한, "::type" 접미어를 붙일 필요도 없다.
```cpp
template<typename T>
using MyAllocList = std::list<T, MyAlloc<T>>;

template<typename T>
class Widget {
private:
    MyAllocList<T> list;  
};
```

* 별칭 템플릿이 더 나은 방식임을 뒤늦게야 깨달은 표준 위원회는 C++11의 모든 형식 변환에 대한 별칭 템플릿 버전들을 C++14에 포함시켰다.
* C++11에 있는 std::변환<T>::type 형태의 각 형식 특질에 대해, C++14에는 std::변환_t 형태의 별칭 템플릿이 있다.

* C++14에는 C++11 버전들도 그대로 남아 있지만, 그것을 사용할 이유는 없다.
* C++14를 사용할 수 없는 경우 만들어 사용하자.
* C++14 표준 명세서의 전자적 저작본("N4140 pdf", "N3337" 검색)을 구한다면 복붙하면 된다.
```cpp
template <class T>
using remove_const_t = typename remove_const<T>::type;

template <class T>
using remove_reference_t = typename remove_reference<T>::type;

template <class T>
using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;
```



## 기억해 둘 사항들

* typedef는 템플릿화를 지원하지 않지만, 별칭 선언은 지원한다.
* 별칭 템플릿에서는 "::type" 접미어를 붙일 필요가 없다. 템플릿 안에서 typedef를 지칭할 때에는 "typename" 접두사를 붙여야 하는 경우가 많다.
* C++14는 C++11의 모든 형식 특질 변환에 대한 별칭 템플릿들을 제공한다.