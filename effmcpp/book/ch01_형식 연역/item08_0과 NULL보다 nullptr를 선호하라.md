# 항목 8: 0과 NULL보다 nullptr를 선호하라

* 요점: 0은 int, 포인터가 아니다.
* NULL도 마찬가지. 컴파일러에 따라 다르게 부여될 수 있다. 0과 마찬가지로 포인터가 아니다.

## NULL의 불확실성 예시

```cpp
// func 오버로딩 함수 정의
void func(int) { cout << "func(int)" << endl; }
void func(bool) { cout << "func(bool)" << endl; }
void func(void*) { cout << "func(void*)" << endl; }

void main()
{
    func(0);        // func(void*)가 아니라 func(int)를 호출
    func(NULL);     // 컴파일 되지 않을 수 있지만 보통은 func(int)를 호출.
    func(nullptr);  // func(void*) 호출

    /*
    func(int)
    func(int)
    func(void*)
    */
}
```

* func(NULL)의 불확실성은 NULL의 구체적인 타입을 구현의 재량에 맡긴 결과이다.
* null pointer의 의미로 func(NULL)을 호출하면 문제가 될 수 있다.

## nullptr

* 정수 타입이 아니다.
* nullptr의 실제 타입은 std::nullptr_t이고 이는 다시 nullptr의 타입으로 정의되는 순환 정의.
* std::nullptr_t는 모든 raw pointer 타입으로 암묵적으로 변환되어 **모든 타입의 포인터**처럼 행동한다.
* 코드 명확도 향상. if(result == nullptr)

## nullptr과 템플릿

```cpp
// mutex 활용
// power coding
int mufunc(shared_ptr<int> spw) { return 0; }
double mufunc2(shared_ptr<double> upw) { return 0.0f; }
bool mufunc3(int* pw) { return true; }

mutex mu1, mu2, mu3;
using MuxGuard = lock_guard<mutex>;

// main
    {
        MuxGuard g(mu1);
        auto result = mufunc(0);
    }
    {
        MuxGuard g(mu2);
        auto result = mufunc2(NULL);
    }
    {
        MuxGuard g(mu3);
        auto result = mufunc3(nullptr);
    }
```

* 이 코드를 템플릿으로 만들면?

```cpp
// template
template<class FuncType, class MuxType, class PtrType>
auto lockAndCall(FuncType func, MuxType& mutex, PtrType ptr) -> decltype(func(ptr))
{
    using MuxGuard = lock_guard<MuxType>;
    MuxGuard g(mutex);
    return func(ptr);
}

// main
    auto result1 = lockAndCall(mufunc, mu1, 0);
    auto result2 = lockAndCall(mufunc2, mu2, NULL);
    /*
    심각도	코드	설명	프로젝트	파일	줄	비표시 오류(Suppression) 상태
    오류	C2672	'lockAndCall': 일치하는 오버로드된 함수가 없습니다.	ConsoleApplication2	c:\work\test\consoleapplication2\consoleapplication2\소스.cpp	52	
    오류	C2893	'unknown-type lockAndCall(FuncType,MuxType &,PtrType)' 함수 템플릿을 특수화하지 못했습니다.	ConsoleApplication2	c:\work\test\consoleapplication2\consoleapplication2\소스.cpp	52	
    오류	C2672	'lockAndCall': 일치하는 오버로드된 함수가 없습니다.	ConsoleApplication2	c:\work\test\consoleapplication2\consoleapplication2\소스.cpp	53	
    오류	C2893	'unknown-type lockAndCall(FuncType,MuxType &,PtrType)' 함수 템플릿을 특수화하지 못했습니다.	ConsoleApplication2	c:\work\test\consoleapplication2\consoleapplication2\소스.cpp	53	
    */

    auto result3 = lockAndCall(mufunc3, mu3, nullptr);
```

* 0과 NULL에 대한 템플릿 타입추론은 int로 된다.
* mufunc에서 요구하는 타입은 shared_ptr<int>인데 int가 절달되어 컴파일 에러가 난다.

## 기억해 둘 사항들

* 0과 NULL보다 nullptr을 선호하라.
* 정수 타입과 포인터 타입에 대한 오버로딩을 피하라.