# 항목 11 : 커스텀 할당자를 제대로 사용하는 방법을 이해하자

# 기본 STL 메모리 관리자의 불편한 점

* default STL 메모리 관리자인 allocator/<T>가 형편없음 : 속도 느림, 메모리 효율 낮음, 단편화(fragmentation) 심함.
* allocator/<T>는 기본적으로 thread safety 하게 구현되어 있음 : 나는 단일 스레드에서만 돌릴건데 불필요한 검사를 하고 싶지 않으면 어떻게?
* 같은 종류 객체를 특정한 힙에 모여있게 하여 메모리 참조 위치 근접성(locality of reference)를 얻기 힘들다.
* 공유메모리를 사용하기 힘들다.
* 이런 고민을 했다면 커스텀 할당자를(custom allocator) 생각해야된다.

cont.