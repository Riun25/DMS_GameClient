#pragma once
#include "StateEnum.h"
#include "IState.h"
#include "IdleState.h"
#include "MoveState.h"
#include "AttackState.h"
#include "LootState.h"
#include "DamagedState.h"
#include "DeadState.h"
#include "Transition.h"
#include "MoneyComponent.h"
#include "FSMHelper.h"

/// 캐릭터의 유한 상태 머신
// FSM은 2가지 구조가 있음
// 1. 공용 FSM을 씀 -> State를 엔티티 별로 별도로 사용함, 메모리 최적화 but 독립적 작용 어려움
// 2. 개별 FSM을 씀 -> 일반적인 구조 : 보스전에서만 사용할 FSM을 만들 예정.
class AllyFSM
{
public:
	AllyFSM(EntityManager* _pEntityManager, PhysicsManager* _pPhysicsManager
		, ResourceManager* _pResourceManager, EventManager* _pEventManager
		, SoundManager* _pSoundManager, RenderManager* _pRenderManager, FSMHelper* _pFSMHelper);
	~AllyFSM();

public:
	void Initialize(); // state들을 생성 - fsm이 살아있는 동안은 돌려쓴다
	void Update(float _dTime, std::shared_ptr<Entity> _entity); // 업데이트 [엔티티]
	IState* GetState(State _state); // 생성한 state들을 StatePool에서 찾아서 가져옴

	void FixedUpdate(float _fixedTime, std::shared_ptr<Entity> _entity);

private:
	void ChangeState(std::shared_ptr<Entity> _entity, State _newStateEnum); // state가 변경 가능한지 확인한다.
	Transition* mpTransition; // transition은 가벼운 스위치 역할을 한다.

	IState* mpIdleState;
	IState* mpMoveState;
	IState* mpAttackState;
	IState* mpLootState;
	IState* mpDamagedState;
	IState* mpDeadState;

	EntityManager* mpEntityManager;
	PhysicsManager* mpPhysicsManager;
	ResourceManager* mpResourceManager;
	EventManager* mpEventManager;
	SoundManager* mpSoundManager;
	RenderManager* mpRenderManager;
	FSMHelper* mpFSMHelper;

	std::unordered_map<std::string, std::function<bool(std::shared_ptr<Entity> _temp)>> mpFuncMap; // 매우 자주 써서 get 함수로 매번 가져오는 것이 더 불리하므로 초반에 포인터로 셋팅해서 사용한다.
};

