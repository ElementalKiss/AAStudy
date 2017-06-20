# 항목 24 : map::operator[]나 map::insert는 효율 문제에 주의하여 선택하자

# 예시(고통의 Widget Class)

```c++
class Widget {
public:
    Widget();
    Widget(double weight);
    Widget& operator=(double weight);
    ...
};
 
...
 
    std::map<int, Widget> mW;
```

* double 값을 이용하여 widget을 초기화 할 수 있다.

## map 자료 구조로 할만한 작업

* map에 값을 넣으며 초기화
* map에 들어가 있는 값을 변경하는 작업.
* 두 경우 모두 아래와 같이 표현할 수 있다.

```c++
mW[1] = 1.50f;
```

## map의 operator[] 연산자의 특징

* 추가 or 갱신(add or update) 기능을 수행하도록 설계되어 있다.

# Map에 값을 넣으며 초기화하기

```c++
mW[1] = 1.50f;
```

* 위와 같은 코드를 풀어 쓰면 이렇게 된다고 할 수 있다.

```c++
auto result = mW.insert(MyWidgetMap::value_type(2, Widget()));
result.first->second = 2.50f;
```

* 한 마디로 Widget의 기본 생성자를 호출하여 만들고 이것을 대입하는 형태가 된다.
* Widget 객체를 원하는 값을 생성자 매개 변수에 넣어 바로 만들어 내는 것이 효율적이다.

# Map에 이미 들어 있는 값을 변경할 때

```c++
mW[k] = v;
```

* 이거 씁시다. 이게 제일 좋아요.

# 책의 결론

* 추가일 경우 insert, 갱신의 경우 operator[]

# EfficientAddOrUpdate 만들기

* insert or update를 알아서 정해주는 녀석.
* 일단 만들긴 했지만 정말 좋은 것일까?

```c++
template <typename MapType, typename KeyArgType, typename ValueArgType>
typename MapType::iterator EfficientAddOrUpdate(MapType& m, const KeyArgType& k, const ValueArgType& v)
{
    typename MapType::iterator lb = m.lower_bound(k);
    if (lb != m.end() && !(m.key_comp()(k, lb->first))) {
        lb->second = v;
        return lb;
    }
    else {
        using MVT = typename MapType::value_type;
        return m.insert(lb, MVT(k, v));
    }
}
```



# 근데 뭔가 이상함

* 코드를 짰을 때 **mW[1] = 1.50f**를 Add and Init 을 해도 기본 생성자를 호출하지만 소멸자는 호출하지 않는다. VS2015 Map의 operator[] 정의를 보니.

```c++
	mapped_type& operator[](key_type&& _Keyval)
		{	// find element matching _Keyval or insert with default mapped
		return (try_emplace(_STD move(_Keyval)).first->second);
		}
```

* Move와 Emplace를 상요하고 있다.
* EfficientAddOrUpdate 함수와 비교를 해도 결국 operator[] 보다 못한 성능인 것 같은 느낌이 아주아주 많이 든다.
* 결국 operator[]가 개짱짱맨이 아닐까 생각한다. 같이 논의해봅시다.



# Code

```c++
// ConsoleApplication1.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include <iostream>
#include <map>

class Widget {
public:
    Widget():_height(.0f), _width(.0f) { std::cout << "Call Default Constructor" << std::endl; }
    ~Widget() { std::cout << "~Call Destructor" << std::endl; }
    Widget(double var): _height(var), _width(var) { std::cout << "Call Conversion Constructor" << std::endl;  }
    Widget& operator=(double var) {
        std::cout << "Call Operator=" << std::endl;
        _height = var;
        _width = var;
        return *this;
    }
    Widget(const Widget& rhs) {
        std::cout << "Call Copy Constructor" << std::endl;
        _height = rhs._height;
        _width = rhs._width;
    }

public:
    // Interface
    void PrintInfo() const {
        std::cout << "height = " << _height << " width = " << _width << std::endl;
    }

private:
    double _height;
    double _width;
};

using MyWidgetMap = std::map<int, Widget>;

template <typename MapType, typename KeyArgType, typename ValueArgType>
typename MapType::iterator EfficientAddOrUpdate(MapType& m, const KeyArgType& k, const ValueArgType& v)
{
    typename MapType::iterator lb = m.lower_bound(k);
    if (lb != m.end() && !(m.key_comp()(k, lb->first))) {
        lb->second = v;
        return lb;
    }
    else {
        using MVT = typename MapType::value_type;
        return m.insert(lb, MVT(k, v));
    }
}

int main(int argc, const char* argv[])
{
    MyWidgetMap mW;

    // 1. Add and Init using operator[]
    mW[1] = 1.50;
    /*
    Call Default Constructor
    Call Operator=
    */
    std::cout << std::endl;

    // 2. 1...?
    auto result = mW.insert(MyWidgetMap::value_type(2, Widget()));
    result.first->second = 2.50f;
    /*
    Call Default Constructor
    Call Copy Constructor
    Call Copy Constructor
    ~Call Destructor
    ~Call Destructor
    Call Operator=
    */
    std::cout << std::endl;

    // 3. Add and Init using value type 
    mW.insert(MyWidgetMap::value_type(3, 3.50f));
    /*
    Call Conversion Constructor
    Call Copy Constructor
    ~Call Destructor
    */
    std::cout << std::endl;

    // 4. Update using operator[]
    mW[1] = 11.50f;
    /*
    Call Operator=
    */
    std::cout << std::endl;

    // 5. EfficientAddOrUpdate... Why efficient?
    EfficientAddOrUpdate(mW, 4, 4.50f);
    EfficientAddOrUpdate(mW, 5, 5.50f);
    /*
    Call Conversion Constructor
    Call Copy Constructor
    ~Call Destructor
    Call Conversion Constructor
    Call Copy Constructor
    ~Call Destructor
    */
    std::cout << std::endl;

    for (auto& w : mW) {
        std::cout << "first = " << w.first << " ";
        w.second.PrintInfo();
    }
    /*
    first = 1 height = 11.5 width = 11.5
    first = 2 height = 2.5 width = 2.5
    first = 3 height = 3.5 width = 2.5
    first = 4 height = 4.5 width = 4.5
    first = 5 height = 5.5 width = 5.5
    */
    return 0;
}
```

