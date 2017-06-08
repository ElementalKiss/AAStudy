# 항목 22: set과 multiset에 저장된 데이터 요소에 대해 키(key)를 바꾸는 일은 피하자

# map / multimap 은 key 변경이 불가능 하다 K가 T const(key), V가 T
# set / multiset 은 key 의 변경이 가능하다 k가 T (key자 value)

## set의 값이 변경 가능한 상태 이지만 stl에따라 iterator가 const로 가져오기도 함  
* (2010)에서는 const 형으로(자료 변경이 거부됨)

## 그러나 이를 회피 할수 있음 const_cast를 이용하여 상수 성질을 날려버릴 수있음
* const_set<TestClass&>(test).SetValue(valeu);

## 이것을 통해 key를 바꾸면 컨테이너가 뒤죽 박죽

## 컴파일은 되지만 값이 바뀌지 않은 코드.
* static_cast<TestClass>(*i).SetValue(value);
* ((TestClass)(*i)).SetValue(values);
* TestClass test(i); test.SetValue(value);
* 다 같은 말 복사된 객체 를 대상으로 하는 행위. 실제 값이 변경 되지 않음

## stl의 버젼에 따라 동작이 다를 수 있음으로 어떠한 버전이든 안전하게 하기 위해선 다음과 같은 순서로
* 1) 컨테이너에서 객체를 find
* 2) 객체를 복사
* 3) 컨테이너에서 기존 객체를 삭제
* 4) 복사한 객체의 내용을 변경
* 5) 복사한 객체를 컨테이너에 insert





