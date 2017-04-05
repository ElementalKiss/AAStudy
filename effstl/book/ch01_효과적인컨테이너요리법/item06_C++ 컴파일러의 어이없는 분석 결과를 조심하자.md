# 항목 6 : C++ 컴파일러의 어이없는 분석 결과를 조심하자

```
list<int> data(istream_iterator<int>(dataFile),
             istream_iterator<int>());
```

* 위의 코드는 리스트 객체가 아니라 함수를 선언한 것이다!!
* 첫번째 매개 변수는 dataFile이란 이름을 가진 형식 매개 변수
* 두번째 매개 변수는 이름이 없으며 istream_ierator<int> 타입을 반환하는 함수 포인터

```
int f(double d);    //< double 타입의 매개 변수를 받는 함수
int f(double(d));   //< 마찬가지
int f(double);      //< 매개변수 이름이 없을 뿐 함수

int g(double(*pf)());   //< 함수 포인터를 매개변수로 받는 함수
int g(double pf());     //< *만 없을 뿐 위와 같음(C/C++에서 유효한 문법)
int g(double());        //< 매개변수의 이름이 없을 뿐 마찬가지
```

```
class Widget{...};
Widget w();             // 이것은 객체가 아닌 함수..
```

* C++의 컴파일 규칙에 하나도 어긋나지 않으며, 규칙에 의하면 함수 선언으로 해석될 수 있는 예가 꽤 많다.
* 이러한 ***금기 사항***을 알아내는 방법을 배우는 것이 C++ 프로그래머가 거쳐가는 진정한 의식

```
list<int> data((istream_iterator<int>(dataFile)),
             istream_iterator<int>());
```

* 첫번째 매개 변수를 괄호로 둘러 싸면 객체가 제대로 생성된다.
* 하지만 모든 컴파일러가 이렇지는 않는다.
* 필자가 틀리다고 설명한 선언 형식을 사용해야 한다.
* 코드의 이식성을 떨어뜨리며 근시안적인 방법..
* 컴파일러가 지금은 잘못 해석하겠지만 나중에는 제대로 고쳐지는 것인가..?

```
ifstream dataFile("ints.dat");
istream_iterator<int> dataBegin(dataFile);
istream_iterator<int> dataEnd;
list<int> data(dataBegin, dataEnd);
```

* 더 좋은 방법은 익명 객체 선언을 iterator에 쓰지 말고, 각 반복자 객체의 이름을 만들어 주는것
* STL 프로그래밍 스타일에 거꾸로 가는 것이지만, 이 정도는 혼란을 주는 코드 대신 지불하는 가격
