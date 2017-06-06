# 연산자 오버로딩 방식

## 1. 멤버함수의 형태로 오버로딩

```c++
class Point
{
public:
    Point(int x=0, int y=0) : xpos(x), ypos(y) {}
    Point operator + (const Point& pos) // operator+ overloading
    {
        return Point(this->xpos + pos.xpos, this->ypos + pos.ypos);
    }

private:
    int xpos;
    int ypos;
}
```

## 2. 전역함수의 형태로 오버로딩

```c++
class Point
{
public:
    Point(int x=0, int y=0) : xpos(x), ypos(y) {}
    friend ostream& operator << (ostream& os, const Point& pos)

private:
    int xpos;
    int ypos;
}

ostream& operator << (ostream& os, const Point& pos) // operator<< overloading
{
    os << pos.xpos << ", " << pos.ypos << endl;
    return os;
}
```