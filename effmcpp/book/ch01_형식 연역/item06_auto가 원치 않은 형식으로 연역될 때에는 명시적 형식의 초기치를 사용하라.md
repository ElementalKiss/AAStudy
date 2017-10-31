# 항목 6: auto가 원치 않은 타입으로 추론될 때에는 명시적 타입의 초기치를 사용하라.

## 시작하기에 앞서
* proxy(대리자) class?
    1. 보이지 않는 proxy class
    2. 보이는 proxy class
* explicitly typed initializer idiom? (타입 명시 초기치 관용구... 어... 그래...)

## 저자가 말하고자 하는 바
* `auto someVar = 보이지 않는 proxy class 반환 형식의 표현식`을
* `auto someVar = static_cast<someVarType> (proxy class 반환 형식의 표현식)`로 표현하자

## 왜?
```c++
std::vector<bool> features(const Widget& w);
...
Widget w;
bool highPriority = features(w)[5];
processWidget(w, highPriority);
```

- 위젯의 피쳐들 중 5번재 피쳐를 가져와 활용하는 코드
- 빌드도 잘되며 실행도 잘된다.
- 이제 그 좋다던 `auto`를 써보자.

```c++
auto highPriority = features(w)[5];
```

- 빌드는 문제 없지만, 런타임 시 미정의 동작 발생,
- `_DEBUG_ERROR("vector<bool> iterator not dereferencable");`

## 문제 원인
- auto로 받은 highPrioirty의 타입은 `std::vector<bool>::reference`, 즉 proxy 객체이다.

## 따라가기
### Step 1 : 왜 std::vector<bool>::reference를 반환해?
- `std::vector<T>`의 operator[]는 T&를 돌려주지만,
- `std::vector<bool>`의 operator[]는 bool&를 돌려주지 못한다.
- 이유는, c++에서 비트에 대한 참조는 금지되어 있기 때문.
- 따라서 bool&처럼 동작하는 proxy 객체, `std::vector<bool>::reference` 객체를 반환한다.
- `std::vector<bool>::reference` 객체가 하는 일은, `std::vecor<bool>`를 가리키는 포인터와 5번째 인자를 의미하는 _offset값을 가지고 결과 bool값을 얻어내는 기능을 한다.
### Step 2 : 문제는 왜 발생 해?
- auto의 경우는 `std::vector<bool>::reference`로 타입 추론이 되어버리고,
- 그 결과 `std::vector<bool>::reference highPriority = features(w)[5];`인 코드로,
- features(w)[5]의 임시 반환 값 temp를 highPriority에 값 복사를 한다.
- 이제 temp는 사라지고, highPriority가 가지고 있는 _ptr값은 대상을 잃게 되는 문제가 발생한다.
- 따라서 위의 미정의 동작이 발생한 것.
### Step 3 : 기존은 왜 문제가 없었지?
- 기존 `bool highPriority = features(w)[5]`에서는 암묵적 형변환이 발생하여 bool값을 가진다.

### Step 4 : std::vector<bool>::reference 구조?
```c++
//개인적인 std::vector<bool>::reference의 상상도
{
    ...
private:
    const unsigned int* _ptr; // 시작 포인터
    unsigned int _offset; // 5 값
}

// std::vector
// CLASS _Vb_reference
template<class _Alloc>
    class _Vb_reference
        : public _Vb_iter_base<_Alloc>
    {   // reference to a bit within a base word
        ...
```

- 필요하면 std::vector 까보세요...

## 해결법
- auto를 우리가 원하는 타입으로 추론하게 유도하자.
- explicitly typed initializer idiom (타입 명시 초기치 관용구...)를 이용.
```c++
auto highPriority = static_cast<bool>(features(w)[5]);
```

## 결론 
* `auto someVar = 보이지 않는 proxy class 반환 형식의 표현식`을
* `auto someVar = static_cast<someVarType> (proxy class 반환 형식의 표현식)`로 표현하자

