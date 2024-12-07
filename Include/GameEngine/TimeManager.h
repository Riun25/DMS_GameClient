#pragma once
#include "pch.h"

/// <summary>
/// 게임 엔진에서 시간을 관리하는 매니저 클래스입니다.
/// QueryPerformance를 사용해 Windows 환경에서 사용하도록 작성되었습니다.
/// 추후 chrono 라이브러리를 사용해서 플랫폼 확장을 고려할수 있습니다.
/// 2024.04.22 _Doyo;
/// </summary>
class TimeManager
{
public:
	TimeManager() = default;
	~TimeManager() = default;

	bool Initialize();
	void Update();
	void Finalize();

	float TotalTime() const;
	float DeltaTime() const;
	float FixedDeltaTime() const;
	float FPS() const;

	void Reset();
	void Start();
	void Stop();

	// 30, 60, 120, 144
	void SetTargetFPS(const float _targetFPS);

	void SetFixedDeltaTime(const float _fDTime);
	void SetDeltaTime(const float _dTime);

	std::chrono::system_clock::time_point GetSystemTime();

	float GetTargetFPS() const { return m_targetFPS; }
private:
	double m_secPerCount = 0;
	double m_deltaTime = 0;
	double m_fixedDeltaTime = 1.0 / 60.0;
	float m_maxDeltaTime = 0.1f;  // 델타 타임을 최대 0.1초로 제한

	uint64_t m_baseTime = 0;
	uint64_t m_pausedTime = 0;
	uint64_t m_stopTime = 0;
	uint64_t m_prevTime = 0;
	uint64_t m_curTime = 0;

	bool m_stopped = false;
	float m_targetFPS = 144.f;	// 목표 fps
	float m_currentFPS = 0.f;     // 현재 FPS를 저장
	int m_frameCount = 0;
	double m_elapsedTime = 0.0;
};
