#pragma once
#include "StateEnum.h"
#include "IState.h"
#include "Transform.h"
#include "EnemyComponent.h"

class DeadStateE : public IState
{
public:
	DeadStateE(EntityManager* _pEntityManager, PhysicsManager* _pPhysicsManager
		, ResourceManager* _pResourceManager, EventManager* _pEventManager
		, SoundManager* _pSoundManager)
		: IState(_pEntityManager, _pPhysicsManager, _pResourceManager, _pEventManager, _pSoundManager) {};
	~DeadStateE() {};

public:
	void Initialize() override;
	void Enter(std::shared_ptr<Entity> _entity) override;
	void Update(float _dTime, std::shared_ptr<Entity> _entity) override;
	void Exit(std::shared_ptr<Entity> _entity) override;
	void FixedUpdate(float _fixedTime, std::shared_ptr<Entity> _entity) override {}

};
