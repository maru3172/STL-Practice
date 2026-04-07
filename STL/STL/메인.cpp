//---------------------------------------------------------------------------------------
// 2025. 1. STL 월910 목910		총 정리 실습 - 애너그램 찾기
//---------------------------------------------------------------------------------------
// 단어들 파일 단어 개수 - 369882개
// 정렬 - O
// 중복된 단어 - X
// 길이가 가장 긴 단어 - 31글자, dichlorodiphenyltrichloroethane
// 길이가 같은 단어들을 화면에 출력
// 사용자가 입력한 단어가 사전에 있는가?
// 있으면 anagram을 출력
//---------------------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <list>
#include <ranges>
#include "save.h"
#include "STRING.h"

struct PS : std::pair<std::string, std::string> {
	PS(std::string s) : pair(s, s) {
		std::sort(first.begin(), first.end());
	}
};

int main()
{
	save("메인.cpp");
	std::ifstream in{ "단어들.txt" };
	if (not in) {
		std::cout << "파일을 열 수 없습니다." << std::endl;
		exit(0);
	}

	std::vector<PS> v;
	v.reserve(40'0000);
	std::string s;
	while (in >> s)
		v.push_back(s);
	std::cout << "단어 개수 - " << v.size() << std::endl;

	auto PSas = [](const PS& a, const PS& b) {
		return a.first < b.first;
		};
	std::sort(v.begin(), v.end(), PSas);

	// [문제] 전체 anagram 쌍을 컨테이너에 저장하라.
	std::vector<std::list<std::string>> anagrams;
	auto i{ v.begin() };
	int cnt{};
	while (true) {
		// 인접한 곳 찾고
		i = std::adjacent_find(i, v.end(), [](const PS& a, const PS& b) {
			return a.first == b.first;
			});
		if (i == v.end())
			break;
		
		// 같지 않은 구간까지 찾고
		auto j = std::find_if_not(i + 1, v.end(), [i](const PS& a) {
			return i->first == a.first;
			});

		std::list<std::string> con;
		for (auto k = i; k < j; ++k)
			con.push_back(k->second);
		anagrams.push_back(con);

		// 끝나면 마지막 구간으로 가 다시 탐색
		i = j;
	}

	std::cout << "모두 " << anagrams.size() << "개의 애너그램 쌍을 찾았습니다." << std::endl;

	std::sort(anagrams.begin(), anagrams.end(), [](const std::list<std::string>& a, const std::list<std::string>& b) {
		return a.size() > b.size();
		});

	for (int i = 0; i < 10; ++i) {
		std::cout << anagrams[i].size() << " - ";
		for (const std::string& word : anagrams[i])
			std::cout << word << " ";
		std::cout << std::endl;
	}
}
