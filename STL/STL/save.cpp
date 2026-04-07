//-------------------------------------------------------------------
// save.cpp - 한 학기 강의를 잘 저장하는 함수
//														2025. 3. 13
//-------------------------------------------------------------------
#include <string_view>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <chrono>

void save(std::string_view fileName)
{
	// 1. 인자로 전달된 fileName을 읽기 모드로 연다.
	std::ifstream input{ fileName.data() };	// RAII

	if (not input)
	{
		std::cout << fileName << "을 열 수 없습니다!" << std::endl;
		exit(20250310);
	}

	// 저장했다는 사실을 화면에 표시하자.
	std::cout << fileName << "(" << std::filesystem::file_size(fileName) << " bytes) 저장하였습니다." << std::endl;

	// 2. 쓰기 모드로 저장할 파일을 (덧붙이기 모드로) 연다.
	std::ofstream output{ "2025 1학기 STL 월910목910 강의저장.txt", std::ios::app }; // { } uniform initializer

	// 시간 기록
	auto now = std::chrono::system_clock::now(); // epoch로 부터 경과된 tick 수를 얻는다.
	auto utc = std::chrono::system_clock::to_time_t(now); // UTC타임으로 변경
	auto lt = std::localtime(&utc); // 아주 오래된 C함수라 저 타입으로 보내야함 -> 지역시간으로 변경
	auto old = output.imbue(std::locale("ko_KR")); // 이전 상태를 반드시 세이브 해두고 형식을 바꿔라 출력 스트림의 지역을 변경하고

	output << '\n' << '\n';
	output << "========================================" << '\n';
	output << "저장한 시간: " << std::put_time(lt, "%c %A") << '\n';
	output << "========================================" << '\n';
	output << "\n";

	output.imbue(old); // 변경 이전으로 되돌린다.

	// 3. 파일을 읽어서 저장할 파일에 덧붙인다.
	// while은 매번 조건을 평가한다. 안 좋음, for 루프도 마찬가지
	std::copy(std::istreambuf_iterator{ input }, {}, std::ostreambuf_iterator{ output }); //리턴 타입 프로토타입 컴파일러 지시 - 컴파일타임에 결정되어라, 1. 처음부터 2. 끝까지 전부 읽어내줘 3. 어디로?
}
