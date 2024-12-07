#pragma once
#include "Scene.h"
#include "UnitSystem.h"
#include "PlayerSystem.h"
#include "ProjectileSystem.h"
#include "NPCSystem.h"
#include "SceneData.h"
#include "LevelManager.h"

class StageScene1_3
	: public Scene
{
public:
	/// <summary>
	/// 씬에서 기본적으로 사용되는 객체들을 생성자에서 주입해줍니다.
	/// </summary>
	StageScene1_3(entt::registry& _registry, const std::string& _name
		, EventManager* _pEventManager, RenderManager* _pRenderManager
		, PhysicsManager* _pPhysicsManager, InputManager* _pInpueManager
		, WorldManager* _worldManager, UIManager* _UIManager, EntityManager* _pEntityManager
		, ResourceManager* _pResourceManager, SoundManager* _pSoundManager);

	/// <summary>
	/// 씬을 초기화합니다.
	/// </summary>
	/// <returns>__super::Initialize() 반환</returns>
	virtual bool Initialize() override;

	/// <summary>
	/// 고정된 시간 간격으로 물리 업데이트를 수행합니다.
	/// </summary>
	/// <param name="_dTime">고정된 델타타임</param>
	virtual void FixedUpdate(float _dTime) override;

	/// <summary>
	/// 매 프레임 업데이트를 수행합니다.
	/// </summary>
	/// <param name="_dTime">프레임 시간</param>
	virtual void Update(float _dTime) override;

	/// <summary>
	/// 후처리 업데이트를 수행합니다.
	/// </summary>
	/// <param name="_dTime">프레임 시간</param>
	virtual void LateUpdate(float _dTime) override;

	/// <summary>
	/// 씬을 종료하고 정리합니다.
	/// </summary>
	virtual void Finalize() override;

	/// <summary>
	/// 씬에 리소스를 추가합니다.
	/// </summary>
	virtual void AddResource() override;

private:
	UnitSystem* mpUnitSystem;
	LevelManager* mpLevelManager;
	AStar* mpAstar;
};
