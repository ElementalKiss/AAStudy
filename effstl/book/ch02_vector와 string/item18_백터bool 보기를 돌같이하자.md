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

CONT.