#include "UnitSystem.h"

UnitSystem::UnitSystem(entt::registry& _registry, EntityManager* _pEntityManager, PhysicsManager* _pPhysicsManager
	, InputManager* _pInputManager, WorldManager* _pWorldManager, RenderManager* _pRenderManager
	, ResourceManager* _pResourceManager, EventManager* _pEventManager
	, UIManager* _pUIManager, SoundManager* _pSoundManager, AStar* _pAstar)
	: mRegistry(_registry)
{
	mpEntityManager = _pEntityManager;
	mpPhysicsManager = _pPhysicsManager;
	mpInputManager = _pInputManager;
	mpWorldManager = _pWorldManager;
	mpRenderManager = _pRenderManager;
	mpResourceManager = _pResourceManager;
	mpEventManager = _pEventManager;
	mpUIManager = _pUIManager;
	mpSoundManager = _pSoundManager;

	// Astar 만들기
	mpAstar = _pAstar;

	// 투사체 시스템 설정
	mpProjectileSystem =  new ProjectileSystem(mRegistry, mpEventManager
		, mpEntityManager, mpPhysicsManager, mpRenderManager, mpSoundManager
		, mpInputManager, mpWorldManager);

	// 장비 매니저 생성
	mpEquipmentManager = new EquipmentManager(mRegistry, mpEntityManager, mpPhysicsManager, mpRenderManager, mpResourceManager);

	// 플레이어 시스템 생성
	mpPlayerSystem = new PlayerSystem(mRegistry, mpEntityManager, mpPhysicsManager
		, mpRenderManager, mpResourceManager, mpEventManager, mpSoundManager, mpEquipmentManager);

	// NPC 시스템 생성
	mpNPCSystem = new NPCSystem(mpEntityManager, mpRenderManager, mpResourceManager, mpSoundManager, mpUIManager, mpProjectileSystem, mpEquipmentManager);
}

UnitSystem::~UnitSystem()
{
	delete mpNPCSystem;
	delete mpPlayerSystem;
	delete mpEquipmentManager;
	delete mpProjectileSystem;
}

void UnitSystem::Initialize()
{
	// 생성한 시스템 및 매니저의 초기화
	mpProjectileSystem->Initialize();
	mpEquipmentManager->Initialize();
	mpPlayerSystem->Initialize(mpProjectileSystem->GetMoneyPosVec(), mpAstar);
	mpNPCSystem->Initialize();
	mpProjectileSystem->SetAstar(mpAstar);
}

ProjectileSystem* UnitSystem::GetProjectileSystem()
{
	return mpProjectileSystem;
}

EquipmentManager* UnitSystem::GetEquipmentManager()
{
	return mpEquipmentManager;

}

PlayerSystem* UnitSystem::GetPlayerSystem()
{
	return mpPlayerSystem;

}

NPCSystem* UnitSystem::GetNPCSystem()
{
	return mpNPCSystem;
}