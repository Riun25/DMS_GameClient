#pragma once
#include "StateEnum.h"
#include "PlayerComponent.h"
#include "Rigidbody.h"
#include "Transform.h"
#include "CapsuleCollider.h"
#include "IState.h"
#include "ProjectileComponent.h"
#include "BoxCollider.h"
#include "Texture3D.h"
#include "FilterEnum.h"
#include "RenderManager.h"
#include "FSMHelper.h"

/// <summary>
/// 공격 상태
/// </summary>
class AttackStateE : public IState
{
public:
	AttackStateE(EntityManager* _pEntityManager, PhysicsManager* _pPhysicsManager
		, ResourceManager* _pResourceManager, EventManager* _pEventManager
		, SoundManager* _pSoundManager, RenderManager* _pRenderManager, FSMHelper* _pFSMHelper)
		: IState(_pEntityManager, _pPhysicsManager, _pResourceManager, _pEventManager, _pSoundManager)
		, mpRenderManager(_pRenderManager), mpFSMHelper(_pFSMHelper)
	{};
	~AttackStateE() {};

public:
	void Initialize() override;
	void Enter(std::shared_ptr<Entity> _entity) override;
	void Update(float _dTime, std::shared_ptr<Entity> _entity) override;
	void Exit(std::shared_ptr<Entity> _entity) override;
	void FixedUpdate(float _fixedTime, std::shared_ptr<Entity> _entity) override;

	void RangedAttack(std::shared_ptr<Entity> _entity);

private:
	//float mRotationSpeed = 0.3f; // 타겟을 향해 회전할 때 보간하는 수치 (0~1.0f)
	FSMHelper* mpFSMHelper;
	RenderManager* mpRenderManager;
	float mAttackSoundTime = 0;
};