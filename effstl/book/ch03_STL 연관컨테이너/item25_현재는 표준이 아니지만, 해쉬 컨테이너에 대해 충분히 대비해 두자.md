# 항목 25 : 현재는 표준이 아니지만, 해위 컨테이너에 대해 충분히 대비해 두자

## 표준 C++ 라이브러리에는 해쉬 테이블이 없다.
* 표준 C++ 라이브러리의 다음 버전에는 해쉬 테이블이 들어간다고는 하지만, 현재 STL에는 없다.
* STL에 완벽하게 호환되는 해쉬 테이블 기반의 연관 컨테이너를 여루 군데에서 구할 수 있다.
* 표준 이름까지 가지고 있으며, hash_set, hash_multiset, hash_map, hash_multimap 등이 있다.
* 아직은 표준이 아니므로 제품마다 구현이 다르게 되어있다.
* 시중에서 구할 수 있는 해쉬 컨테이너 중 가장 대표적인 두 가지로 SGI 버전과 딩컴웨어(Dinkumware)의 버전이 있다.

# 해쉬 컨테이너는 연관 컨테이너이다.
* 다른 연관 컨테이너와 마찬가지로 객체와 그 객체에 대한 비교함수, 할당자의 타입을 알아야 하며, 새로운 특징으로 해쉬 함수(hashing function)을 지정해야 한다.
* 해쉬 컨테이너의 기본 형태이다.
```
template<typename T,
        typename HashFunction,
        typename CompareFunction,
        typename Allocator=allocator<T> >
class hash_container;
```

* SGI버전의 hash_set는 해쉬 함수와 비교 함수에 대한 기본 타입을 제공한다.
```
template<typename T,
        typename HashFunction = hash<T>,
        typename CompareFunction = equal_to<T>,
        typename Allocator=allocator<T> >
class hash_set;
```
* 비교 함수에 대한 기본 타입으로 equal_to를 사용하고 있다.
* SGI의 해쉬 컨테이너는 연관 컨테이너이면서도 두 객체가 같은지 알아볼 때 동등성이 아닌 상등성 검사를 한다.
* 해쉬 컨테이너에겐 정렬 순서가 크게 의미가 없기 때문이다.

* 딩컴웨어의 해쉬 컨테이너는 기본 해쉬 함수와 기본 비교 함수를 문자 특성(traits)과 비슷한 별도의 클래스인 hash_compare에 묶고, 컨테이너 템플릿의 HashingInfo 매개 변수의 기본 매개 변수로 이것을 지정한다.
(문자 특성(traits) : 특정 타입의 정보 혹은 그 타입의 연산에 대한 정보를 캡슐화한 일종의 객체로서, 구체적인 이름 대신에 타입에 구애받지 않고 쓸 수 있는 대표 정보를 모아 둔 집합으로 생각하면 무난하다.)
```
template<typename T, typename CompareFunction>
class hash_compare;
template<typename T,
        typename HashingInfo = hash_compare<T, less<T> >,
        typename Allocator=allocator<T> >
class hash_set;
```
* HasingInfo에는 해쉬 테이블의 최소 버킷 개수도 들어있으며, 버킷 개수에 대한 컨테이너 요소 개수의 최대 허용 비율이 나열 타입으로 들어있다.
* 해쉬 컨테이너는 이 비율이 넘었을 때 해쉬 테이블 내의 버킷 수를 증가시키고, 요소들을 다시 해쉬 처리한다.

* hash_compare는 대강 다음과 같은 형태이다.
```
template<typename T, typename CompareFunction = less<I> >
class hash_compare{
public:
    enum{
        bucket_size = 4,            // 버킷에 대한 요소 개수의 최대 비율
        min_buckets = 8,            // 버킷의 최소 개수
    };
    size_t operator()(const T&) const;          // 해쉬 함수
    bool operator()(const T&, const T&) const;  // 비교 함수
    ...     // 이하 생략
}
```
* 여기서 operator()를 오버로딩 한다.
* hash_compare와 비슷한 클래스를 직접 만들어 사용할 수 있도록 만들어져 있다.
* bucket_size, min_buckets, 두개의 operator() 함수, 생략된 부분만 제대로 갖추면 딩컴웨어에서 제공하는 hash 컨테이너를 완벽히 조정할 수 있다.

* SGI 는 요소를 단일 연결 리스트로 저장하고, 딩컴웨어는 이중 연결 리스트로 저장한다. 따라서 지원되는 반복자의 부류도 달라진다.
* SGI가 메모리는 조금 적게 쓴다.
* 어떤게 최적일지는 각자 판단해야 한다.

* 결론은 STL과 호환되는 해쉬 컨테이너들을 주변에서 어렵지 않게 구할 수 있다는 것