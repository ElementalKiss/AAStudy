# 항목 41: 이동이 저렴하고 항상 복사되는 복사 가능 매개변수에 대해서는 값 전달을 고려하라.

## 복사 되도록 만들어진 함수 매개 변수
* overloading 구현 > lvalue 인수는 복사하되 rvalue 인수는 이동
	``` cpp
	class Widget46 {
	public:
		void AddName(const std::string& newName) {
			_names.push_back(newName);
		}
		void AddName(std::string&& newName) {
			_names.push_back(std::move(newName));
		}

	private:
		std::vector<std::string> _names;
	};
	```
	* 관리를 해야 할 대상이 늘어나는 번거로움.
	* 인라인화 되지 않는다면 목적 코드(object code)에도 함수가 두 개 존재하게 되며 프로그램의 크기도 늘어남.
* 함수 템플릿을 이용하여 구현
	```cpp
	class Widget46_2 {
	public:
		template<typename T>
		void AddName(T&& newName) {
			_names.push_back(std::forward<T>(newName));
		}
	private:
		std::vector<std::string> _names;
	};
	```
	* 템플릿이기 때문에 구현이 헤더 파일에 있어야 함.
	* object code에는 이 템플릿의 서로 다른 인스턴스가 여러개 포함될 수 있음 (템플릿은 왼값/오른값, std::string, std::string으로 변환 가능한 형식들에 대해서도 인스턴스화 됨)
	* 보편 참조(universial reference)로 전달할 수 없는 인수 형식 때문에 곤란
	* 부적절한 형식의 인수 전달 시 컴파일러가 난해한 오류 메시지 출력

## 참조 전달 접근 방식의 문제는 값 전달 방식을 사용하여 해결
```cpp
class Widget {
public:
	void AddName(std::string newName) {
		_names.push_back(std::move(newName));
	}

private:
	std::vector<std::string> _names;
};
```
* std::move의 합당성
	* 보통의 경우 std::move는 오른값 참조에 쓰임
	* newName이 호출자가 넘겨준 것과는 완전히 독립적인 객체로 newName을 변경해도 호출자에게는 영향 없음
	* newName의 마지막 사용이므로 다른 대상으로 이동해도 함수의 나머지 부분에는 영향을 미치지 않음

```cpp
Widget w;
...
std::string name("Bart");
w.AddName(name); // call lvalue, 복사 생성
...
w.AddName(name + "Jenne"); //call rvalue, 이동 생성.
```
* 두 번째 호출은 std::string에 대한 operator+ 호출로부터 생성되는 std::string 객체로 초기화되기 때문에 rvalue 호출.

## 구현 방식 별 비용 분석
* overloading 구현 방식
호출자가 넘겨준 인수가 왼값이든 오른값이든 newName 참조
	* lvalue: 복사 1회.
	* rvalue: 이동 1회.
* 함수 템플릿 구현 방식
호출자가 넘겨준 인수가 왼값이든 오른값이든 newName 참조
	* lvalue: 복사 1회.
	* rvalue: 이동 1회.
	* std::string 이외의 인수가 전달 되는 상황은 제외하고 가정
* 값 전달 구현 방식
호출자가 넘겨준 인수가 왼값이든 오른값이든 newName 생성
	* lvalue: 복사 1회, 이동 1회
	* rvalue: 이동 2회.

## 값 전달 방식 사용 시 주의 할 점.
* 무조건 값 전달 방식을 사용하라는 것이 아니라 고려하라는 것.
* 복사 가능 매개변수에 대해서만 값 전달을 고려 해야 함.
	* 이동 전용 형식의 overloading 구현
	lvalue 복사는 복사 생성자를 호출하지만 이동 전용 형식에서는 복사 생성자가 비활성 되어 있으므로 rvalue 참조 버전만 구현하면 됨.
		```cpp
		class Widget {
		public:
			...
			void setPtr(std::unique_ptr<std::string>&& ptr) {
				p = std::move(ptr);
			}
		private:
			std::unique_ptr<std::string> p;
		}

		Widget w;
		...
		w.setPtr(std::make_unique<std::string>("Modern C++"));
		```
		멤버 p로 이동 시킬 때 이동 비용 1회 발생
	* 값 전달 방식 구현
		```cpp
		class Widget {
		public:
			...
			void setPtr(std::unique_ptr<std::string> ptr) {
				p = stds::move(ptr);
			}
		private:
			std::unique_ptr<std::string> p;
		}

				Widget w;
		...
		w.setPtr(std::make_unique<std::string>("Modern C++")); 
		```
		매개변수 ptr로 이동 생성 후, 멤버 p로 이동. 이동 비용 2회 발생.
* 값 전달은 이동이 저렴한 매개변수에 대해서만 고려 해야 함.
* 항상 복사되는 매개변수에 대해서만 고려 해야 함.
	``` cpp
	class Widget46 {
	public:
		void AddName(std::string&& newName) {
			if (newName.length() >= minLen && newName.length() <= maxLen) {
				_names.push_back(std::move(newName));
			}
		}

	private:
		std::vector<std::string> _names;
	};
	```
	조건에 부합하지 못해서 _names에 아무것도 추가 하지 않아도 newName의 생성/파괴 비용 발생.


	