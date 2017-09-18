# 항목 45: count, find, binary_search, lower_bound, upper_bound, equal_range를 제대로 파악해 두자

## STL 컨테이너에서 요소를 찾는 방법은 다양하다

- 정렬되지 않은 컨테이너에 대해서 **O(n)**
  + `count`
  + `find`
- 정렬된 컨테이너에 대해서 **O(log n)**
  + `binary_search`
  + `lower_bound`
  + `upper_bound`
  + `equal_range`

## `std::count`

- 찾고자 하는 값이 있는가?
- 있다면 몇 개나 있는가?

```c++
list<Widget> lw;
Widget w;
/* ... some code! */
if (count(lw.begin(), lw.end(), w)) {
    // has 'w' in 'lw'
}
```

## `std::find`

- 원하는 값을 가진 첫 번째 객체에 대한 접근이 가능

```c++
auto it = find(lw.begin(), lw.end(), w);
if (it != end()) {
    Widget& wiget = *it;
    // found 'w'
}
```

## `std::binary_search`

- 찾고자 하는 값이 있는가?
- 컨테이너가 정렬이 되어있다면 `std::find`보다 훨씬 빠르다

```c++
vector<Widget> vw;
/* ... some code! */
sort(vw.begin(), vw.end());
Widget w;
/* ... some code! */
if ( binary_search(vw.begin(), vw.end(), w) ) {
    // has 'w' in 'vw'
}
```

## `std::lower_bound`, `std::upper_bound`

- 찾았을 경우: 원하는 값을 가진 첫 번째 객체에 대한 접근
- 못 찾았을 경우: 해당 요소가 삽입될 수 있는 위치

```c++
auto it = lower_bound(vw.begin(), vw.end(), w);
if ( i != vw.end() && ( !(*it < w) && !(w < *it) ) ) {
    // found 'w'
}
```

- 위 코드의 `!(*it < w) && !(w < *it)` 부분을 보자. `*it == w`랑 같다고 생각하면 이는 오산이다.  
  `std::binary_search`, `std::lower_bound`, `std::upper_bound`, `std::equal_range`는 equality(상등성)가  
  아닌 equivalance(동등성)를 기반으로 검사를 하기 때문에 같은 기준으로 값이 **동등**한가를 판단해야 한다.

## `std::equal_range`

```c++
vector<Widget> vw;
/* ... some code! */
sort(vw.begin(), vw.end());
/* ... some code! */
typedef vector<Widget>::iterator VWIter;
typedef pair<VWIter, VWIter> VWIterPair;
VWIterPair p = equal_range(vw.begin(), vw.end(), w);
if (p.first != p.second) {
    // found 'w'
}
```

- `equal_range`가 반환하는 `pair`의 `first`와 `second`는 각각 `lower_bound`와 `upper_bound`의  
  반환값이다.
- `lower_bound`와 `upper_bound` 모두 equivalance(동등성)를 기반으로 비교를 하기 때문에 `std::lower_bound`  
  코드에서 나왔던 `!(*it < w) && !(w < *it)` 이러한 체크를 할 필요가 없다.

## 기타

- `std::set`이나 `std::map`과 같은 컨테이너의 경우 애초에 자료구조가 정렬되어 있기 때문에 위와 같은  
  알고리즘보다는 이들 컨테이너에 최적화 된 멤버 함수를 쓰는 것이 더 낫다.

## 총정리

![alt text](https://github.com/ElementalKiss/AAStudy/blob/master/effstl/book/image/item45.jpg "search table")