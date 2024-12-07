#pragma once
#include "pch.h"
#include "Singleton.h"

class DearsGraphicsEngine;
class TimeManager;
class InputManager;
class ResourceManager;
class SoundManager;
class EventManager;
class WorldManager;
class PhysicsManager;
class RenderManager;
class UIManager;
class EntityManager;
class IManager;
class FileManager;
class GameInfo;

/// <summary>
/// 2024.05.20 _Doyo;
/// </summary>
class GameEngine
{
public:
	GameEngine() = delete;
	GameEngine(HWND _hWnd, int _width, int _height);
	~GameEngine() = default;

	bool Initialize();
	void Run();
	void FixedUpdate(float _fixedDeltaTime);
	void Update(float _dTime);
	void Render(float _dTime);
	void LateUpdate(float _dTime);
	void Finalize();

	void BeginRender();
	void EndRender();

public:
	TimeManager* GetTimeManager() const;
	InputManager* GetInputManager() const;
	ResourceManager* GetResourceManager() const;
	SoundManager* GetSoundManager() const;
	EventManager* GetEventManager() const;
	WorldManager* GetWorldManager() const;
	PhysicsManager* GetPhysicsManager() const;
	RenderManager* GetRenderManager() const;
	UIManager* GetUIManager() const;
	EntityManager* GetEntityManager() const;

private:
	// 매니저 객체들의 포인터
	TimeManager* m_pTimeManager;
	InputManager* m_pInputManager;
	ResourceManager* m_pResourceManager;
	SoundManager* m_pSoundManager;
	EventManager* m_pEventManager;
	WorldManager* m_pWorldManager;
	PhysicsManager* m_pPhysicsManager;
	RenderManager* m_pRenderManager;
	UIManager* m_pUIManager;
	EntityManager* m_pEntityManager;
	FileManager* m_pFileManager;
	GameInfo* m_pInfo;

	// 그래픽스 엔진 객체
	std::shared_ptr<DearsGraphicsEngine> m_pGraphicsEngine;

public:
	entt::registry m_registry;

	HWND m_hWnd;
	int m_width;
	int m_height;

private:
	float m_fixedDeltaTime;
	float m_deltaTime;
	float m_accumulator;
};

