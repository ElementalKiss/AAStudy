# 항목 10 : 할당자(allocator)의 일반적인 사항과 제약 사항에 대해 잘 알아두자

- 할당자는 라이브러리 제작자들이 near와 far 포인터를 구분하는 일 등에 구애받지 않을 수 있도록, 메모리 모델에 대한 추상층(?)으로 개발된 것
- STL의 할당자는 메모리 관리자의 개발을 용이하게 하기 위하여 설계되었지만, 몇몇 부분에서의 효율성 저하가 판명되었다.
- 이 때문에 C++ 표준화 위원회는 효율설 타격을 피하기 위하여, 표준안에 객체로서의 할당자의 역할에 변화를 주었다?

- operator new, operator new[]와 마찬가지로 STL 할당자는 하위수준의 메모리 할당 및 해제를 맡아주지만, 할당자의 클라이언트 인터페이스는 operator new, operator new[] 와 닮은 구석이 없다.
- 대부분의 STL 표준 컨테이너는 절대로 메모리 할당자를 요구하지 않는 지경에 이름

# 왜 할당자가 나쁜가
## 포인터와 참조자에 대한 typedef로 정의된 타입
- 할당자는 allocator<T>::pointer 와 allocator<T>::reference 라는 typedef 타입을 제공, 사용자 정의 할당자도 이런 typedef 타입을 제공하도록 정해져 있다.
- C++ 에서 참조자를 흉내낼 수 있는 방법은 없다. operator.를 오버로딩해야 하는데 언어적으로 허용되지 않는다.
- 참조자처럼 동작하도록 만드는 것이 바로 프록시 객체의 예인데, 프록시 객체는 몇 가지 문제점을 가지고 있다.(항목 18 참고)
- 표준안을 보면 모든 할당자에 있어서 pointer에 대한 typedef 타입을 T*과 같게, reference에 대한 typedef 타입을 T&과 같게 가정하도록 허용하고 있다.
- typedef 타입을 무시하고 그대로의 포인터와 참조자를 '직업'사용할 수 있다.
- 새로운 포인터 타입과 참조자 타입을 제대로 제공한 할당자를 찾는다 해도 STL 구현 코드 자체에서 그냥 무시해 버릴 수 있다.

## 할당자는 객체이다
- 즉 할당자는 멤버 함수, 중첨(nested) 타입과 typedef 타입을 가질 수 있다는 뜻
- 하지만, 표준안에 의하면 같은 타입의 모든 할당자 객체는 동등하며 상등 비교를 수행한다
```
template<typename T>
class SpecialAllocator{...};
typedef SpecialAllocator<Widget> SAW;

list<Widget, SAW> L1;
list<Widget, SAW> L2;
...
L1.splice(L1.begin(), L2);
```
- splice는 복사가 일어나지 않으며, 노드가 다른 자리로 찾아갈 뿐
- 이때 L1이 없어진다고 가정하면 L1은 물론 L2에서 옮겨진 노드도 해제해야 한다.
- L1의 할당자는 원래 L2의 할당자에 의해 할당되었던 노드를 해제해야 한다.
- 어떤 할당자에 의해 할당된 메모리(L2 같은)를 다른 할당자 객체(L1)로 해제할 수 있어야 한다.

### 할당자의 타입이 동등해야 한다는 제약
- 이식이 가능한 할당자
- 다른 STL 구현 코드에서도 제대로 동작하는 할당자
- 객체는 상태(state)를 가지지 않는다.
- 이식이 가능한 객체는 비정적(non-static) 데이터 멤버를 전혀 가지지 않는다.

## 할당자는 저수준 메모리  할당을 수행한다는 점에서 operator new와 같지만, 인터페이스는 전혀 다르다.
```
void *operator new(size_t bytes);
pointer allocator<T>::allocate(size_type numObjects);
```
|   table    |  args   |           return            |
|------------|:-------:|:---------------------------:|
|operator new|바이트 수| void*                       |
|  allocator | 객체 수 | T*(pointer라는 typedef 타입)|
- allocate에서 반환되는 포인터는 T 객체를 가리키지 않는다. 만들어지지도 않았기 때문..
- 실제 T 객체를 메모리에 만드는 쪽은 allocate의 호출자(caller)로 정해져있다.
- - operator new를 만들어 썼을 때의 노하우를 커스텀 할당자에 적용하기가 복잡하다.

## 대부분의 표준 컨테이너는 자신이 생성될 때 같이 붙어온 할당자를 한 번도 호출하지 않습니다.
```
list<int> L;
set<Widget, SAW> s;
```
- 메모리 할당시 전혀 호출되지 않는다.
```

template<typename T, typename Allocator = allocator<T>>
class list {
private:
  Allocator alloc;
  struct ListNode {
    T data;
    ListNode *prev;
    ListNode *next;
  };
  ...
};
```
- list<T> 는 위와 같이 구현되어 있을 것이다.
- 리스트에 새 노드가 하나 추가되면 할당자에서 필요한 메모리를 떼어와야 한다.
- 하지만 필요한 것은 T에대한 메모리가 아닌 ListNode에 대한 메모리이다.
- Allocator는 쓸모 없다.

```
template<typename T>
class allocator{
public:
  template<typename U>
  struct rebind {
    typedef allocator<U> other;
  };
  ...
};
```

# 커스텀 할당자를 작성할 때 기억할 것

- 할당자를 템플릿으로 만든다. 템플릿 매개 변수에는 메모리를 할당하고자 하는 객체의 타입을 나타내는 T를 사용
- pointer와 reference라는 typedef 타입을 제공하되, 항상 pointer는 T*, reference는 T&이 되도록 하자.
- 할당자에는 객체별 상태를 절대 주지 않으며, 일반적으로 할당자는 비정적 데이터 멤버를 가질 수 없다.
- 할당자의 allocate 멤버 함수에는 필요한 객체의 개수를 매개 변수로 넘긴다. byte 수가 아님. 또한, 이 함수는 T* 포인터를 반환(T객체는 생성되지 않았지만) 한다는걸 잊지 말자.
- 표준 연관 컨테이너에서 필요로 하는 rebind 라는 중첩 템플릿을 꼭 제공하자.


- 할당자를 만들려면 기존의 할당자 껍데기를 규약대로 다시 만들고 거기에 멤버 함수 몇 개, 특히 allocate와 deallocate를 새로 끼워 넣어 땜질하는 일이 대부분이다.
- 괜찮은 할당자 껍데기로, 조스티스가 만든 할당자 예제 웨베 페이지나 오스턴이 쓴 "What Are Allocators Good For?" 에 있는 코드를 추천한다.