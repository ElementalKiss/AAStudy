# 항목 14: 예외를 방출하지 않을 함수는 noexcept로 선언하라

## C++98의 throw()과 C++11의 noexcept

## 인터페이스 설계

## 최적화 여지

## 예외중립성

## 결론

* noexcept는 함수의 인터페이스의 일부. 해당 함수를 호출하는 사람이 noexcept 여부에 의존할 수 있다.
* noexcept 함수는 non-noexcept 함수보다 최적화의 여지가 크다.
* noexcept는 move 연산들과 swap, 메모리 해제 함수, 소멸자들에 유용하다.
* 대부분의 함수는 noexcept가 아니라 예외에 중립적이다.