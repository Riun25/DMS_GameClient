#pragma once

enum class ClassType
{
	MERCENARY,	// 밸런스
	ARCHER,		// 장거리 딜러
	MAID,		// 한방딜
	MONK,		// 근거리 공속
	NINJA,		// 원거리 공속
	VIKING,		// 탱커
	WIZARD,		// 광역 원거리 딜러

	/// 적군 클래스 
	RVC_0,		// 듀토리얼용
	RVC_R1,
	RVC_C,

	/// 디버깅을 위한 요소
	LAST,
};