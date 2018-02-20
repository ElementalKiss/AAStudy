# 항목 26: forwarding reference에 대한 overloading을 피하라

## 작성하려는 함수

* 매개변수 : const std::string& name
* 함수 내용 : log(now), names.emplace(name)

```
std::multiset<std::string> names;

void LogAndAdd(const std::string& name)
{
    auto now = std::chrono::system_clock::now();
    log(now);
    names.emplace(name);
}
```

## LogAndAdd 함수의 비효율성

```
std::string myPetName("SerKey");
LogAndAdd(myPetName); // case 1: lvalue 전달
LogAndAdd(std::string("SerKey")); // case 2: rvalue 전달
LogAndAdd("SerKey"); // case 3: literal value 전달
```

### case 1

* lvalue 전달.
* 매개변수 name은 변수 myPetName에 binding.
* names.emplace로 전달되는데 lvalue이므로 복사는 불가피.

### case 2

* rvalue 전달.
* 매개변수 name은 std::string("SerKey")로 생성된 rvalue로 binding 된다.
* 하지만 name 자체는 lvalue라서 names에 복사를 하게 된다.
* **중요) 이동을 한다면 복사 비용을 줄일 수 있다.**

### case 3

* rvalue 전달.
* 매개변수 name은 literal value로 생성된 임시 std::string 객체에 binding.
* case 2처럼 name은 names에 복사.
* **중요) 애초에 전달 된 인수가 문자열 literal이므로 std::string 객체 생성 없이 바로 names.emplace에 전달하면 복사는 커녕 이동 비용조차 필요 없다.**

## 개선된 LogAndAdd

to be continue....

## 정리

* forwarding reference에 대한 overloading으로 다른 함수를 만들어도 거의 forwarding reference 버전이 불리는 경우가 많다.
* perfect forwarding 생성자 구현에 문제가 많다. 이유는 좀 더 읽어보고...