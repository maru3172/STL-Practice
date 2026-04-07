//-----------------------------------------------------------------------------
// STRING.h - STL의 동작을 이해하기 위한 class / std::string과 유사하게 작성
//
// 2025. 4. 10 - 시작
// 2025. 4. 14 - 이동의미론(move semantics)구현
// 2025. 5. 1 - operator==
// 2025. 5. 15 - begin(), end() 제공
// 2025. 5. 19 - rbegin(), rend() 제공 - > class 아니면 안됨
// 2025. 5. 22 - 반복자가 질문에 응답하도록 한다
// 2025. 5. 22 - begin(), end()과 반복자를 리턴하도록 한다
// 2025. 5. 26 - sort 가능하도록 필요한 연산자를 모두 코딩(C++ 연산자 오버로딩을 잘 이해해야 가능)
//-----------------------------------------------------------------------------
#pragma once
#include <memory>
#include <iterator>
#include <iostream>

class STRING_Iterator {
public:
	using difference_type = std::ptrdiff_t;
	using value_type = char;
	using pointer = char*;
	using reference = char&;
	using iterator_category = std::random_access_iterator_tag;

public:
	// 2025. 5. 26 special 함수인 default ctor를 요구
	STRING_Iterator() = default;

	STRING_Iterator(char* p) : p{ p } {}

	// 의미를 제대로 코딩해야함
	// 2025. 5. 26 수정
	STRING_Iterator& operator++() {
		++p;
		return *this;
	}

	// 2025. 5. 26 수정
	// *p <- 이 연산의 결과는 l-value가 아님, &를 리턴하고 const를 제거
	char& operator*() {
		return *p;
	}

	// cv-qualifier는 오버로딩으로 구분가능하다
	char& operator*() const {
		return *p;
	}

	// <=> 작성 후 주석처리
	// 뭐가 안되는데? 다시 살렸음
	bool operator==(const STRING_Iterator& rhs) const {
		return p == rhs.p;
	}

	// sort가 동작하도록 필요 연산자 추가
	// 2025. 5. 26
	difference_type operator-(const STRING_Iterator& rhs) const {
		return p - rhs.p;		// 연산 결과가 클래스의 불변성(invariants)을 해치나 관찰 - 멤버변수가 변경될 가능성이 있는가
	}

	// 나 자신을 리턴한다 - 나 자신& 레퍼런스
	STRING_Iterator& operator--() {
		--p;
		return *this;
	}

	// 모든 relational operation을 할 수 있게 <=>를 정의해 본다
	// <, <=, ==, !=, >=, >			<=>
	// rhs = right hand side, lhs = left hand side
	auto operator<=>(const STRING_Iterator& rhs) const {
		return p <=> rhs.p;
	}
	
	STRING_Iterator operator+(difference_type n) const {
		return p + n;
	}

	STRING_Iterator operator-(difference_type n) const {
		return p - n;
	}
private:
	// 디폴트 세팅, 메모리에 모든 비트를 초기화 - = nullptr
	char* p{};
};

// iterator adaptor - 반복자의 인터페이스 제공, 그러나 실제는 다르게 동작
// 표준 반복자가 제공해야할 5가지 타입을 제공
class STRING_Reverse_Iterator {
public:
	using difference_type = std::ptrdiff_t; // 포인터 간의 거리를 정의
	using value_type = char;
	using pointer = char*;
	using reference = char&;
	using iterator_category = std::random_access_iterator_tag;

public:
	explicit STRING_Reverse_Iterator(char* p) : p{ p } { } // explicit - 인자로부터 제멋대로 생성자 변환을 막는다.

	// 반복자라면 제공해야 할 기본 동작
	void operator++() {
		--p;
	}

	char operator*() const {
		return *(p - 1);
	}

	// <=> spaceship operator - 삼방향 비교 연산자, 모든 비교가 다 해결되는, 3 ways operator
	bool operator==(const STRING_Reverse_Iterator& rhs) const {
		return p == rhs.p;
	}
private:
	char* p;
};

class STRING {
public:
	STRING(); // 진짜 special 함수
	~STRING(); // <- 할 이유 없음
	STRING(const char* str);

	//복사생성자와 복사할당연산자
	STRING(const STRING&);
	STRING& operator=(const STRING&);

	//이동생성자와 이동할당연산자
	STRING(STRING&&);
	STRING& operator=(STRING&&);

	// 기본정렬을 위한 < 2025. 4. 14
	// 사전식 정렬하도록 수정 - 2025. 5. 15
	bool operator<(const STRING& rhs) const;
	// 같은 객체인지 비교 == - 2025. 5. 1
	bool operator==(const STRING& rhs) const;

	size_t size() const;

	// 2025. 5. 15
	STRING_Iterator begin() const;
	STRING_Iterator end() const;

	// 2025. 5. 19
	STRING_Reverse_Iterator rbegin() const;
	STRING_Reverse_Iterator rend() const;
// private:
public:
	size_t num{};
	std::unique_ptr<char[]> p{};

	size_t id;			// 2025. 4. 10 관찰을 위한 객체 고유 번호

	friend std::ostream& operator<<(std::ostream& os, const STRING& s);
	friend std::istream& operator>>(std::istream&, STRING&);

	static size_t gid;	// 2025. 4. 10 고유 번호 생성
};
