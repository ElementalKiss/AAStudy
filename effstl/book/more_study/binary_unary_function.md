# `unary_function` / `binary_function`

## 사전적 의미

- `unary`: **단항(1)** 의
- `binary`: **이항(2)** 의
- 즉, `unary_function`은 인자를 **하나** 받는 `boolean`값을 반환하는 함수의 베이스 템플릿
  + (e.g) `even`, `odd`, `not`, etc.
- `binary_function`은 인자를 **두 개** 받는 `boolean`값을 반환하는 함수의 베이스 템플릿
  + (e.g) `less`, `greater`, etc.

## 정의

### c++98

```c++
template <class T>
struct less : binary_function<T,T,bool>
{
  bool operator() (const T& x, const T& y) const { return x<y; }
};
```

- `binary_function`을 상속받고 있는데, 이것의 역할은 STL컨테이너에서 사용하는데 필요한
  `typedef`를 대신 해준다

### c++11

```c++
template <class T>
struct less
{
  bool operator () (const T& x, const T& y) const { return x<y; }
  typedef T first_argument_type;
  typedef T second_argument_type;
  typedef bool result_type;
};
```

- 기존 `c++98`에서 쓰였던 것은 너무 자세해서 표준에서 내려갔다

- STL컨테이너에서 `less` 함수객체를 사용할 때, `first_argument_type`,
  `second_argument_type`, `result_type`을 사용하기 때문에 클래스 내에 `typedef`를 해주었다

- 이외에도 여러 클래스들이 상위호환들이 나옴에 따라 다음과 같이 교체되었다

|                      c++98                  |         c++11         |
|:-------------------------------------------:|:---------------------:|
|`std::unary_function`, `std::binary_function`|`std::function`, lambda|
|     `std::mem_fun`, `std::mem_fun_ref`      |     `std::mem_fn`     |
|   `std::bind1st`, `std::bind2nd`            |      `std::bind`      |

## 기타

- `unary_function`, `binary_function`이 `c++11`에 오면서 사라진 것은 아니다. 표준에서
  내려갔을 뿐. 하지만 이마저도 `c++17`에서 아예 사라지는것으로 결정이 되었다.