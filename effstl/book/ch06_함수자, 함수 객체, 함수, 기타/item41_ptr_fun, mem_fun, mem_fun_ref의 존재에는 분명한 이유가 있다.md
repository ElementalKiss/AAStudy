# 항목 41 : ptr_fun, mem_fun, mem_fun_ref의 존재에는 분명한 이유가 있다

## 아주 중요한 일을 하는 함수이고, 일차적인 구실은 C++가 원래부터 가진 문법적 모순(inconsistency)을 감싸주는 것

* f란 이름의 함수와 x란 이름의 객체가 하나 있다.
* 여기서 x에 대해 f를 호출하고 싶으며, 호출하는 위치는 x의 멤버 함수의 바깥쪽
* 이 때 C++이 제공할 수 있는 문법은 다음 세가지다
```
[문법 #1]
f(x);   // f가 멤버 함수가 아닌 경우

[문법 #2]
x.f();  // f가 x의 멤버 함수, x는 객체이든지 객체에 대한 참조자

[문법 #3]
p->f(); // f가 x의 멤버이며, p는 x객체에 대한 포인터
```

이제, Widget 객체를 검사할 수 있는 함수가 하나 있다고 가정
```
void test(Widget& w);   //w를 검사해서 기준을 통과하지 않으면 "failed" 표시
```

그리고 Widget 객체의 컨테이너가 하나 있다.
```
vector<Widget> vw;
```

이때 vw에 들어 있는 모든 Widget 객체를 테스트하려면 for_each를 사용할 수 있다.
```
for_each(vw, begin(), vw.end(), test);      // [호출 #1]: 컴파일 된다.
```

하지만, test가 보통 함수가 아니고 Widget의 멤버 함수라면
```
class Widget {
public:
    void test();
}
...

for_each(vw.begin(), vw.end(), &widget::test);  // [호출 #2]: 컴파일 안 된다.
```

for_each를 써서 Widget* 포인터의 컨테이너에 대해 Widget::test를 호출하도록 하면?
```
list<Widget*> lpw;  // lpw 는 Widget 객체에 대한 포인터를 가지고 있다.
for_each(lpw.begin(), lpw.end(), &Widget::test); // [호출 #3]: 역시 컴파일 안 된다..
```

* 호출 #1의 for_each 내부에서는 비멤버 함수에 객체를 넘겨 호출하고 있기 때문에 문법 #1이 사용 되어야 한다.
* 호출 #2의 for_each는 컨테이너 안에 들어 있는 객체에 대해 멤버 함수를 호출하고 있으니 문법 #2가 사용되어야 한다.
* 호출 #3에서는 객체가 아닌 포인터 이므로 문법 #3을 사용해야 한다.

### 서로 다른 세 가지 for_each가 필요하지만, 한 가지 뿐..

* 어떻게 구현되어 있는지 예상해보자면
```
template<typename InputIterlator, typename Function>
Function for_each(InputIterator begin, InputIterator end, Function f)
{
    while (begin != end) f(*begin++);
}
```
* f 부분을 보면 for_each 는 문법 #1을 사용한다는 사실을 알 수 있다.


## mem_fun, mem_fun_ref 는 멤버 함수를 호출할 때 문법 #1을 사용하도록 해준다.

* 이들은 함수 템플릿이며, 매개 변수의 개수와 멤버 함수의 const 여부에 따라 약간씩 변경된 형태가 몇 가지 더 있다.

```
template<typename R, typename C>
mem_fun_t<R,C> mem_fun(R (C::*pmf)());
```
* 매개 변수를 받지 않고 const 멤버가 아닌 멤버 함수에 대한 mem_fun의 선언문
* C는 클래스이며 R은 포인터로 가리켜지는 멤버 함수(pmf)의 반환값 타입
* mem_fun 은 멤버 함수의 포인터, pmf를 받아 mem_fun_t 타입의 객체를 반환한다.
* 즉, 이것은 멤버 함수 포인터를 가지고 있으면서 operator()에 넘겨진 객체에 대해 멤버 함수를 호출해 주는 operator()를 제공하는 함수자 클래스

[호출 #3] 에 적용한 예제
```
list<Widget*> lpw;
...
for_each(lpw.begin(), lpw.end(), mem_fun(&Widget::test)); // 컴파일이 된다!
```
* for_each 는 Widget::test의 포인터를 가지고 있는 mem_fun_t 타입의 객체를 받는다.
* for_each는 lpw내의 Widget* 포인터에 대해 mem_fun_t 객체를 불러오는데, 이때 문법 #1을 쓴다.
* 그리고, mem_fun_t 객체는 바로 Widget* 포인터에 대해 Widget::test를 호출한다.

* mem_fun 은 문법 #3을 문법 #1 처럼 사용하게 해주는 어댑터
* mem_fun_ref 는 문법 #2를 문법 #1에 맞춰주는 어댑터

```
for_each(vw, begin(), vw.end(), ptr_fun(test));      // 호출 #1과 똑같이 컴파일되고 동작한다.
```
* STL 컴포넌트에 함수를 넘길 때 항상 ptr_fun 을 태운다고 생각하면 편하다.
* STL이 전혀 신경쓰지 않으며, 성능 저하도 없고 꼭 필요한 경우에만 써도 된다.
* mem_fun과 mem_fun_ref 는 멤버 함수를 넘길 때에는 꼭 사용해야 한다.
