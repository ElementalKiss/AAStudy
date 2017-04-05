# More Study

더 공부한 내용에 대해 정리해봅시다.

* 반복자 무효화
* Deque
* std::function

# 반복자 무효화

* 컨테이너의 삽입 삭제가 일어나면 메모리 재할당 및 이동에 의해 반복자가 더 이상 정확한 요소를 가리키지 못하게 되는 현상
* 일반적으로 원소의 위치가 변경 될 경우 발생.
    * 삽입 시 무효화
        * 중간에 한 요소가 삽입되면 뒤 쪽에 위치한 요소들은 삽입된 개수만큼 이동하므로 뒤쪽 요소를 가리키던 반복자는 무효화 됨
        * 삽입된 위치의 무효화 되지 않을 수도 있으나 재할당의 경우에는 메모리가 바뀌어 무효화 됨.
    * 삭제 시 무효화
        * 삭제 구간의 뒤쪽은 무효화 되지만 앞쪽은 문제 없음. (삭제시에는 메모리 재할당이 절대 일어나지 않아 앞쪽 요소는 원래 자리를 항상 유지함)
        * 리스트는 삽입, 삭제에 의해 다른 노드들이 이동 되는 것이 아니므로 반복자 무효화 현상이 훨씬 덜함. 삽입은 전혀 무효화 되지 않으며 삭제는 삭제된 반복자만 무효화 된다.

# Deque

## 요약

* Double Ended Queue의 약자. 시퀀스 컨테이너.

## 특징

* vector의 특징과 유사하다.
* 내부적으로 Stack과 Queue가 결합된 자료구조로 구현이 까다롭다.
* Front와 Back의 삽입, 삭제 성능이 좋다. 이를 제외한 랜덤 위치의 성능은 vector보다 떨어진다.

## Deque의 사용처

* 앞과 뒤에서 삽입, 삭제를 자주하는 로직인 경우. ex) 서버의 패킷 처리.
* 데이터에 대한 검색이 거의 없는 경우.

# std::function

# 기본 형태
* std::function<반환형(파라미터 자료형)> 이름

# 왜 사용 하는가?
* 함수 포인터는 함수만 호환이 가능하지만 std::function은 static 함수 포인터 말고도 functor(()연산자를 오버로딩한 객체), 멤버 함수 포인터, 람다 함수를 담을 수 있음.
* 함수 포인터는 반환값이 명시적으로 같은 타입이 아닌 것을 대입하면 컴파일 에러가 발생하지만 std::function은 함수 반환값이 암시적 형변환이 가능한 타입이면 대입 가능

# 사용법
* std::bind를 통해 특정 클래스의 멤버 함수를 함수로 만들어 사용 가능.
함수의 인자값 사용 개수 여부도 변경 가능함.

```
void Func( int iA, float fB, bool bC );

auto funcA = std::bind( Func, 10, 2.0f, true );
funcA(); 

auto funcB = std::bind( Func, 10, std::placeholders::_1, true );
funcB( 2.0f );

auto funcC = std::bind( Func, std::placeholders::_2, std::placeholders::_1, true );
funcC( 2.0f, 10 );

auto funcD = std::bind( Func, std::placeholders::_2, std::placeholders::_1, std::placeholders::_3 );
funcD( 2.0f, 10, true );

auto funcF = std::bind(&ClassA::Func, this, std::placeholders::_1, std::placeholders::_2)

```

