# more study: return auto

```c++
// ConsoleApplication1.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.

#include "stdafx.h"
#include <iostream>

using namespace std;

int main()
{
  int a = 0;

  auto ReturnReference = [&a]() -> auto& {
    return a;
  };

  auto ReturnPointer = [&a]() -> auto* {
    return &a;
  };

  auto ReturnValue = [&a]() -> auto {
    return a;
  };

  auto& r1 = ReturnReference();
  cout << "r1: " << r1 << "\n";
  r1 = 10;
  cout << "r1: " << r1 << "\n";
  cout << "---------------" << "\n";

  auto r2 = ReturnReference(); // copy
  cout << "r2: " << r2 << "\n";
  r2 = 20;
  cout << "r2: " << r2 << "\n";
  cout << "---------------" << "\n"; 

  auto&& r3 = ReturnReference();
  cout << "r3: " << r3 << "\n";
  r3 = 30;
  cout << "r3: " << r3 << "\n";
  cout << "---------------" << "\n";

  auto* p1 = ReturnPointer();
  cout << "p1: " << *p1 << "\n";
  *p1 = 100;
  cout << "p1: " << *p1 << "\n";
  cout << "---------------" << "\n";

  auto p2 = ReturnPointer();
  cout << "p2: " << *p2 << "\n";
  *p2 = 200;
  cout << "p2: " << *p2 << "\n";
  cout << "---------------" << "\n";

  auto&& p3 = ReturnPointer();
  cout << "p3: " << *p3 << "\n";
  *p3 = 300;
  cout << "p3: " << *p3 << "\n";
  cout << "---------------" << "\n";

  auto&& v1 = ReturnValue();
  cout << "v1: " << v1 << "\n";
  v1 = 1000;
  cout << "v1: " << v1 << "\n";
  cout << "---------------" << "\n";

/*
r1: 0
r1: 10
---------------
r2: 10
r2: 20
---------------
r3: 10
r3: 30
---------------
p1: 30
p1: 100
---------------
p2: 100
p2: 200
---------------
p3: 200
p3: 300
---------------
v1: 300
v1: 1000
---------------
*/

  return 0;
}
```

# 정리
* return auto& > auto& : 레퍼런스 접근
* return auto& > auto : 값이 복사됨
* return auto* > auto* : 포인터 접근
* return auto* > auto : 포인터 접근
