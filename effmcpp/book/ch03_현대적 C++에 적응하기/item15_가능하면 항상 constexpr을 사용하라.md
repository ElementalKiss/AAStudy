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
* constexpr 함수는 컴파일 상수를 인수로 해서 호출된 경우 컴파일 시점 상수를 산출한다.
* 런타임 시점에 알려지는 값으로 실행하면 런타임에 값을 산출한다.
* 이렇게 말하면 constexpr 함수가 종잡을 수 없는 행동을 한다고 생각할 수 있지만 이 함수에 대한 올바른 관점은 다음과 같다.
- 컴파일 시점 삼수를 여구하는 문맥에 constexpr 함수를 사용할 수 있다.
  그런 문맥에서 constexpr 함수에 넘겨주는 인수가 컴파일 시점에서 알려진다면, 함수의 결과는 컴파일 도중 계산된다.
  하지만 인수가 컴파일 시점에 알려지지 않는다면 코드의 컴파일이 거부된다.
- 컴파일 시점에서 알려지지 않는 인수로 constexpr 함수를 호출하면 보통 함수처럼 작동한다.
  즉, 그 결과는 런타임 시점에서 계산된다.
  이는 같은 연산을 수행하는 함수를 두 버전으로 구현할 필요가 없음을 뜻한다.
  그냥 하나의 constexpr 함수를 두 가지 용도로 사용하면 된다.

### 예제

* 여러 가지 방식으로 실행할 수 있는 어떤 실험의 결과를 담는 자료구조가 필요하다.
  실험에 관련된 환경 조건이 n개이고 각 조건이 가질 수 있는 상태가 세 가지라고 하면, 가능한 조합은 3^n가지 이다.
  따라서 조건들의 모든 조합에 대한 실험 결과를 저장하려면 자료구조의 크기는 적어도 3^n개여야 한다.

  각 실험 결과가 하나의 int 값이고 n이 컴파일 도중에 알려진다고 가정할 때, 그러한 자료구조로 std::array가 적당할 것이다.
  그런데 컴파일 도중에 3^n을 계산하려면 어떻게 해야 할까?

  std::pow가 있지만 std::pow는 부동 소수점에 대해 작동하며 constexpr이 아니다.
  int를 반환하는 pow를 만들자.

```cpp
constexpr
int pow(int base, int exp) noexcept
{
    ...
}

constexpr auto numConds = 5;                    // 조건의 개수

std::array<int, pow(3, numConds)> results;
```
* pow 앞에 constexpr이 있다고 해서 pow가 반드시 const 값을 돌려주는 것은 아니다.
  constexpr은 오직 base와 exp가 컴파일 시점 상수일 때에만 pow의 결과를 컴파일 시점 상수로 사용할 수 있다는 뜻이다.

```cpp
auto base = readFromDB("base");     // 실행 시점에서
auto exp =  readFromDB("exponent"); // 값들을 구한다.

auto baseToExp = pow(base, exp);    // 실행 시점에서 pow 함수를 호출한다.
```
* 이처럼 실행 시점의 문맥에서 사용할 수도 있다.

## C++11과 C++14의 제약
* constexpr 함수는 컴파일 시점 값들로 호출했을 때 반드시 컴파일 시점 결과를 산출할 수 있어야 하므로, 구현에 일정한 제약들이 따른다. 그런데 C++11과 C++14의 제약들이 조금 다르다.

* C++11에서 constexpr 함수는 실행 가능 문장이 많아야 하나여야 하고, 보통 그 문장은 return 문일 수 밖에 없다.
  else-if 대신 삼항 연산자, 루프 대신 재귀를 사용하여 해결할 수 있다.
```cpp
constexpt int pow(int base, int exp) noexcept
{
    return (exp == 0 ? 1 : base * pow(base, exp - 1));
}
```

* C++14에서는 constexpr 함수에 대한 제약이 상당이 느슨해져서, 다음과 같은 구현이 허용된다.
```cpp
constexpr int pow(int base, int exp) noexcept
{
    auto result = 1;
    for (int i = 0; i < exp; ++i) {
        result *= base;
    }
    return result;
}
```

## constexpr 함수는 반드시 리터럴 형식들을 받고 돌려주어야 한다.
* 리터럴 형식은 컴파일 도중에 값을 결정할 수 있는 형식이다. C++11에서 void를 제외한 모든 내장 형식이 리터럴 형식에 해당한다.
  그리고 생성자와 적절한 멤버 함수들이 constexpr인 사용자 형식도 리터럴 형식이 될 수 있다.
```cpp
class Point {
public:
    constexpr Point(double xVal = 0, double yVal = 0) noexcept
    : x(xVal), y(yVal)
    {}

    constexpr double getX() const noexcept { return x; }
    constexpr double getY() const noexcept { return y; }

    void setX(double newX) noexcept { x = newX; }
    void setY(double newY) noexcept { y = newY; }

private:
    double x;
    double y;
}
```
* 여기서 Point의 생성자를 constexpr로 선언할 수 있는 이유는, 주어진 인수들이 컴파일 시점에서 알려진다면 생성된 Point 객체의 자료 멤버들의 값 역시 컴파일 시점에서 알려질 수 있기 때문이다.
* 따라서, 그런 식으로 초기화된 Point 객체는 constexpr 객체가 될 수 있다.
```cpp
constexpr Point p1(9.4, 27.7);      // OK, constexpr 생성자가 컴파일 시점에서 실행됨
constexpr Point p2(28.8, 5.3);
```

* 컴파일 시점에서 실행된 Point 객체에 대해 호출된다면 get 멤버 함수 역시 constexpr이 될 수 있다.
  그런 경우 x, y의 값을 컴파일 도중에 알 수 있기 때문이다.

* Point의 get 함수들을 호출한 결과들로 또 다른 constexpr 객체를 초기화 하는 constexpr 함수를 작성하는 것이 가능하다.
```cpp
constexpr Point midPoint(const Point& p1, const Point& p2) noexcept
{
    return { (p1.getX() + p2.getX() / 2),
             (p1.getY() + p2.getY() / 2) };
}

constexpr auto mid = midPoint(p1, p2);
```

* 초기화 과정에 생성자, get 함수, 비멤버 함수의 호출이 관여하는 객체 mid를 읽기 전용 메모리 안에 생성할 수 있다는 것이다.
* 또한, mid.getX() * 10 같은 표현식을 템플릿 인수나 열거자의 값을 지정하는 표현식에서 사용할 수 있다.
* 이는, 전통적으로 컴파일 시점에서 일어나는 일들과 실행시점에서 일어나는 일들 사이의 아주 엄격한 경계선이 이제는 흐려지기 시작했으며, 실행시점에서 수행했던 계산들의 일부를 컴파일 시점으로 옮길 수 있음을 뜻한다. (컴파일 시간은 길어지겠지만 소프트웨어는 더욱 빨라질 것이다.)


### C++11에서는 두 가지 제약 때문에 Point의 set 함수를 constexpr로 선언할 수 없다.
* 첫째로, 이 함수들은 작동 대상 객체를 수정하는데, C++11에서 constexpr 멤버 함수는 암묵적으로 const로 선언된다.
* 둘째로, 이 함수들은 반환 형식이 void인데, C++11에서 void는 리터럴 형식이 아니다.
* C++14에서는 두 제약이 사라져서 set 함수들도 constexpr이 될 수 있다.

```cpp
class Point         //< C++14
{
public:
    constexpr void setX(double newX) noexcept
    { x = newX; }
    constexpr void sety(double newY) noexcept
    { y = newY; }
}
```

* 이를 이용해서 이런 함수를 작성할 수 있다.
```cpp
// 원점을 기준으로 p와 대칭인 Point 객체를 돌려준다(C++14)
constexpr Point reflection(const Point& p) noexcept
{
    Point result;

    result.setX(-p.getX());
    result.setY(-p.getY());

    return result;
}

constexpr Point p1(9.4, 27,7);
constexpr Point p2(28.8, 5.3);
constexpr auto mid = midPoint(p1, p2);

constexpr auto reflectedMid = reflection(mid);
```

## 이 항목의 조언은
* 가능한 한 항상 constexpr을 사용하라는 것이다.
* 그 이유는 constexpr 객체와 함수 모두, 비 constexpr 객체나 함수보다 더 넓은 문맥에서 사용 가능하다는 것이다.

* constexpr이 객체나 함수의 인터페이스의 일부라는 점을 명심하자.
* constexpr을 지정한다는 것은 "이 함수(도는 객체)를 C++이 상수 표현식을 요구하는 문맥에서 사용할 수 있다."라고 밝히는 것이다.
* "가능한 한 항상" 부분에는 constexpr을 가능하게 하기 위해 적용한 제약들을 최대한 오래 유지하겠다는 의지가 포함된다는 것을 명심하자.

## 기억해 둘 사항들
* constexpr 객체는 const이며, 컴파일 도중에 알려지는 값들로 초기화된다.
* constexpr 함수는 그 값이 컴파일 도중에 알려지는 인수들로 호출하는 경우에는 컴파일 시점 결과를 산출한다.
* constexpr 객체나 함수는 비constexpr 객체나 함수보다 광범위한 문맥에서 사용할 수 있다.
* constexpr은 객체나 함수의 인터페이스의 일부이다.