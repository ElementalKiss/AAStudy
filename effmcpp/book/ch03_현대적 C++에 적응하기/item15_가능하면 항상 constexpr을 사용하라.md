# 항목 15: 가능하면 항상 constexpr을 사용하라.

## 용어

* const expression

    constexpr 키워드는 특정 함수를 컴파일 시간에 평가하도록 함

## C++11에서 가장 헷갈리는 단어 constexpr
* constexpr을 객체에 적용했을 때에는 const의 강회된 버전처럼 작용하지만, 함수에 적용하면 상당히 다른 의미로 작용한다.
* 개념적으로, constexpr은 어떠한 값이 단지 상수일 뿐만 아니라 컴파일 시점에서 알려진다는 점을 나타낸다.
* constexpr의 결과가 반드시 const인 것은 아니라는 점과 그 값이 반드시 컴파일 시점에서 알려진다는 보장도 없다는 점을 이야기하고 시작한다.
* 이는 결함이 아닌 **의도된 기능** 이며, constexpr의 **장점** 들이다.

## 우선 constexpr이 객체에 적용된 경우
* constexpr이 적용된 객체는 실제로 const이며, 그 값은 실제로 컴파일 시점에서 알려진다.
* 상수이자 컴파일 시점에서 알려진 정수 갓을 C++에서 정수 상수 표현식(intergral constant expression)이 요구되는 문맥에서 사용할 수 있다.
* 배열 크기, 정수 템플릿 인수(std::array 객체의 길이 등), 열거자 값, 정합(alignment) 지정자를 지정하는 등의 여러 문맥에 사용할 수 있다.
* 변수를 constexpr로 선언하면 된다. 그러면 컴파일 시점에 상수임을 보장한다.

```cpp
    int sz;
    ...
    constexpr auto arraySize = sz;      //< ERROR!
    std::array<int, sz> data;           //< ERROR!

    constexpr auto arraySize2 = 10;     //< OK
    std::array<int, arraySize2> data2;  //< OK
```

* const가 constexpr에서와 동일한 보장을 제공하지 않음
* const 객체가 반드시 컴파일 시점에서 알려지는 값으로 초기화되지는 않기 때문
```cpp
    int sz;
    ...
    const auto arraySize = sz;          //< OK
    std::array<int, arraySize> data;    //< ERROR
```

* 모든 constexpr 객체는 const이지만 모든 const 객체가 constexpr인 것은 아니다.
* 어떤 변수의 값을 반드시 컴파일 시점 상수를 요구하는 문맥에서 사용할 수 있어야 한다면 constexpr을 써야한다.

## constexpr 함수
