#pragma once
#include "PhysicsDefine.h"
#include "pch.h"
#include "physx/PxQueryFiltering.h"

class RaycastQueryFilterCallback :
	public physx::PxQueryFilterCallback
{
public:
	RaycastQueryFilterCallback(const std::string& targetName);

	// 각 액터의 충돌을 처리할 때 호출됩니다.
	physx::PxQueryHitType::Enum preFilter(const physx::PxFilterData& filterData
		, const physx::PxShape* shape, const physx::PxRigidActor* actor, physx::PxHitFlags& queryFlags) override;

	physx::PxQueryHitType::Enum postFilter(const physx::PxFilterData& filterData
		, const physx::PxQueryHit& hit, const physx::PxShape* shape, const physx::PxRigidActor* actor) override;

private:
	std::string mTargetName;

};

