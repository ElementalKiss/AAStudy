# 항목 22: Pimpl 관용구를 사용할 때에는 특수 멤버 함수들을 구현 파일에서 정의하라

```cpp
class Widget {
public:
	Widget();
private:
	std::string _name;
	std::vector<double> _data;
	Gadget g1, g2, g3;
};
```
## Pimpl 관용구
* 클래스의 자료 멤버들을 구현 클래스(또는 구조체)를 가리키는 포인터로 대체 후 일차 클래스에 쓰이는 자료 멤버들을 그 구현 클래스로 옮기고 포인터를 통해서 그 자료 멤버들이 간접적으로 접근하는 기법
* 위의 예시는 #include를 이용해 여러 파일을 포함 해야 하고 관련된 헤더의 내용 변경으로 컴파일을 다시 해야하는 불행한 사태가 발생 할 수 있지만 Pimpl 관용구를 적용해서 해결할 수 있다.

### C++98
```cpp
// header
class Widget {
public:
	Widget();
	~Widget();
private:
	struct Impl; // 불완전 형식(incomplete type): 선언만하고 정의는 하지 않은 형식
	Impl *pImpl;
};

// cpp
#include "widget.h"
#include "gadget.h"
#include <string>
#include <vector>

struct Widget::Impl {
    std::string _name;
    std::vector<double> _data;
	Gadget g1, g2, g3;
};

Widget::Widget() : pImpl(new Impl)
{
}

Widget::~Widget()
{
    delete pImpl
}
```

### unique_ptr을 이용하기
* 시도1
```cpp
// header
class Widget {
public:
	Widget();
	~Widget();
private:
	struct Impl;
	std::unique_ptr<Impl> pImpl;
};

// cpp
#include "widget.h"
#include "gadget.h"
#include <string>
#include <vector>

struct Widget::Impl {
    std::string _name;
    std::vector<double> _data;
	Gadget g1, g2, g3;
};

Widget::Widget() : pImpl(std::make_unique<Impl>())
{
}

Widget::~Widget() {} // Widget::~Widget() = default;
```

** std::unique_ptr가 파괴될 때 자신이 가리키는 객체를 자동으로 삭제하므로 따로 소멸자를 만들지 않았으나 이는 컴파일 되지 않음
```
1>c:\program files (x86)\microsoft visual studio\2017\community\vc\tools\msvc\14.12.25827\include\memory(2125): error C2027: 정의되지 않은 형식 'Widget::Impl'을(를) 사용했습니다.
1>c:\users\heome\source\repos\consoleapplication1\consoleapplication1\widget.h(8): note: 'Widget::Impl' 선언을 참조하십시오.
1>c:\program files (x86)\microsoft visual studio\2017\community\vc\tools\msvc\14.12.25827\include\memory(2124): note: 클래스 템플릿 멤버 함수 'void std::default_delete<_Ty>::operator ()(_Ty *) noexcept const'을(를) 컴파일하고 있습니다.
1>        with
1>        [
1>            _Ty=Widget::Impl
1>        ]
1>c:\program files (x86)\microsoft visual studio\2017\community\vc\tools\msvc\14.12.25827\include\memory(2339): note: 컴파일 중인 함수 템플릿 인스턴스화 'void std::default_delete<_Ty>::operator ()(_Ty *) noexcept const'에 대한 참조를 확인하십시오.
1>        with
1>        [
1>            _Ty=Widget::Impl
1>        ]
1>c:\program files (x86)\microsoft visual studio\2017\community\vc\tools\msvc\14.12.25827\include\memory(2175): note: 컴파일 중인 클래스 템플릿 인스턴스화 'std::default_delete<_Ty>'에 대한 참조를 확인하세요.
1>        with
1>        [
1>            _Ty=Widget::Impl
1>        ]
1>c:\program files (x86)\microsoft visual studio\2017\community\vc\tools\msvc\14.12.25827\include\memory(2222): note: 컴파일 중인 클래스 템플릿 인스턴스화 'std::_Unique_ptr_base<_Ty,_Dx>'에 대한 참조를 확인하세요.
1>        with
1>        [
1>            _Ty=Widget::Impl,
1>            _Dx=std::default_delete<Widget::Impl>
1>        ]
1>c:\users\heome\source\repos\consoleapplication1\consoleapplication1\widget.h(9): note: 컴파일 중인 클래스 템플릿 인스턴스화 'std::unique_ptr<Widget::Impl,std::default_delete<_Ty>>'에 대한 참조를 확인하세요.
1>        with
1>        [
1>            _Ty=Widget::Impl
1>        ]
1>c:\program files (x86)\microsoft visual studio\2017\community\vc\tools\msvc\14.12.25827\include\memory(2125): error C2338: can't delete an incomplete type
1>c:\program files (x86)\microsoft visual studio\2017\community\vc\tools\msvc\14.12.25827\include\memory(2127): warning C4150: 불완전한 형식 'Widget::Impl'에 대한 포인터를 삭제했습니다. 소멸자가 호출되지 않습니다.
1>c:\users\heome\source\repos\consoleapplication1\consoleapplication1\widget.h(8): note: 'Widget::Impl' 선언을 참조하십시오.
```
	** 위와 같은 문제가 발생하는 이유는 Widget이 파괴되는 시점에 Widget의 소멸자가 없으므로 컴파일러가 생성하는 특수 멤버함수에 관한 통상적인 규칙들에 의해 컴파일러가 대신 소멸자를 작성해줌.
소멸자에서는 std::unique_ptr이 삭제 되면서 Impl 생포인터에 대해서 delete가 이루어지게 되는데 이 때 생 포인터가 불완전한 형식을 가리키지 않는지가 C++11의 static_assert를 이용하여 점검함.
	** 컴파일러는 형식의 정의를 보게 되면 완전한 형식의 정의로 간주하기 때문에 소멸자 정의를 추가해서 소멸자가 불리는 시점에 형식의 정의가 존재하는 것을 알려주면 문제 없이 해결.

* 시도2
```cpp
//header
class Widget {
public:
	Widget();
	~Widget();

	//Widget(Widget&& rhs) = default; 
	//Widget& operator=(Widget&& rhs) = default;
	//안된다고 했지만 컴파일 잘 됨.

	Widget(Widget&& rhs);
	Widget& operator=(Widget&& rhs);

	Widget(const Widget&& rhs);
	Widget& operator=(const Widget&& rhs);

private:
	struct Impl;
	std::unique_ptr<Impl> pImpl;
};

// cpp
struct Widget::Impl {
	std::string _name;
	std::vector<double> _data;
	Gadget g1, g2, g3;
};

Widget::Widget() : pImpl(std::make_unique<Impl>())
{
}

Widget::~Widget() = default;
Widget::Widget(Widget&& rhs) = default;
Widget& Widget::operator=(Widget&& rhs) = default;

Widget::Widget(const Widget&& rhs) :pImpl(nullptr)
{
	if (rhs.pImpl) {
		pImpl = std::make_unique<Impl>(*rhs.pImpl);
	}
}

Widget& Widget::operator=(const Widget&& rhs)
{
	if (!rhs.pImpl) {
		pImpl.reset();
	} else if (!pImpl) {
		pImpl = std::make_unique<Impl>(*rhs.pImpl);
	} else {
		*pImpl = *rhs.pImpl;
	}

	return *this;
}
```cpp

	** 소멸자를 선언 한 경우 이동, 복사 연산과 관련된 작업을 직접 해주어야 함.
	** 이동 생성자와 이동 할당 연산자를 헤더에 선언만 하면 소멸자와 동일한 문제가 발생하기 때문에 cpp 파일에 정의를 하는 동일한 작업 필요
	** 복사 생성자와 복사 할당 연산자의 경우 std::unique_ptr 같은 이동 전용 형식이 있는 클래스에 대해서는 컴파일러가 복사 연산들을 작성해주지 않고 작성한다고 해도 shallow copy만 진행 되기 때문에 직접 정의 해주어야 한다.

## shared_prt을 이용하기
```cpp
class Widget {
public:
	Widget();
private:
	struct Impl;
	std::shared_ptr<Impl> pImpl;
};
```
* Widget 객체 내부에서 pImpl 포인터를 독점적으로 소유하기 때문에 적합한 구현은 아니지만 작성하였을 경우 unique_ptr와 같은 문제들은 발생하지 않음.
* unique_ptr의 삭제자의 형식은 포인터 형식의 일부이지만 shared_ptr은 포인터 형식의 일부가 아니기 때문에 위와 같은 현상 발생.

## 기억해 둘 사항들
* Pimpl 관용구는 클래스 구현과 클래스 클래스 클라이언트 사이의 컴파일 의존성을 줄임으로써 빌드 시간이 감소한다.
* std::unique_ptr 형식의 pImpl 포인터를 사용 할 때에는 특수 멤버 함수들을 클래스 헤더에 선언하고 구현 파일에서 구현해야 한다. 컴파일러가 기본으로 작성하는 함수 구현들이 사용하기에 적합한 경우에도 그렇게 해야 한다.
* 위의 조언은 std::unique_ptr에 적용 될 뿐, std::shared_ptr는 적용 되지 않는다.








