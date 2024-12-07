#pragma once
#include "CollisionCallback.h"
#include "EventListener.h"

class PhysicsManager;
class EventManager;
class EntityManager;

/// <summary>
/// 게임에서 실제로 사용될 물리 시뮬레이션 이벤트 콜백 객체입니다.
/// </summary>
class CustomCallback
	: public physx::PxSimulationEventCallback 
	, public EventListener
{
public:
	CustomCallback(PhysicsManager* _pPhysicsManager, EventManager* _pEventManager, EntityManager* _pEntityManager);
	~CustomCallback();

	/// <summary>
	/// 충돌 이벤트 콜백 메서드 
	/// </summary>
	/// <param name="_pairHeader">충돌한 두 객체에 대한 정보를 포함하는 구조체입니다.</param>
	/// <param name="_pairs">충돌한 객체 쌍들의 배열입니다. 각 배열 요소는 충돌 쌍에 대한 세부 정보를 포함합니다.</param>
	/// <param name="_nbPairs">충돌한 객체 쌍의 수를 나타냅니다.</param>
	 void onContact(const physx::PxContactPairHeader& _pairHeader, const physx::PxContactPair* _pairs, physx::PxU32 _nbPairs) override;

	/// <summary>
	/// 트리거 이벤트 콜백 메서드 
	/// </summary>
	/// <param name="_pairs">트리거된 객체 쌍들의 배열입니다. 각 배열 요소는 트리거 이벤트에 대한 정보를 포함합니다.</param>
	/// <param name="_count">트리거된 객체 쌍의 수를 나타냅니다.</param>
	 void onTrigger(physx::PxTriggerPair* _pairs, physx::PxU32 _count) override;

	/// <summary>
	/// 제약 조건 파괴 이벤트 콜백 메서드 
	/// </summary>
	/// <param name="_constraints">파괴된 제약들의 배열입니다. 각 배열 요소는 파괴된 제약에 대한 정보를 포함합니다.</param>
	/// <param name="_count">파괴된 제약의 수를 나타냅니다.</param>
	 void onConstraintBreak(physx::PxConstraintInfo* _constraints, physx::PxU32 _count) override;

	/// <summary>
	/// 객체가 깨어나는 이벤트 콜백 메서드 
	/// </summary>
	/// <param name="_actors">깨어난 객체들에 대한 포인터 배열입니다.</param>
	/// <param name="_count">깨어난 객체의 수를 나타냅니다.</param>
	 void onWake(physx::PxActor** _actors, physx::PxU32 _count) override;

	/// <summary>
	/// 객체가 잠드는 이벤트 콜백 메서드 
	/// </summary>
	/// <param name="_actors">잠든 객체들에 대한 포인터 배열입니다.</param>
	/// <param name="_count">잠든 객체의 수를 나타냅니다.</param>
	 void onSleep(physx::PxActor** _actors, physx::PxU32 _count) override;

	/// <summary>
	/// 시뮬레이션 상태 업데이트 이벤트 콜백 메서드 
	/// </summary>
	/// <param name="_bodyBuffer">업데이트된 강체 객체들의 포인터 배열입니다.</param>
	/// <param name="_poseBuffer">각 강체 객체의 새로운 위치와 회전을 나타내는 배열입니다.</param>
	/// <param name="_count">업데이트된 강체의 수를 나타냅니다.</param>
	 void onAdvance(const physx::PxRigidBody* const* _bodyBuffer, const physx::PxTransform* _poseBuffer, const physx::PxU32 _count) override;

	 /// <summary>
	 /// pair<PxRigidActor*, PxRigidActor*>를 이벤트 인자로 전달
	 /// first: TYPE_UNIT, second: TYPE_PROJECTILE
	 /// </summary>
	 /// <param name="_pEvent">std::pair<physx::PxRigidActor*, physx::PxRigidActor*></param>
	 //void HandleProjectileHit(const Event& _pEvent);
	 void HandleProjectileHit(physx::PxRigidActor* _unit, physx::PxRigidActor* _projectile);

	 /// <summary>
	 /// pair<PxRigidActor*, PxRigidActor*>를 이벤트 인자로 전달
	 /// first: TYPE_UNIT, second: TYPE_WEAPON
	 /// </summary>
	 /// <param name="_pEvent">std::pair<physx::PxRigidActor*, physx::PxRigidActor*></param>	
	 //void HandleMeleeWeaponHit(const Event& _pEvent);
	 void HandleMeleeWeaponHit(physx::PxRigidActor* _unit, physx::PxRigidActor* _weapon);

	 /// <summary>
	 /// pair<PxRigidActor*, PxRigidActor*>를 이벤트 인자로 전달
	 /// first: TYPE_UNIT, second: TYPE_GOLD
	 /// </summary>
	 /// <param name="_pEvent">std::pair<physx::PxRigidActor*, physx::PxRigidActor*></param>
	 //void HandleCoinCollision(const Event& _pEvent);
	 void HandleCoinCollision(physx::PxRigidActor* _unit, physx::PxRigidActor* _gold);

	 void HandleWallCollision(physx::PxRigidActor* _wall, physx::PxRigidActor* _projectile);

private:

	PhysicsManager* m_pPhysicsManager;
	EntityManager* m_pEntityManager;
};

