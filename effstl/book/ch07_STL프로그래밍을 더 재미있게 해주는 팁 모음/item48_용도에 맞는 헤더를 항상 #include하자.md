# 항목 48: 용도에 맞는 헤더를 항상 #include 하자

## 서론

* 마이너한 경위긴 하지만 STL 프로그래밍 중에 느끼는 좌절 중 하나는 어떤 플랫폼에서 컴파일되는 소프트웨어를 다른 플ㄽ폼으로 옮기려면 #include 지시자를 몇 개 더 붙여야 한다는 점이다
* 이런 짜증스러운 일은 C++의 표준안이(C의 표준안과 달리) 표준 헤더 사이의 인클루드 관계를 명확하게 정해두지 않았기 때문에 생긴 것이다.
* 이런 융통성 때문에 라이브러리 제품들이 서로 다른 인클루드 관계를 가지게 되었다.


* 필자는 다섯 개의 STL 플랫폼(A, B, C, D, E)으로 몇 가지 실험을 해서 간접적으로 어떤 헤더가 다른 헤더를 #include 하고 있는지를 알 수 있었습니다.

## 결과

* A와 C의 경우, vector는 string을 #include 한다.
* C의 경우 algorithm은 string을 #include 한다.
* C와 D의 경우, iostream은 iterator를 #include 한다.
* D의 경우, iostream은 string과 vector를 #include 한다.
* D와 E의 경우, string은 algorithm을 #include 한다.
* 모든 경우에서 set은 functional을 #include 한다.

## 표준 STL 과나련 헤더에 무엇이 있는지 간단히 정리했다.
* 거의 모든 컨테이너들은 같은 이름의 헤더에 정의되어 있다. 이를테면 벡터는 <vector>, 리스트는 <list>.
* <set>은 set과 multiset, <map>은 map과 multimap을 선언하고 있다.
* accumulate, inner_product, adjacent_diffence, partial_sum 네개의 알고리즘은 <numeric>에 선언되어 있다.
* 위 네 개를 제외한 모든 알고리즘이 <algorithm>에 선언되어 있다.
* istream_iterator와 istreambuf_iterator를 포함한 특별한 종류의 반복자는 <iterator>에 선언되어 있습니다.
* 표준 함수자(less<T> 등)와 함수자 어댑터(not1, bind2nd 등)은 <functional>에 선언되어 있다.

## 결론
* 헤더 안에 선언된 것을 사용할 때에는 언제든지 목적에 맞는 헤더를 #include하라는 것이 필자의 당부
* 굳이 인클루드하지 않아도 되는 환경일지라도..
* 조금만 부지런하면 이후에 다른 플랫폼으로 이식할 때 시달리게 될 스트레스를 줄일 수 있다.