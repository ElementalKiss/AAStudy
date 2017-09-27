# Item50 STL 관련 웹 사이트와 친구하자
- 관련 웹사이트
    - [sgi STL](https://www.sgi.com/tech/stl/)
    - [STLport](http://www.stlport.org/)
    - [Boost](http://www.boost.org/)
---
1. [sgi STL](https://www.sgi.com/tech/stl/)
    * 다양한 비표준 컴포넌트를 제공
        * 해쉬 연관 컨테이너
            * hash_map, hash_multimap, hash_set, hash_multiset...
        * 단일 연결 리스트 컨테이너
            * slink(singly linked list container)
        * 매우 긴 문자열을 담을 수 있는 string류의 컨테이너
            * rope
        * 다양한 비표준 함수자 객체와 어댑터들
            * select1st, select2nd
2. [STLport](http://www.stlport.org/)
    * sgi STL을 좀 더 다양한 플랫폼에 빌드할수 있도록 포팅한것
    * 디버그 모드 제공
3. [Boost](http://www.boost.org/)
    * 1997년 자신들이 표준으로 올리자고 주장했던 표준 라이브러리의 기능이 제거된거에 불만을 품고 나온것이 바로 [Boost](http://www.boost.org/)
    * boost 취지
        * > 라이브러리를 확장한 결과는 그 자체가 현재의 기술이 되며, 언젠가는 차후의 표준으로 제안될수 있을 것입니다. Boost.org를 통해 라이브러리를 공개하는것은 그러한 방법중의 하나입니다  

    * shared_ptr, shared_array...
    * 표준함수자의 제약들을 해결하기 위해 함수자를 재설계
    ```c++
    //참조자에 대한 참조자 문제
    class A {
        public:
        // ctr and etc ...
        A*   clone(B* container);
    };
    transform(availableObjs.begin(), availableObjs.end(), back_inserter(clonedObjs), bind1st(mem_fun(&A::clone), container));
    ```
    ```c++
    // boost 버젼
    std::transform(availableObjs.begin(), availableObjs.end(), back_inserter(clonedObjs), boost::bind<A*>(boost::mem_fn(&  A::clone), _1, container)); 
    ```
    ---