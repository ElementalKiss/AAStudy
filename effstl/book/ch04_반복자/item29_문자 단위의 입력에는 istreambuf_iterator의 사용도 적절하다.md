# 항목 29 : 문자 단위의 입력에는 istreambuf_iterator의 사용도 적절하다.

## 상황 : 어떤 텍스트 파일을 읽어 string 객체에 복사

* STL적 생각이라음 다음과 같은 코드가 나온다.

```c++
    ifstream inputFile("input.txt");
    string fileData((istream_iterator<char>(inputFile)), istream_iterator<char>());

    cout << fileData << endl;
    // powersuperhyperelki
```

* 하지만 결과는 고통. istream_iterator는 실제 스트림 읽기를 수행할 때 opeartor >> 함수를 사용하는데 이 연산자 함수는 공백 문자를 건너 뛴다.



## 해결법1. 공백 문자를 건너뛰지 못하게 한다. 

```c++
    ifstream secondStream("input.txt");
    secondStream.unsetf(ios::skipws);
    string secondFile((istream_iterator<char>(secondStream)), istream_iterator<char>());

    cout << secondFile << endl;
    /*
        power super
        hyper elki
    */
```

* skipws 플래그를 설정해제(unset)하여 공백을 건너 뛰지 않고 바르게 값을 가져올 수 있다.
* 해결은 했지만 느리다. Why? Why? WIFI~
* operator>> 함수는 서식화 입력(formatted input)을 수행한다.
* 서식화 입력은 사용자가 해야할 자질구레한 뒷처리들을 의미 : iostream 객체를 생성하고 소멸, 동작에 영향을 미치는 스트림 플래그 체크, 읽는 도중 발생하는 에러 체크, 스트림 예외 마스크 체크 등등.
* 한 마디로 비 효율적인 작업이 너무 많다는 것이다.

## 해결2. istreambuf_iterator를 사용한다.

```C++
    ifstream thirdStream("input.txt");
    string thirdFile((istreambuf_iterator<char>(thirdStream)), istreambuf_iterator<char>());
    cout << thirdFile.c_str() << endl;
    /*
        power super
        hyper elki
    */
```

* 객체를 스트림 자체의 버퍼를 직접 건드려 다음 문자들을 바로 읽어 들인다.
* 내부 동작 설명이 있지만 그렇게 중요해보이지 않는다. 벤치 마크로 대략 40%나 빠르다고 한다.