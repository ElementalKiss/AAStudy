# 항목 49: STL에 관련된 컴파일러 진단 메시지를 해석하는 능력을 가지자

## Example Code
```cpp
string s(10)
```

## Compile Error (VC++ 컴파일러)
```cpp
#1
error C2664: `__thiscall std::basic_string<char,struct 
std::char_traits<char>,class std::allocator<char>
>::std::basic_string<char,struct std::char_traits<char>,class 
std::allocator<char>>(const class std::allocator<char> &)` : cannot convert 
parameter 1 from `const int` to `const class std::allocator<char> &`

#2
Reason: cannot convert from `const int` to `const class std::allocator<char>`

#3
No constructor could take the source type, or constructor overload resolution was ambiguous
```

## 해석
* \#1
    * string은 클래스가 아니라, 다음을 이용하여 만든 typedef 타입이다.
    * basic_string(char, char_traits\<char>, allocator\<char>)
    * 따라서 basic_sring(char, char_traits\<char>, allocator\<char>)를 string으로 치환하자.
    ```cpp
    error C2664: `__thiscall string::string(const class
    std::allocator<char> &)` : cannot convert parameter 1 from `const int` to
    `const class std::allocator<char> &`
    ```
    * string 생성자에 넘겨진 매개 변수의 타입이 문제라는 의미로 이전보다 확실해 짐
    * 근데 좀 이상하지 않음? 할당자를 받아들이는 생성자로 연결 되었네?
    * (몇 가지 이유로...) 생성자에서 하나의 매개 변수를 전달 받으면 무조건 할당자 생성자로 컴파일 시도
    * 따라서 이 진단 메시지는 틀렸음!
* \#2 신비롭게 튀어나온 할당자 관련 오류
* \#3 생성자 호출이 잘 못 되었다는 오류 (O)

# 좀 더 심화
## Example Code
```cpp
class NiftyEmailProgram {
private:
    typedef map<string, string> NicknameMap;
    NicknameMap nicknames   // 필명을 전자메일 주소에 연관시킨 맵
public:
...
    void ShowEmailAddress(const string& nickname) const;
};

void NiftyEmailProgram::ShowEmailAddress(const string& nickname) const
{
    ...
    NicknameMap::iterator i = nicknames.find(nickname);
    if (i != nicknames.end()) ...
    ...
}
```
## Compile Error
![alt text](https://github.com/ElementalKiss/AAStudy/blob/master/effstl/book/image/item49_compile_error_msg_01.png "error msg 01")

## 해석
* 배운데로 basic_string을 string으로 치환
![alt text](https://github.com/ElementalKiss/AAStudy/blob/master/effstl/book/image/item49_compile_error_msg_02.png "error msg 02")
* std::_Tree 라이브러리 제작자들을 위한 예약어, 즉 STL 컴포넌트를 구현하는데 사용되는 내부 템플릿
* std::map\<class string,class string,struct std::less\<class string >,class std::allocator\<class string>> 맵의 정확한 타입.
* 그럼 또 치환 해야겠지? 필자는 NickNameMap으로 치환,
![alt text](https://github.com/ElementalKiss/AAStudy/blob/master/effstl/book/image/item49_compile_error_msg_03.png "error msg 03")
* 여전히 알아보기 힘드네,
* _Tree에 넘겨지는 매개 변수를 SOMETHING으로 치환,
![alt text](https://github.com/ElementalKiss/AAStudy/blob/master/effstl/book/image/item49_compile_error_msg_04.png "error msg 04")
* const_iterator를 iterator로 변환하려는 행위, 즉 const 상수성 위반입니다.
* 문제가 되는 코드 다시 살펴보면, 
```cpp
void NiftyEmailProgram::ShowEmailAddress(const string& nickname) const
{
    ...
    NicknameMap::iterator i = nicknames.find(nickname);
    if (i != nicknames.end()) ...
    ...
}
```
* nicknames는 비상수 타입의 객체지만, const_iterator를 반환하고 있네?
* 멤버 함수가 const로 선언되어 있기 때문,

## 핵심 결론
* STL 컴파일 에러에 대응하는 방법을 읽히자.
* 즉, 머리 속으로, 긴 템플릿을 짧은 기호 코드(니모닉)로 바꿔 읽을 줄 아는 공력을 키우자.

## 추가 팁
1. 에러 메시지가 포인터 타입을 언급하였다면?
    - vector와 string의 경우, 반복자는 포인터다.
    - iterator를 가지고 실수 했을 경우
2. back_insert_iterator, front_insert_iterator, insert_iterator 언급하였다면?
    - back_inserter, front_inserter, inserter를 호출할 때 실수 했을 경우
3. binder1st, binder2nd를 언급한다면?
    - bind1st, bind2nd를 호출할 때 실수 했을 경우
4. 듣도 보도 못한 대입 연산자를 언급하는 경우?
    - 출력 반복자 (ostream_iterator, ostreambuf_iterator, back_inserter, front_inserter, inserter에서 반환되는 반복자들)는 대입 연산자 (operator=)의 내부에서 출력 혹은 삽입하기 때문에 이부분에서 실수 하였는지 확인,
5. STL 알고리즘 내부가 잘 못 되었다고 언급하면?
    - 알고리즘과 함께 사용한 타입에 문제가 있다.
    - 즉, 잘못된 부류의 반복자를 넘겼다던가,
6. 컴파일러가 너가 의도하는 바를 모르겠다고 언급하면?
    - #include 안한거

