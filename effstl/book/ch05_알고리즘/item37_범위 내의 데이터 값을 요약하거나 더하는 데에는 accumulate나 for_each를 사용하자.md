# 항목 37: 범위 내의 데이터 값을 요약하거나 더하는 데에는 accumulate나 for_each를 사용하자

## accumulate
* 내부적으로 double 타입의 변수를 사용해서 자신이 계산하는 값의 합을 저장
* 두 개의 반복자와 초기값을 받아들이는 형태
    * 입력 반복자이기만 하면 동작하므로 istream_iterators와 istream-buf_iterators 적용 가능
```
list<double> Id;
...
double sum = accumulate(Id.begin(), Id.end(), 0.0);
```
* 두 개의 반복자와 초기값, 요약용 함수를 받아들이는 형태 
```
string::size_type;
stringLengthSum(string::size_type sumSoFar, const string& s)
{
    return sumSoFar + s.size();
}

set<string> ss;
...
string::size_type lengthSum = accumulate(ss.begin(), ss.end(), 0, stringLengthSum);
```
* 나쁜 예제
    * accumulate에 넘겨지는 함수가 부가적 효과를 가지게 되므로 표준안 내용과 상이됨
    * for_each를 이용하여 해결 해보자.
```
struct Point {
    Point(double initX, double initY) : x(initX), y(initY) {}
    double x, y;
};
```
```
class PointAverage : public binary_function <Point, Point, Point> {
public:
    PointAverage() : xSum(0), ySum(0), numPoints(0) {}
    
    const Point operator()(const Point& avgSoFar, const Point& p)
    {
        ++numPoints;
        xSum += p.x;
        ySum += p.y;
        return Point(xSum / numPoints, ySum / numPoints);
    }

private:
    size_t numPoints;
    double xSum;
    double ySum;

};
```
```
std::list<Point> Ip;
...
Point avg = std::accumulate(Ip.begin(), Ip.end(), Point(0, 0), PointAverage());
```

## for_each
* 범위와 그 범위내의 요소에 대해 호출할 함수(대체 함수 객체)를 받아들이는 형태
* 알고리즘에 넘겨지는 함수는 자신이 처리 할 단 하나의 요소만을 받아 들이며, for_each는 자신의 수행을 마칠 때 해당 함수를 반환.
* for_cah에 넘겨지는 함수는 부가적 효과를 가져도 문제 없음.
```
struct Point {...};
class PointAverage : public std::unary_function <Point, void> {
public:
    PointAverage() : xSum(0), ySum(0), numPoints(0) {}

    void operator()(const Point& p)
    {
        ++numPoints;
        xSum += p.x;
        ySum += p.y;
    }
    Point result() const {
        return Point(xSum / numPoints, ySum / numPoints);
    }

private:
    size_t numPoints;
    double xSum;
    double ySum;
};

std::list<Point> Ip;
...
Point avg = std::for_each(Ip2.begin(), Ip2.end(), PointAverage2()).result();
```

## size_type
* 어떤 컨테이너의 size를 호출했을 때 반환되는 값의 타입
* Container::size_type은 size_t를 예쁘게 포장한 것
