# 항목 46: 알고리즘의 매개 변수로는 함수 대신 함수 객체가 괜찮다

# 서론 
* 변수를 가진 클레스를 조작 하는 것이 직접 변수를 조작 하는것 보다 느리다.
* 가상함수 테이블 call 비용 이 생각 보다 비싸다. 등등을 찾아볼수 있음
* 함수객체가 함수를 넘기는 것 보다 빠르다 (금번 주제)
* http://egloos.zum.com/minjang/v/1973793

# 어쩌서? 추상화로인한 손실을 보는데도 빠를 수 있는가?
* 컴파일러로부터 템플릿 인스턴스화 과정에서 inlining의 효과를 본다
* 함수에 inline를 선언후 함수를 전달 하게 되면 이는 함수 포인터로 전달된다
* 인라인 확인법  http://blog.naver.com/sandal0394/220505141338

# 인스턴스 결과..
~~~
inline
bool doubleGrater(double d1, double d2)
{
    return d1 > d2;
}

void sort(v.begin(), v.end(), doubleGrater);

-- 는 아래와 같다
void sort(vector<double>::iterator first,
          vector<double>::iterator last, 
          bool (*comp*)(double, double));

~~~
# STL 플랫폼이 const 멤버 함수(string::size)를 처리 하는데 버그를 가지고 있다
* 근데 실제로는 컴파일 이되버려...
* 모호 성에 관해서 말 하는데 잘 모르겠다
* mem_fun_ref(&std::string::size) 쓰면 인라인 화되지 않으니 차라리 호환성을 위해 함수자로. 
~~~
    std::set<std::string> s;
  
    std::transform(s.begin(), s.end(), 
        std::ostream_iterator<std::string::size_type>(std::cout, "\n"),
        mem_fun_ref(&std::string::size));

    struct StringSize : public std::unary_function<std::string, std::string::size_type> {
        std::string::size_type operator()(const std::string& s) const
        {
            return s.size();
        }
    };

    std::transform(s.begin(), s.end(), 
        std::ostream_iterator<std::string::size_type>(std::cout, "\n"), 
        StringSize());
~~~

# 컴파일러가 인스턴스화에 모호 한 경우
* 예도 컴파일러(2017) 가 받아들임...
* 표현식이 애매해서 템플릿을 인스턴스 화하지 못하는 케이스가 포함된 코드라고 하는데
* 이게 애매 하다라.. avarage<typename std::iterator_traits<inputIter1>::value_type>

~~~
template<typename FPType>
FPType avarage(FPType val1, FPType val2)
{
    return (val1 + val2) / 2;
}

template<typename inputIter1, 
         typename inputIter2>
void writeAvarages(inputIter1 begin1, 
                   inputIter1 end1, 
                   inputIter2 begin2, 
                   std::ostream& s)
{
    std::transform(
        begin1, end1, begin2, 
        std::ostream_iterator<typename std::iterator_traits<inputIter1>::value_type>(s, "\n"),
        avarage<typename std::iterator_traits<inputIter1>::value_type> //에러 안남
    );
}
~~~

# 그래서 이렇게 고치라고 함
* transform 안에 호출되는 Avarage::operator() inline 대상
* 기존 avarage는 함수 템플릿 이라서... 

~~~
template<typename FPType>
FPType avarage(FPType val1, FPType val2)
{
    return (val1 + val2) / 2;
}

template<typename FPType>
struct Avarage : public std::binary_function<FPType, FPType, FPType> {
    FPType operator()(FPType val1, FPType val2) const
    {
        return avarage(val1, val2);
    }
};

template<typename inputIter1, 
         typename inputIter2>
void writeAvarages(inputIter1 begin1, 
                   inputIter1 end1, 
                   inputIter2 begin2, 
                   std::ostream& s)
{
    std::transform(
        begin1, end1, begin2, 
        std::ostream_iterator<typename std::iterator_traits<inputIter1>::value_type>(s, "\n"),
        Avarage<typename std::iterator_traits<inputIter1>::value_type>()
    );
}
~~~~

# 결론 
* 알고리즘 매개변수로 함수 객체가 딱!
* 컴파일러 환경에도 견고 하게 대응 함
* 어셈을 까봐야 할판임... 