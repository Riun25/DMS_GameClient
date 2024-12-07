#pragma once
enum class GameState
{
	NORMAL,				// 게임 시작시 기초 상태
	START_ANIMATION,	// 게임 시작 전 카메라 무빙 상태
	PRE_PLACEMENT,		// 카메라 무빙 후 UI 처리
	PLACEMENT,			// 배치
	PRE_PLAY,			// 게임 시작 전 처리
	PLAY,				// 게임 실행
	NONE_UI,			// UI 가리는 처리
	POST_PLAY_ANIMATION,// 승패 완료 후 카메라 무빙 상태
	POST_PLAY,			// 카메라 무빙 후 UI 처리
	RESULT,				// 결과 창 처리
	RESTART,			// 게임 재시작 상태

	TUTORIALEND,		// 튜토리얼 엔딩 상태. 아무것도 처리하지 않는다.
	NEXTLEVEL, 

	LAST,
};

enum class SettingState
{
	OPEN,         // 세팅창을 여는 버튼을 누른 상태
	ADJUST,       // 세팅창을 조정 중인 상태
	CLOSED,       // 세팅창을 닫는 버튼을 누른 상태
	NORMAL,		  // 세팅창이 닫혀져 있는 평균 상태
	NONE,		  // 오랫동안 움직임이 없을 때 바뀌는 상태(추후에 할 것)	

	LAST,
};

enum class PopUpState
{
	OPEN_MAINMENU,	// 팝업창을 여는 버튼을 누른 상태
	OPEN_UNITDEPLOY,// 팝업창을 여는 버튼을 누른 상태
	OPEN_EXITGAME,	// 팝업창을 여는 버튼을 누른 상태
	OPEN_SKIP,		// 스킵 팝업창을 여는 버튼을 누른 상태
	MAINMENU,		// 메인 메뉴 팝업 조정
	UNITDEPLOY,		// 유닛 배치 팝업 조정
	EXITGAME,		// 종료 팝업 조정
	SKIP,
	CLOSED,			// 팝업창을 닫는 버튼을 누른 상태
	NORMAL,			// 팝업이 닫혀있는 상태

	LAST,
};

enum class UIAnimationState
{
	POSTGAMESTART,	// 게임 스타트 누른 후 애니메이션 작동(위로 상승)
	NORMAL,			// 일반적인 작동 씬(메인 메뉴, 배틀, 로딩) -> 이 부분은 개인 씬마다 코딩이 따로 되어야 함
	PRELOADING,		// 로딩 전 애니메이션 작동(위로 상승)
	POSTLOATING,	// 로딩 후 애니메이션 작동(아래로 하강)
	PREBATTLE,		// 전투 전 애니메이션 작동(구름 오픈)
	POSTBATTLE,		// 전투 후 애니메이션 작동(구름 닫힘)
	PRETUTORIAL,	// 튜토리얼 전 애니메이션 작동 (구름 오픈) -> 서로 상태가 좀 다르므로 그냥 분리한다.
	POSTTUTORIAL,	// 튜토리얼 후 애니메이션 작동 (구름 닫힘)

	VISIBLE,		// UI를 보이게 하는 기능 -> 이 부분은 개인 씬마다 코딩이 따로 되어야 함
	UNVISIBLE,	// UI를 안 보이게 하는 기능 -> 이 부분은 개인 씬마다 코딩이 따로 되어야 함
	NEXTSCENE,		// 다음 씬으로 넘어가라는 지시자 역할 -> 이 부분은 개인 씬마다 코딩이 따로 되어야 함

	// [메인 화면] : NORMAL(메인) -> PREGAMESTART -> NEXTSCENE ->
	// [로딩 화면] : PRELOADING -> NORMAL(로딩) -> POSTLOADING -> NEXTSCENE ->
	// [전투 화면] : PREBATTLE -> NORMAL(배틀) -> ??? -> NEXTSCENE ->
	// [엔팅 화면] : NORMAL(배틀) -> ??? -> NEXTSCENE(엔딩) -> ??? -> 크레딧

	/// 튜토리얼 시작 부분도 다 바뀌어야 함
	/// 배틀 씬 시작 부분 FadeINout 전체 삭제되고 UI로 대체 될 것임(구름)

	LAST,
};