#pragma once
#include "World.h"
#include "SceneNumEnum.h"



class GameEngine;
class EventManager;

class GameWorld
	: public World
{
public:
	GameWorld(entt::registry& _registry, GameEngine* _pGameEngine, EventManager* _pEventManager);
	~GameWorld() = default;

	/// <summary>
	/// 월드를 초기화합니다.
	/// </summary>
	/// <returns></returns>
	virtual bool Initialize();

	/// <summary>
	/// 고정된 프레임 업데이트
	/// </summary>
	/// <param name="_dTime">고정된 프레임 간격</param>
	virtual void FixedUpdate(float _dTime);

	/// <summary>
	/// 업데이트
	/// </summary>
	/// <param name="_dTime">프레임 간격</param>
	virtual void Update(float _dTime);

	/// <summary>
	/// 후처리 업데이트
	/// </summary>
	/// <param name="_dTime">프레임 간격</param>
	virtual void LateUpdate(float _dTime);

	/// <summary>
	/// 월드를 종료하고 정리합니다.
	/// </summary>
	virtual void Finalize();

 	void SetScene(const SceneName& _sceneName);

private:
	void ChangeScene(const Event& _event);

	void ExitGameProcess(const Event& _event);

	void AddResource();

	void LoadWorldSetting();

	void CreateSceneFromTable();

private:
	GameEngine* m_pEngine;

	bool m_flyingCamera;
	bool m_inputSceneChange;
};

