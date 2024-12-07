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
	TimeManager* mpTimeManager;
	InputManager* mpInputManager;
	ResourceManager* mpResourceManager;
	SoundManager* mpSoundManager;
	EventManager* mpEventManager;
	WorldManager* mpWorldManager;
	PhysicsManager* mpPhysicsManager;
	RenderManager* mpRenderManager;
	UIManager* mpUIManager;
	EntityManager* mpEntityManager;
	FileManager* mpFileManager;
	GameInfo* mpInfo;

	// 그래픽스 엔진 객체
	std::shared_ptr<DearsGraphicsEngine> mpGraphicsEngine;

public:
	entt::registry mRegistry;

	HWND mHWnd;
	int mWidth;
	int mHeight;

private:
	float mFixedDeltaTime;
	float mDeltaTime;
	float mAccumulator;
};

