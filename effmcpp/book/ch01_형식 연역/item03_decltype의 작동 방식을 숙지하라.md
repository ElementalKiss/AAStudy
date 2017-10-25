# 항목 3: decltype의 작동 방식을 숙지하라

## decltype
* 주어진 이름이나 표현식의 형식(declared type)을 알려줌
    ``` cpp
    const int i = 0;         // decltype(i) is const int
    
    bool f(const Widget& w); // decltype(w) is const Widget&
                             // decltype(f) is bool(const Widget&)
    struct Point {
        int x, y;            // decltype(Point::x) is int
    };                       // decltype(Point::y) is int
    
    Widget w;                // decltype(w) is Widget
    
    if (f(w)) …              // decltype(f(w)) is bool
    
    template<typename T>     // simplified version of std::vector
    class vector {
    public:
        …
        T& operator[](std::size_t index);
        …
    };
    vector<int> v;          // decltype(v) is vector<int>
    …
    if (v[0] == 0) …        // decltype(v[0]) is int&
    ```
* 함수의 반환 형식이 매개변수에 의존하는 경우에 자주 사용
    * C++11
        ``` cpp
        template<typename Container, typename Index>
        auto authAndAccess(Container& c, Index i) // 이 때 auto는 타입 추론과 관련 없음, 함수의 반환 형식을 매개 변수 목록 다음에 선언하겠다는 뜻
        -> decltype(c[i]) // 반환 형식에 매개변수 이용 가능
        {
            authenticateUser();
            return c[i];
        }
        ```
    * C++14
        ``` cpp
        template<typename Container, typename Index>
        auto authAndAccess(Container& c, Index i)
        {
            authenticateUser();
            return c[i]; // 반환 형식은 c[i]로부터 추론됨
        }
        ```
* 템플릿 타입 추론과정에서 초기화 표현식의 참조성이 무시되는 것의 영향을 받는 경우
    ``` cpp
    std::deque<int> d; // C++14;
    …
    authAndAccess(d, 5) = 10;
    ```
    - deque의 operator[]는 int&를 리턴하지만 authAndAccess에 대한 auto 반환 타입 추론 과정에서 참조가 제거됨
    - 함수의 반환값인 int는 Rvalue로 Rvalue에 10을 배정하려 하므로 컴파일 실패

## decltype(auto)
* 위 문제를 해결하여 함수의 반환값이 실제 반환값과 동일하게 변경하려면 decltype(auto)를 사용하면 해결됨
    ``` cpp
    template<typename Container, typename Index> // C++14
    decltype(auto) // auto는 해당 형식이 추론되어야 함을 뜻하며, decltype은 타입 추론 과정에서 decltype 타입 추론 규칙들이 적용 되어야함을 뜻함
    authAndAccess(Container& c, Index i)
    { 
        authenticateUser();
        return c[i];
    }
    ```
* 변수 선언시에도 사용 가능
    ``` cpp
    Widget w;
    const Widget& cw = w;
    auto myWidget1 = cw;           // auto type deduction:
                                   // myWidget1's type is Widget
    decltype(auto) myWidget2 = cw; // decltype type deduction:
                                   // myWidget2's type is
                                   // const Widget&
    ```
## 함수의 매개변수를 Rvalue 참조로 사용하는 경우
* C++11
    ``` cpp
    template<typename Container, typename Index>
    auto
    authAndAccess(Container&& c, Index i)
    -> decltype(std::forward<Container>(c)[i])
    {
        authenticateUser();
        return std::forward<Container>(c)[i];
    }
    ```
* C++14
    ``` cpp
    template<typename Container, typename Index>
    decltype(auto)
    authAndAccess(Container&& c, Index i)
    {
        authenticateUser();
        return std::forward<Container>(c)[i];
    }
    ```
## 이름이 아닌 형식이 T인 Lvalue 표현식의 경우
* T가 아닌 T& 리턴
* Lvalue 표현식에는 Lvalue 참조가 포함 되어 있으므로 큰 문제가 되지 않음.
    ```cpp
    int x = 0; // decltype(x) is int;
               // decltype((x)) is int&;

    decltype(auto) f1()
    {
        int x = 0;
        …
        return x; // decltype(x) is int, so f1 returns int
    }

    decltype(auto) f2()
    {
        int x = 0;
        …
        return (x); // decltype((x)) is int&, so f2 returns int&
    }
    ```

## 기억해 둘 사항들
* decltype은 항상 변수나 표현식의 형식을 아무 수정없이 보고한다
* decltype은 형식이 T이고 이름이 아닌 Lvalue 표현식에 대해서는 항상 T& 형식을 보고한다
* C++14는 decltype(auto)를 지원한다. decltype(auto)는 auto처럼 초기치로부터 타입을 추론하지만, 그 타입 추론 과정에서 decltype의 규칙들을 적용한다.

## std::forward
* 템플릿 전용 함수로 데이터형 매개변수를 필히 명시 해야함. (인자추론 불가)
* 매개변수가 Rvalue 혹은 Rvalue 참조인 경우에 대해 매개변수의 Rvalue 반환, 그렇지 않은 경우 매개변수 그대로 반환.