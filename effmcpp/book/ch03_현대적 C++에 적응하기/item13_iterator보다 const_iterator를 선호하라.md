# 항목 13: iterator보다 const_iterator를 선호하라

## 개요

- 가능한한 항상 const를 사용하라
- 이런 관행은 iterator에도 적용
- 수정할 필요가 없는 iterator에는 항상 const_iterator를 붙이자

```c++
// 예제 1
std::vector<int> values;
std::vector<int>::iterator it =
     std::find(values.begin(),values.end(), 1983);
   values.insert(it, 1998);
```
- 예제1을 const_iterator로 바꾸는것은 c++98에서는 간단하지가 않다.
```c++
// c++98
// 예제 2
using IterT = std::vector<int>::iterator;
using ConstIterT = std::vector<Int>::const_iterator;

std::vector<int> values;
ConstIterT ci = std::find(static_cast<ConstIterT>(values.begin()),
                            static_cast<ConstIterT>(values.end()),
                            1983);
values.insert(static_cast<IterT>(ci), 1998);
```
- c++98에서는 비const 컨테이너에서 const_iterator를 얻는 방법이 없다.
- c++98에서는 insert를 위치를 interator로만 받는다. const_iterator는 허용되지 않음
```c++
// c++11
// 예제 3
using IterT = std::vector<int>::iterator;
using ConstIterT = std::vector<Int>::const_iterator;

std::vector<int> values;
auto it = std::find(values.cbegin(), values.cend, 1983);
values.insert(it, 1998);
```

- c++11로 maximally generic 라이브러리 코드 작성시 문제
- begin, end, cbegin, cend등 함수는 멤버함수가 아니라 비멤버함수로도 제공을 해야함
```c++
// 예제 4
template<typename C, typename V>
void findAndInsert(C& container, const V& targetVal, const V& insertVale)
{
using std::cbegin;
using std::cend;
auto it = std::find(cbegin(container), cend(container), targetVal);
container.insert(it, insertVal);
}
```
- 하지만 c++11은 표준화 과정에 begin, end만 추가가 됨
- 그래서 cbegin, cend, rbegin, rend, crbegin, crend는 추가되지 않음
- 그럼 직접 구해보자
```c++
// 예제 5
template <class C>
auto cbegin(const C& container)->decltype(std::begin(container))
{
    return std::begin(container);
}
// http://en.cppreference.com/w/cpp/iterator/begin
```
- 일반적인 array에서도 동작
    1. std::begin은 배열의 첫번째 원소를 가리키는 포인터를 반환
    2. const 배열의 원소는 const이다.
    3. const 배열에 대해 std::begin()이 반환하는 포인터는 const를 가리키는 포인터이다.
    4. 그리하여 const 가리키는 포인터는 배열의 const_iterator이다.
# 결론
- iterator보다 const_iterator를 사용하자
- generic code를 구현할때는 non-member 함수를 사용하자