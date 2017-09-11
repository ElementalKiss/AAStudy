# 43. 어설프게 작성한 루프보다는 알고리즘이 더 낫다

## 알고리즘을 사용하는 이유
* 효율성
* 정확성
* 유지보수성
---
1. 효율성
    ~~~ c++
    // 루프 사용
    list<Widget> lw;
    for(list<Widget>::iterator i = lw.begin(); i!= lw.end(); ++i){
        i->redraw();
    }
    //lw.end()의 지속적인 호출  
    ~~~
    ~~~ c++
    // 알고리즘 사용
    for_each(lw.begin(), lw.end(), mem_fun_ref(&Widget::redraw));
    //
    ~~~
    lw.end()가 for처럼 지속적으로 호출되지 않는다.
---
2. 정확성  
    ~~~ c++
    //루프 사용  
    deque<double>::iterator insertLocation = d.begin();
    for(size_t i = 0; i < numDoubles; ++i){
        insertLocation = d.insert(insertLocation, data[i] + 41);
        ++insertLocation;
    }
    ~~~
    ~~~ c++
    // 알고리즘 사용
    transform(data, data + numDoubles, inserter(d, d.begin()), bind2nd(plus<int>(), 41));
    ~~~
    부주의한 반복자 관리를 방지시켜줌.
---
3. 유지보수성  
    STL이 내놓은 알고리즘만 70여개 (지금은 더 많을듯)  
    STL알고리즘의 이름 그 자체가 정보를 가지고 있다.  
    for, while 무엇을 하는지 파악하려면 내부로직을 봐야함.  
    하지만 알고리즘은 이름만으로도 파악이 됨.
---
## 결론
* for, while, do와 같은 저열한 코드대신 insert, find, for_each와 같은 세련된 코드를 써깔때 우리의 소프트웨어는 더욱 더 밝고 아름답게 변할것이다.

