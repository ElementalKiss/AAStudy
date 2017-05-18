# 항목 16 : 기존의 C API에 vector와 string을 넘기는 방법을 알아두자

# vector를 C API에 전달 하는 법
* vector<int> v;
* 벡터가 비어 있지 않은 경우 &v[0], 비어 있는 경우는 문제가 발생하니 empty 함수로 확인 필요
* begin은 반복자를 리턴하므로 주의하되 쓰려거든 &*v.begin() 사용

# string을 C API에 전달 하는 법
* string s;
* s.c_str() 호출, 비어 있는 경우에도 널 문자에 대한 포인터를 반환해줌.
* 주의 해야 할 것은 여러개의 널 포인터중에 첫 번째 것을 끝으로 간주함.
* c_str()은 문자열 데이터의 사본의 포인터라는 것 주의.

# vector 객체를 C API를 통해 초기화 하기
```
size_t fillArray(double *pArray, size_t arraySize);

vector<double> vd(maxNumDoubles);
vd.resize(fillArray(&vd[0], vd.size());
```

# string 객체를 C API를 통해 초기화 하기
```
size_t fillArray(char *pArray, size_t arraySize);

vector<char> vc(maxNumDoubles);
size_t charsWritten = fillString(&vc[0], vc.size());
string s(vc.begin(), vc.begin()+charsWritten);
```

# deque, set 객체를 C API를 통해 초기화 하기
```
size_t fillArray(double *pArray, size_t arraySize);

vector<double> vd(maxNumDoubles);
size_t charsWritten = fillString(&vc[0], vc.size());

deque<double> d(vd.begin(), vd.end());
list<double> l(vd.begin(), vd.end());
set<double> (vd.begin(), vd.end());
```

# 결론
* 데이터를 배열에 복사한 후 C API에 넘길 수도 있지만 배열 크기가 어떻게 될지 모르는 이상 배열 버퍼를 동적으로 할당하고 귀찮으니 STL을 사용하자!
