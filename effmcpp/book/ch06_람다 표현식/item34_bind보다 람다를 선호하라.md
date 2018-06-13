# 항목 34: std::bind보다 람다를 선호하라.

* std::bind가 돌려준 함수 객체는 바인드 객체라고 칭함.
* 예제
  * 경보를 설정하는 함수 (특정 지점부터 한 시간 이후에 30초간 소리를 냄)
    * lambda
      ```cpp
      using Time = std::chrono::steady_clock::time_point;
      enum class Sound { Beep, Siren, Whistle };
      using Duration = std::chrono::steady_clock::duration;

      void setAlarm(Time t, Sound s, Duration d);

      // c++11
      auto setSoundL = [](Sound s) {
          using namespace std::chrono;
          setAlarm(steady_clock::now() + hours(1), s, seconds(30));
      };

      // c++14
      auto setSoundL = [](Sound s) {
          using namespace std::chrono;
          using namespace std::literals;

          setAlarm(steady_clock::now() + 1h, s, 30s);
      };
      ```
    * std::bind
      ```cpp
      using namespace std::chrono;
      using namespace std::literals;
      using namespace std::placeholders;

      auto setBoudnB = std::bind(setAlarm, steady_clock::now() + 1h, _1, 30s);
      ```
      1. bind 인자값 중 하나를 함수 객체 생성 시점이 아닌 호출 시점에 정하고 싶은 경우 std::placeholders 이용하여 해결. 예제의 경우 _1 자리에 대체.
      2. std::bind 호출에서 인수 "steady_clock::now() + 1h"는 setAlarm이 아니라 std::bind로 전달되며 해당 표현식이 평가되어서 나오는 시간이 std::bind가 생성한 바인드 객체에 저장됨.
      즉, std::bind가 호출되고 한 시간이 지난 이후에 경보가 발생하게 됨.

      ```cpp
      // c++11
      auto setBoudnB = std::bind(setAlarm, 
                                 std::bind(std::plus<steady_clock::time_point>(), 
                                           std::bind(steady_clock::now()), 1h),
                                 _1, 30s);
      // c++14
      auto setBoudnB = std::bind(setAlarm, 
                                 std::bind(std::plus<>(), 
                                           std::bind(steady_clock::now()), 1h),
                                 _1, 30s);
      ```
      위의 문제를 해결하기 위해서 std::bind를 이용하여 호출 시점에 바인드 객체를 통해 시간을 계산하도록 변경.
  * 경보 설정 함수를 오버로딩 했을 때
    ```cpp
      void setAlarm(Time t, Sound s, Duration d, Volumne v);
    ```
    * lambda는 문제 없이 동작
      또한, SetSoundL에 대한 함수 호출 연산자 안에서 SetAlarm 호출은 컴파일러가 통상적으로 인라인화 할 수 있는 함수 호출임.
    * std::bind는 문제 발생
      컴파일러 입장에서는 어떤 함수를 std::bind로 넘겨야 할 지 알 수 없음.
      ```
      1>c:\users\heome\source\repos\consoleapplication2\consoleapplication2\emcpp_36.cpp(38): error C2672: 'bind': 일치하는 오버로드된 함수가 없습니다.
      1>c:\users\heome\source\repos\consoleapplication2\consoleapplication2\emcpp_36.cpp(38): error C2783: 'std::_Binder<std::_Unforced,_Fx,_Types...> std::bind(_Fx &&,_Types &&...)': '_Fx'의 템플릿 인수를 추론할 수 없습니다.
      1>c:\program files (x86)\microsoft visual studio\2017\community\vc\tools\msvc\14.12.25827\include\functional(1092): note: 'std::bind' 선언을 참조하십시오.
      ```
      컴파일이 가능하게 하려면 setAlarm을 적절한 함수 포인터 형식으로 캐스팅 해야함.
      ```cpp
    	using SetAlarm3ParamType = void(*)(Time t, Sound s, Duration d);
	    auto setBoudnB2 = std::bind(static_cast<SetAlarm3ParamType>(setAlarm),
								                  std::bind(std::plus<>(),
										                        std::bind(steady_clock::now()), 1h), _1, 30s);
      ```
      함수 포인터를 통한 함수 호출은 컴파일러가 인라인화할 가능성이 낮은데 std::bind 호출 역시 setAlarm을 가리키는 함수 포인터를 전달하기 때문에 인라인화될 가능성이 낮음.
    * 주어진 인수가 최솟값과 최댓값 사이에 있는지 여부를 돌려주는 함수
      * lambda
        ```cpp
        // c++11
        auto betweenL = [lowVal, highVal](const int& val) {
          return lowVal <= val && val <= highVal;
        };

        // c++14
        auto betweenL2 = [lowVal, highVal](const auto& val) {
          return lowVal <= val && val <= highVal;
        };
        ```
      * std::bind
        ```cpp
        using namespace std::placeholders;
        // c++11
        auto betweenB = std::bind(std::logical_and<bool>(),
                                  std::bind(std::less_equal<int>(), lowVal, _1),
                                  std::bind(std::less_equal<int>(), _1, highVal));

        // c++14
        auto betweenB2 = std::bind(std::logical_and<>(),
                                   std::bind(std::less_equal<>(), lowVal, _1),
                                   std::bind(std::less_equal<>(), _1, highVal));
        ```
        1. std::logical_and는 두 개의 함수 객체를 받아 리턴 값을 && 연산 후 bool값 리턴
        2. std::less_equip는 두 개의 인자값을 전달 받아 <= 로 비교 후 bool값 리턴
    * Widget 객체의 압축 복사본을 만드는 함수
      * lambda
        ```cpp
        Widget w;

        auto compressRateL = [w](CompLevel lev) {
          return compress(w, lev);
        }

        compressRateL(CompLevel::High);
        ```
        값의 갈무리 방식과 매개 변수 전달 방식이 명백함. (두 개 다 값으로 전달)
      * std::bind
        ```cpp
        auto compressRateB = std::bind(compress, w, _1);
        auto compressRateB = std::bind(compress, std::ref(w), _1);
        ```
        1. std::bind는 주어진 인수들을 항상 복사하기 때문에 값으로 전달됨
        2. std::ref를 통해 참조로 전달 가능.
      * 바인드 객체
        ```cpp
        compressRateB(CompLevel::High);
        ```
        바인드 객체에 전달되는 모든 임수는 참조로 전달 되므로 참조 전달 됨.
        (바인드 객체의 함수 호출 연산자가 완벽 전달을 사용하기 때문)
  * C++14에서의 std::bind 사용은 합당하지 않으나 c++11에서는 제한된 두 가지 경우에서는 합당함.
    * 이동 갈무리(move capture)
      c++11 람다는 이동 갈무리를 지원하지 않는데 람다와 std::bind 조합을 통해 이동 갈무리를 흉내내는 것은 가능
    * 다형적 함수 객체(polymorphic function object)
      객체를 템플릿화된 함수 호출 연산자와 묶으려 할 때 유용.
      ```cpp
      class PolyWidget {
      public:
        template<typename T>
        void operator()(const T& param) const;
      };

      PolyWidget pw;
      auto boundPW = std::bind(pw, _1);

      boundPW(1930);
      boundPW(nullptr);
      boundPW("RoseBud");
      ```
      c++11의 람다로는 불가능하지만 c++14의 람다에서는 가볍게 해결!
      ```cpp
      auto boundPW = [pw](const auto& parma) {
          pw(param);
      }
      ```
  * 기억해 둘 사항들
    * std::bind를 사용하는 것보다 람다가 더 읽기 쉽고 표현력지 호다. 그리고 더 효율적일 수 있다.
    * c++14가 아닌 c++11에서는 이동 갈무리를 구현하거나 객체를 템플릿화한 함수 호출 연산자에 묶으려 할 때 std:;bind가 유용할 수 있다.

        





