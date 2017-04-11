# 항목 9 : 데이터를 삭제할 때에도 조심스럽게 선택할 것이 많다  

## 생각해 볼 자료형  
- 연속 메모리 컨테이너: vector, deque, string, list
  - remove time complexity: O(n)
- 표준 연관 컨테이너: set, multiset, map, multimap
  - remove time complexity: O(log n)


## 컨테이너 내의 특정한 값을 모두 제거하는 경우
- 1963을 찾는 경우를 생각해보자
### 연속 메모리 컨테이너
```
c.erase( remove(c.begin(), c.end(), 1963),  // vector, deque, string
         c.end() );

c.remove(1963);                             // list
```
- list는 양방향 반복자를 지원하기 때문에 list의 멤버함수인 remove가 효과적이다.
### 표준 연관 컨테이너
```
c.erase(1964);  // set, multiset, map, multimap
```
- 연관 컨테이너는 remove 알고리즘을 사용하면 컨테이너 값을 덮어써서 컨테이너를 변형시킬 수 있음


## 컨테이너 내의 특정한 조건을 만족하는 값을 제거하는 경우
- `bool badValue(int x)`: 정수 x의 결과 참인 값만을 제거하는 경우를 생각해보자
### 연속 메모리 컨테이너
```
bool badValue(int x);

c.erase( remove_if(c.begin(), c.end(), badValue), // vector, deque, string
         c.end() );
         
c.remove_if(1963);                                // list
```
- list는 양방향 반복자를 지원하기 때문에 list의 멤버함수인 remove가 효과적이다.
### 표준 연관 컨테이너
#### Bad Case
```
AssocContainer<int> c;
for (AssocContainer<int>::iterator i = c.begin(); i != c.end(); ++i) {
  if ( badValue(*i) )
    c.erase(i);
}
```
- for문의 body에서 iterator i를 지우기 때문에 ++i를 할 수가 없다... **ERROR**
#### Good Case
```
AssocContainer<int> c;
for (AssocContainer<int>::iterator i = c.begin(); i != c.end(); /* nothing */) {
  if ( badValue(*i) )
    c.erase(i++);
  else 
    ++i;
}
```


## 제거에서 그치지 않고 다른 작업도 할 경우
### 연속 메모리 컨테이너
- erase-remove 는 다른 작업을 넣을수가 없다 따라서...
```
for (SeqContainer<int>::iterator i = c.begin(); i != c.end();) {
  if ( badValue(*i) ) {
    logFile << "Erasing " << *i << '\n';
    i = c.erase(i)
  } else {
    ++i;
  }
```
- 연속 메모리 컨테이너의 경우 `c.erase(i)`는 바로 뒤의 요소를 가리키는 반복자를 반환
### 표준 연관 컨테이너
```
AssocContainer<int> c;
for (AssocContainer<int>::iterator i = c.begin(); i != c.end(); /* nothing */) {
  if ( badValue(*i) ) {
    logFile << "Erasing " << *i << '\n';
    c.erase(i++);
  } else {
    ++i;
  }
}
```
