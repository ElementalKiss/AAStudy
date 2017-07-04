# 항목 28: reverse_iterator에 대응되는 기점 반복자(base iterator)를 사용하는 방법을 정확하게 이해하자

# reverse_iterator base멤버 함수를 호출 하면 iterator를 얻을수 있음

# iterator를 설정하는 코드 & 요소값
![alt text](https://github.com/ElementalKiss/AAStudy/blob/master/effstl/book/image/item28_img1.jpg "1")

# reverse_iterator 를 iterator로 변환하면 서로 가르 키는 위치가 다르다 그래서 insert나 erase의 위치가 다르다 ri 는 i가 아니다 라고 설명함

![alt text](https://github.com/ElementalKiss/AAStudy/blob/master/effstl/book/image/item28_img2.png "2")