# 항목 const_iterator나 reverse_iterator, const_reverse_iterator도 좋지만 역시 쓸만한 것은 iterator이다

# STL 표준 컨테이너는 네 종류의 반복자 (iterator)를 제공함 
* iterator
* const_iterator
* reverse_iterator
* const_reverse_iterator

# container<T>라는 컨테이너가 있다면 iterator는 T*와 같은 개념으로 const_iterator는 const T*와 같은 개념

# iterator와 const_iterator는 증가 시키면 다음 요소를 가리키지만  reverse_iterator와 const_reverse_iterator는 증가 시키면 이전 요소를 가리킴.

# vector<T>에서 제공 하는 insert, erase 함수들은 모두 매개 변수 타입으로 iterator를 받음.
* iterator insert(iterator position, const T& x);
* iterator erase(iterator position);
* iterator insert(iterator rangeBegin, iterator rangeEnd);


# iterator의 상호 변환 관계


         ---> const_iterator <--- (base) 
iterator --->                           const_revers_iterator
         <--- (base) revers_iterator ---->

# 화살표에 관계는 컴파일러 추론에 의한 변환이 가능 하다는 뜻

