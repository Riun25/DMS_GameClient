#pragma once
#include "pch.h"
#include "Singleton.h"

class TimeManager;
class InputManager;
class ResourceManager;
class SoundManager;
class EventManager;
class WorldManager;

/// <summary>
/// 게임 엔진의 매니저들을 관리하는 매니저 클래스입니다.
/// 하위 매니저들의 생성, 소멸 및 초기화 등을 담당합니다.
/// 2024.05.20 _Doyo;
/// </summary>
class GameManager 
{
public:
	bool Initialize();
	void FixedUpdate();
	void Update();
	void LateUpdate();
	void Finalize();


private:
    // 매니저 객체들의 포인터
	TimeManager* m_pTimeManager;
	InputManager* m_pInputManager;
	ResourceManager* m_pResourceManager;
	SoundManager* m_pSoundManager;
	EventManager* m_pEventManager;
	WorldManager* m_pWorldManager;


private:
	entt::registry m_registry;

};

