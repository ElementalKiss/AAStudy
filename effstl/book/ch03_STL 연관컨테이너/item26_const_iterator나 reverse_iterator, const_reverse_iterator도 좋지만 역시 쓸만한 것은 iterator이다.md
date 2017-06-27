# 항목 const_iterator나 reverse_iterator, const_reverse_iterator도 좋지만 역시 쓸만한 것은 iterator이다

# STL 표준 컨테이너 반복자 (iteraotr)를 제공함 
# 이는 container<T> 라고 정의 된 컨테이 너의 T* 해당
# 반복자는 총 4가지 
* iterator
* const_iterator
* revers_iterator
* const_revers_iterator

# const 계열과 non const 계열로 나누며 정방향 역방향(revers) 을 가진다

# 반복자를 증기시키면 컨테이너를 이동한다

# vector<T> 를 예로 제공되는 insert, erase를 예로 든다
iterator insert(iterator position, const T& x);
iterator erase(iterator position);
iterator insert(iterator rangeBegin, iterator rangeEnd);

# 이들의 매개 변수 타입으로 iterator를 요구함

# iterator의 상호 변환 관계

         ---> const_iterator <--- (base) 
iterator --->                           const_revers_iterator
         <--- (base) revers_iterator ---->

# 화살표에 관계는 컴파일러 추론에 의한 변환이 가능 하다는 뜻

