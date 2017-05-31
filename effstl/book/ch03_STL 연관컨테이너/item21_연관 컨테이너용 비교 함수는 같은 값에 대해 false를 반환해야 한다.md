# 항목 21: 연관 컨테이너용 비교 함수는 같은 값에 대해 false를 반환해야 한다

# less_equal를 썼을 때 발생하는 악몽
```
set<int, less_equal<int>> s;
s.insert(10);
s.insert(10);
```
* less_equal 사용 시 비교 함수는 <= 가 사용되어 결과는 false 출력

# string 비교 함수의 악몽
```cpp
struct StringPtrGreater :
    public binary_function<const string*, const string*, bool> {
    bool operator()(const string *ps1, const string *ps2)
    {
        return !(*ps1 < *ps2)
    }
}
```
* 위와 같이 작성하게 되면 <의 부정은 아쉽게도 >이 아닌 >=로 잘못된 결과가 초래됨
```cpp
struct StringPtrGreater :
    public binary_function<const string*, const string*, bool> {
    bool operator()(const string *ps1, const string *ps2)
    {
        return *ps2 < *ps1
    }
}
```
* 위와 같이 작성하여야 문제가 없음

# multiset의 악몽
```cpp
multiset(int, less_equal<int>> s;
s.insert(10);
s.insert(10);
```
* 위 컨테이너에 equal_range를 사용하면 두 개의 사본에 대해서 반복자 쌍을 얻을 수 있을 것 같으나 불가능
* equal_range는 같은 값의 범위가 아님 동등한 값의 범위를 식별하기 때문에

# 꼭 기억 해야 할 것
* 비교 함수의 반환값은 어떤 값이 다른 값보다 정렬 순서에서 앞에 오는지의 여부.
* 비교 할 두 개의 값이 같다면 비교 함수에서는 false를 반환 해야 함.
