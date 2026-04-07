//-------------------------------------------------------------
// 2025.1.STL 월910 목910		3월 31일 월요일		(4주 2일)
//-------------------------------------------------------------
// CALLABLE TYPES - 호출가능한 타입
// 1. 함수
//-------------------------------------------------------------
#INCLUDE <IOSTREAM>
#INCLUDE <FSTREAM>
#INCLUDE <ALGORITHM>
#INCLUDE "SAVE.H"

// [문제] "메인.CPP" 파일의 소문자를 대문자로 "변환"하여 "메인 대문자.CPP"에 저장하라
// _언더바 들어간건 표준 컴파일러 만드는 마소 사람들이 쓰는 변수임. 쓰면 안됨.
// M_SNM 헝가리안 로테이션 문법으로 너무 구시대적임. VISUAL STUDIO를 믿자.

INT MAIN()
{
	STD::IFSTREAM IN{ "메인.CPP" };
	IF (NOT IN)
		RETURN 2025;

	STD::OFSTREAM OUT{ "메인 대문자.CPP" };

	// 이건 아냐, 루프를 직접 코딩하네?
	// STL에서 강조하는 건 루프를 굳이 프로그래밍 할 필요 없다.
	// 대체할 알고리즘
	// 루프를 안쓰는 이유, 첫번째 병렬처리 가능성이 있음.
	STD::TRANSFORM(STD::ISTREAMBUF_ITERATOR<CHAR>{IN}, { }, STD::OSTREAMBUF_ITERATOR<CHAR>{OUT},
		[] (CHAR C) {
			RETURN TOUPPER(C);
		}); // 입력 파일 시작, 파일 끝, 출력파일은?, 변신방법?<- 어떻게? 내가 지정 가능!
	// 그러나 우선 람다는 함수가 아니다.

	SAVE("메인.CPP");
}
