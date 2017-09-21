# 항목 47: 쓰기 전용(write-only) 코드는 만들지 말자

## 결론

* 혼자만 읽을 수 있는 코드를 짜지 말자.
* 코드를 쓰기보다는 읽기를 더 많이 하는 것이 소프트웨어 공학의 본질이자 이치.
* STL은 훌륭한 도구지만 쓰기 전용 코드는 피하라.

## 예시

* 책 예제가 너무 개떡같아서 조금 바꿨음.
* vector\<int>가 있을 때 
* **x 보다는 작고 y보다 큰 값을 삭제**

### 한 줄로 끝낸 코드

```cpp
    // 코드1
    intVec.erase(
        std::remove_if(std::find_if(intVec.begin(), intVec.end(),
                bind2nd(greater_equal<int>(), y)),
            intVec.end(), bind2nd(less<int>(), x)),
        intVec.end()
    );
```

* 마치 함수형 언어 Lisp이나 Scheme 같은 느낌의 괄호 난무 코드가 된다.
* 마치 자신이 천재인마냥 erase-remove 합성 구문 + find를 써서 STL 프로그래밍 실력을 펼쳐도 협업하는 상황에선 최대한 삼가야 한다.

### 풀어쓴 코드

```cpp
    // 코드2
    auto rangeBegin = find_if(intVec.begin(), intVec.end(),
        bind2nd(greater_equal<int>(), y));
    intVec.erase(remove_if(rangeBegin, intVec.end(),
        bind2nd(less<int>(), x)), intVec.end());
```

* 쪼개서 이쁘게.

## 코드

```cpp
#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
using namespace std;

void main()
{
    vector<int> intVec;
    int x, y = 0;
    x = 10;
    y = 5;

    const int INT_VECTOR_SIZE = 20;
    intVec.reserve(INT_VECTOR_SIZE);
    
    for (int i = 0; i < INT_VECTOR_SIZE; i++) {
        intVec.push_back(i);
    }

    auto printVector = [](vector<int>& vec) {
        for (auto& i : vec) {
            cout << i << endl;
        }

        cout << "----------------" << endl;
    };

    // 코드1
    intVec.erase(
        std::remove_if(std::find_if(intVec.begin(), intVec.end(),
                bind2nd(greater_equal<int>(), y)),
            intVec.end(), bind2nd(less<int>(), x)),
        intVec.end()
    );

    printVector(intVec);

    // 코드2
    auto rangeBegin = find_if(intVec.begin(), intVec.end(),
        bind2nd(greater_equal<int>(), y));
    intVec.erase(remove_if(rangeBegin, intVec.end(),
        bind2nd(less<int>(), x)), intVec.end());

    printVector(intVec);

    /*
        0
        1
        2
        3
        4
        10
        11
        12
        13
        14
        15
        16
        17
        18
        19
        ----------------
        0
        1
        2
        3
        4
        10
        11
        12
        13
        14
        15
        16
        17
        18
        19
        ----------------
    */

    return;
}
```