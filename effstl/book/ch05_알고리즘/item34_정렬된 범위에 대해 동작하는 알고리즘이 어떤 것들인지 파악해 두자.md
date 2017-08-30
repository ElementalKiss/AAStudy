# 항목 34: 정렬된 범위에 대해 동작하는 알고리즘이 어떤 것들인지 파악해 두자

* 기본적으로 정렬된 데이터를 넘겨 동작하는 알고리즘들이 있다는 이야기.

## binary_search, lower_bound, upper_bound, equal_range

* 반드시 정렬된 데이터.
* 내부적으로 이진 탐색을 사용해 값을 찾는다.

## set_union, set_intersection, set_difference, set_symmetric_difference

* 기본적으로 선형시간으로 동작.
* 정렬하지 않은 데이터는 터무니 없이 느리다.

## merge, inplace_merge

* 두 정렬된 범위를 받아 합치는 알고리즘으로 하나라도 정렬되지 않으면 동작하지 않는다.

## includes

* 어떤 범위 안에 값이 있는지 여부를 알아보는 알고리즘. 정렬되어 있음을 가정하고 동작한다.
* 기본적으로 선형시간이지만 정렬되어 있지 않다면 느리다.

## unique, unique_copy

* 정렬되지 않아도 잘 동작하긴 한다.
* 표준안 : 연속으로 이어진 동일한 요소들 중에서 첫 번째 것만을 남기고 다 없앤다.
* unique의 동작상 중복되는 값을 지우기 위해서 remove와 비슷한 동작을 하게 된다. 그래서 미리 정렬한 데이터를 넣는 것이 효율적이다.

## 비교 함수는 그 범위를 정렬할 때 사용한 기준에 맞게 동작해야 한다.

```c++
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <string>
using namespace std;

void main()
{
    vector<int> v;
    v.push_back(5);
    v.push_back(4);
    v.push_back(3);
    v.push_back(2);
    v.push_back(1);

    auto print = [](string name, bool result1, bool result2, bool result3) {
        cout << name << " results" << endl;
        cout << "isExists5 = " << result1 << endl;
        cout << "isExists5UsingLess = " << result2 << endl;
        cout << "isExists5UsingGreater = " << result3 << endl << endl;
    };

    // _STD sort(_First, _Last, less<>());
    sort(v.begin(), v.end());
    print(
        "비교함수 공백, 미사용",
        binary_search(v.begin(), v.end(), 5),
        binary_search(v.begin(), v.end(), 5, less<int>()),
        binary_search(v.begin(), v.end(), 5, greater<int>())
        );
    
    sort(v.begin(), v.end(), less<int>());
    print(
        "less 사용",
        binary_search(v.begin(), v.end(), 5),
        binary_search(v.begin(), v.end(), 5, less<int>()),
        binary_search(v.begin(), v.end(), 5, greater<int>())
    );

    sort(v.begin(), v.end(), greater<int>());
    print(
        "greater 사용",
        binary_search(v.begin(), v.end(), 5),
        binary_search(v.begin(), v.end(), 5, less<int>()),
        binary_search(v.begin(), v.end(), 5, greater<int>())
    );

    /*
    비교함수 공백, 미사용 results
    isExists5 = 1
    isExists5UsingLess = 1
    isExists5UsingGreater = 0

    less 사용 results
    isExists5 = 1
    isExists5UsingLess = 1
    isExists5UsingGreater = 0

    greater 사용 results
    isExists5 = 0
    isExists5UsingLess = 0
    isExists5UsingGreater = 1
    */
}
```

* binary_search 수행 전에 컨테이너가 greater를 통해 sort 된 상태이다.
* 기본 요소를 오름차순으로 검색하기 때문에 비정상적인 결과가 나올 수 있다.
* binary_search 수행 시에 greater 비교함수를 명시해주면 정상동작 한다.


## 정리

* 11개의 알고리즘에 정렬된 범위를 넘겨야 효율이 좋다.
* 알고리즘에 넘길 범위를 미리 정렬하고 그 정렬 원리에 맞게 동작하는 비교 함수를 작성해야한다.(명랑 STL 사회는 너무 오글거린다...)