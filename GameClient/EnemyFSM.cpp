#pragma once
#include "EnemyFSM.h"
#include "CapsuleCollider.h"

EnemyFSM::EnemyFSM(EntityManager* _pEntityManager, PhysicsManager* _pPhysicsManager
	, ResourceManager* _pResourceManager, EventManager* _pEventManager
	, SoundManager* _pSoundManager, RenderManager* _pRenderManager, FSMHelper* _pFSMHelper)
{
	mpEntityManager = _pEntityManager;
	mpPhysicsManager = _pPhysicsManager;
	mpResourceManager = _pResourceManager;
	mpEventManager = _pEventManager;
	mpSoundManager = _pSoundManager;
	mpRenderManager = _pRenderManager;
	mpFSMHelper = _pFSMHelper;
	mpTransition = new Transition();
}

EnemyFSM::~EnemyFSM()
{
	delete mpDeadState;
	delete mpDamagedState;
	delete mpLootState;
	delete mpAttackState;
	delete mpMoveState;
	delete mpIdleState;
	delete mpTransition;
}

void EnemyFSM::Initialize()
{
	//  state 생성
	mpIdleState = new IdleStateE(mpEntityManager, mpPhysicsManager, mpResourceManager, mpEventManager, mpSoundManager);
	mpMoveState = new MoveStateE(mpEntityManager, mpPhysicsManager, mpResourceManager, mpEventManager, mpSoundManager, mpFSMHelper);
	mpAttackState = new AttackStateE(mpEntityManager, mpPhysicsManager, mpResourceManager, mpEventManager, mpSoundManager, mpRenderManager, mpFSMHelper);
	mpLootState = new LootStateE(mpEntityManager, mpPhysicsManager, mpResourceManager, mpEventManager, mpSoundManager, mpFSMHelper);
	mpDamagedState = new DamagedStateE(mpEntityManager, mpPhysicsManager, mpResourceManager, mpEventManager, mpSoundManager);
	mpDeadState = new DeadStateE(mpEntityManager, mpPhysicsManager, mpResourceManager, mpEventManager, mpSoundManager);

	mpIdleState->Initialize();
	mpMoveState->Initialize();
	mpAttackState->Initialize();
	mpLootState->Initialize();
	mpDamagedState->Initialize();
	mpDeadState->Initialize();

	// 공통적인 함수 맵을 받아온다.
	mpFuncMap = mpFSMHelper->GetFuctMap();
}

// Transition에서 변경 가능한지 확인한다.
void EnemyFSM::ChangeState(std::shared_ptr<Entity> _entity, State _newStateEnum)
{
	/// 0. 공통 변수 선언
	auto& enemyComp = _entity->GetComponent<EnemyComponent>();

	/// 1. 변경 가능한 상태가 아니면 종료
	if (mpTransition->IsCanTransitionTo(_newStateEnum) == false)
	{
		//return;
		//_entity->GetComponent<AnimationComponent>().mpNextTargetAnimation = mpResourceManager->Get_Animation("Character@Death.fbx");
	}

	/// 2. 현재 상태가 nullptr이 아니라면 현재 상태를 나가고 새 상태로 들어갈 준비를 한다.
	if (enemyComp.mCurrentSM != nullptr)
	{
		enemyComp.mCurrentSM->Exit(_entity);
	}

	enemyComp.mCurrentSM = GetState(_newStateEnum);
	enemyComp.mOwnState = _newStateEnum;

	/// 3. 현재 상태가 nullptr이 아니라면 새 상태로 들어간다.
	if (enemyComp.mCurrentSM != nullptr)
	{
		enemyComp.mCurrentSM->Enter(_entity);
	}
}

void EnemyFSM::Update(float _dTime, std::shared_ptr<Entity> _entity)
{
	/// 0. 공통 변수 선언
	auto& enemyComp = _entity->GetComponent<EnemyComponent>();

	/// 2. State 변경 : 기획 문서 참고
	// 유닛의 현재 상태가 존재하지 않는다면, 저장된 정보로부터 상태를 불러와서 셋팅한다.
	if (enemyComp.mCurrentSM == nullptr)
	{
		enemyComp.mCurrentSM = GetState(enemyComp.mOwnState);
	}
	else //유닛의 현재 상태가 존재한다면
	{
		// 현재 상태와 새로운 상태를 비교하기 위하여, 새로운 스테이트 상태를 선언한다.
		State newStateEnum = enemyComp.mOwnState;

		// 타겟은 무조건 trs가 있어야 하므로 없다면 타깃이 없음을 선언하고 nullptr로 바꿔준다.
		if (enemyComp.mpTarget != nullptr && enemyComp.mpTarget->HasComponent<Transform>() == false)
		{
			enemyComp.mpTarget = nullptr;
			newStateEnum = State::IDLE;
			enemyComp.mCurrentSM = mpIdleState;
		}

		// 유닛의 현재 상태에 맞는 state 상태의 Update 함수를 실행시킨다.
		enemyComp.mCurrentSM->Update(_dTime, _entity);

		// -------------------> DEAD
		if (mpFuncMap["To_DeadE"](_entity))
		{
			newStateEnum = State::DEAD;
		}
		///Dead가 아니라면
		else if (!mpFuncMap.empty())
		{
			auto& tempTarget = enemyComp.mpTarget;

			if (!tempTarget)
			{
				newStateEnum = State::IDLE;
			}
			else
			{
				if (mpFuncMap["To_DamagedE"](_entity))
				{
					newStateEnum = State::DAMAGED;
				}
				else
				{
					switch (newStateEnum)
					{
					case State::IDLE:
					{
						if (mpFuncMap["Target_Is_MoneyE"](_entity))
						{
							if (mpFuncMap["Conflict_MoneyE"](_entity)) //f
							{
								newStateEnum = State::LOOT;
							}
							else
							{
								newStateEnum = State::MOVE; // a
							}
						}
						else if (mpFuncMap["Target_Is_Player"](_entity))
						{
							if (mpFuncMap["Player_In_Range"](_entity)) //b
							{
								newStateEnum = State::ATTACK;
							}
							else
							{
								newStateEnum = State::MOVE;
							}
						}
					}
					break;
					case State::MOVE: // ㄱ, ㄷ, ㅁ
					{
						if (mpFuncMap["None_TargetE"](_entity)) // ㄱ
						{
							newStateEnum = State::IDLE;
						}
						else if (mpFuncMap["Target_Is_MoneyE"](_entity) && mpFuncMap["Conflict_MoneyE"](_entity))
						{
							newStateEnum = State::LOOT;
						}
						else if (mpFuncMap["Target_Is_Player"](_entity) && mpFuncMap["Player_In_Range"](_entity)) // ㅁ
						{
							newStateEnum = State::ATTACK;
						}
					}
					break;
					case State::ATTACK: // ㄴ, e, d
					{
						if (mpFuncMap["None_TargetE"](_entity)) // ㄴ
						{
							newStateEnum = State::IDLE;
						}
						else if (mpFuncMap["Target_Is_MoneyE"](_entity))
						{
							if (mpFuncMap["Conflict_MoneyE"](_entity))
							{
								newStateEnum = State::LOOT;
							}
							else
							{
								newStateEnum = State::MOVE;
							}
						}
						else if (mpFuncMap["Target_Is_Player"](_entity))
						{
							if (!mpFuncMap["Player_In_Range"](_entity))
							{
								newStateEnum = State::MOVE;
							}
						}
					}
					break;
					case State::LOOT: // c, ㄹ, ㅂ
					{
						if (mpFuncMap["None_TargetE"](_entity)) // ㅂ
						{
							newStateEnum = State::IDLE;
						}
						else if (mpFuncMap["Target_Is_Player"](_entity))
						{
							if (mpFuncMap["Player_In_Range"](_entity))
							{
								newStateEnum = State::ATTACK; // ㄹ
							}
							else
							{
								newStateEnum = State::MOVE;// c
							}
						}
						else if (mpFuncMap["Target_Is_MoneyE"](_entity))
						{
							if (!mpFuncMap["Conflict_MoneyE"](_entity))
							{
								newStateEnum = State::MOVE; //c
							}
						}
					}
					break;
					case State::DAMAGED: // ㅅ, ㅇ, ㅈ, ㅊ
					{
						if (mpFuncMap["None_TargetE"](_entity)) // ㅅ
						{
							newStateEnum = State::IDLE;
						}
						else if (mpFuncMap["Target_Is_Player"](_entity))
						{
							if (mpFuncMap["Player_In_Range"](_entity))
							{
								newStateEnum = State::ATTACK; // ㄹ
							}
							else
							{
								newStateEnum = State::MOVE;// ㅇ
							}
						}
						else if (mpFuncMap["Target_Is_MoneyE"](_entity))
						{
							if (mpFuncMap["Conflict_MoneyE"](_entity))
							{
								newStateEnum = State::LOOT; // ㅈ
							}
							else
							{
								newStateEnum = State::MOVE;// ㅇ
							}
						}
					}
					break;
					default:
						break;
					}
				}
			}
		}

		if (mpTransition->IsTransit(enemyComp.mOwnState, newStateEnum))
		{
			ChangeState(_entity, newStateEnum);
		}
	}
}

IState* EnemyFSM::GetState(State _state)
{
	switch (_state)
	{
	case State::IDLE:
		return mpIdleState;
		break;
	case State::MOVE:
		return mpMoveState;
		break;
	case State::ATTACK:
		return mpAttackState;
		break;
	case State::LOOT:
		return mpLootState;
		break;
	case State::DAMAGED:
		return mpDamagedState;
		break;
	case State::DEAD:
		return mpDeadState;
		break;
	default:
		break;
	}
}

void EnemyFSM::FixedUpdate(float _fixedTime, std::shared_ptr<Entity> _entity)
{
	if (_entity->GetComponent<EnemyComponent>().mCurrentSM)
	{
		_entity->GetComponent<EnemyComponent>().mCurrentSM->FixedUpdate(_fixedTime, _entity);
	}
}
