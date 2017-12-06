# 항목 16: const 멤버 함수를 스레드에 안전하게 작성하라

## 여러 개의 스레드에서 roots() 함수 호출이 일어난다면?
```cpp
class Polynomial { 
    public:  
    using RootsType = std::vector<double>;    

    RootsType roots() const { 
        if (!rootAreValid) {
            ...
            rootAreValid = true;
        }

        return rootVals;
    }

    private:
        mutable bool rootsAreValid{ false };
        mutable RootType rootVals{};
};
```
* roots 함수는 cosnt 멤버 함수로서 읽기 연산만 할 것처럼 보여서 안전해 보임
* 하지만 roots 함수 내부에서 rootsAreValid와 rootVals를 수정 할 가능성이 있어 위험

## 스레드 안전성을 위해 뮤텍스를 사용
```cpp
class Polynomial {
    public:
        using RootsType = std::vector<double>;
        
        RootsType roots() const
        {    
            std::lock_guard<std::mutex> g(m);     // 뮤텍스를 잠근다
            if (!rootsAreValid) {                 
                …                                 
                rootsAreValid = true;    
            }
            return rootVals;  
        }                                         // 뮤텍스를 푼다
                                             
    private:  
        mutable std::mutex m;
        mutable bool rootsAreValid{ false }; 
        mutable RootsType rootVals{}; 
    }; 
```
* std::mutex 형식의 객체 m을 잠그고 푸는 멤버 함수들은 비const지만 roots함수 내에서는 m이 const 객체로 간주 되기 때문에 mutable로 선언
* std::mutex를 복사하거나 이동할 수 없기 때문에 m을 Polynomial에 추가하면 Polynomial의 복사와 이동 능력도 사라진다는 점을 잊지 말 것

## 함수 호출 횟수를 세는 정도의 간단한 연산은 std::atomic 사용
```cpp
class Point {                               
    double distanceFromOrigin() const noexcept
    {                                              
        ++callCount;
        return std::sqrt((x * x) + (y * y));
    }
    
    private:  
        mutable std::atomic<unsigned> callCount{ 0 };
        double x, y;
};
```
* std::mutex처럼 복사와 이동 불가

## std::atomic의 미숙한 사용
```cpp
class Widget { 
    public:
        ...

        int magicValue1() const
        {    
            if (cacheValid) {
                return cachedValue;    
            }         
            else {      
                auto val1 = expensiveComputation1();      
                auto val2 = expensiveComputation2();      
                cachedValue = val1 + val2;               
                cacheValid = true;                       
                return cachedValue;    
            }  
        }

        int magicValue2() const  
        {    
            if (cacheValid) {
                return cachedValue;
            } else {      
                auto val1 = expensiveComputation1();      
                auto val2 = expensiveComputation2();      
                cacheValid = true;                        
                return cachedValue = val1 + val2;          
            }  
        }
 
    private:
        mutable std::atomic<bool> cacheValid{ false };  
        utable std::atomic<int> cachedValue;
    }; 
```
* 여러 스레드가 Widget:magicValue1() 함수 호출 시 cacheValid가 새로 배정 되기 이전에 접근 하게 되면 동일한 계산이 여러 번 수행 하게 되는 셈.
* 여러 스레드가 Widget:magicValue2() 함수 호출 시 cacheValid가 값이 먼저 배정되므로 cachedValue 값이 부정확 함.

## 동기화가 필요한 대상이 여럿인 경우는 뮤텍스를 이용하자
class Widget {
    public:  
    …
    int magicValue() const
    {    
        std::lock_guard<std::mutex> guard(m);
         if (cacheValid) {
             return cachedValue;
        } else {
            auto val1 = expensiveComputation1();      
            auto val2 = expensiveComputation2();      
            cachedValue = val1 + val2;      
            cacheValid = true;      
            return cachedValue;    
        }
    }                                         
    private:  
        mutable std::mutex m;  
        mutable int cachedValue;                  
        mutable bool cacheValid{ false };         
}; 

## 기억해 둘 사항들
* 동시적 문맥에서 쓰이지 않을 것이 확실한 경우가 아니라면 const 멤버 함수는 스레드에 안전하게 작성하라
* std::atomic 변수는 뮤텍스에 비해 성능상의 이점이 있지만, 하나의 변수 또는 메모리 장소를 다룰 때에만 적합하다.


