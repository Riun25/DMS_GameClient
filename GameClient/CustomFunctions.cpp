#include "CustomFunctions.h"

CustomFunctions::CustomFunctions()
{
	// -------------------> DEAD
	mFuncMap["To_Dead"] = [](std::shared_ptr<Entity> _entity)->bool
		{
			if (_entity->HasComponent<PlayerComponent>())
			{
				if (_entity->GetComponent<PlayerComponent>().mHP <= 0)
				{
					// 충돌 판정 off
					_entity->RemoveComponent<CapsuleCollider>();
					return true; // newStateEnum = State::DEAD;
				}
			}
			return false; //newStateEnum = State::IDLE;
		};

	mFuncMap["To_DeadE"] = [](std::shared_ptr<Entity> _entity)->bool
		{
			if (_entity->HasComponent<EnemyComponent>())
			{
				if (_entity->GetComponent<EnemyComponent>().mHP <= 0)
				{
					// 충돌 판정 off
					_entity->RemoveComponent<CapsuleCollider>();
					return true; // newStateEnum = State::DEAD;
				}
			}
			return false; //newStateEnum = State::IDLE;
		};

	// -------------------> DAMAGED
	mFuncMap["To_Damaged"] = [](std::shared_ptr<Entity> _entity)->bool
		{
			if (_entity->HasComponent<PlayerComponent>())
			{
				if (_entity->GetComponent<PlayerComponent>().mIsAttacked == true)
				{
					return true;
				}
			}
			return false;
		};

	mFuncMap["To_DamagedE"] = [](std::shared_ptr<Entity> _entity)->bool
		{
			if (_entity->HasComponent<EnemyComponent>())
			{
				if (_entity->GetComponent<EnemyComponent>().mIsAttacked == true)
				{
					return true;
				}
			}
			return false;
		};

	// -------------------> 타겟 == 돈	
	mFuncMap["Target_Is_Money"] = [](std::shared_ptr<Entity> _entity)->bool
		{
			if (_entity->HasComponent<PlayerComponent>())
			{
				return _entity->GetComponent<PlayerComponent>().mpTarget->HasComponent<MoneyComponent>();
			}
		};

	mFuncMap["Target_Is_MoneyE"] = [](std::shared_ptr<Entity> _entity)->bool
		{
			if (_entity->HasComponent<EnemyComponent>())
			{
				return _entity->GetComponent<EnemyComponent>().mpTarget->HasComponent<MoneyComponent>();
			}
		};

	// ------------------->  타겟 == nullptr	
	mFuncMap["None_Target"] = [](std::shared_ptr<Entity> _entity)->bool
		{
			if (_entity->HasComponent<PlayerComponent>())
			{
				return _entity->GetComponent<PlayerComponent>().mpTarget == nullptr;
			}
			return false;
		};

	mFuncMap["None_TargetE"] = [](std::shared_ptr<Entity> _entity)->bool
		{
			if (_entity->HasComponent<EnemyComponent>())
			{
				return _entity->GetComponent<EnemyComponent>().mpTarget == nullptr;
			}
			return false;
		};

	// ------------------->  타겟 == 적(이건 이제 아군 유닛에게만 쓸 함수가 됩니다.)
	mFuncMap["Target_Is_Enemy"] = [](std::shared_ptr<Entity> _entity)->bool
		{
			if (_entity->HasComponent<PlayerComponent>())
			{
				auto& playerComp = _entity->GetComponent<PlayerComponent>();
				// 일단 타겟이 돈이면 안 됨 + 보스도 있을 걸 예상해서 이리 짬
				if (!playerComp.mpTarget->HasComponent<MoneyComponent>())
				{
					if (playerComp.mpTarget->HasComponent<EnemyComponent>())
					{
						return true;
					}
					else
					{
						return false;
					}
				}
			}
			return false;
		};

	mFuncMap["Target_Is_Player"] = [](std::shared_ptr<Entity> _entity)->bool
		{
			if (_entity->HasComponent<EnemyComponent>())
			{
				auto& enemyComp = _entity->GetComponent<EnemyComponent>();
				// 일단 타겟이 돈이면 안 됨 + 보스도 있을 걸 예상해서 이리 짬
				if (!enemyComp.mpTarget->HasComponent<MoneyComponent>())
				{
					if (enemyComp.mpTarget->HasComponent<PlayerComponent>())
					{
						return true;
					}
					else
					{
						return false;
					}
				}
			}
			return false;
		};

	/// 이 부분은 추후에 돈의 위치가 수정되면 수정해야 한다.
	// 돈과 유닛의 충돌 (타겟 == 돈(고정), 돈과 충돌?)
	mFuncMap["Conflict_Money"] = [this](std::shared_ptr<Entity> _entity)->bool
		{
			if (_entity->HasComponent<PlayerComponent>())
			{
				auto& playerComp = _entity->GetComponent<PlayerComponent>();

				if (playerComp.mpTarget->HasComponent<MoneyComponent>())									   //타겟이 돈을 가지고 있는가?
				{
					auto& a1 = _entity->GetComponent<Transform>().m_localPosition;
					//auto& a2 = playerComp.mpTarget->GetComponent<ProjectileComponent>().m_targetPosition;
					auto& a2 = playerComp.mpTarget->GetComponent<Transform>().m_localPosition;

					//float halfTileScale = mTileScale * 0.5f;
					//Vector2 currentPos = Vector2(static_cast<int>((a1.x + halfTileScale) / mTileScale), static_cast<int>((a1.z + halfTileScale) / mTileScale));
					//Vector2 targetPos = Vector2(static_cast<int>((a2.x + halfTileScale) / mTileScale), static_cast<int>((a2.z + halfTileScale) / mTileScale));

					float distance = pow(a1.x - a2.x, 2) + pow(a1.z - a2.z, 2);

					// 에이스타 오류를 줄이기 위하여, distance 보간을 추가하도록 한다. (대각선 거리 1.414의 보정  사용)
					if (playerComp.mpTarget->GetComponent<ProjectileComponent>().m_isTriggered || 1.0f > distance)
					{
						return true;
					}
				}
			}
			return false;
		};

	mFuncMap["Conflict_MoneyE"] = [this](std::shared_ptr<Entity> _entity)->bool
		{
			if (_entity->HasComponent<EnemyComponent>())
			{
				auto& playerComp = _entity->GetComponent<EnemyComponent>();

				if (playerComp.mpTarget->HasComponent<MoneyComponent>())									   //타겟이 돈을 가지고 있는가?
				{
					auto& a1 = _entity->GetComponent<Transform>().m_localPosition;
					//auto& a2 = playerComp.mpTarget->GetComponent<ProjectileComponent>().m_targetPosition;
					auto& a2 = playerComp.mpTarget->GetComponent<Transform>().m_localPosition;

					//float halfTileScale = mTileScale * 0.5f;
					//Vector2 currentPos = Vector2(static_cast<int>((a1.x + halfTileScale) / mTileScale), static_cast<int>((a1.z + halfTileScale) / mTileScale));
					//Vector2 targetPos = Vector2(static_cast<int>((a2.x + halfTileScale) / mTileScale), static_cast<int>((a2.z + halfTileScale) / mTileScale));

					float distance = pow(a1.x - a2.x, 2) + pow(a1.z - a2.z, 2);

					// 에이스타 오류를 줄이기 위하여, distance 보간을 추가하도록 한다. (대각선 거리 1.414의 보정  사용)
					if (playerComp.mpTarget->GetComponent<ProjectileComponent>().m_isTriggered || 1.5f > distance)
					{
						return true;
					}
				}
			}
			return false;
		};

	// 사거리 내에 존재하는가? attackRange >= distance
	mFuncMap["Enemy_In_Range"] = [](std::shared_ptr<Entity> _entity)->bool
		{
			if (_entity->HasComponent<PlayerComponent>())
			{
				auto& playerComp = _entity->GetComponent<PlayerComponent>();
				if (playerComp.mpTarget->HasComponent<EnemyComponent>())
				{
					float distance = (_entity->GetComponent<Transform>().m_localPosition
						- playerComp.mpTarget->GetComponent<Transform>().m_localPosition).Length();

					if (playerComp.mAttackRange >= distance)
					{
						return true;
					}
					else
					{
						return false;
					}
				}
			}
			return false;
		};

	mFuncMap["Player_In_Range"] = [](std::shared_ptr<Entity> _entity)->bool
		{
			if (_entity->HasComponent<EnemyComponent>())
			{
				auto& enemyComp = _entity->GetComponent<EnemyComponent>();

				if (enemyComp.mpTarget->HasComponent<PlayerComponent>())
				{
					float distance = (_entity->GetComponent<Transform>().m_localPosition
						- enemyComp.mpTarget->GetComponent<Transform>().m_localPosition).Length();

					if (enemyComp.mAttackRange >= distance)
					{
						return true;
					}
					else
					{
						return false;
					}
				}
			}
			return false;
		};
}
