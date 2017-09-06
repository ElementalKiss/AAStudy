# 항목 40 : 함수자 클래스는 어댑터 적용이 가능하게(adaptable) 만들자

## Widget 예제

* Widget 포인터를 담는 리스트가 있고 '관심있는' Widget 객체를 가리키는 포인터를 찾는 함수가 있다.

```c++
list<Widget*> widgetPtrs;
bool isInteresting(const Widget* pw);
```

* isInteresting이 true를 반환하는 첫 번째 포인터를 찾고 싶다.

```c++
auto it = find_if(widgetPtrs.begin(), widgetPtrs.end(), isInteresting);

if (it != widgetPtrs.end()) {
    // do someting
}
```

* isInteresting이 false인 것을 찾고 싶다면?

```c++
auto it = find_if(widgetPtrs.begin(), widgetPtrs.end(), not1(isInteresting));

* **not1**(isInteresting)을 사용하면 될 것 같지만 컴파일 에러가 난다.

/*
심각도	코드	설명	프로젝트	파일	줄	비표시 오류(Suppression) 상태
오류	C2207	'std::unary_negate<_Fn1>::_Functor': 클래스 템플릿의 멤버는 함수 형식을 가져올 수 없습니다.	ConsoleApplication1	c:\program files (x86)\microsoft visual studio\2017\community\vc\tools\msvc\14.10.25017\include\xfunctional	475	
오류	C2510	'_Fn1': '::' 왼쪽은 클래스/구조체/공용 구조체이어야 합니다.	ConsoleApplication1	c:\program files (x86)\microsoft visual studio\2017\community\vc\tools\msvc\14.10.25017\include\xfunctional	461	
오류	C3646	'argument_type': 알 수 없는 재정의 지정자입니다.	ConsoleApplication1	c:\program files (x86)\microsoft visual studio\2017\community\vc\tools\msvc\14.10.25017\include\xfunctional	461	
오류	C4430	형식 지정자가 없습니다. int로 가정합니다. 참고: C++에서는 기본 int를 지원하지 않습니다.	ConsoleApplication1	c:\program files (x86)\microsoft visual studio\2017\community\vc\tools\msvc\14.10.25017\include\xfunctional	461	
오류	C2825	'_Fn1': '::'가 뒤에 나오면 클래스 또는 네임스페이스여야 합니다.	ConsoleApplication1	c:\program files (x86)\microsoft visual studio\2017\community\vc\tools\msvc\14.10.25017\include\xfunctional	461	
*/
```

* 해결을 위해선 isInteresting 함수를 ptr_fun으로 감싸는 것.
* ptr_fun은 typedef 타입 몇 개를 쓸 수 있게 만들어 주는 역할.
* 일개 함수 포인터인 isInteresting이 not1이 요구하는 typedef를 가지고 있지 않음.

```c++
auto it = find_if(widgetPtrs.begin(), widgetPtrs.end(), not1(ptr_fun(isInteresting)));
```

## 함수 어댑터를들은 typedef를 요구한다.

* STL의 4대 표준 함수 어댑터(not1, not2, bind1st, bind2nd) 모두가 이런 typedef 타입이 있어야 작동.
* STL과 호환되는 SGI, boost로 마찬가지.
* 이렇게 어댑터 적용이 가능(Adaptable)하게 만든 것을 추천.

## 이런 typedef?

* argument_type
* first_argument_type
* second_argument_type
* result_type

## 어댑터를 직접 만드는 것이 아님!(이 책에선 다루지 않음.) 어떻게 사용할 수 있는가?

* 기본 클래스, 구조체에서 필요한 typedef를 상속으로 가져오면 됨.
* 한 개의 매개 변수를 받는 oeprator()를 가진 함수자라면 unary_function. 두 개라면 binary_function
* 템플릿이므로 두 함 수 다 바로 상속은 안됨.
* 대신 템플릿으로 만든 구조체에서 상속 시켜야됨.

### 예제

```c++
template<class T>
class MeetsThreshold : public unary_function<Widget, bool> {
private:
    const T _threshold;
public:
    MeetsThreshold(const T& threshold);
    bool operator()(const Widget&) const;
};

struct WidgetNameCompare : public binary_function<Widget, Widget, bool> {
    bool operator()(const Widget& lhs, const Widget& rhs) const;
};
```

* MeetsThreshold는 클래스로 구현, WidgetNameCompare는 구조체로 구현.
* 캡슐화, 스타일 등, 지 알아서.
* 특이한 점 : WidgetNameCompare 구조체의 operator()의 매개 변수 타입은 const Widget& 이지만 binary_function에 넘겨지는 비포인터(non-pointer) 타입은 const가 없음. 이유는 알면 다친다고 함.. 맞아 뒤져도 좋으니 그냥 설명 좀;;)

```c++
struct WidgetNameCompare : public binary_function<const Widget*, const Widget*, bool> {
    bool operator()(const Widget* lhs, const Widget* rhs) const;
};
```

* 포인터를 사용할때는 꼭 const를 붙여주기.

## **정리**

* 함수자 클래스에 여러 개의 호출 형태를 넣는 것은 사파.
* 어댑터 적용성을 고려하고 짜야 정파. 함수자 클래스 설계에 유의하기.

## 구현 예시

```c++
#include <iostream>
#include <list>
#include <algorithm>
#include <functional>
using namespace std;

class Widget
{
public:
    Widget();
    ~Widget();
    Widget(int num);
    int GetWidth() const { return _width; }
    int GetHeight() const { return _height; }
private:
    int _width;
    int _height;
};

Widget::Widget()
{
    _width = 0;
    _height = 0;
}

Widget::~Widget()
{
}

Widget::Widget(int num)
{
    _width = _height = num;
}

bool isInteresting(const Widget* pw)
{
    return pw->GetWidth() % 5 == 3;
}

template<class T>
class MeetsThreshold : public unary_function<const Widget*, bool> {
private:
    const T _threshold;
public:
    MeetsThreshold(const T& threshold) : _threshold(threshold) {}
    bool operator()(const Widget* w) const {
        return w->GetHeight() == _threshold;
    }
};

struct WidgetNameCompare : public binary_function<const Widget*, const Widget*, bool> {
    bool operator()(const Widget* lhs, const Widget* rhs) const { return lhs->GetWidth() == rhs->GetWidth(); }
};

int main(int argc, const char* argv)
{
    list<Widget* > widgetPtrs;
    for (size_t i = 0; i < 100; i++) {
        widgetPtrs.push_back(new Widget(i));
    }

    auto print = [](const Widget* w) { cout << "Width = " << w->GetWidth() << " Height = " << w->GetHeight() << endl; };

    auto it = find_if(widgetPtrs.begin(), widgetPtrs.end(), not1(ptr_fun(isInteresting)));
    print(*it);

    auto it2 = find_if(widgetPtrs.begin(), widgetPtrs.end(), not1(MeetsThreshold<int>(0)));
    print(*it2);

    const Widget* w = new Widget(5);
    auto it3 = find_if(widgetPtrs.begin(), widgetPtrs.end(), bind2nd(WidgetNameCompare(), w));
    print(*it3);

    delete w;
    for (auto*& widget : widgetPtrs) delete widget;
    widgetPtrs.clear();

    return 0;
}

/*
Consol
Width = 0 Height = 0
Width = 1 Height = 1
Width = 5 Height = 5
계속하려면 아무 키나 누르십시오 . . .
*/
```

## c++11에서는?

* `ptr_fun`, `mem_fun`, `mem_fun_ref` 이런 친구들이 c++11에서는 표준에서 내려가고
  곧 나올 c++17에서는 아예 삭제되어 컴파일 에러가 날 예정

* 하지만 이는 전부 `std::function`으로 대체할 수 있다
  - 인자를 받아서 무언가를 수행하는 것들을 전부 담을 수 있는 함수 자료형
  - lambda, 함수포인터, 함수객체(함수자), 멤버함수 포인터, `bind` 표현식 등...

```c++
auto it = find_if(widgetPtrs.begin(), widgetPtrs.end(), not1(ptr_fun(isInteresting)));
// 위 코드를 c++11이 좋아하는 코드로 바꾸면...
auto it = find_if(widgetPtrs.begin(), widgetPtrs.end(), not1(std::function<bool(const Widget*)>(isInteresting)));
```
