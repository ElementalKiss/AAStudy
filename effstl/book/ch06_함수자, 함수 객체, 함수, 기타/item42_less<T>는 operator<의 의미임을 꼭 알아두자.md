# 항목 42 : less<T>는 operator<의 의미임을 꼭 알아두자

## less<T>는 opertor<를 이용하여 비교한다.
* Widget 클래스가 있다고 가정하자.
* 이 객체는 무게와 최대 속도를 가지고 있다. (weight, maximum speed)

```cpp
class Widget {
public:
    ...
    size_t weight() const;
    size_t maxSpeed() const;
    ...
};
...

// Widget 객체는 기본적으로 무게를 기준으로 정렬된다.
bool operator<(const Widget& lhs, const Widget& rhs){
    return lhs.weight() < rhs.weight();
}
```

## multiset<Widget>을 하고 싶다.
* 기본 비교 함수는 less<Widget>.
* less<Widget>은 Widget 객체 두개에 대해 operator<를 호출 동작.
* 이런 상황에서 최대 속도(maxSpeed)로 정렬하고 싶어!

* 방법 1. less<Widget>을 템플릿 한정화 시킨다.
```cpp
template<>
struct std::less<Widget>: public std::binary_function<Widget, Widget, bool> {
    bool operator()(const Widget& lhs, const Widget& rhs) const {
        return lhs.maxSpeed() < rhs.maxSpeed();
    }
};
```

## 문제?
* 1. "std 네임 스페이스는 신성한 곳이다. 미천한 프로그래머 따위가 어디 감히! (난 라이브러리 개발자다!)
* 2. 황당함 최소화의 원칙 (principle of least astonishment)에 위배된다!
    less를 호출하면 operator< 가 호출 될걸로 생각했는데? 이건 뭐 안에서 지맘대로 동작하게 되어있네?

## 그러면?
* 함수자 클래스를 만들면 돼.
```cpp
struct MaxSpeedCompare: public binary_function<Widget, Widget, bool> {
    bool operator()(const Widget& lhs, const Widget& rhs) const {
        return lhs.maxSpeed() < rhs.maxSpeed();
    }
};

// 사용할 땐?
multiset<Widget, MaxSpeedCompare> widgets;
// 비교
multiset<Widget> widgets;
```

## 결론
* less<T>는 T라는 두 객체에 대해 operator<를 호출하여 동작한다.
* less 가지고 장난치지 말자.
* 필요하다면 less라고 불리지 않는 특수한 함수자 클래스를 만들어 사용하자.

## 옆길로 새서...
* std 내의 STL 컴포넌트를 수정하는 일은 금지되어 있으나,
* 약간의 땜빵(tinkering)은 허용되는 예시
```cpp
namespace std {
    template<typename T>
    struct less< boost::shared_ptr<T> > : public binary_function<boost::shared_ptr<T>, shared_ptr<T>, bool> {
        bool operator()( const boost::shared_ptr<T>& a, const boost::shared_ptr<T>& b) const {
            return less<T*>()(a.get(),b.get());
        }
    }
};
```
* 스마트 포인터의 정렬동작을 본래의 포인터의 정렬과 똑같이 하기 위한 코드일 뿐.
* less를 제대로 호출하고 있음.

# 끗!
