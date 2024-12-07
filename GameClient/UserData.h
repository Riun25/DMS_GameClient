#pragma once
#include "IData.h"
#include <nlohmann/json.hpp>
#include <fstream>

class UserData 
	: public IData
{
public:
	struct GraphicsData								// 그래픽 설정
	{
		std::string screenResolution = "1920x1080"; // 스크린 크기 설정
		bool fullscreen = false;                    // 풀스크린 모드로 시작
		bool vsync = false;                         // VSync 설정
		bool drawShadow = true;                     // 그림자 렌더링
	} graphics;

	struct AudioData								// 오디오 설정
	{
		int masterVolume = 3;						// 마스터 볼륨
		int bgmVolume = 3;							// 배경음 볼륨
		int sfxVolume = 3;							// 효과음 볼륨
	} audio;

	struct ConfigData								
	{
		int fps = 60;								// 프레임 제한
	} config;

	struct GameplayData								// 인게임 플레이 관련 설정
	{
		int moneyAmount = 30;						// 돈 투척금액
		float moneySpeed = 1.5f;					// 돈 속도
		float projectileSpeed = 1.0f;				// 투사체 속도
	} gameplay;

	struct DebugData								// 디버그용 기능 활성화/비활성화
	{               
		bool drawCollider = false;					// 콜라이더 렌더링
		bool drawFps = false;						// FPS 및 델타타임 출력
		bool drawCameraInfo = false;				// 카메라 정보 출력
		bool flyingCamera = false;					// 플라잉 카메라 및 직교 전환
		bool inputSceneChange = true;				// 키 입력으로 씬 전환
		bool logging = true;						// 로그 작성
	} debug;

	UserData(const std::string& filePath)
		: m_filePath(filePath) 
	{
	}

	void Load();
	void Save() const;

private:
	std::string m_filePath;

};
