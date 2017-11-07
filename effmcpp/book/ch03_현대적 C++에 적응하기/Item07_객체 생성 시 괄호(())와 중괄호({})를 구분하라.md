# 항목 7: 객체 생성 시 괄호(())와 중괄호({})를 구분하라

* C++11에서 객체 생성 구문이 아주 다양해짐
```c++
int x(0); // initializer is in parentheses
int y = 0; // initializer follows "="
int z{ 0 }; // initializer is in braces
int z = { 0 }; // initializer uses "=" and braces
```

* 다양해진 초기화 구문으로 = 사용시 혼동이 생김
```c++
Widget w1; // call default constructor
Widget w2 = w1; // not an assignment; calls copy ctor
w1 = w2; // an assignment; calls copy operator=
```
## Braced initialization
* 새로운 방식의 컨테이너 초기화도 가능
```c++
std::vector<int> v{ 1, 3, 5 }; // v's initial content is 1, 3, 5
```
    
* non-static date member도 초기화 가능
```c++
class Widget {
private:
int x{ 0 }; // fine, x's default value is 0
int y = 0; // also fine
int z(0); // error!
};
```
* uncopyable 오브젝트도 초기화 가능
```c++
std::atomic<int> ai1{ 0 }; // fine
std::atomic<int> ai2(0); // fine
std::atomic<int> ai3 = 0; // error!
```
* 암묵적 축소변환도 방지해줌
```c++
double x, y, z;
int sum1{ x + y + z }; // error! sum of doubles may not be expressible as int
int sum2(x + y + z); // okay (value of expression truncated to an int)
int sum3 = x + y + z;
```
* C++’s most vexing parse에 면역(?)이다.
```c++
Widget w1(10); // call Widget ctor with argument 10
Widget w2(); // most vexing parse! declares a function named w2 that returns a Widget!
Widget w3{}; // calls Widget ctor with no args
```
## Braced initialization 사용시 유의점
```c++
class Widget {
public:
  Widget(int i, bool b); // ctors not declaring
  Widget(int i, double d); // std::initializer_list params
  ...
};
Widget w1(10, true); // calls first ctor
Widget w2{10, true}; // also calls first ctor
Widget w3(10, 5.0); // calls second ctor
Widget w4{10, 5.0}; // also calls second ctor
```
* std::initializer_list 생성자 추가
```c++
class Widget {
public:
  Widget(int i, bool b);
  Widget(int i, double d);
  Widget(std::initializer_list<long double> il); // added
}
Widget w1(10, true); // uses parens and, as before,
                     // calls first ctor
Widget w2{10, true}; // uses braces, but now calls
                     // std::initializer_list ctor
                     // (10 and true convert to long double)
Widget w3(10, 5.0); // uses parens and, as before,
                    // calls second ctor
Widget w4{10, 5.0}; // uses braces, but now calls
                    // std::initializer_list ctor
                    // (10 and 5.0 convert to long double)
```
* 복사 생성이나 move연산자일때도 std::initializer_list생성자가 끼어든다.
```c++
class Widget {
public:
  Widget(int i, bool b);
  Widget(int i, double d);
  Widget(std::initializer_list<long double> il);
  operator float() const; // added
};
Widget w5(w4); // uses parens, calls copy ctor
Widget w6{w4}; // uses braces, calls
               // std::initializer_list ctor
               // (w4 converts to float, and float
               // converts to long double)
Widget w7(std::move(w4)); // uses parens, calls move ctor
Widget w8{std::move(w4)}; // uses braces, calls
                          // std::initializer_list ctor
                          // (for same reason as w6)
```
* 맞는 생성자가 있더라도 std::initializer_list생성자로 추론
```c++
class Widget {
public:
  Widget(int i, bool b);
  Widget(int i, double d);
  Widget(std::initializer_list<bool> il);
};
Widget w{10, 5.0}; // uses braces, calls
               // std::initializer_list ctor
               // (w4 converts to float, and float
               // converts to long double)
// Widget(std::initializer_list<std::string> il) 으로 변경해서 해결됨
```
* 빈중괄호{}는 인수 없음 뜻
```c++
class Widget {
public:
  Widget(); // default ctor
  Widget(std::initializer_list<int> il); // std::initializer
... };
Widget w1; // calls default ctor
Widget w2{}; // also calls default ctor
Widget w3(); // most vexing parse! declares a function!
Widget w4({}); // calls std::initializer_list ctor with empty list
Widget W5{{}};
```
* std::initializer_list 생성자를 쓰는곳
```c++
std::vector<int> v1(10, 20); // 모든 요소가 20인 10개 element를 가진 container 생성
std::vector<int> v2{10, 20}; // 10, 20 두 요소를 가진 container 생성
```
## 결론
{}는 ()와 다르게 다양한 문맥에 적용이 가능 좁히기 변환 방지 c++ 성가신 구문해석에 자유롭다는 점   
()는 c++전통과의 일관성, auto가 std::initializer_list를 추론할수 없다는점, 그리고 객체생성시 의도치 않게 std::initializer_list 생성자가 호출되는 일이 없다는점   
둘다 장단점이 있으니 둘다 사용하되 하나만 일관되게 사용하자.

