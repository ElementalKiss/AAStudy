# 항목 38: 함수자 클래스는 값으로 전달되도록(pass-by-value) 설계하자

## `std::for_each`를 보고 생각해보자

```c++
template <class InputIt, class UnaryFunction>
UnaryFunction for_each(InputIt first, InputIt last, UnaryFunction f);
```

- `std::for_each`는 템플릿으로 `UnaryFunction` 타입을 받음. 내부에서 이를 호출하기 때문에
  `UnaryFunction`은 함수객체나 람다와 같이 호출가능한 형식이어야 한다.

- 인자를 보면 `UnaryFunction&`이나 `UnaryFunction&&`이 아닌 `UnaryFunction`으로,
  값이 복사돼서 사용됨을 알 수 있다.

- STL에서 **pass-functors-by-value**로 인자를 받음으로서 말하고 싶은 것은 다음과 같다
  + 함수 객체를 최대한 작게 만들어라
  + 함수 객체를 단형성(monomorphic)으로 만들어라
    * 상속 받지 말라는 뜻이다. 자식 클래스를 부모 클래스 타입으로 `pass-by-value`를 하게 되면
      값이 잘리게 된다.

- *하지만* C++에서 다형성을 포기하다는건 C++에서 "++"을 빼겠다는 것과 다름없다! 또한, 필요에
  따라 큼직해진 함수 객체가 있을 수도 있다.

## "pass-functors-by-value" 규칙을 어기지 않고 다형성을 가진 함수자를 정의하는 방법

### 문제 상황

```c++
tempalte <typename T>
class BPFC : public unary_function<T, void>
{
private:
    Widget w;
    int x;
    /*
     * many large-sized elements...
     */

public:
    virtual void operator () (const T& val) const;
};
```

- BPFC: "Big Polymorphic Functor Class"

- `unary_function`은 C++11에 와서 표준에서 제외되었지만, 다른 클래스를 상속받을 수도 있으므로
  그런 상황을 가정하자.

- BPFC는 `unary_function`을 상속받고 있으므로, BPFC의 객체인 함수객체를 `std::for_each`에
  pass-by-value 방식으로 넘기면 문제가 생긴다.

### 해결 방법

```c++
template <typename T>
class BPFCImpl : public unary_function<T, void>
{
private:
    Widget w;
    int x;
    /*
     * many large-sized elements...
     */

public:
    virtual void operator () (const T& val) const;
    friend class BPFC<T>;
};

template <typename T>
class BPFC
{
private:
    BPFCImpl<T>* pImpl;

public:
    void operator () (const T& val) const
    {
        pImpl->operator()(val);
    }
};
```

- 실제로 pass-by-value 방식으로 넘겨지게 될 BPFC의 함수자는 단형성이다.

- 필요한 가상함수를 구현부 클래스인 BPFCImpl에다가 몰아넣고 BPFC 쪽에서는 이에 대한
  포인터만을 들고 있다.

- 결과적으로 크기도 작고 단형성을 유지하면서도, 많은 데이터를 액세스 할 수 있고, 다형성
  클래스처럼 동작하는 함수자 클래스를 만들 수 있다!

## 결론

- STL함수들은 함수자를 받을 때 "pass-by-value"로 받으니 이를 명심하자

- 내용이 다른 얘기가 길어졌지만 어디까지나 함수자를 STL에 넘길 때에는 다음 사항이 매우 중요하다.
  + 함수자는 작게 만들 것
  + 함수자는 다형성을 없앨 것