#pragma once
#include "StateEnum.h"
#include "IState.h"
#include "Rigidbody.h"

/// <summary>
/// 이 State는 현재 아군과 완전히 동일합니다.
/// </summary>
class DamagedStateE : public IState
{
public:
	DamagedStateE(EntityManager* _pEntityManager, PhysicsManager* _pPhysicsManager
		, ResourceManager* _pResourceManager, EventManager* _pEventManager
		, SoundManager* _pSoundManager)
		: IState(_pEntityManager, _pPhysicsManager, _pResourceManager, _pEventManager, _pSoundManager) {};
	~DamagedStateE() {};

public:
	void Initialize() override;
	void Enter(std::shared_ptr<Entity> _entity) override;
	void Update(float _dTime, std::shared_ptr<Entity> _entity) override;
	void Exit(std::shared_ptr<Entity> _entity) override;
	void FixedUpdate(float _fixedTime, std::shared_ptr<Entity> _entity) override {}

};

