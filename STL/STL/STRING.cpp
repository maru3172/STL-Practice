//-----------------------------------------------------------------------------
// STRING.cpp - STL의 동작을 이해하기 위한 class / std::string과 유사하게 작성
//
// 2025. 4. 10 - 시작
// 2025. 4. 10 - 복사생성/복사할당연산자, 스페셜 함수의 동작 관찰
// 2025. 4. 14 - 선택적 관찰하도록 전역변수를 사용
// 2025. 4. 14 - 이동의미론(move semantics)구현
// 2025. 5. 1 - operator==
// 2025. 5. 15 - std::string과 같이 사전식 정렬하도록 수정
// 2025. 5. 15 - begin(), end()
// 2025. 5. 19 - rbegin(), rend() 제공
// 2025. 5. 22 - 반복자가 질문에 응답하도록 한다
// 2025. 5. 22 - begin(), end()과 반복자를 리턴하도록 한다
//-----------------------------------------------------------------------------
#include <memory>
#include <print>
#include <algorithm>
#include "STRING.h"

size_t STRING::gid{ 0 };
bool observe{ false };

STRING::STRING()
	: id{ ++gid }
{
	if (observe) {
		std::println("[{:6}] {:<16} 자원수:{:4}, 주소:{:16} 자원의 주소:{:16}",
			id, "디폴트 생성자", num, (void*)this, (void*)p.get());
	}
}

STRING::~STRING()
{
	if (observe) {
		std::println("[{:6}] {:<16} 자원수:{:4}, 주소:{:16} 자원의 주소:{:16}",
			id, "소멸자", num, (void*)this, (void*)p.get());
	}
}

STRING::STRING(const char* str)
	: num{ strlen(str) }, id{ ++gid }
{
	p.release();
	p = std::make_unique<char[]>(num);
	memcpy(p.get(), str, num);
	
	if (observe) {
		std::println("[{:6}] {:<16} 자원수:{:4}, 주소:{:16} 자원의 주소:{:16}",
			id, "생성자(char*)", num, (void*)this, (void*)p.get());
	}
}

STRING::STRING(const STRING& other)
	: num{ other.num }, id{ ++gid }
{
	p = std::make_unique<char[]>(num);
	memcpy(p.get(), other.p.get(), num);
	// *this = other로 줄이기 가능

	if (observe) {
		std::println("[{:6}] {:<16} 자원수:{:4}, 주소:{:16} 자원의 주소:{:16}",
			id, "복사생성자", num, (void*)this, (void*)p.get());
	}
}

STRING& STRING::operator=(const STRING& other)
{
	if (this == &other) // &other address of <-주소연산자 | int& - 레퍼런스 int&& - 우측값 레퍼런스
		return *this;
	// assign: 둘다 메모리를 차지하고 있을 때 가능
	num = other.num;
	p.release(); // 기존 객체를 반환하고
	p = std::make_unique<char[]>(num); // 메모리를 그만큼 만들어
	memcpy(p.get(), other.p.get(), num); // 복사시킨다.

	if (observe) {
		std::println("[{:6}] {:<16} 자원수:{:4}, 주소:{:16} 자원의 주소:{:16}",
			id, " copy operator=", num, (void*)this, (void*)p.get());
	}
	return *this;
}

STRING::STRING(STRING&& other)
	: num{ other.num }, id{ ++gid }
{
	p.reset(other.p.release());
	// other.num = 0;			// 안해도 됨.
	if (observe) {
		std::println("[{:6}] {:<16} 자원수:{:4}, 주소:{:16} 자원의 주소:{:16}",
			id, "이동 생성자", num, (void*)this, (void*)p.get());
	}
}

STRING& STRING::operator=(STRING&& other)
{
	if (this == &other)
		return *this;
	num = other.num;
	p.release(); // 내가 확보한 자원을 해제
	p.reset(other.p.release());
	if (observe) {
		std::println("[{:6}] {:<16} 자원수:{:4}, 주소:{:16} 자원의 주소:{:16}",
			id, " move operator=", num, (void*)this, (void*)p.get());
	}
	return *this;
}

bool STRING::operator<(const STRING& rhs) const
{
	return std::lexicographical_compare(p.get(), p.get() + num, rhs.p.get(), rhs.p.get() + rhs.num);   // 사전식 정렬을 위해 수정;
}

bool STRING::operator==(const STRING& rhs) const
{
	return std::equal(p.get(), p.get() + num , &rhs.p[0], &rhs.p[num]);
}

size_t STRING::size() const
{
	return num;
}

STRING_Iterator STRING::begin() const
{
	return p.get();
}

STRING_Iterator STRING::end() const
{
	return p.get() + num;
}

STRING_Reverse_Iterator STRING::rbegin() const
{
	return STRING_Reverse_Iterator(p.get() + num);
}

STRING_Reverse_Iterator STRING::rend() const
{
	return STRING_Reverse_Iterator(p.get());
}

std::ostream& operator<<(std::ostream& os, const STRING& s)
{
	for (int i = 0; i < s.size(); i++)
		os << s.p[i];
	return os;
}

std::istream& operator>>(std::istream& is, STRING& s)
{
	std::string str;
	is >> str;

	s.num = str.size();
	s.p.release();
	s.p = std::make_unique<char[]>(s.num);
	memcpy((void*)s.p.get(), str.data(), s.num);

	return is;
}