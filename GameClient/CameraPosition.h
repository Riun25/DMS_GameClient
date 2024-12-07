#pragma once
#include "DearsGameEngineAPI.h"

/// <summary>
/// 카메라 이동 연출에 사용될 특정 위치, 각도들
/// </summary>
struct CameraPosition
{
	// perspective
	Vector3 mainPositionPerspective = Vector3(-16.46, 18.06, -16.62);
	Vector3 mainRotationPerspective = Vector3(0, -0.8, 0.6);

	// orthographic
	Vector3 mainPosOrtho = Vector3(-16.46, 18.06, -16.62);
	Vector3 mainRotOrtho = Vector3(0.57735, -0.57735, 0.57735);

	// 전투 씬 시작 위치
	Vector3 startPos = Vector3(0.f, 34.f, -17.5f);
	Vector3 startRot = Vector3(0, -0.93, 0.37);

	// 전투 씬 1(이동) // 비행기 타고 이동
	//Vector3 pos1 = Vector3(0, 17, -13.5);
	Vector3 rot1 = Vector3(0, -0.8, 0.6);

	// 전투 씬 1(이동2) // 비행기 타고 이동
	Vector3 pos2 = Vector3(0, 6, -1.5);
	Vector3 rot2 = Vector3(0.13, -0.7, 0.52);

	// 전투 씬 2(페이드 아웃 후) // 용병단장을 보기
	Vector3 pos3 = Vector3(16, 4, 0);
	Vector3 rot3 = Vector3(1, 0, 0);

	// 전투 씬 3 뒤로 빼기
	Vector3 pos4 = Vector3(-3, 10, 0);
	Vector3 rot4 = Vector3(0.8, -0.6, 0);

	// 전투 씬4(시점 변환 후)
	Vector3 pos5 = Vector3(-16.4, 31.79, -16.77);
	Vector3 rot5 = mainRotOrtho;

	// 전투씬 5 (결과1) : 뒤로 빼서 용병들 뛰는 거 보여줌
	Vector3 pos6 = Vector3(21, 6.4, 0);
	Vector3 rot6 = Vector3(-1, 0, 0);

	// 전투씬 6 (결과2) : 용병왕으로 줌인 됨
	Vector3 pos7 = Vector3(-12.f, 4.0f, 3.35f);
	Vector3 rot7 = Vector3(-1.f, -0.2f, 0.0f);


};