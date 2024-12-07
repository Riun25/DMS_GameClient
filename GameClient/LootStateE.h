#pragma once
#include "StateEnum.h"
#include "EnemyComponent.h"
#include "MoneyComponent.h"
#include "ProjectileComponent.h"
#include "FSMHelper.h"
#include "Transform.h" /// 이거 임시로 한 거니까 나중에 없앨 것
#include "IState.h"

class LootStateE : public IState
{
public:
	LootStateE(EntityManager* _pEntityManager, PhysicsManager* _pPhysicsManager
		, ResourceManager* _pResourceManager, EventManager* _pEventManager
		, SoundManager* _pSoundManager, FSMHelper* _pFSMHelper)
		: IState(_pEntityManager, _pPhysicsManager, _pResourceManager, _pEventManager, _pSoundManager)
		, mpFSMHelper(_pFSMHelper) {};
	~LootStateE() {};

public:
	void Initialize() override;
	void Enter(std::shared_ptr<Entity> _entity) override;
	void Update(float _dTime, std::shared_ptr<Entity> _entity) override;
	void Exit(std::shared_ptr<Entity> _entity) override;
	void FixedUpdate(float _fixedTime, std::shared_ptr<Entity> _entity) override {}

private:
	FSMHelper* mpFSMHelper;
};

