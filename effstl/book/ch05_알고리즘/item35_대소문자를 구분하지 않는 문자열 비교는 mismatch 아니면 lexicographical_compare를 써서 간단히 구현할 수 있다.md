# 항목 35: 대소문자를 구분하지 않는 문자열 비교는 mismatch 아니면 lexicographical_compare를 써서 간단히 구현할 수 있다

## 어떡하면 대소문자를 구분하지 않는 문자열 비교를 할 수 있을까?
* strcmp류의 정도만 원한다면 간단하지만, strcmp가 넘보기 힘든 언어별 문자열(영어,한글 외의 문자)의 처리까지 가능하게 하려면 어렵다.

## 이번 항목에서는 STL로 "쉽게 할 수 있는 정도"만 다룬다.
* 국제화 문제는 고려하지 않는다.
* 어려운 방법은 STL의 영역을 넘으며, 로케일을 다루는 방법을 익혀야 한다.(부록 A에 방법이 실려있다.)

## 인터페이스 별로 두 가지 방법을 본다
* 한 가지는 strcmp와 비슷한 인터페이스(음수, 0, 양수를 반환)
* 또 하나는 operator< 비슷한 인터페이스(true 또는 false 반환)

## 우선 두 개의 문자가 "대소문자의 여부만 제외하고" 같은지를 알아내는 방법
```
int ciCharCompare(char c1, char c2)
{
    int lc1 = tolower(static_cast<unsigned char>(c1));
    int lc2 = tolower(static_cast<unsigned char>(c2));

    if (lc1 < lc2) return -1;
    if (lc1 > lc2) return 1;
    return 0;
}
```
* 음수, 0, 양수를 반환하는 strcmp의 형식을 따른다.
* char 가 부호가 있어도 tolower를 호출하기 전에 unsigned char로 캐스팅하면 된다.

## 다음은 문자열 비교 함수

```
int ciStringCompare(const string& s1, const string& s2)
{
    if (s1.size() <= s2.size()) return ciStringCompareImpl(s1, s2);
    else return -ciStringCompareImpl(s2, s1);
}
```
* 두 문자열의 관계에 따라 음수, 0, 양수를 반환
* 짧은 문자열 쪽을 첫째 매개 변수로 넘기는 것이 전제 조건

```
int ciStringCompareImpl(const string& s1, const string& s2)
{
    typedef pair<string::const_iterator, string::const_iterator> PSCI;
    PSCI p = mismatch(s1.begin(), s1.end(), s2.begin(), not2(ptr_fun(ciCharCompare)));

    if (p.first == s1.end()) {                  // true 이면, s1과 s2가 같던지
        if (p.second == s2.end()) return 0;     // s1이 s2보다 길이가 짧던지..
        else return -1;
    }
    return ciCharCompare(*p.first, *p.second);
}
```
* mismatch 알고리즘을 사용하며, mismatch는 두 개의 범위를 받아 서로 대응되는 값이 달라지기 시작하는 첫 위치를 찾아 준다(범위마다 하나씩 두 개).

* 두 문자열이 어긋나는 첫 위치만 알 수 있으면 어떤 문자열이 다른 문자열보다 앞서는지를 결정하는 일은 무척 쉽다.

* not2(ptr_fun(ciCharCompare)) 이 구문은 두 개의 문자가 일치하면 true를 반환해 주어야 한다.
* 왜냐하면 mismatch는 자기가 사용하는 구문이 false를 반환하면 즉시 중단한다.

* ptr_fun이 필요한 이유는 항목 41에서..

## 두 번째 방법은 STL 의 규정에 맞춘 술어 구문을 만들어, 이것을 연관 컨테이너의 비교 함수처럼 사용하는 것이다.

```
bool ciCharLess(char c1, char c2)
{
    return tolower(static_cast<unsigned char>(c1)) < tolower(static_cast<unsigned char>(c2));
}
```

```
bool ciStringCompare2(const string& s1, const string& s2)
{
    return lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end(), ciCharLess);
}
```
* lexicographical_compare 는 쉽게 말해 strcmp를 일반화한 알고리즘으로 두 요소의 대소 관계를 정할 수 있는 기준을 만들어 넣을 수 있다.
* s1과 s2가 어극나는 첫 번째 위치를 찾는다.
* 두 문자열이 동일한 값을 가지면 false를 반환한다.
* (앞 요소와 뒷 요소를 번갈아 가면서 전달)

## 결론(?)

* 필자가 각별히 신경쓰는 부분이 이식성이지만, 이번 항목에서 알아본 함수와 비슷한 것들이 이미 표준 C 라이브러리 등의 비표준 확장 함수로 만들어져 있고, 폭넓게 사용되고 있다.

```
int ciStringCompare(const string& s1, const string& s2)
{
    return stricmp(s1.c_str(), s2.c_str()); // or strcmpi
}
```
* stricmp, strcmpi 는 오직 문자열 비교만을 하도록 최적화 된 함수이기 때문에 mismatch나 lexicographical_compare 등의 범용 알고리즘보다 긴 문자열 처리에서 월등히 빠르다.