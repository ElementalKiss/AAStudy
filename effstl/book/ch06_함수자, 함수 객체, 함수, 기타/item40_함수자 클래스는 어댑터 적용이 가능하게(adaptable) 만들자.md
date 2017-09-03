# 항목 40 : 함수자 클래스는 어댑터 적용이 가능하게(adaptable) 만들자

## Widget 예제

* Widget 포인터를 담는 리스트가 있고 '관심있는' Widget 객체를 가리키는 포인터를 찾는 함수가 있다.

```c++
list<Widget*> widgetPtrs;
bool isInteresting(const Widget* pw);
```

* isInteresting이 true를 반환하는 첫 번째 포인터를 찾고 싶다.

```c++
auto it = find_if(widgetPtrs.begin(), widgetPtrs.end(), isInteresting);

if (it != widgetPtrs.end()) {
    // do someting
}
```

* isInteresting이 false인 것을 찾고 싶다면?
* **not1**(isInteresting)을 사용하면 될 것 같지만 컴파일 에러가 난다.

```c++
auto it = find_if(widgetPtrs.begin(), widgetPtrs.end(), not1(isInteresting));
```