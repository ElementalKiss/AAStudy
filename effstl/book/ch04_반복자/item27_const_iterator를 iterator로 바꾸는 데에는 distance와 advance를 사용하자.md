# 항목 27 : const_iterator를 iterator로 바꾸는 데에는 distance와 advance를 사용하자

# 단순히 캐스팅을 해보면 어떻게 되는지 보자

```c++
typedef deque<int> IntDeque;
typedef IntDeque::iterator Iter;
typedef IntDeque::const_iterator ConstIter;

ConstIter ci;
/*
 * ci를 원하는 곳을 향하게 한다
 * ci가 가리키는 값을 바꾸고 싶은 상황
 */

Iter i1(ci);                    // 둘 모두
Iter i2(const_cast<iter>(ci));  // 에러가 난다...
```

- `list`, `set`, `multiset`, `map`, `multimap`와 같은 컨테이너 타입에서 `const_iterator`와 `iterator`는 서로 다른 클래스이다. 둘은 `string`과 `complex<double>`만큼이나 멀다!

- `vector`, `string`은 컴파일이 된다. 왜냐하면 이 둘은 반복자가 포인터를 기반으로 구현되어있기 때문.
  + `vector<T>::iterator`는 `T*`와 같고,
  + `vector<T>::const_iterator`는 `const T*`와 같다.
  + 심지어 이 둘이 단순 포인터로 동작할 때는 release모드일 때 뿐이고, debug모드에서는 클래스가 된다.

- 결론적으로 어떤 컨테이너든 간에 `iterator`를 `const_iterator`로 바꾸는 일은 항상 좋지 못하다!

# `advance`와 `distance`를 이용해보자

```c++
typedef deque<int> IntDeque;
typedef IntDeque::iterator Iter;
typedef IntDeque::const_iterator ConstIter;

IntDeque d;
ConstIter ci;
/*
 * ci를 원하는 곳을 향하게 한다
 * ci가 가리키는 값을 바꾸고 싶은 상황
 */

Iter i(d.begin());
advance(i, distance(i, ci));
```

- `distance`는 같은 컨테이너를 가리키고 있는 두 반복자 사이의 거리를 알려준다.

- `advance`는 어떤 반복자를 지정된 거리만큼 이동시킨다.

- `iterator`를 새로 만든 뒤에 `const_iterator`가 가리키는 곳으로 이동을 시킨 것.

- 하지만 위의 코드는 아이디어 뿐, 실제로 **컴파일 되지 않는다**.

# `distance` 함수를 살펴보자

```c++
template<typename InputIterator>
distance(InputIterator first, InputIterator last);
```

- 이전의 `advance(i, distance(i, ci))`를 보면 `distance`함수에 `i`와 `ci`가 같이 넘겨졌다. 하지만, `distance`의 parameter는 둘이 같은 타입을 받는다.

- 따라서 컴파일러가 `InputIterator`의 타입을 결정할 수 없어서 오류를 낸다.

- 다음과 같은 방법으로 컴파일러가 타입을 추측하는 일을 사전에 방지할 수 있다.

```c++
advance(i, distance<ConstIter>(i, ci));
```

- `iterator` 타입인 `i`는 컴파일러에 의해 `const_iterator`로 저절로 바뀌기 때문에 컴파일 에러가 나지 않는다!