# 항목 23: 연관 컨테이너 대신에 정렬된 vector 를 쓰는 것이 좋을 때가 있다

- 빠른 데이터 검색을 지원하는 자료구조를 생각하면 보통 '연관 컨테이너'를 떠올릴 것이다.
- 하지만 데이터 **검색시간만**을 생각한다면...?

1. 해쉬 컨테이너: O(1)
2. Vector (sorted): O(log n)
3. 연관 컨테이너: O(log n)

- 검색시간만 생각하자면 연관 컨테이너가 가장 느리다

## 연관 컨테이너 vs Vector

|  | 연관 컨테이너 | Vector (sorted) |
| --- | --- | --- |
| 구현 | balanced binary search tree | array |
| Spatial Locality | 낮음 | 높음 |
| 오버헤드 | data, pointer x 3 | data |
| 탐색시간 | O(log n) | O(log n) |
| 삽입시간 | O(log n) | O(n) |

- 연관 컨테이너는 balanced binary search tree 구조를 가지고 있다.
  따라서 트리 내의 노드끼리 서로 포인터를 통해 참조하게 되어있다. (Parent, LeftChild, RightChild)
  그렇기 때문에 연관 컨테이너는 필연적으로 vector 보다 많은 용량을 필요로 한다.

- 연관 컨테이너에서 오는 문제점은 오버헤드 뿐만 아니라 spatial locality에 대한 문제점도 있다.
  연관 컨테이너는 포인터를 통해 서로 참조하기 때문에 element끼리 서로 붙어있을 필요(?)가 없다.
  이는 컨테이너의 사이즈가 매우 커졌을 경우 한 메모리 페이지(일반적으로 4096bytes)를 벗어나게 되고,
  따라서 page fault가 일어날 가능성이 커지게 된다.

- 하지만 sorted vector도 치명적인 단점이 있다. 검색을 위해 정렬된 상태를 유지해야 하기 때문에
  삽입시간/삭제시간이 오래걸린다.

## Sorted Vector 에서 값을 찾는 방법

### binary_search
```c++
vector<Widget> vw;
/*
 * million inserts to vw
 */
sort(vw.begin(), vw.end());

Widget w; // wanna find w in vw

if ( binary_search(vw.begin(), vw.end(), w) ) {
    // there is w in vw :(
}
```

- binary_search는 vector에 대해 binary search를 수행한 뒤 결과를 bool로 반환하기 때문에
  위의 경우 w의 존재유무만 알 수 있고 iterator는 얻을 수 없다.

### lower_bound / upper_bound
```c++
vector<Widget> vw;
/*
 * million inserts to vw
 */
sort(vw.begin(), vw.end());

Widget w; // wanna find w in vw

vector<Widget>::iterator i = lower_bound(vw.begin(), vw.end(), w);
if ( (i != vw.end()) && !(*i < w) ) {
    // w found :)
}
```

- `lower_bound`에서 조심해야 할 것은 `lower_bound`는 특정한 요소를 찾아주는 것이 아니라,
  특정한 요소를 삽입했을때 정렬이 깨지지 않는 위치를 찾아주는 함수이다. 따라서 `lower_bound`는
  `vw` 안에 `w`가 없어도 유효한 위치를 반환한다.
  + (e.g.) `{1, 2, 4, 4, 5}`와 같은 배열에서
    * `lower_bound`로 `3`을 찾을 경우: 2
    * `upper_bound`로 `4`를 찾을 경우: 4
  + 따라서 위 블록코드의 `!(*i < w)`가 의미하는 바는 "vector 안에 실제로 값이 존재하는가"이다.

### equal_range
```c++
vector<Widget> vw;
/*
 * million inserts to vw
 */
sort(vw.begin(), vw.end());

Widget w; // wanna find w in vw

pair<vector<Widget>::iterator, vector<Widget>::iterator> range =
    equal_range(vw.begin(), vw.end(), w);
if (range.first != range.second) {
    // w found :)
}
```

- `equal_range`는 단순히 `lower_bound`와 `upper_bound`의 `pair`를 반환한다.
- 따라서 `range.first != range.second`는 "vector 안에 w가 있다"를 의미한다.

## Sorted Vector로 map/multimap을 대신하는 방법

- `vector`에 `pair<K, V>`를 넣는다!
  + 기존 `map`에 들어가는 페어는 `pair<const K, V>`이지만, `vector`를 정렬할 때에는
    각 요소의 값이 대입 연산을 통해 이동하기 때문에 `const`를 빼줘야 한다.

- `pair`에 기본적으로 주어지는 `operator<`는 `pair`의 두 멤버 데이터를 모두 비교하기 때문에,
  정렬에 사용할 비교 함수를 만들어야한다.

- 이때, **정렬용 비교함수**와 **탐색용 비교함수**를 따로 두어야 한다. 정렬할 때에는 두 개의
  `pair`를 매개 변수로 받지만, 탐색은 키 값만을 가지고 이루어지기 때문이다.

- 다음은 `vector`를 정렬/탐색 하는데에 필요한 *비교 함수객체*의 예시이다.

```c++
typedef pair<string, int> Data;

class DataCompare
{
public:
    /* 정렬용 비교함수 */
    bool operator () (const Data& lhs, const Data& rhs) const
    {
        return keyLess(lhs.first, rhs.first);
    }

    /* 탐색용 비교함수0 */
    bool operator () (const Data& lhs, const Data::first_type& k) const
    {
        return keyLess(lhs.first, k);
    }

    /* 탐색용 비교함수1 */
    bool operator () (const Data::first_type& k, const Data& rhs) const
    {
        return keyLess(k, rhs.first);
    }

private:
    bool keyLess(const Data::first_type& k1, const Data::first_type& k2) const
    {
        return k1 < k2;
    }
}
```

- 위의 `DataCompare` 함수객체를 사용해서 실제 `vector<Data>`를 정렬 및 탐색해보자.

```c++
vector<Data> vd;
/*
 * million inserts to vd
 */
sort(vd.begin(), vd.end(), DataCompare());

string s; // key string to find

vector<Data>::iterator i = lower_bound(vd.begin(), vd.end(), s, DataCompare());
if ( (i != vd.end()) && !(i->first < s) ) {
    // found Data by key 's'
}
```

- 데이터를 탐색하는 방법은 위에 주어졌던 세 가지 방법(1.binary_search 2.lower_bound/upper_bound
  3.equal_range)를 모두 사용할 수 있다. 하지만 예제의 복잡도가 걱정되어 `lower_bound`만 적었다.
  `vector`의 요소가 `pair`인 것을 제외하곤 모두 같으므로 궁금한 사람은 만들어보도록 합시다.

## 필자의 당부

- 항상 `sorted vector`가 `연관 컨테이너`보다 우수하다는 것이 아니다.

- 데이터의 **삽입/탐색/제거**가 무작위로 동시에 일어나는 경우에는 `연관 컨테이너`가 더 우수하다.

- 하지만, 여러분의 프로그램이 **삽입/탐색/제거** 순으로 3단계의 순서를 따라 자료구조를 조작하는 타입이라면
  진짜 `map`을 사용하여 프로그래밍하는 것보다 실행속도도 훨씬 빠르고 메모리 사용량도 엄청나게 줄어듭니다!!