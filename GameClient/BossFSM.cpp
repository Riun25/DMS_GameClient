//#include "BossFSM.h"
//#include "CapsuleCollider.h"
//
//BossFSM::BossFSM(ClientAnimator* _animator)
//{
//	Initialize(_animator);
//	currentSM = GetState(State::IDLE);
//	currentSM->Initialize();
//	mpTransition = new Transition(State::IDLE);
//	//currentSM->Enter(); // 이 주석 지우지 마세요. 살려야 하는데 고민을 미루는 중임.
//	MakeFunction();
//}
//
//BossFSM::~BossFSM()
//{
//	delete mpTransition;
//}
//
//void BossFSM::Initialize(ClientAnimator* _animator)
//{
//	mStatePool[State::IDLE] = new IdleState(_animator);
//	mStatePool[State::MOVE] = new MoveState(_animator);
//	mStatePool[State::ATTACK] = new AttackState(_animator);
//	mStatePool[State::DEAD] = new DeadState(_animator);
//}
//
//void BossFSM::MakeFunction()
//{
//	/// 적군, 아군 공용
//// IDLE -------------> ATTACK
//	mFuncMap["Idle_To_Attack"] = [](std::shared_ptr<Entity> a)->bool
//		{
//			// is아군 != 타깃is아군 && 공격 쿨타임 <= 0
//			if ((a->GetComponent<PlayerComponent>().mIsAlly != a->GetComponent<PlayerComponent>().mpTarget->GetComponent<PlayerComponent>().mIsAlly)
//				&& a->GetComponent<PlayerComponent>().mAttackCoolTime <= 0)
//			{
//				return true;
//			}
//			return false; // State::IDLE;
//		};
//
//	// IDLE --------------> MOVE
//	mFuncMap["Idle_To_Move"] = [](std::shared_ptr<Entity> a)->bool
//		{
//
//			if (a->GetComponent<PlayerComponent>().mpTarget->GetComponent<PlayerComponent>().mIsMoney == true ||
//				a->GetComponent<PlayerComponent>().mIsAlly != a->GetComponent<PlayerComponent>().mpTarget->GetComponent<PlayerComponent>().mIsAlly)
//			{
//				return true; // State::MOVE;
//			}
//			return false; // State::IDLE;
//		};
//
//	// IDLE --------------> LOOT
//	mFuncMap["Idle_To_Loot"] = [](std::shared_ptr<Entity> a)->bool
//		{
//			// 사거리 계산
//			if ((a->GetComponent<PlayerComponent>().mpTarget->GetComponent<PlayerComponent>().mIsMoney == true))
//			{
//				return true;
//			}
//			return false; // State::LOOT;
//		};
//
//	// MOVE --------------> IDLE  
//	mFuncMap["Move_To_Idle"] = [](std::shared_ptr<Entity> a)->bool
//		{
//			//목표 == nullptr ||  공격 대상이 적이고, 사거리 < 목표와의 거리)
//			if (!(a->GetComponent<PlayerComponent>().mpTarget) ||
//				a->GetComponent<PlayerComponent>().mIsAlly != a->GetComponent<PlayerComponent>().mpTarget->GetComponent<PlayerComponent>().mIsAlly
//				&& a->GetComponent<PlayerComponent>().mAttackRange > (a->GetComponent<PlayerComponent>().mpTarget->GetComponent<Transform>().m_localPosition - a->GetComponent<Transform>().m_localPosition).Length())
//			{
//				return true;
//			}
//			return false; // State::MOVE;
//		};
//
//	// MOVE ------------> LOOT
//	mFuncMap["Move_To_Loot"] = [](std::shared_ptr<Entity> a)->bool
//		{
//			if ((a->GetComponent<PlayerComponent>().mpTarget->GetComponent<PlayerComponent>().mIsMoney == true))
//			{
//				return true;
//			}
//			return false; // State::LOOT;
//
//		};
//
//	// ATTACK ------------> IDLE
//	mFuncMap["Attack_To_Idle"] = [](std::shared_ptr<Entity> a)->bool
//		{
//			//목표 == 돈 ||  공격 애니메이션 시간 끝남 -> 이 공격 애니메이션 시간이 끝나는 거는... AttackState에서 처리해야 하나?
//			// 타겟이 돈으로 바뀜...?
//			if ((a->GetComponent<PlayerComponent>().mpTarget->GetComponent<PlayerComponent>().mIsMoney == true))
//			{
//				return true;
//			}
//			return false; // State::ATTACK;
//		};
//
//	// LOOT --------------> IDLE
//	mFuncMap["Loot_To_Idle"] = [](std::shared_ptr<Entity> a)->bool
//		{
//			// 타겟이 없어짐 && 루팅 시간이 끝남
//			if (!(a->GetComponent<PlayerComponent>().mpTarget))// &&
//				//지나간 시간 > a->GetComponent<PlayerComponent>().mLootingTime)
//			{
//				return true; // State::IDLE;
//			}
//			return false; // State::LOOT;
//		};
//
//	// // -------------------> DEAD
//	mFuncMap["To_Dead"] = [](std::shared_ptr<Entity> _entity)->bool
//		{
//			if (_entity->GetComponent<PlayerComponent>().mHP <= 0)
//			{
//				// 충돌 판정 off
//				_entity->RemoveComponent<CapsuleCollider>();/// 이게 맞는지 모르겠네... 그냥 바닥 아래로 내려버려도 될 듯함
//				return true; // newStateEnum = State::DEAD;
//			}
//			else
//			{
//				return false; //newStateEnum = State::IDLE;
//			}
//		};
//
//	// 돈과 유닛의 충돌
//	mFuncMap["Conflict_Money"] = [](std::shared_ptr<Entity> _entity)->bool
//		{
//			float distance = (_entity->GetComponent<Transform>().m_localPosition
//				- _entity->GetComponent<PlayerComponent>().mpTarget->GetComponent<Transform>().m_localPosition).Length();
//
//
//			if (distance <= 0.1f)
//			{
//				// 충돌 판정 ON
//				return true;
//			}
//			else
//			{
//				return false;
//			}
//		};
//
//	// 적이고, 사거리 내에 존재하는가?
//	mFuncMap["Enemy_In_Range"] = [](std::shared_ptr<Entity> _entity)->bool
//		{
//			float distance = (_entity->GetComponent<Transform>().m_localPosition
//				- _entity->GetComponent<PlayerComponent>().mpTarget->GetComponent<Transform>().m_localPosition).Length();
//
//
//			if (_entity->GetComponent<PlayerComponent>().mIsAlly != _entity->GetComponent<PlayerComponent>().mpTarget->GetComponent<PlayerComponent>().mIsAlly
//				&& _entity->GetComponent<PlayerComponent>().mAttackRange > distance)
//			{
//				return true;
//			}
//			else
//			{
//				return false;
//			}
//		};
//}
//
//IState* BossFSM::GetState(State _state)
//{
//	auto it = mStatePool.find(_state);
//	if (it != mStatePool.end())
//	{
//		return it->second;
//	}
//	return nullptr;
//}
//
//void BossFSM::ChangeState(std::shared_ptr<Entity> _entity, State _newStateEnum)
//{
//	if (mpTransition->IsCanTransitionTo(_newStateEnum))
//	{
//		if (currentSM != nullptr)
//		{
//			currentSM->Exit(_entity);
//		}
//
//		currentSM = GetState(_newStateEnum);
//		//currentStateEnum = _newStateEnum;
//		_entity->GetComponent<PlayerComponent>().mOwnState = _newStateEnum;
//
//		if (currentSM != nullptr)
//		{
//			currentSM->Enter(_entity);
//		}
//	}
//	else
//	{
//		std::cout << "Transition from " << static_cast<int>(_entity->GetComponent<PlayerComponent>().mOwnState)
//			<< " to " << static_cast<int>(_newStateEnum) << " is not allowed.\n";
//	}
//}
//
//void BossFSM::Update(float _dTime, std::shared_ptr<Entity> _entity)
//{
//	///TODO_Riun : BOSS 추가할 것 + PowerPoint 참고
//	if (currentSM == nullptr)
//	{
//		_entity->GetComponent<PlayerComponent>().mOwnState = _entity->GetComponent<PlayerComponent>().mOwnState;
//	}
//	else
//	{
//		currentSM->Update(_dTime, _entity);
//
//		State newStateEnum = _entity->GetComponent<PlayerComponent>().mOwnState;
//
//
//		// -------------------> DEAD
//		if (mFuncMap["To_Dead"](_entity))
//		{
//			newStateEnum = State::DEAD;
//		}
//		///Dead가 아니라면
//		else if (!mFuncMap.empty())
//		{
//			auto& tempTarget = _entity->GetComponent<PlayerComponent>().mpTarget;
//
//			if (!tempTarget)
//			{
//				newStateEnum = State::IDLE;
//			}
//			else
//			{
//				// 사거리 계산
//				float distance = (_entity->GetComponent<Transform>().m_localPosition
//					- tempTarget->GetComponent<Transform>().m_localPosition).Length();
//
//				switch (newStateEnum)
//				{
//				case State::IDLE:
//				{
//					if (mFuncMap["Conflict_Money"](_entity))
//					{
//						newStateEnum = State::LOOT;
//					}
//					else
//					{
//						if (mFuncMap["Enemy_In_Range"](_entity))
//						{
//							if (_entity->GetComponent<PlayerComponent>().mAttackCoolTime <= 0)
//							{
//								newStateEnum = State::ATTACK;
//							}
//							else
//							{
//								newStateEnum = State::IDLE;
//							}
//						}
//						else
//						{
//							newStateEnum = State::MOVE;
//						}
//					}
//				}
//				break;
//				case State::MOVE:
//				{
//					if (mFuncMap["Conflict_Money"](_entity))
//					{
//						newStateEnum = State::LOOT;
//					}
//					else
//					{
//						if (mFuncMap["Enemy_In_Range"](_entity))
//						{
//							newStateEnum = State::IDLE;
//						}
//						else
//						{
//							newStateEnum = State::MOVE;
//						}
//					}
//				}
//				break;
//				case State::ATTACK:
//				{
//					if (_entity->GetComponent<PlayerComponent>().mAttackCoolTime < 1.3f) /// [★Riun : 임의 변수임! 고칠 것!]
//					{
//						newStateEnum = State::IDLE;
//					}
//					else
//					{
//						newStateEnum = State::ATTACK;
//					}
//				}
//				break;
//				case State::LOOT:
//				{
//					if (_entity->GetComponent<PlayerComponent>().mIsMoney = true &&
//						_entity->GetComponent<PlayerComponent>().mRecogRange > distance)  /// [★Riun : 임의 변수임! 고칠 것!] enter 시작된 시점부터의 시간 
//					{
//						newStateEnum = State::LOOT;
//					}
//					else
//					{
//						newStateEnum = State::IDLE;
//					}
//				}
//				break;
//				default:
//					break;
//				}
//			}
//		}
//
//		if (mpTransition->IsTransit(_entity->GetComponent<PlayerComponent>().mOwnState, newStateEnum))
//		{
//			ChangeState(_entity, newStateEnum);
//		}
//	}
//}
