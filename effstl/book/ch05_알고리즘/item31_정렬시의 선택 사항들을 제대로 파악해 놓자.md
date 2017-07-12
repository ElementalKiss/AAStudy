# 항목 31 : 정렬시의 선택 사항들을 제대로 파악해 놓자

* 정렬을 하겠다고 생각하는 대다수의 프로그래머는 sort 라는 알고리즘을 떠올릴 것이며, sort는 훌륭한 알고리즘이다.

## 예를 들어 Widget의 벡터를 가지고 있을 때, 상위 20개의 widget만 정렬하고 나머진 상관 없는 경우
* 부분 정렬이 필요하다면 partial_sort라 불리는 알고리즘이 있다.
```
bool qualityCompare(const Widget& lhs, const Widget& rhs) {
    // lhs 가 rhs 보다 좋다면 true 반환
}
...
// 품질이 높은 20개의 요소를 widgets의 앞에 순서대로 놓는다.
partial_sort(widgets.begin(),               // first
                widgets.begin() + 20,       // mid
                widgets.end(),              // last
                qualityCompare);
...
```

## 상위 20개가 필요하지만 순서는 상관 없는 경우
* nth_element는 지정한 n번째 요소 까지는 높은 요소를 가지고 있지만 정렬된 상태는 아니다.
* 지정한 n번째에 있는 요소만 정렬된 상태
```
nth_element(widgets.begin(),
            widgets.begin() + 20,
            widgets.end(),
            qualityCompare);
```

## 이들 알고리즘은 동등한 값을 가진 요소가 여러 개일 때 '내키는 대로' 동작한다.
* 전체 정렬에 있어서는 약간의 조정을 해준 수 있다.
* sort 알고리즘 중 몇 개는 "순서 유지 정렬(stable sort)"을 제공한다.
* 두 개의 값이 동등하다면 정렬 후에도 그 둘의 상대적인 위치가 변하지 않는다.
* 정렬되기 전에 A가 B보다 앞에 있고 둘이 동등하다면, 순서 유지 정렬 알고리즘은 정렬 후에도 A가 B의 앞에 있도록 한다.
* 순서 유지성이 없는 알고리즘은 이렇게 동작하지 않는다.
* partial_sort, nth_element는 순서 유지성을 가지고 있지 않다.
* stable_sort 는 순서 유지성을 가지고 있다.

## nth_element는 용도가 꽤 다양하다.
* 주어진 범위 내의 중앙값을 찾거나 특정한 백분위 위치에 있는 값을 찾는 데에도 사용할 수 있다.
```
vector<Widget>::iterator begin(widgets.begin());
vector<Widget>::iterator end(widgets.end());

vector<Widget>::iterator goalPos;   // 원하는 위치를 가리키는 iter

//> 중앙 값을 가진 Widget 객체를 찾는다.(정렬된 벡터를 전제 조건으로)
goalPos = begin + widgets.size() / 2;
nth_element(begin, goalPos, end, qualityCompare);
```
* goalPos는 중앙값을 가지는 Widget 객체를 가리킨다.

```
//> 75번째 백분위 위치에 있는 객체를 찾는다.
vector<Widget>::size_type goalOffset = 0.25 * widget.size();
nth_element(begin, begin + goalOffset, end, qualityCompare);
```
* begin + goalOffset 은 75번째 백분위 요소

* nth_element는 상위 n개의 요소나 특정 위치의 요소를 찾아 내기만 해도 될 때 알맞다.

## 등급별로 객체를 골라내야 하는 경우
* partition은 주어진 범위에 있는 요소의 위치를 재배열하는데, 기준에 맞는 요소는 모두 그 범위의 앞에 몰아 놓는다.
* 예를 들어 2등급 이상의 객체를 모두 앞쪽에다가 옮기고 싶다고 한다면
```
bool hasAcceptableQuality(const Widget& w) {
    // Widget 객체의 등급이 2등급 이상인지 여부를 반환
}

vector<Widget>::iterator goodEnd = partition(widgets.begin(),
                                                widgets.end(),
                                                hasAcceptableQuality);
```
* 2등급 객체는 앞으로 몰아 놓고, 그 다음 객체 중 처음 것을 가리키는 반복자를 반환한다.
* widgets.begin()과 goodEnd 사이에는 1등급과 2등급 객체들이 모두 들어 있는 상태
* goodEnd와 widgets.end() 사이에는 3등급 이하의 객체들이 모두 들어있는 상태
* 반약 동등한 등급을 가진 객체들이 상대적인 위치를 유지("순서 유지")해야 한다면 stable_partition을 사용하면 된다.

## sort, sortable_sort, nth_element 알고리즘은 임의 접근 반복자와 함께 써야 하기 때문에, vector, string, deque, C++ 배열 뿐이다.
* 표준 연관 컨테이너의 경우 애초부터 비교 함수를 사용하여 요소 데이터들을 정렬해 놓기 때문에 정렬 알고리즘을 사용한다는 것 자체가 말이 안된다.
* list 는 이러한 알고리즘을 사용할 수 없지만 자체적으로 sort라는 멤버 함수를 제공한다.(list::sort는 순서 유지성을 가진다.)

## list에 partial_sort나 nth_element를 꼭 사용하고자 한다면 간적접으로 할 수 밖에 없다.
* 방법 중 하나는 list의 요소를 임의 접근 반복자를 지원하는 컨테이너에 복사한 후 원하는 알고리즘을 적용하는 방법
* 또 하나는 list::iterator의 컨테이너를 만들고 이 컨테이너에 알고리즘을 적용한 후에 각 반복자를 통해 요소에 접근하는 방법
*  세번째는 반복자를 정렬해 놓은 컨테이너의 정보를 써서, 원하는 위치에 리스트의 요소를 splice하는 것
*  이 외에도 생각하기 나름

* partition과 stable_partition은 양방향 반복자 이상이면 모두 OK라는 점에서 STL 표준 시퀀스 컨테이너 모두에 대해 동작하라 수 있다.

# 정리
* 전체 정렬을 수행할 필요가 있을 땐 sort나 stable_sort
* 상위 n개 요소만 정렬 하려면 partial_sort
* 상위 n개를 뽑되 순서는 상관 없다면 nth_element
* 표준 시퀀스 컨테이너에 요소들을 어떤 기준에 만족하는 것과 아닌 것을 구분해서 모으려면 partition, stable_partition
* list는 partition, stable_partition은 사용 가능,
  sort와 stable_sort 대신에 list::sort 사용 가능,
  partial_sort나 nth_element의 기능이 필요하다면 간적접인 방법으로..

## 수행 성능
* 작업량이 많은 알고리즘이 시간이 오래 걸리며,
* 순서 유지 알고리즘이 그렇지 않은 것보다 느리다.
* 수행 성능 순서대로 정렬해보면
1. partition
2. stable_partition
3. nth_element
4. partial_sort
5. sort
6. stable_sort

* 알고리즘을 선택하는 요인은 수행 성능이 아니라 필요한 일을 제대로 파악하는 것이다!