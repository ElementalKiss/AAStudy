# 항복 31: 기본 갈무리 모드를 피하라

### c++11 기본 갈무리(이하 캡쳐) 모드는 두가지
1. capture by reference
    - dangling의 위험이 존재
2. capture by value
    - 값캡쳐도 dangling위험이 존재
    - 자기 완결적(self-contained)일것 같지만 그렇지 않은 경우도 존재
---
```c++
using FilterContainer = std::vector<std::function<bool(int)>>;

FilterContainer filters;

void addDivisorFilter()
{
    auto calc1 = computeSomeValue1();
    auto calc2 = computeSomeValue2();

    auto divisor = computeDivisor(calc1, calc2);

    filters.emplace_back([&](int value){ return value % divisor == 0;});
}
```
- addDivisorFilter함수가 반환이 되면 divisor가 존재하지 않음.
---
```c++
class Widget{
public:
    void addFilter() const;

private:
    int divisor;
};

void Widget::addFilter() const
{
    filters.emplace_back([=](int value){ return value % divisor == 0;});
}
```
- capture는 오직 람다가 생성된 범위 안에서 보이는, static이 아닌 지역 변수에만 적용.
- = 기본캡쳐모드가 이나면 컴파일이 되지 않는다. 암묵적으로 this로 쓰이기 때문
```c++
void Widget::addFilter() const
{
    auto currentObjPtr = this;
    
    filters.emplace_back([currentObjPtr](int value){ return value % currentObjPtr->divisor == 0;} );
}
```
- Widget객체의 수명에 의해 제한됨

- 그럼 어떻게 해결 하나? 멤버변수의 지역복사본을 만들어서 그 복사본을 캡쳐하면 해결
```c++
// c++11
void Widget::addFilter() const
{
    auto divisorCopy = divisor;

    filters.emplace_back([divisorCopy](int value){ return value % divisorCopy == 0;});
}

// c++14
void Widget::addFilter() const
{
    filters.emplace_back([divisor = divisor](int value){ return value % divisor == 0;});
}
```
---
- 적정 저장소의 객체는 캡쳐가 되지 않는다. 당연한 소리겠지만 람다안에서 사용할수 있다.
```c++
void addDivisorFilter()
{
    static auto calc1 = computeSomeValue1();
    static auto calc2 = computeSomeValue2();

    static auto divisor = computeDivisor(calc1, calc2);

    filters.emplace_back([=](int value){ return value % divisor == 0;});

    ++divisor;
}
```

## 정리
- capture by reference는 dangling위험이 있다.
- capture by value라도 포인터를 캡쳐하면 dangling될수 있고, 자기 완결적이지 않다.