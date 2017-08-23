# 항목 37: 범위 내의 데이터 값을 요약하거나 더하는 데에는 accumulate나 for_each를 사용하자

## accumulate
* 내부적으로 double 타입의 변수를 사용해서 자신이 계산하는 값의 합을 저장
* 두 개의 반복자와 초기값을 받아들이는 형태
** 입력 반복자이기만 하면 동작하므로 istream_iterators와 istream-buf_iterators 적용 가능
```
list<double> Id;
...
double sum = accumulate(Id.begin(), Id.end(), 0.0);
```
* 두 개의 반복자와 초기값, 요약용 함수를 받아들이는 형태 
```
string::size_type;
stringLengthSum(string::size_type sumSoFar, const string& s)
{
    return sumSoFar + s.size();
}

set<string> ss;
...
string::size_type lengthSum = accumulate(ss.begin(), ss.end(), 0, stringLengthSum);
```

## size_type
* 어떤 컨테이너의 size를 호출했을 때 반환되는 값의 타입
* Container::size_type은 size_t를 예쁘게 포장한 것
