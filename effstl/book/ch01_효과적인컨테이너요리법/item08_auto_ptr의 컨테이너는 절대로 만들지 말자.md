# 항목 8 : auto_ptr의 컨테이너는 절대로 만들지 말자

# COAP(Container of Auto_Ptr)은 절대 금지
* 메모리 누수를 해결하는 간단하고 효율적인 해결책으로 몇몇 프로그래머들은 찬양하지만 이식이 불가능 하기 때문에 사용을 권장하지 않음.

# auto_ptr
* auto_ptr 하나를 복사하면 auto_ptr이 가르키는 객체의 소유권은 복사하는 auto_ptr로 옮겨지고 복사되는 auto_ptr은 NULL로 세팅됨.
* auto_ptr을 복사하는 것은 '그 포인터의 값을 바꾸는 것'