# 항목 18: vector\<bool> 보기를 돌같이 하자

# vector\<bool>의 두 가지 잘못되어 있는 점

## STL 컨테이너가 아니다.
* C++ 표준안 23.1절에 수록된 요구사항을 만족하지 않는다.
* 요구사항: container c가 type T의 객체를 담는다면 c에서 operator[]가 지원되어야 한다.

```
// 표준안이라면 만족해야되는 코드.
T* p = &c[0]; // operator[]가 반환해 주는 주소값으로 T*를 초기화.

...

// 안되는 코드.
std::vector<bool> boolContainer;
bool* pb = &boolContainer[0];
/*
심각도	코드	설명	프로젝트	파일	줄	비표시 오류(Suppression) 상태
오류	C2440	'초기화 중': 'std::_Vb_reference<std::_Wrap_alloc<std::allocator<_Other>>> *'에서 'bool *'(으)로 변환할 수 없습니다. 
*/

```

## 실제로 bool을 담고 있지도 않다.

* 공간을 줄이기 위해 bool을 압축(pack)시킨 데이터 표현 방식을 쓴다.
* 대개는 한 바이트를 여덟 개의 bool을 담을 수 있게 구현되어 있다.(bitfield를 써서 bool을 저장한 것 처럼 흉내)

# 진짜 bool과 가짜 bool의 차이는 무엇?

* 가짜 bool은 bool에 대한 포인터를 만들 수 없다.
* 비트 필드의 각 비트에 대한 참조자를 만들 수 없다.
* 그렇기 때문에 operator는 보통 프록시 객체(proxy object)를 반환하도록 구현되어 있다.

```
template<typename Allocator>
vector<bool, Allocator> {
public:
    class reference { ... };

	reference operator[](size_type n);
};
```

```
// auto로 타입 확인해보기
auto autoPb = &boolContainer[0];
// std::_Vb_iter_base<std::_Wrap_alloc<std::allocator<unsigned int> > >	{_Myptr=0x00000000 {???} _Myoff=0 }	std::_Vb_iter_base<std::_Wrap_alloc<std::allocator<unsigned int> > >
```

* 어쨋든 위의 사항을 만족하지 못하므로 STL 컨테이너라고 할 수 없지만 표준안에는 속해 있다. 거의(almost) 만족하기 때문에!
* 템플릿 설계 시 꺼낸 컨테이너 요소의 주소가 내부 객체 타입의 포인터여야 정상 동작하는 템플릿이라면 '거의' 만족 하는 것은 치명적이다.

# 그러면 vector\<bool>과 같은 컨테이너가 필요하다면 어떻게?

* deque\<bool>: 진짜 bool을 저장할 수 있는 STL 컨테이너.
* bitset: STL 컨테이너는 아니지만 표준 C++ 라이브러리에 속한다. STL 컨테이너와 달리 요소의 개수가 컴파일 타임에 고정, 요소 삽입과 삭제 불가능, 반복자 없음. 그러나 비트 조작 자체는 편리.

# 잡소리

* C++ 표준화 위원회에서 프록시(Proxy) 객체 기반의 컨테이너 구성의 시험대가 vector\<bool>이었다.