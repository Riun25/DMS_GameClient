#pragma once
enum class TutorialState
{
	CLOUD,			// 구름 시작
	FADE,			// 페이드 상태
	INTRO,          // 튜토리얼 소개
	THROW_O,		// 장애물이 있는 곳에 동전을 던지기
	THROW_O_WAIT,	// 다른 동전들을 삭제하고, 하나의 동전을 던진 걸 유지하기
	THROW,			// 동전 던지기
	Enemy_Play,		// 적군을 플레이 시킨다.
	BUTTON_M,		// 근거리 클래스 버튼을 누름
	DEPLOY_M,       // 근거리 유닛 배치 / 원거리 클래스 버튼 활성화
	THROW_WAIT,		// 다른 동전들을 삭제하고, 하나의 동전을 던진 걸 유지하기
	ALLY_Play,		// 아군 유닛 작동
	BUFF,			// 버프 상태 / 돈을 5회 이상 던짐
	ENEMY,	        // 적군 소개
	MONEY,			// 돈을 증가시켜 넣어줌
	FINAL,          // 20명 배치할 때까지 대기
	PLAYING,		// 전투 시작 버튼 누르기
	POSTPLAYEING,	// 대화창 처리를 위한 state
	END,			// 결과창 대신 활성화
	LAST,
};