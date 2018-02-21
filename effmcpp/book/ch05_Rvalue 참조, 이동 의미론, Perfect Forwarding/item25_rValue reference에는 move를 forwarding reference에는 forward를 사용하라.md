#### * 예제 코드
- rValue reference를 받는 widget 개체를 선언
```c++
class Widget {
    Widget(Widget&& rhs);
};
```
## 1. rValue reference 인 경우
```c++
class Widget{
public:
    Widget(Widget&& rhs)
     : name(std::move(rhs.name)),
      p(std:move(rhs.p))
    {}
private:
    std::string name;
    std::shared_ptr<SomeDataStructrue> p;
};
```
## 2. forwarding reference 인 경우
```c++
class Widget{
public:
    template<typename T>
    void setName(T&& newName) {
        name = std::forward<T>(newName);
    }
}
```
#### 하지만 forwarding reference에 move를 사용하는 경우
```c++
class Widget{
public:
    template<typename T>
    void setName(T&& newName){
        name = std::move(newName);
    }
};
std::string getWidgetName(); //함수

Widget w;
auto n = getWidgetName();
w.setName(n); //move 연산으로 n은 unspecified value
```
#### 그래서 const 버젼으로 오버로딩을 해결하려하면
```c++
class Widget{
public:
    void setName(std::string&& newName) {
        name = std::move(newName);
    }
    void setName(const std::string& newName) {
        name = newName;
    }
};
```
#### 해결은 되지만 유지보수비용만 증가하게 됨
- 템플릿함수가 비템플릿함수 두개로 변경
- 효율성이 떨어짐
    - w.setName("hello world") 인경우 임시 객체가 생성되어 비용이 증가
- 코드의 scalability가 떨어짐
    - make_shared, make_unique인 경우 내부적으로 forward를 사용
## 3. 사용시 유의점
1. move나 forward 사용시 참조의 마지막에 사용하자
```c++
template<typename T>
void setSignText(T&& text){
    sign.setText(text);
    auto now = std::chrono::system_clock::now();
    signHistory.add(now, std::forward<T>(text)); // 마지막에 사용
}
```
2. 함수가 return by value이고 그값이 rValue reference나 forwarding reference 타입인 경우
```c++
Matrix operator+(Matrix&& lhs, const Matrix& rhs){
    lhs += rhs;
    return std::move(lhs);
}
```
- std::move덕분에 컴파일러는 lhs를 함수의 반환값 장소로 이동한다.
- std::move가 없다면 계산을 하고 반환값장소로 복사를 함.
```c++
template<typename T>
Fraction reduceAndCopy(T&& frac){
    frac.reduce();
    return std::forward<T>(frac);
}
```
- forwarding reference도 마찬가지
3. rvo(return value optimization) 대상이 되는곳에서 사용하지 말자
```c++
Widget makeWidget(){
    Widget w;
    return w;
}
```
### 정리
1. rvalue referecne에는 move를 forwarding reference에는 forward를 사용하자.
2. 값전달(return by value)함수일때도 move나 forward를 사용하자.
3. rvo 대상함수에 move나 forward를 사용하지 말자.

