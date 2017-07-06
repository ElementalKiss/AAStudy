# 항목 28: reverse_iterator에 대응되는 기점 반복자(base iterator)를 사용하는 방법을 정확하게 이해하자

# 
![alt text](https://github.com/ElementalKiss/AAStudy/blob/master/effstl/book/image/item28_img1.jpg "1")
# 5개의 값을 설정한 메모리 내용 및 iterator debug 내용 
# reverse_iterator base멤버 함수를 호출 하면 iterator를 얻을수 있음
# reverse_iterator(ri) ,iterator(i) 의 값을 봐보자 가르키는 위치가 다르다

#
![alt text](https://github.com/ElementalKiss/AAStudy/blob/master/effstl/book/image/item28_img3.png "3")
# insert의 경우 3의 뒤에 값이 들어감 으로 이치에 맞는다고 봄

#
![alt text](https://github.com/ElementalKiss/AAStudy/blob/master/effstl/book/image/item28_img2.png "2")
# delete의 경우 i사용 하여 삭제 할 경우 내용은 4가 되며 이를 보정 하기 위한 코드의 예시
# 1) 본래 vector이외의 자료 구조에서는 컴파일이 된다고 한다 (하지만 vector도 된다)
# 2) 책에서는 두번쨰의 코드를 추천한다

#맺음
# reverse_iterator의 base를 호출 한다고 꼭 이에 대응하는 iterator가 나오지 않는다
# 요소 삽입이 목적인 경우는 맞지만 삭제가 목적인 경우 그렇지 않다
# reverse_iterator의 base를 호출 할때 그 반복자로 무엇을 할 것인 정확하게 계획 하고 있어야 한다



