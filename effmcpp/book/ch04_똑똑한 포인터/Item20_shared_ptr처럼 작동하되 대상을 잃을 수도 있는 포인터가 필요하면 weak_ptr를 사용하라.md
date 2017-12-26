# 항목 20: shared_ptr처럼 작동하되 대상을 잃을 수도 있는 포인터가 필요하면 weak_ptr를 사용하라



## 정리

* std::shared_ptr처럼 작동하되 대상을 잃을 수도 있는 포인터가 필요하면 std::weak_ptr를 사용하라.
* std::weak_ptr의 잠재적인 용도로는 캐싱, 관찰자 목록, 그리고 std::shared_ptr 순환 고리 방지가 있다.