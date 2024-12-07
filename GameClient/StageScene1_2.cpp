#include "StageScene1_2.h"
#include "DearsGameEngineAPI.h"
#include "PhysicsManager.h"
#include "ResourceManager.h"
#include "PlayerComponent.h"
#include "EnemyComponent.h"
#include "WeaponComponent.h"
#include "ProjectileComponent.h"
#include "CustomFilter.h"
#include "MoneyGunComponent.h"
#include "IndicatorComponent.h"
#include "NPCComponent.h"
#include "SceneNumEnum.h"

StageScene1_2::StageScene1_2(entt::registry& _registry, const std::string& _name
	, EventManager* _pEventManager, RenderManager* _pRenderManager
	, PhysicsManager* _pPhysicsManager, InputManager* _pInputManager, WorldManager* _worldManager
	, UIManager* _UIManager, EntityManager* _pEntityManager, ResourceManager* _pResourceManager
	, SoundManager* _pSoundManager)
	: Scene(_registry, _name, _pEventManager, _pRenderManager, _pPhysicsManager
		, _pInputManager, _worldManager, _UIManager, _pEntityManager, _pResourceManager, _pSoundManager)
{
	mpUnitSystem = nullptr;
	mpLevelManager = nullptr;
	mpAstar = nullptr;
}

bool StageScene1_2::Initialize()
{
	/// 리소스 추가
	AddResource();

	return Scene::Initialize();
}

void StageScene1_2::FixedUpdate(float _dTime)
{
}

void StageScene1_2::Update(float _dTime)
{
}

void StageScene1_2::LateUpdate(float _dTime)
{
}

void StageScene1_2::Finalize()
{
	Scene::Finalize();
}

void StageScene1_2::AddResource()
{
}
