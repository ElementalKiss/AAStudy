# 항목 32 : 요소를 정말로 제거하고자 한다면 remove 류의 알고리즘에는 꼭 erase를 붙여 사용하자

# `std::remove`는 요소를 제거하지 않는다!

```c++
vector<int> v;
for (int i = 1; i <= 10; ++i) {
    v.push_back(i);
}
cout << v.size();       // 당연히 10
v[3] = v[5] = v[9] = 99;
remove(v.begin(), v.end(), 99);
cout << v.size();       // 여전히 10!??
```
- `remove`를 사용했음에도 벡터 `v`의 사이즈가 변하지 않았다.

```c++
template<class ForwardIterator, class T>
ForwardIterator remove(ForwardIterator first,
                       ForwardIterator last,
                       const T& value);
```
- 실제 `remove`가 하는 일은 인자로 받은 *제거되지 않는 값*들을 모조리 앞으로 이동시키는 것
- 위 행동을 마친 뒤 *제거되지 않는 값* 중 가장 마지막 것의 바로 뒤를 가리키는 반복자를 리턴한다

# `std::remove`가 작동하는 과정

```c++
vector<int>::iterator newEnd(remove(v.begin(), v.end(), 99));
```
- 위에서 정의했던 벡터 `v`를 대상으로 반복자 `newEnd`가 어떻게 되는지를 관찰해보자

## 1. `remove`가 적용되기 전  
![alt text](https://github.com/ElementalKiss/AAStudy/blob/master/effstl/book/image/item32_img01.png "before remove")

## 2. `remove`가 적용되는 중  
![alt text](https://github.com/ElementalKiss/AAStudy/blob/master/effstl/book/image/item32_img04.png "while removing")
- 인자로 전달 받은 `value`가 있다면 그 자리는 빈자리로 보고 뒤의 요소를 복사해와서 덮어쓰게 된다

## 3. `remove`가 적용된 후  
![alt text](https://github.com/ElementalKiss/AAStudy/blob/master/effstl/book/image/item32_img03.png "remove result")
- 원래 `99`인 요소는 세 개가 있었지만, 나머지 요소를 덮어씌워 나가면서 `99`는 두 개가 손실됐다  
  하지만 `99`는 제거될 것이기 때문에 문제가 되지 않는다
- `remove`가 리턴한 반복자 `newEnd`는 *제거되지 않는 값* 중 가장 마지막인 `9`의 바로 뒤를 가리키게 된다
- 실제로 요소를 제거한 것이 아니기 때문의 컨테이너의 `size`는 바뀌지 않았다

# `std::remove`의 실제 사용법

```c++
vector<int> v;
/*
 * do something
 */
v.erase( remove(v.begin(), v.end(), 99), v.end() );
```
- 위와 같이 대부분의 경우에 `erase-remove` 합성문의 형태로 쓰인다

# 기타

- `std::list::remove`는 위의 `erase-remove` 합성문을 하나로 합쳐놓은 것으로 요소를 실제로 삭제한다!
- `std::list::remove_if`, `std::list::unique` 또한 `erase-remove` 합성문과 같은 알고리즘을 사용한다