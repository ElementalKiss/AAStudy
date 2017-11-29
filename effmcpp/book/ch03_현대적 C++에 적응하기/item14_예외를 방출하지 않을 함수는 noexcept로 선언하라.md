# 항목 14: 예외를 방출하지 않을 함수는 noexcept로 선언하라

## 예외 명세(exception specification)
### C++98

* 프로그래머들 사이에 C++98의 예외 명세가 득보다 실이 크다고 여겨지고 있다.
* 함수가 방출(emit)하는 예외를 요약해야 했으며 함수의 구현이 변경되면 명세도 함께 변경해야 한다. 이는 클라이언트 코드가 깨질 수 있는 위험이 있다.

### C++11

* C++11 제정 과정에서 예외 방출에 대해 '하나라도 예외를 방출하는가' 여부라는 점에 초점을 두게 되었다.
* C++11의 예외 명세는 '예외를 하나라도 던지는지' '하나도 던지지 않는지'의 이분법적 정보가 배경에 깔려있다.
* C++98의 예외 명세도 유효하나, 비권장(deprecate) 기능으로 분류된다.
* 예외를 방출하지 않는 함수는 **noexcept**라는 키워드를 사용하면 된다.

## 인터페이스 설계

* 함수를 noexcept로 선언하면 해당 함수를 호출하는 코드에서 예외 안정성과 효율성을 확보할 수 있다.
* noexcept가 없다면 인터페이스 명세가 허술함을 의미한다.

## 최적화 여지, 더 나은 목적 코드

* noexcept의 또 하나의 장점은 컴파일러가 더 나은 목적 코드(object code)를 산출하는데 도움이 된다는 것이다.
```c++
// 둘 다 예외를 방출하지 않음.
int f(int x) throw();   // C++98
int f(int x) noexcept;  // C++11
```

* 어떤 예외가 발생하여 f 바깥으로 나올 때, throw를 이용하면 스택 되감기를 한다.(rewind)
* noexcept로 선언하면 스택 되감기를 **할 수도** 있고 **안 할 수도** 있다.
* 컴파일러의 최적화기(optimizer)는 예외가 함수 밖으로 전파 될 때 선택의 **유연성**을 가진다.


## move 연산

* push_back의 move로 새로운 메모리를 생성할 경우 내부에서 예외가 발생할 수 있다.
* move는 "가능하면 move하되 필요하면 copy" 한다의 전략을 사용한다.
* push_back과 같은 함수는 예외를 발생하지 않아야 C++98의 copy 연산을 C++11의 move 연산으로 대체할 수 있다.
* 컴파일러는 함수의 noecept를 보고 move 연산이 에외를 방출하지 않음을 알아보고 최적화의 여지가 생긴다.(조삼모사...?)

## swap

```c++

// 실제 함수 내부
template<class _Ty1,
	class _Ty2>
	struct pair
	{	// store a pair of values
	typedef pair<_Ty1, _Ty2> _Myt;
	typedef _Ty1 first_type;
    typedef _Ty2 second_type;
    
    ...

    	void swap(_Myt& _Right)
		_NOEXCEPT_OP(_Is_nothrow_swappable<_Ty1>::value
			&& _Is_nothrow_swappable<_Ty2>::value)
		{	// exchange contents with _Right
		if (this != _STD addressof(_Right))
			{	// different, worth swapping
			_Swap_adl(first, _Right.first);
			_Swap_adl(second, _Right.second);
			}
		}

	_Ty1 first;		// the first stored value
	_Ty2 second;	// the second stored value
    };

// 책에서 기술한 함수
template <class T1, class T2>
struct pair {
    ...
    void swap(pair& p) noexcept(noexcept(swap(first, p.first)) &&
                                noexcept(swap(second, p.second)));
    ...
};
```

* _NOEXCEPT_OP, _Is_nothrow_swappable 얘들은 결국 안에 들어가보면 noexcept와 연관되어 있다.
* 이 함수는 조건부 noexcept이다.
* 인자로 넘어오는 요소들의 swap도 noexcept여야 이 함수가 noexcept로 최적화 여지가 생긴다는 의미.
* swap 함수를 구현 할때는 이것을 고려하고 noexcept로 코드를 짜는게 바람직하다.

## 예외 중립적(exception_neutral)이지 않은 함수

* 함수는 일반적으로 예외에 중립적이다.
* 하지만 예외를 전혀 방출하지 않는 것이 자연스러운 구현인 함수들도 있다.
* 이를 noexcept로 구현하면 최적화에 큰 도움이 된다.
* 함수가 예외를 리턴하면 status code가 많이 생기고 구현이 복잡해진다. 유지보수가 어려워진다.
* 메모리 해제 함수, 소멸자들은 암묵적으로 noexcept이며 직접 선언할 필요가 없다.

## wide contract와 narrow contract

* wide contract는 전제 조건이 없이 프로그램의 상태에 무관하게 호출 할 수 있는 함수.
* 호출자가 전달하는 인수들에 그 어떤 제약도 가하지 않는다.
* narrow contract는 그 이외.
* narrow contract로 설계한 noexcept 함수는 예외를 발생하지 않고 미정의 행동을 하기 때문에 클라이언트의 오류 처리가 어렵다.
* 그러므로 wide contract로 설계된 noexcept 함수를 구현하는 것이 좋다.(라이브러리 개발자들은 이러한 경향을 보인다.) 

## 결론

* noexcept는 함수의 인터페이스의 일부. 해당 함수를 호출하는 사람이 noexcept 여부에 의존할 수 있다.
* noexcept 함수는 non-noexcept 함수보다 최적화의 여지가 크다.
* noexcept는 move 연산들과 swap, 메모리 해제 함수, 소멸자들에 유용하다.
* 대부분의 함수는 noexcept가 아니라 예외에 중립적이다.
