# 항목 48: 용도에 맞는 헤더를 항상 #include 하자

* STL 프로그래밍 중에 어떤 플랫폼에서 컴파일되는 소프트웨어를 다른 플랫폼으로 옮기려면 #include 지시자를 몇 개 더 붙여야 하는 경우가 발생한다.
* C++의 표준안이(C의 표준안과 달리) 표준 헤더 사이의 인클루드 관계를 명확하게 정해두지 않았기 때문에 생긴 것이다.
* 이런 융통성 때문에 라이브러리 제품들이 서로 다른 인클루드 관계를 가지게 되었다.

* 필자는 다섯 개의 STL 플랫폼(A, B, C, D, E)으로 몇 가지 실험을 해서 간접적으로 어떤 헤더가 다른 헤더를 #include 하고 있는지를 알아냈다.

## 조사 내용

1. A와 C의 경우, \<vector>는 \<string>을 #include 한다.
2. C의 경우 \<algorithm>은 \<string>을 #include 한다.
3. C와 D의 경우, \<iostream>은 \<iterator>를 #include 한다.
4. D의 경우, \<iostream>은 \<string>과 \<vector>를 #include 한다.
5. D와 E의 경우, \<string>은 \<algorithm>을 #include 한다.
6. 모든 경우에서 \<set>은 \<functional>을 #include 한다.

* 이식이 잘 안된다고 하여 컴파일러나 라이브러리를 탓하지 말자.
* 필요한 헤더를 #include 할 책임은 여러분에게 있다.

## 표준 STL 관련 헤더에 무엇이 있는지 간단히 정리했다.

* 거의 모든 컨테이너들은 같은 이름의 헤더에 정의되어 있다. 이를테면 벡터는 \<vector>, 리스트는 \<list>
* \<set>은 set과 multiset, \<map>은 map과 multimap을 선언하고 있다.
* accumulate, inner_product, adjacent_diffence, partial_sum 네 개의 알고리즘은 \<numeric>에 선언되어 있다.
* 위 네 개를 제외한 모든 알고리즘이 \<algorithm>에 선언되어 있다.
* istream_iterator와 istreambuf_iterator를 포함한 특별한 종류의 반복자는 \<iterator>에 선언되어 있다.
* 표준 함수자(less<T> 등)와 함수자 어댑터(not1, bind2nd 등)은 \<functional>에 선언되어 있다.

## 결론

* 헤더 안에 선언된 것을 사용할 때에는 언제든지 목적에 맞는 헤더를 #include 하라는 것이 필자의 당부
* 굳이 인클루드하지 않아도 되는 환경일지라도..
* 조금만 부지런하면 이후에 다른 플랫폼으로 이식할 때 시달리게 될 스트레스를 줄일 수 있다.