# 항목 42: 삽입 대신 생성 삽입을 고려하라
- insert 대신 emplace를 고려하라

## 삽입 : 생성 삽입
- push_back : emplace_back
- push_front : emplace_front
- insert : emplace, emplace_hint (associative container)
- insert_after : emplace_after


## 문제 상황
- push_back과 emplace_back 비교

```c++
// vector push_back 구현부
template <class T, class Allocator = allocator<T>>
class vector {
public:
    ...
    void push_back(const T& x); // lValue insert
    void push_back(T&& x);      // rValue insert
    ...
};

// test code
std::vector<std::string> vs;	// std::string 컨테이너
vs.push_back("xyzzy");  // 문자열 리터럴
vs.push_back(std::string("xyzzy"));	// 임시 std::string 객체를 생성 전달
```

- 'vs.push_back("xyzzy");'의 타입 : const char[6] 
- vector T 타입 : std::string
- 임시 std::string 객체 생성 후 전달
 + 1. 문자열 리터럴 "xyzzy"로부터 임시 std::string 생성
 + 2. 임시 생성된 std::string이 push_back에 오른값으로 전달
   * std::vector 안에서 전달받은 오른값을 통한 복사본 생성 (이동 생성자)
 + 3. push_back 반환 후 temp 파괴 (소멸자)
- 2 단계에서 복사본 생성 시 문자열 리터럴 인자("xyzzy")를 바로 전달해 주면?
 + 1번 생성 불필요
 + 3번 소멸 불필요
 + 2번 단계인 std::vector 안에서 인자를 전달 받아 객체 1번만 생성
- 이 행동을 emplace_back이 해준다.
 + 생성자에 인자를 완벽 전달한다.

```c++
vs.emplace_back("xyzzy");  // vector 안에서 std::string("xyzzy")로 객체 생성
// 1번 생성으로 끝

// 이렇게 쓸 수 도
vs2.emplace_back(50, 'x');  // 'x' 문자 50개 인자로 전달
// vs2 size는 1, vs는 'xxxxxxxxxx.........' 값을 가짐
```

## 그럼 항상 push_back 대신 emplace_back만 사용하면 될까?
- 결론은 꼭 그렇지 않다.
- 성능의 이점을 얻기 위핸 직접 측정해봐야 한다.

```c++
std::string queenOfDisco("Donna Summer");
vs.push_back(queenOfDisco);	// vs 안에서 복사 생성
vs.emplace_back(queenOfDisco);	// 마찬가지
```
- 이론 상으론 항상 emplace_back을 쓰면 상황에 따라 빠를 것 같다.
- 얼핏 그럴 듯 해 보이지만, 이론과 실제는 다르다
- 표준 라이브러리의 현재 구현들에서는 push_back이 더 빠른 경우도 존재
 + 특정 짓긴 어렵다.
 + 전달되는 인수 타입, 컨테이너 종류, 요청된 곳, 예외 안정성, 중복 검사하는 컨테이너인지 등 다양한 요인이 영향을 미친다.
 + 따라서 정확한 성능의 이점을 얻기위핸 직접 측정!

## 개똥같지?
- 식별하는데 바람직한 발견법(heuristic)이 있음!
- 3 조건이 모두 성립한다면 항상 생성 삽입(emplace)가 성능 좋음!

### 1. 추가할 값이 컨테이너에 배정(할당,operator=) 되는 것이 아니라 생성된다.
- 새로 생성할 때만 emplace가 의미있음.
- 이미 존재하는 데이터에는 이동 배정(할당)을 행한다.
- 이동 배정(할당)을 위해선 임시 객체가 필요하다.
- 임시 객체가 존재한다는 건 이미 emplace가 아니지?
```c++
std::vector<std::string> vs;
vs.emplace(vs.begin(), "xyzzy"); // vs[0]에 이동 배정(할당) 발생할 듯
```
- 컨테이너가 데이터를 추가할 때 생성하는지, 배정(할당) 하는지 어떻게 알지?
 + 라이브러리 구현자가 결정함...
 + 개똥같지?
 + 역시 발견법 (heuristic)이 있음.
  * 노드 기반 컨테이너는 생성시 새 값 추가
  * 표준 컨테이너는 대부분 노드 기반
  * 비 노드 기반 컨테이너 : vector, deque, string (array 논외 : insert가 없음)
- 번역이 잘 못 되었던지, 저자가 실수를 했던지, 내용이 굉장히 이상함.

### 2. 추가할 인수 타입이 컨테이너 타입과 다르다.
- 타입이 달라야 임시 객체를 생성할 것이고,
- 임시 객체가 생성되어야 push_back과 emplace 상의 성능 차이가 날 것이다.
- push_back은 임시 객체를 생성할 것이고, emplace는 인자를 전달할 것이다.
- 즉 타입이 다르면 emplace는 인자전달을 함으로써 성능 상 이점을 얻을 수 있다.

### 3. 컨테이너 기존 값과의 중복 때문에 새 값을 거부할 우려가 없다.
- 중복 제한의 경우(set, map ... ) emplace는
- 새 값으로 노드 생성
- 기존 컨테이너 노드들과 비교
- 있으면 파괴 (생성 파괴 낭비)
- 없으면 노드 링크
- 즉 중복 관련 문제가 없을 경우에 emplace를 사용


## 그 외에 emplace 사용 시 고려해야 될 점 2 가지

### 1. 자원 관리 관련
```c++
void killWidget(Widget* pWidget); // 커스텀 삭제자
std::list<std::shared_ptr<Widget>> ptrs; // 자원 관리 컨테이너

// ex) push_back
ptrs.push_back(std::shared_ptr<Widget>(new Widget, killWidget));
ptrs.push_back({new Widget, killWidget});
// 둘다 임시 객체를 만듦
// 1. new Widget을 가진 std::shared_ptr<Widget> 임시 객체 생성
// 2. push_back 시 예외 발생 (노드 생성 메모리 부족)
// 3. 예외 전파 및 임시 객체 파괴 (killWidget 호출, Widget 해제)
// 임시 객체의 중요성

// ex) emplace_back
ptrs.emplace_back(new Widget, killWidget);
// 1. new Widget 생 포인터가 완벽 전달,
// 2. emplace_back 예외 발생 (노드 생성 메모리 부족)
// 3. 예외 전파 및 Widget 자원 누수
// 관리 해주는 임시 객체가 없기 때문

// 앞서 배운 자원 관리 객체를 생성하는 올바른 방법
// item 21 참고
std::shared_ptr<Widget> spw(new Widget, killWidget); // 관리 객체 생성
ptrs.push_back(std::move(spw));	// 오른값 전달
ptrs.emplace_back(std::move(spw)); // 마찬가지j
```
- 위와 같이 자원 관리 객체를 추가할 경우는 emplace 이점이 별로 없음
- 자원 관리 클래스의 유효성은 자원이 즉시 전달되어야 한다.
- emplace는 완벽전달을 통해, 자원 전달 시점이 지연된다.
- 따라서 위와 같은 문제가 발생함

### 2. explicit 생성자들과의 상호 작용 방식 관련
- emplace는 insert에선 거부 당했을 형식 변환들을 수행 할 수 있다.

```c++
// regex 생성자
explicit std::regex(const char*);

// explicit
std::regex r = nullptr	// 1, error! 컴파일 실패
std::regex r(nullptr)	// 2, 컴파일 OK, 미정의 동작

// push_back & emplace_back
std::vector<std::regex> regexes;

regexes.push_back(nullptr) // 3, error! 컴파일 실패
regexes.emplace_back(nullptr);	// 4, 컴파일 OK, 미정의 동작
```
- 1. 복사 초기화 (copy initialization)
- 2. 직접 초기화 (direct initialization)
 + explicit 생성자에서는 1은 사용할 수 없지만, 2는 사용할 수 있음.
- 3. push_back은 복사 초기화를 사용
- 4. emplace_back은 직접 초기화를 사용하여 인자를 넘기는 방식 

# 정리
- 이론적으로, emplace 들은 insert 보다 더 효율적
- 실질적으로, 더 효율적이려면 3가지 조건을 따져야 한다.
 + 1. 컨테이너에 추가할 값이 배정이 아니라 생성이어야 함
 + 2. 컨테이너에 전달할 인수 타입이 컨테이너 타입과 달라야 함
 + 3. 그 값이 중복된 값이어도 상관없는 컨테이너여야 함
- emplace를 사용할 경우 챙겨야 할 2가지 특이 상황
 + 1. 자원 관리 객체를 전달 받는지, 그렇다면 예외 상황은 점검 했는지,
 + 2. explicit 생성자로 인한 형식변환은 신경 썼는지,

# 끝
