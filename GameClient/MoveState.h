#pragma once
#include "IState.h"
#include "StateEnum.h"
#include "FSMHelper.h"
#include "AStar.h"

class MoveState : public IState
{
public:
	MoveState(EntityManager* _pEntityManager, PhysicsManager* _pPhysicsManager
		, ResourceManager* _pResourceManager, EventManager* _pEventManager
		, SoundManager* _pSoundManager, FSMHelper* _pFSMHelper)
		: IState(_pEntityManager, _pPhysicsManager, _pResourceManager, _pEventManager, _pSoundManager)
		, mpFSMHelper(_pFSMHelper) {};
	~MoveState() {};

public:
	void Initialize() override;
	void Enter(std::shared_ptr<Entity> _entity) override;
	void Update(float _dTime, std::shared_ptr<Entity> _entity) override;
	void Exit(std::shared_ptr<Entity> _entity) override;
	void FixedUpdate(float _fixedTime, std::shared_ptr<Entity> _entity) override;

private:
	FSMHelper* mpFSMHelper;

	float mAnimationRation = 0.3f; // 기획에서 제공한 값으로, 가장 기본 단위 값

	float mThreshold = 1.0f; // 1.0이상 값, 이게 작을 수록 더 정밀하다. 
	float mLengthThresHold = 0.06f; // 타깃과 위치가 너무 가까울 때는 빙빙 돌기 때문에 방지하고자 넣는 보간용 코드다.

	// 회전 속도
	//float mRotationSpeed = 0.3f;

	float testSpeedCoef = 0.1f; // 그냥 플밍이 속도 조정하고 싶을 때 쓰는 계수다.
};

