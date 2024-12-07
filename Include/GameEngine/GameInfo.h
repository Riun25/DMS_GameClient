#pragma once
#include <windows.h>

class TimeManager;

class GameInfo
{
public:
	GameInfo(TimeManager* _pTimeManager);
	~GameInfo();

	float GetFPS() const;
	float GetFrameTime() const;
	float GetCPUInfo();

	// GPU 사용량
	double GetVRAMInfo() const;

	// RAM 사용량
	unsigned int GetRAMInfo() const;

	// 	// 렌더링에 소요된 시간
	// 	float GetRenderTime() const { return m_RenderTime; }
	// 	void SetRenderTime(const float val) { m_RenderTime = val; }
	// 
	// 	// 물리 연산에 소요된 시간
	// 	float GetPhysicsTime() const { return m_physicsTime; }
	// 	void SetPhysicsTime(const float val) { m_physicsTime = val; }

private:
	TimeManager* m_pTimeManager;

	float mFPS = 0;			// 초당 렌더링하는 프레임 수
	float mFrameTime = 0;		// 한 프레임을 렌더링하는 데 걸리는 시간
	float mCPUInfo = 0;		// CPU 사용량
	// 	float m_RenderTime;		// 렌더링 작업에 소요되는 시간
	// 	float m_physicsTime;	// 물리 연산에 소요되는 시간

	ULARGE_INTEGER mLastCPU, mLastSysCPU, mLastUserCPU;
	int mNumProcessors;
	HANDLE mSelf;
};