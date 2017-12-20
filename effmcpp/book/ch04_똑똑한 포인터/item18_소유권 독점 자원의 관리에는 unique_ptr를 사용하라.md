# 항목 18: 소유권 독점 자원의 관리에는 std::unique_ptr를 사용하라.

## 4장 똑똑한 포인터
### 들어가기 앞서

- raw pointer의 단점
* 선언만 보고, 객체 하나를 가리키는지, 배열을 가리키는지 알 수 없다.
* 선언만 보고, 사용 후 파괴 해야 되는지 알 수 없다. (소유 여부)
* 파괴 해야 한다고 해도, 어떻게 파괴해야 되는지 알 수 없다. (delete vs 특정 함수)
* delete로 파괴한다고 알아도, delete인지 delete[]인지 알 수 없다.
* 위의 내용이 다 만족되었다고 해도, 프로그램 모든 경로에서 정확히 한 번 일어남을 보장하지 못 한다. (예외)
* 포인터가 가리키는 메모리 장소에 유효한 객체가 존재하는 지 확인 할 수 없다.

- 위의 단점을 보안하기 위한 smart pointer가 탄생
* 그 종류로 auto_ptr, unique_ptr, shared_ptr, weak_ptr가 있다.

* auto_ptr은 비권장, unique_ptr을 사용,

* auto_ptr : c++98의 유물,
* c++11의 이동 개념이 없음, 
* 이를 복사 연산들로 흉내 냄, 
* 그로 인한 부작용들이 나타남 (복사 시 null 설정, 컨테이너에 저장 불가능)

## 독점 소유권 개념을 가진 unique_ptr
* 기본적으로 raw pointer에 가깝게 작고 빠르다.
* unique_ptr은 자신이 가리키는 객체를 독점 소유한다.
* 따라서 이동 전용 포인터다. 복사 불가능하다.
* 이동을 하면, 소유권은 옮겨간다.
* 소멸 시, 자신이 가리키는 자원을 파괴한다. (기본 : raw pointer에 delete)

## 용도
* hierarchy 안의 객체를 생성하는 팩토리 함수에서 사용.

```cpp
/* hierarchy */
class Investment() {
public:
    // ...
    virtual ~Investment();
    // ...
}

class Stock:
    public Investment { ... };

class Bond:
    public Investment { ... };

class RealEstate:
    public Investment { ... };

/* factory function */
template<typename... Ts>
std::unique_ptr<Investment>
makeInvestment(Ts&&... params);

/* use */
{
    //...
    auto pInvestment = makeInvestment( params );
    //...
}   // *pInvestment is destoryed
```

## custom deleter
```cpp
// custom deleter (lambda)
auto delInvmt = [](Investment* pInvestment)
{
    makeLogEntry(pInvestment);
    delete pInvestment;
};

template<typename... Ts>
std::unique_ptr<Investment, decltype(delInvmt)>
makeInvestment(Ts&&... params)
// auto makeInvestment(Ts&&... params) // c++14
{
    std::unique_ptr<Investment, decltype(delInvmt)> pInv(nullptr, delInvmt);

    if ( /* Stock 객체 생성 경우 */ )
    {
        pInv.reset(new Stock(std::forward<Ts>(params)...));
    }
    else if ( /* Bond */ )
    {
        pInv.reset(new Bond(std::forward<Ts>(params)...));
    }
    else if ( /* RealEstate */ )
    {
        pInv.reset(new RealEstate(std::forward<Ts>(params)...));
    }
    return pInv;
}
```

## unique_ptr의 이점
* makeInvestment 호출 결과를 하나의 auto 변수에 저장한다 가정할 때,
* 사용자는 해당 객체가 소멸될 때 특별한 동작을 함을 알 필요가 없다.
* 프로그램의 모든 경로에서 파괴가 정확히 한 번만 일어남이 보장된다.
* 파괴 방식에 대해 신경쓸 필요가 없다.

## 코드 설명
* unique_ptr의 두번째 인자는 custom deleter 타입이며, 따라서 decltype()으로 전달해 준다.
* custom deleter는 classType* 타입의 매개변수를 받는다. 이를 통해 객체 삭제를 시도한다.
* 이는 base class 를 통해 derived class의 객체도 삭제함을 의미한다.
* 따라서 base class에는 virtual destruct가 존재하여야 한다.

## 추가 설명
* custom deleter 를 사용한 unique_ptr은 1워드에서 2워드로 증가,
* 만약 custom deleter가 함수 객체면 그 크기는 함수 객체의 크기를 더한 값,
* 따라서 custom deleter를 만들 때에는 lambda를 선호하자.

## 추가 unique_ptr 용도
* Pimpl (pointer to implementation idiom) 패턴 : Item 22에서,
* 미리 살펴보면, 포인터를 통해 구현부 클래스를 접근 방법,
* 포인터는 헤더에, 구현부 클래스는 cpp에,
* 긴 빌드시간을 줄일 수 있다.

## unique_ptr 형태들 
unique_ptr<T> : 개별을 위한 ptr, operator*, operator->를 제공,
unique_ptr<T[]> : 배열을 위한 ptr, operator[]를 제공,
unique_ptr<T[]>보단, 거의 항상 array, vector, string이 더 좋은 선택이다.

## unique_ptr은 shared_ptr로 변환이 쉽고 효율적이라는 특징도 있다,
```cpp
std::shared_ptr<Investment> sp = makeInvestment( params );
```

## 정리
* unique_ptr은 독점 소유 개념의 자원관리를 위한, 작고 빠른 이동 전용 포인터이다.
* 기본은 delete로 자원 파괴하지만, custom deleter를 이용하여 자원 파괴를 할 수 있다.
* 이 때는 unique_ptr 객체의 크기도 커진다.
* unique_ptr을 shared_ptr로 손쉽게 변환할 수 있다.




