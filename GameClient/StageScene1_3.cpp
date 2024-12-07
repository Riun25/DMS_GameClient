#include "StageScene1_3.h"

StageScene1_3::StageScene1_3(entt::registry& _registry, const std::string& _name
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

bool StageScene1_3::Initialize()
{
	/// 리소스 추가
	AddResource();

	return Scene::Initialize();
}

void StageScene1_3::FixedUpdate(float _dTime)
{
}

void StageScene1_3::Update(float _dTime)
{
}

void StageScene1_3::LateUpdate(float _dTime)
{
}

void StageScene1_3::Finalize()
{
	Scene::Finalize();
}

void StageScene1_3::AddResource()
{
}
