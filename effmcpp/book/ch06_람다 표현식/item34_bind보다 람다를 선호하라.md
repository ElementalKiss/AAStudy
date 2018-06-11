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




