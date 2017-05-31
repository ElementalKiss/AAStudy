# 항목 20: 포인터를 저장하는 연관 컨테이너에 대해서는 적합한 비교(비교 함수자) 타입을 정해주자

## 예제

```cpp
set<string*> ssp;
ssp.insert(new string("Anteater"));
ssp.insert(new string("Wombat"));
ssp.insert(new string("Lemur"));
ssp.insert(new string("Penguin"));

set<string*>::const_iterator i = ssp.begin();
for ( ; i != ssp.end(); ++i) {
    cout << *i << endl;
}
```

* 셋이 자동 정렬을 해줄테니 문자열이 알파벳 순서대로 출력될 것 같지만, 결과는 16진수의 숫자가 출력된다...
* ssp는 포인터를 담고 있기 때문에 `*i` 는 string 객체가 아니라 **string 객체의 포인터**이다.
* (앞의 예제는 항목 43에 나올 가이드라인, 즉 루프를 직접 만들지 말라는 교훈을 예고하는 좋은 예제)

## copy 알고리즘을 사용해보자

```cpp
copy(ssp.begin(), ssp.end(),
        ostream_iterator<string>(cout, "\n"));
```

* 에러 발생, 위의 코드는 컴파일 되지 않는다.
* `ostream_iterator`는 자신이 스트림으로 출력해 보낼 객체의 타입에 대해 알아야 하기 때문에, 이 객체에게 이 타입이 `string`이라고 알려주게 되면 컴파일러는 `ssp`에 들어 있는 요소의 타입과 다르다는 것을 알아채므로 컴파일을 거부한다.
* 이래서 타입 안정성이 필요한 것

* 첫번째 코드에서 `*i`를 `**i`로 바꾸면 아마 결과는 볼 수 있을 것이다.
    + (안된다; `(*i)->c_str()`로 대체)
    + 우리가 원하는 결과가 아니다.
* 알파벳 순서가 아닌, 포인터 값에 의해 정렬된 결과를 보게 될 것이다.
* 24분의 1의 확률로 원하는 값이 나올 수도..

## 알파벳 순서로 string* 포인터를 정렬해보자

```cpp
set<string*> ssp;
set<string*, less<string*>> ssp;
set<string*, less<string*>, allocator<string*>> ssp;
```

* 위의 세 코드는 같다.(less, allocator는 생략된 것)
* 기본적으로 주어지는 `less<string*>` 이라는 비교 함수자 클래스말고, 용도에 맞게 직접 만들어야 한다.

```cpp
struct StringPtrLess :
    public binary_function<const string*, const string*, bool> 
{
    bool operator() (const string *ps1, const string *ps2) const {
        return *ps1 < *ps2;
    }
};
```

* 이 클래스를 `ssp`의 비교 함수자 클래스로 넣어주자.
* 알고리즘을 쓰고 싶다면, `string*` 포인터를 받아, 이것을 역참조해서 화면에 출력해 주는 함수만 있으면 된다.
* 물론 `set`은 `StringPtrLess`를 비교 함수자 타입으로 사용하고 있어야 한다.

## 다음은 for_each 알고리즘으로 깔끔해진 모습

```cpp
void print(const string *ps)
{
    cout << *ps << endl;
}

for_each(ssp.begin(), ssp.end(), print);
```

* 타입에 관계없이 포인터를 역참조하여 동작하는 함수자 클래스를 만들어서, 이것을 `transform`과 `ostream_iterator`와 함께 사용하면 더 깔끔한 코드를 만들 수 있다.

```cpp
// 이 타입의 함수자에 T*이 넘겨지면, const T& 를 반환한다.
struct Dereference {
    template <typename T>
    const T& operator() (const T* ptr) const
    {
        return *ptr;
    }
};

// ssp에 들어있는 요소를 역참조용 함수자를 통해 변환하고 그 결과를 cout으로 보낸다.
transform(ssp.begin(), ssp.end(),
        ostream_iterator<string>(cout, "\n"),
        Dereference());
```

* 이번 챕터의 핵심 포인트는 **"포인터를 담는 연관 컨테이너를 만들어 쓸 때에는 컨테이너에 담긴 포인터 값을 기준으로 정렬이 이루어진다는 사실을 기억해야 한다는 점"** 이다.
* 따라서 함수자 클래스를 거의 항상 만들어야 한다.

## 필자가 분명 "비교 타입(Comparison type)" 이라고 했다.

* 여기서 "그냥 set 객체의 비교 함수를 간단히 만들면 되지 않나?" 할 수도 있다.
* 아래 처럼

```cpp
bool StringPtrLess(const string* ps1, const string* ps2)
{
    return *ps1 < *ps2;
}

set<string, StringPtrLess> ssp; // Function "StringPtrLess" is not a type name
```

* `ssp`의 비교 함수로 `stringPtrLess`를 쓰려하면 컴파일이 안된다.
* set 템플릿에 들어가는 세 개의 매개 변수는 타입(type)이어야 한다.
* 내부적으로 인스턴스화하여 함수를 만들 수 있는 타입을 원한다.

## 포인터의 연관 컨테이너를 만드는 일과 컨테이너의 비교 타입을 지정하는 일은 실과 바늘의 관계라고 생각하자!

* 대부분  이 때 만들게 되는 비교 타입은 포인터를 역참조한 후에, 그 포인터가 가리키는 객체를 비교하게 될 것이다.(앞의 `StringPtrLess` 처럼)
* 이런 코드 작성이 많을 경우 비교 함수자의 템플릿을 하나 준비해 두자.

```cpp
// 매개 변수는 값으로 전달되며, 값 자체를 포인터로 간주
struct DereferenceLess {
    template<typename PtrType>
    bool operator() (PtrType pT1, PtrType pT2) const
    {
        return *pT1 < *pT2;
    }
};

set<string*, DereferenceLess> ssp;  //StringPtrLess 대신 사용할 수 있다.
```

## 한가지 더!

* 이번 항목은 포인터를 담는 연관 컨테이너에 관한 이야기이지만, 연관 컨테이너에 담는 객체가 포인터처럼 동작하는 객체인 경우에도 마찬가지이다.
* 이를테면 스마트 포인터나 반복자를 가리키는 경우
* `DereferenceLess` 는 물론, 이번 항목에서 이야기한 해결책은 모두 적용할 수 있다.
* T*의 연관 컨테이너에 대한 비교 타입으로 만들어 본 이 템플릿은 T 객체에 대한 반복자나 스마트 포인터를 담는 연관 컨테이너의 비교 타입으로도 적합하다.
