# 프록시 객체란?

- 말그대로 다른 무언가와 이어지는 인터페이스의 역할을 함
- 핵심 기능의 실행은 다른 객체에 위임하고 부가적인 기능을 제공하는 객체
- 여러 객체에 공통으로 적용될 수 있는 기능을 구현

# 문제 상황

- **1 또는 0** 만을 array에 저장하고 싶다고 가정해보자.

```c++
struct wrongArray {
    int wrgArr[10];
    int& operator[](int i) {
        return wrgArr[i];
    }
};
```

- 이는 문제가 있다. 왜냐하면 `arr[1] = 42` 이런 사용을 막을 수가 없다...

# 프록시 객체를 사용하여 해결

```c++
#include <iostream>

struct aProxy {
    aProxy(int& r) : mPtr(&r) {}

    void operator = (int n) {
        if (n > 1) {
            throw "not binary digit";
        }
        *mPtr = n;
    }

private:
    int* mPtr;
};

struct array {
    int arr[10];
    aProxy operator[](int i) {
        return aProxy(arr[i]);
    }
};

int main() {
    try {
        array a;
        a[0] = 1;   // ok
        a[0] = 42;  // throws exception
    }
    catch (const char* e) {
        std::cout << e << std::endl;
    }
}
```

- `array`의 리턴값이 프록시 객체인 `aProxy`를 경유해서 error handling이 가능해졌다