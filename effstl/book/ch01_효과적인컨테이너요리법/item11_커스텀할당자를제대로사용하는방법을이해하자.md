# 항목 11 : 커스텀 할당자를 제대로 사용하는 방법을 이해하자

# 기본 STL 메모리 관리자의 불편한 점

* default STL 메모리 관리자인 allocator/<T>가 형편없음 : 속도 느림, 메모리 효율 낮음, 단편화(fragmentation) 심함.
* allocator/<T>는 기본적으로 thread safety 하게 구현되어 있음 : 나는 단일 스레드에서만 돌릴건데 불필요한 검사를 하고 싶지 않으면 어떻게?
* 같은 종류 객체를 특정한 힙에 모여있게 하여 메모리 참조 위치 근접성(locality of reference)를 얻기 힘들다.
* 공유메모리를 사용하기 힘들다.
* 이런 고민을 했다면 커스텀 할당자를(custom allocator) 생각해야된다.

# 예제 1. 공유 메모리 활용

## malloc과 free 함수를 본떠서 만든 루틴

```
void* mallocShared(size_t bytesNeeded);
void freeShared(void* ptr);
```

## 가정 : 이것을 STL 컨테이너에 넣어 공유 메모리 매커니즘을 쓰고 싶다.

```
template<typename T>
class SharedMemoryAllocator {
public:
    pointer allocate(size_type numObjects, const void* localityHint = 0)
	{
	    return static_cast<pointer>(mallocShared(numObjects*sizeof(T)));
	}

	void deallocate(pointer ptrToMemory, size_type numObjects)
	{
	    freeShared(ptrToMemory);
	}
}
```

## 이렇게 만든 SharedMemoryAllocator의 사용하기

```
typedef vector<double, SharedMemoryAllocator<double>> SharedDoubleVec;

{
    SharedDoubleVec v; // 내부 요소들이 공유 메모리에 있게 되는 벡터를 생성한다.
}
```

* v는 스택 기반으로 만들어지는 객체이다.
* 그래서 공유 메모리로 사용할 수 없다. (너무도 당연한 것.)

## 진짜 공유 메모리처럼 사용하려면?

```
// SharedDoubleVec 객체를 담을 수 있는 충분한 공유 메모리를 할당(allocate)한다.
void* pVectorMemory = mallocShared(sizeof(SharedDoubleVec));

// 전용 new(placement new)를 써서 SharedDoubleVec 객체를 메모리에 생성(construct)한다.
SharedDoubleVec* pv = new (pVectorMemory) SharedDoubleVec;

// 객체 사용

pv->~SharedDoubleVec(); // 공유 메모리 내에 있는 객체 소멸

freeShared(pVectorMemory); // 공유 메모리에 있는 처음 메모리 단위를 해제(deallocate)한다.

```

* 공유 메모리에 필요한 만큼을 떼어 온다.
* 이 공간에 vector를 생성한다.
* 만든 벡터를 사용 후 소멸자 호출.
* 벡터가 점유하고 있던 메모리를 해제.

# 예제 2. 특수 heap 메모리 활용

## Heap 클래스

```
class Heap1 {
public:
    static void* alloc(size_t numBytes, const void* memoryBlockToBeNear);
    static void dealloc(void* ptr);
};

class Heap2 {}; // 같은 인터페이스
```

* 메모리 할당과 해제용으로 정적 멤버 함수가 들어 있다.

## Heap1과 Heap2 등의 클래스를 사용하도록 설계 된 할당자 생성한다

```
template<typename T, typename Heap>
SpecialHeapAllocator{
public:
    pointer allocate(size_t numObjects, const void* localityHint = 0)
	{
	    return static_cast<pointer>(Heap::alloc(numObjects* size_t(T), localityHint));
	}

    void deallocate(pointer ptrToMemory, size_type numObjects)
	{
	    Heap::dealloc(ptrToMemory);
	}
}
```

## SpecialHeapAllocator 사용하기

```
// v와 s의 요소는 Heap1에 모두 모아 놓는다.
vector<int, SpecialHeapAllocator<int, Heap1>> v;
set<int, SpecialHeapAllocator<int, Heap1>> s;

// L과 m의 요소는 Heap2에 모두 모아 놓는다.
list<Widget, SpecialHeapAllocator<Widget, Heap2>> L;
map<int, string, less<int>, SpecialHeapAllocator<pair<const int, string>, Heap2>> m;
```

* 중요한 점은 Heap1과 Heap2는 객체가 아니라 type이다.
* 같은 타입의 할당자는 모두 동등해야 한다의 제약만 잘 지키면 된다.