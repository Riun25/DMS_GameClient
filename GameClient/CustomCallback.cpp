#include "CustomCallback.h"
#include "CustomFilter.h"
#include "PhysicsManager.h"
#include "EventManager.h"
#include "EntityManager.h"
#include "Event.h"
#include "Entity.h"

#include "PlayerComponent.h"
#include "EnemyComponent.h"
#include "Transform.h"
#include "ProjectileComponent.h"
#include "MoneyComponent.h"

CustomCallback::CustomCallback(PhysicsManager* _pPhysicsManager, EventManager* _pEventManager, EntityManager* _pEntityManager)
	:m_pPhysicsManager(_pPhysicsManager), EventListener(_pEventManager), m_pEntityManager(_pEntityManager)
{
}

CustomCallback::~CustomCallback()
{
}

void CustomCallback::onContact(const physx::PxContactPairHeader& _pairHeader, const physx::PxContactPair* _pairs, physx::PxU32 _nbPairs)
{
	for (physx::PxU32 i = 0; i < _nbPairs; i++)
	{
		const physx::PxContactPair& cp = _pairs[i];

		if (cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			physx::PxRigidActor* actor0 = _pairHeader.actors[0]->is<physx::PxRigidActor>();
			physx::PxRigidActor* actor1 = _pairHeader.actors[1]->is<physx::PxRigidActor>();

			std::string name = actor0->getName() + std::string(", ") + actor1->getName();
			DLOG(LOG_INFO, "onContact: " + name);

			if (actor0 && actor1)
			{
				physx::PxShape* shape0;
				physx::PxShape* shape1;

				actor0->getShapes(&shape0, 1);
				actor1->getShapes(&shape1, 1);

				physx::PxFilterData filterData0 = shape0->getSimulationFilterData();
				physx::PxFilterData filterData1 = shape1->getSimulationFilterData();

				if (filterData0.word0 & TYPE_WEAPON || filterData1.word0 & TYPE_WEAPON)
				{
					int a = 0;
				}

				if ((filterData0.word0 & TYPE_GOLD) && (filterData1.word0 & TYPE_GROUND))
				{
					filterData0.word1 |= ATTR_ON_GROUND;
					m_pPhysicsManager->SetFilterData(shape0, filterData0);
					m_pPhysicsManager->SetFilterData(shape1, filterData1);
				}
				if ((filterData1.word0 & TYPE_GOLD) && (filterData0.word0 & TYPE_GROUND))
				{
					filterData1.word1 |= ATTR_ON_GROUND;
					m_pPhysicsManager->SetFilterData(shape0, filterData0);
					m_pPhysicsManager->SetFilterData(shape1, filterData1);
				}

				// 
				if ((filterData0.word0 & TYPE_UNIT) && (filterData0.word1 & ~ATTR_IS_DEAD))
				{
					if (filterData1.word0 & TYPE_WEAPON)
					{
						HandleMeleeWeaponHit(actor0, actor1);
					}
					else if (filterData1.word0 & TYPE_GOLD)
					{
						HandleCoinCollision(actor0, actor1);
					}
				}
				else if ((filterData1.word0 & TYPE_UNIT) && (filterData1.word1 & ~ATTR_IS_DEAD))
				{
					if (filterData0.word0 & TYPE_WEAPON)
					{
						HandleMeleeWeaponHit(actor1, actor0);
					}
					else if (filterData0.word0 & TYPE_GOLD)
					{
						HandleCoinCollision(actor1, actor0);
					}
				}

			}
		}
	}
}

void CustomCallback::onTrigger(physx::PxTriggerPair* _pairs, physx::PxU32 _count)
{
	for (physx::PxU32 i = 0; i < _count; i++)
	{
		const physx::PxTriggerPair& tp = _pairs[i];

		if (tp.status & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			// 액터에서 첫 번째 셰이프 가져오기
			physx::PxRigidActor* actor0 = tp.triggerActor->is<physx::PxRigidActor>();
			physx::PxRigidActor* actor1 = tp.otherActor->is<physx::PxRigidActor>();

			std::string name = actor0->getName() + std::string(", ") + actor1->getName();
			DLOG(LOG_INFO, "onTrigger: " + name);

			if (actor0 && actor1)
			{
				physx::PxShape* shape0;
				physx::PxShape* shape1;

				actor0->getShapes(&shape0, 1);
				actor1->getShapes(&shape1, 1);

				physx::PxFilterData filterData0 = shape0->getSimulationFilterData();
				physx::PxFilterData filterData1 = shape1->getSimulationFilterData();

				if (filterData0.word0 & TYPE_UNIT)
				{
					if ((filterData1.word0 & TYPE_PROJECTILE) && (filterData0.word1 & ~ATTR_IS_DEAD))
					{
						HandleProjectileHit(actor0, actor1);
					}
				}
				else if (filterData1.word0 & TYPE_UNIT)
				{
					if ((filterData0.word0 & TYPE_PROJECTILE) && (filterData1.word1 & ~ATTR_IS_DEAD))
					{
						HandleProjectileHit(actor1, actor0);
					}
				}

				if ((filterData0.word0 & TYPE_UNIT) && (filterData0.word1 & ~ATTR_IS_DEAD))
				{
					if (filterData1.word0 & TYPE_WEAPON)
					{
						//m_pEventManager->TriggerEvent(Event("HandleMeleeWeaponHit", actorPair));
						HandleMeleeWeaponHit(actor0, actor1);
					}
					else if (filterData1.word0 & TYPE_GOLD)
					{
						HandleCoinCollision(actor0, actor1);
					}

				}
				else if ((filterData1.word0 & TYPE_UNIT) && (filterData1.word1 & ~ATTR_IS_DEAD))
				{
					if (filterData0.word0 & TYPE_WEAPON)
					{
						HandleMeleeWeaponHit(actor1, actor0);
					}
					else if (filterData0.word0 & TYPE_GOLD)
					{
						HandleCoinCollision(actor1, actor0);
					}

				}

				if (((filterData0.word0 & TYPE_GROUND) || (filterData0.word0 & TYPE_OBSTACLE))
					&& (filterData1.word0 & TYPE_PROJECTILE))
				{
					HandleWallCollision(actor0, actor1);
				}
				else if ((filterData0.word0 & TYPE_PROJECTILE)
					&& ((filterData1.word0 & TYPE_GROUND) || (filterData1.word0 & TYPE_OBSTACLE)))
				{
					HandleWallCollision(actor1, actor0);
				}

			}
		}
	}
}

void CustomCallback::onConstraintBreak(physx::PxConstraintInfo* _constraints, physx::PxU32 _count)
{
	PX_UNUSED((_constraints));
	DLOG(LOG_INFO, "onConstraintBreak");
}

void CustomCallback::onWake(physx::PxActor** _actors, physx::PxU32 _count)
{
	PX_UNUSED((_actors));
	DLOG(LOG_INFO, "onWake");
}

void CustomCallback::onSleep(physx::PxActor** _actors, physx::PxU32 _count)
{
	PX_UNUSED((_actors));
	DLOG(LOG_INFO, "onSleep");
}

void CustomCallback::onAdvance(const physx::PxRigidBody* const* _bodyBuffer, const physx::PxTransform* _poseBuffer, const physx::PxU32 _count)
{
	DLOG(LOG_INFO, "onAdvance");
}

void CustomCallback::HandleProjectileHit(physx::PxRigidActor* _unit, physx::PxRigidActor* _projectile)
{

	/// 투사체가 캐릭터에 충돌했을 때의 이벤트 처리 로직
	//auto actorPair = _pEvent.GetDataAs<std::shared_ptr<std::pair<physx::PxRigidActor*, physx::PxRigidActor*>>>().value();
	// 유닛의 엑터
	//auto first = actorPair->first;
	auto first = _unit;
	// 투사체의 엑터
	//auto second = actorPair->second;
	auto second = _projectile;

	// 유닛의 엔티티
	auto firstEntity = m_pPhysicsManager->GetEntityFromActor(first);
	// 투사체의 엔티티
	auto secondEntity = m_pPhysicsManager->GetEntityFromActor(second);

	DLOG(LOG_INFO, "HandleProjectileHit: " + std::to_string(firstEntity->GetUID()) + '/' + std::to_string(secondEntity->GetUID()));
	// 투사체의 공격력
	auto& damage = secondEntity->GetComponent<ProjectileComponent>().m_damage;

	// 투사체의 공격력만큼 유닛의 HP를 감소시킴
	if (firstEntity->HasComponent<PlayerComponent>())
	{
		auto& hp = firstEntity->GetComponent<PlayerComponent>().mHP;
		//std::cout << "hp: " << hp << '/';
		hp -= damage;
		//std::cout << "damage: " << damage << "/current hp: " << hp;
		secondEntity->GetComponent<ProjectileComponent>().m_isTriggered = true;
	}
	else if (firstEntity->HasComponent<EnemyComponent>())
	{
		auto& hp = firstEntity->GetComponent<EnemyComponent>().mHP;
		//std::cout << "hp: " << hp << '/';
		hp -= damage;
		//std::cout << "damage: " << damage << "/current hp: " << hp;
		secondEntity->GetComponent<ProjectileComponent>().m_isTriggered = true;
	}
}

void CustomCallback::HandleMeleeWeaponHit(physx::PxRigidActor* _unit, physx::PxRigidActor* _weapon)
{
	/// 근거리 무기가 캐릭터에 충돌했을 때의 이벤트 처리 로직
	//auto actorPair = _pEvent.GetDataAs<std::shared_ptr<std::pair<physx::PxRigidActor*, physx::PxRigidActor*>>>().value();
	// 공격받는 유닛의 엑터
	//auto first = actorPair->first;
	auto first = _unit;
	// 무기의 엑터
	//auto second = actorPair->second;
	auto second = _weapon;

	// 공격받는 유닛의 엔티티
	auto firstEntity = m_pPhysicsManager->GetEntityFromActor(first);
	// 무기의 엔티티
	auto secondEntity = m_pPhysicsManager->GetEntityFromActor(second);

	DLOG(LOG_INFO, "HandleMeleeWeaponHit: " + std::to_string(firstEntity->GetUID()) + '/' + std::to_string(secondEntity->GetUID()));

	auto name1 = firstEntity->GetName();
	auto name2 = secondEntity->GetName();
	std::cout << name1 << " / " << name2 << std::endl;

	if (firstEntity->HasComponent<PlayerComponent>())
	{
		// 공격한 캐릭터의 공격력
		float attackPower = 0.f;
		if (secondEntity->GetComponent<Transform>().m_pParent->m_pOwner->HasComponent<EnemyComponent>())
		{
			attackPower = secondEntity->GetComponent<Transform>().m_pParent->m_pOwner->GetComponent<EnemyComponent>().mAttackPower;
		}

		// 공격한 캐릭터의 attakPower만큼 HP를 감소시킴
		auto& hp = firstEntity->GetComponent<PlayerComponent>().mHP;
		hp -= attackPower;
	}
	else if (firstEntity->HasComponent<EnemyComponent>())
	{
		// 공격한 캐릭터의 공격력
		auto& attackPower = secondEntity->GetComponent<Transform>().m_pParent->m_pOwner->GetComponent<PlayerComponent>().mAttackPower;

		// 공격한 캐릭터의 attakPower만큼 HP를 감소시킴
		auto& hp = firstEntity->GetComponent<EnemyComponent>().mHP;
		hp -= attackPower;
	}

}

void CustomCallback::HandleCoinCollision(physx::PxRigidActor* _unit, physx::PxRigidActor* _gold)
{
	/// 동전이 캐릭터와 충돌했을 때의 이벤트 처리 로직
	//auto actorPair = _pEvent.GetDataAs<std::shared_ptr<std::pair<physx::PxRigidActor*, physx::PxRigidActor*>>>().value();
	// 유닛의 엑터
	//auto first = actorPair->first;
	auto first = _unit;
	// 동전의 엑터
	//auto second = actorPair->second;
	auto second = _gold;

	// 유닛의 엔티티
	auto firstEntity = m_pPhysicsManager->GetEntityFromActor(first);
	// 동전의 엔티티
	auto secondEntity = m_pPhysicsManager->GetEntityFromActor(second);
	DLOG(LOG_INFO, "HandleCoinCollision: " + std::to_string(firstEntity->GetUID()) + '/' + std::to_string(secondEntity->GetUID()));

	if (!secondEntity->GetComponent<ProjectileComponent>().m_isTriggered)
	{

		if (firstEntity->HasComponent<PlayerComponent>())
		{

			if (firstEntity->GetComponent<PlayerComponent>().mpTarget == secondEntity)
			{
				secondEntity->GetComponent<ProjectileComponent>().m_isTriggered = true;
			}

		}
		if (firstEntity->HasComponent<EnemyComponent>())
		{
			if (firstEntity->GetComponent<EnemyComponent>().mpTarget == secondEntity)
			{
				secondEntity->GetComponent<ProjectileComponent>().m_isTriggered = true;
			}
		}

		secondEntity->GetComponent<MoneyComponent>().m_pTarget = firstEntity;
	}

	// 	// 동전의 금액
	// 	auto& amount = secondEntity->GetComponent<MoneyComponent>().m_amount;
	// 
	// 	// 캐릭터에게 금액만큼 소지금을 추가
	// 	firstEntity->GetComponent<PlayerComponent>().mMoney + amount;

}

void CustomCallback::HandleWallCollision(physx::PxRigidActor* _wall, physx::PxRigidActor* _projectile)
{
	/// 투사체가 벽과 충돌했을 때의 이벤트 처리 로직
	// 벽의 엑터
	auto first = _wall;
	// 투사체의 엑터
	auto second = _projectile;

	// 벽의 엔티티
	auto firstEntity = m_pPhysicsManager->GetEntityFromActor(first);
	// 투사체의 엔티티
	auto secondEntity = m_pPhysicsManager->GetEntityFromActor(second);

	DLOG(LOG_INFO, "HandleWallCollision: " + std::to_string(firstEntity->GetUID()) + '/' + std::to_string(secondEntity->GetUID()));
	secondEntity->GetComponent<ProjectileComponent>().m_isTriggered = true;
}

