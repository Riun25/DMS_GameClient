#pragma once
#include "PhysicsDefine.h"
#include "FilterEnum.h"

/// <summary>
/// 물리 씬의 필터를 설정해주는 클래스입니다.
/// 물리 씬을 생성할때 설정하면 물체들간의 충돌시 어떻게 반응할 것인지 설정할수 있습니다.
/// </summary>
class CustomFilter
{
public:
	static physx::PxFilterFlags CustomFilterShader(
		physx::PxFilterObjectAttributes _attributes0, physx::PxFilterData _filterData0
		, physx::PxFilterObjectAttributes _attributes1, physx::PxFilterData _filterData1
		, physx::PxPairFlags& _pairFlags, const void* _constantBlock, physx::PxU32 _constantBlockSize);

// 	static physx::PxFilterFlags CustomFilterShader2(
// 		physx::PxFilterObjectAttributes _attributes0, physx::PxFilterData _filterData0
// 		, physx::PxFilterObjectAttributes _attributes1, physx::PxFilterData _filterData1
// 		, physx::PxPairFlags& _pairFlags, const void* _constantBlock, physx::PxU32 _constantBlockSize);

};

