# 항목 34: 정렬된 범위에 대해 동작하는 알고리즘이 어떤 것들인지 파악해 두자

* 기본적으로 정렬된 데이터를 넘겨 동작하는 알고리즘들이 있다는 이야기.

## binary_search, lower_bound, upper_bound, equal_range

* 반드시 정렬된 데이터.
* 내부적으로 이진 탐색을 사용해 값을 찾는다.

## set_union, set_intersection, set_difference, set_symmetric_difference

* 기본적으로 선형시간으로 동작.
* 정렬하지 않은 데이터는 터무니 없이 느리다.

## merge, inplace_merge

* 두 정렬된 범위를 받아 합치는 알고리즘으로 하나라도 정렬되지 않으면 동작하지 않는다.

## includes

* 어떤 범위 안에 값이 있는지 여부를 알아보는 알고리즘. 정렬되어 있음을 가정하고 동작한다.
* 기본적으로 선형시간이지만 정렬되어 있지 않다면 느리다.

## unique, unique_copy

* 표준안 : 연속으로 이어진 동일한 요소들 중에서 첫 번째 것만을 남기고 다 없앤다.
* 그래서 정렬해야한다.

