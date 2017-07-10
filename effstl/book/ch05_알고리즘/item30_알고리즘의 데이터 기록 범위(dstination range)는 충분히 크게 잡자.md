# item30_알고리즘의 데이터 기록 범위(dstination range)는 충분히 크게 잡자

## transform 함수 호출 시 세번 째 인자값의 객체가 비어있다면?
```
int transmogrify(int x);

vector<int> values;
...
vector<int> results;
transform(values.begin(), values.end(), results.end(), transmogrify)
```
* 위와 같은 코드는 대재앙을 일으킴, result는 빈 객체이기 때문.

## 위 문제를 해결 하려면?
```
vector<int> results;
transform(values.begin(), values.end(), back_inserter(results), transmogrify)
```
* back_inserter가 반환하는 반복자는 push_back을 호출하도록 되어 있음
(이 함수를 쓰려면 컨테이너는 push_back을 지원해야함, 시퀀스 컨테이너들)

## back_inserter와 반대인 front_inserter
```
list<int> results;
transform(values.begin(), values.end(), front_inserter(results), transmogrify)
```
* deque나 list에서만 사용 가능