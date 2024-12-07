#pragma once
#include "Logger.h"
#include "PhysicsDefine.h"

/// <summary>
/// 간단한 충돌 이벤트 처리를 위한 콜백 객체입니다.
/// 2024.06.03 _Doyo;
/// </summary>
class CollisionCallback
	: public physx::PxSimulationEventCallback
{
public:
	CollisionCallback() = default;
	~CollisionCallback() = default;

	/// <summary>
	/// 충돌 이벤트 콜백 메서드 
	/// </summary>
	/// <param name="_pairHeader">충돌한 두 객체에 대한 정보를 포함하는 구조체입니다.</param>
	/// <param name="_pairs">충돌한 객체 쌍들의 배열입니다. 각 배열 요소는 충돌 쌍에 대한 세부 정보를 포함합니다.</param>
	/// <param name="_nbPairs">충돌한 객체 쌍의 수를 나타냅니다.</param>
	virtual void onContact(const physx::PxContactPairHeader& _pairHeader, const physx::PxContactPair* _pairs, physx::PxU32 _nbPairs) override
	{
		DLOG(LOG_INFO, "onContact()");

		for (physx::PxU32 i = 0; i < _nbPairs; i++)
		{
			const physx::PxContactPair& cp = _pairs[i];
			if (cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
			}
		}
	}

	/// <summary>
	/// 트리거 이벤트 콜백 메서드 
	/// </summary>
	/// <param name="_pairs">트리거된 객체 쌍들의 배열입니다. 각 배열 요소는 트리거 이벤트에 대한 정보를 포함합니다.</param>
	/// <param name="_count">트리거된 객체 쌍의 수를 나타냅니다.</param>
	virtual void onTrigger(physx::PxTriggerPair* _pairs, physx::PxU32 _count) override
	{
		DLOG(LOG_INFO, "onTrigger()");
	}

	/// <summary>
	/// 제약 파괴 이벤트 콜백 메서드 
	/// </summary>
	/// <param name="_constraints">파괴된 제약들의 배열입니다. 각 배열 요소는 파괴된 제약에 대한 정보를 포함합니다.</param>
	/// <param name="_count">파괴된 제약의 수를 나타냅니다.</param>
	virtual void onConstraintBreak(physx::PxConstraintInfo* _constraints, physx::PxU32 _count) override
	{
		DLOG(LOG_INFO, "onConstraintBreak()");
	}

	/// <summary>
	/// 객체 깨어남 이벤트 콜백 메서드 
	/// </summary>
	/// <param name="_actors">깨어난 객체들에 대한 포인터 배열입니다.</param>
	/// <param name="_count">깨어난 객체의 수를 나타냅니다.</param>
	virtual void onWake(physx::PxActor** _actors, physx::PxU32 _count) override
	{
		DLOG(LOG_INFO, "onWake()");
	}

	/// <summary>
	/// 객체 잠듦 이벤트 콜백 메서드 
	/// </summary>
	/// <param name="_actors">잠든 객체들에 대한 포인터 배열입니다.</param>
	/// <param name="_count">잠든 객체의 수를 나타냅니다.</param>
	virtual void onSleep(physx::PxActor** _actors, physx::PxU32 _count) override
	{
		DLOG(LOG_INFO, "onSleep()");
	}

	/// <summary>
	/// 시뮬레이션 상태 업데이트 이벤트 콜백 메서드 
	/// </summary>
	/// <param name="_bodyBuffer">업데이트된 강체 객체들의 포인터 배열입니다.</param>
	/// <param name="_poseBuffer">각 강체 객체의 새로운 위치와 회전을 나타내는 배열입니다.</param>
	/// <param name="_count">업데이트된 강체의 수를 나타냅니다.</param>
	virtual void onAdvance(const physx::PxRigidBody* const* _bodyBuffer, const physx::PxTransform* _poseBuffer, const physx::PxU32 _count) override
	{
		DLOG(LOG_INFO, "onAdvance()");
	}
};