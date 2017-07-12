# item30_알고리즘의 데이터 기록 범위(destination range)는 충분히 크게 잡자

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

## 위와 같이 호출 시 반대가 되는 순서를 원래 순서로 유지 하고 싶다면?
```
list<int> results;
transform(values.rbegin(), values.rend(), front_inserter(results), transmogrify)
```

## 임의의 위치에 삽입 하고 싶다면?
```
vector<int> results;
transform(values.begin(), values.end(), inserter(results, results.begin() + results.size() / 2), transmogrify)
```
* result의 중간에 삽임됨
* 한 번에 한 개만 객체 삽입이 이루어지므로 연속 메모리 컨테이너에 대해서는 상당한 오버헤드

## reserve를 써서 해결해보면?
```
vector<int> values;
vector<int> results;
...
results.reserve(results.size() + values.size());

transform(values.begin(), values.end(), inserter(results, results.begin() + results.size() / 2), transmogrify)
```
* reserve를 사용하였기 때문에 메모리 재할당은 일어나지는 않지만 원래 있던 요소들이 밀려나는 비용은 여전히 발생
* reserve 함수는 컨테이너의 용량(capacity)만 증가시킬 뿐, 컨테이너의 요소 크기는 변하지 않음.

## 위 코드의 효율을 높이기 위한 잘못된 방법
```
vector<int> values;
vector<int> results;
...
results.reserve(results.size() + values.size());

transform(values.begin(), values.end(), results.end(), transmogrify)
```
* 초기화 되지 않은 메모리에 무엇을 한들 뭐가 되리오

## 위 코드의 효율을 높이기 위한 좋은 방법
```
vector<int> values;
vector<int> results;
...
results.reserve(results.size() + values.size());

transform(values.begin(), values.end(), back_inserter(results), transmogrify)
```

```
vector<int> values;
vector<int> results;
...

if (results.size() < values.size()) {
    results.reserve(values.size());    
}

transform(values.begin(), values.end(), back_inserter(results), transmogrify)
```

```
results.clear();
results.reserve(values.size());

transform(values.begin(), values.end(), back_inserter(results), transmogrify)
```

## 목적지 범위를 지정해야 하는 알고리즘 사용시에는 목적지 범위의 크기를 미리 확보하거나  알고리즘이 실행 될 때 자동으로 증가하도록 만들어야 한다는 것을 잊지 말자.

## 삽입 반복자
* back_inserter
* front_inserter
* inseter
* ostream_iterator = 또 나옴! 찾아볼 것