# 항목 29 : 문자 단위의 입력에는 istreambuf_iterator의 사용도 적절하다.

```c++
    ifstream inputFile("input.txt");
    string fileData((istream_iterator<char>(inputFile)), istream_iterator<char>());

    cout << fileData << endl;
    // powersuperhyperelki
```



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



```C++
    ifstream thirdStream("input.txt");
    string thirdFile((istreambuf_iterator<char>(thirdStream)), istreambuf_iterator<char>());
    cout << thirdFile.c_str() << endl;
    /*
        power super
        hyper elki
    */
```



전체적 주제는 istreambuf_iterator를 써라

