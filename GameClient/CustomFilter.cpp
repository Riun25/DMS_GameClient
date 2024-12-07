#include "CustomFilter.h"

physx::PxFilterFlags CustomFilter::CustomFilterShader(physx::PxFilterObjectAttributes _attributes0, physx::PxFilterData _filterData0, physx::PxFilterObjectAttributes _attributes1, physx::PxFilterData _filterData1, physx::PxPairFlags& _pairFlags, const void* _constantBlock, physx::PxU32 _constantBlockSize)
{
	// 지면과 충돌했을 경우
	if ((_filterData0.word0 & TYPE_GROUND) || (_filterData1.word0 & TYPE_GROUND))
	{
		// 지면끼리는 충돌하지 않음
		if ((_filterData0.word0 & TYPE_GROUND) && (_filterData1.word0 & TYPE_GROUND))
		{
			return physx::PxFilterFlag::eSUPPRESS;
		}
		else if ((_filterData0.word0 & TYPE_UNIT) || (_filterData1.word0 & TYPE_UNIT))
		{
			_pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT | physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
			return physx::PxFilterFlag::eDEFAULT;
		}
		// 근접무기는 충돌감지하지 않음
		else if ((_filterData0.word0 & TYPE_WEAPON) || (_filterData1.word0 & TYPE_WEAPON))
		{
			return physx::PxFilterFlag::eSUPPRESS;
		}
		// 투사체의 경우 객체 삭제시키는 로직 실행
		else if ((_filterData0.word0 & TYPE_PROJECTILE) || (_filterData1.word0 & TYPE_PROJECTILE))
		{
			_pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT | physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
			return physx::PxFilterFlag::eDEFAULT;
		}
		// 돈의 on ground 플래그 추가
		else if (_filterData0.word0 & TYPE_GOLD)
		{
			_filterData0.word1 |= ATTR_ON_GROUND;
			_pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT | physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
			return physx::PxFilterFlag::eDEFAULT;

		}
		else if (_filterData1.word0 & TYPE_GOLD)
		{
			_filterData1.word1 |= ATTR_ON_GROUND;
			_pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT | physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
			return physx::PxFilterFlag::eDEFAULT;

		}

		_pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;
		return physx::PxFilterFlag::eDEFAULT;
	}

	// 장애물과 충돌했을 경우
	if ((_filterData0.word0 & TYPE_OBSTACLE) || (_filterData1.word0 & TYPE_OBSTACLE))
	{
		// 다른 장애물과는 충돌하지 않음
		if ((_filterData0.word0 & TYPE_OBSTACLE) && (_filterData1.word0 & TYPE_OBSTACLE))
		{
			return physx::PxFilterFlag::eSUPPRESS;
		}
		// 근접무기는 충돌감지하지 않음
		else if ((_filterData0.word0 & TYPE_WEAPON) || (_filterData1.word0 & TYPE_WEAPON))
		{
			return physx::PxFilterFlag::eSUPPRESS;
		}
		// 투사체의 경우 sleep상태로 바꿈
		else if ((_filterData0.word0 & TYPE_PROJECTILE) || (_filterData1.word0 & TYPE_PROJECTILE))
		{
// 			_pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT | physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
// 			return physx::PxFilterFlag::eDEFAULT;
			return physx::PxFilterFlag::eSUPPRESS;
		}
		else if ((_filterData0.word0 & TYPE_GOLD) || (_filterData1.word0 & TYPE_GOLD))
		{
			return physx::PxFilterFlag::eSUPPRESS;
		}

		// 지면, 유닛, 다른 장애물과는 정상적으로 충돌함
		_pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;
		return physx::PxFilterFlag::eDEFAULT;
	}

	// 유닛과 충돌했을 경우
	if ((_filterData0.word0 & TYPE_UNIT) || (_filterData1.word0 & TYPE_UNIT))
	{
		if ((_filterData0.word1 & ATTR_IS_DEAD) || (_filterData1.word1 & ATTR_IS_DEAD))
		{
			return physx::PxFilterFlag::eSUPPRESS;
		}

		// 유닛끼리는 서로 충돌 반응함
		if ((_filterData0.word0 & TYPE_UNIT) && (_filterData1.word0 & TYPE_UNIT))
		{
// 			if ((_filterData0.word1 & ATTR_ALLY) != (_filterData1.word1 & ATTR_ALLY))
			{
				_pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;
				return physx::PxFilterFlag::eDEFAULT;
			}
// 			else
// 			{
// 				return physx::PxFilterFlag::eSUPPRESS;
// 			}
		}
		// 다른 팀의 근접무기는 충돌 후 트리거 발동
		else if ((_filterData0.word0 & TYPE_WEAPON) || (_filterData1.word0 & TYPE_WEAPON))
		{
			// 서로 다른 팀일 경우
			if ((_filterData0.word1 & ATTR_ALLY) != (_filterData1.word1 & ATTR_ALLY))
			{
				_pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT | physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
				return physx::PxFilterFlag::eDEFAULT;
			}

			// 이외의 다른 객체들과는 충돌하지 않음
			return physx::PxFilterFlag::eSUPPRESS;
		}
		else if ((_filterData0.word0 & TYPE_GOLD) || (_filterData1.word0 & TYPE_GOLD))
		{
			if ((_filterData0.word1 & ATTR_ON_GROUND) || (_filterData1.word1 & ATTR_ON_GROUND))
			{
				_pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT | physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
				return physx::PxFilterFlag::eDEFAULT;
			}

			return physx::PxFilterFlag::eSUPPRESS;
		}
		else if ((_filterData0.word0 & TYPE_PROJECTILE) || (_filterData1.word0 & TYPE_PROJECTILE))
		{
			// 서로 다른 팀일 경우
			if ((_filterData0.word1 & ATTR_ALLY) != (_filterData1.word1 & ATTR_ALLY))
			{
				_pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT | physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
				return physx::PxFilterFlag::eDEFAULT;
			}

			return physx::PxFilterFlag::eSUPPRESS;
		}

	}

	// 돈과 충돌했을 경우
	if ((_filterData0.word0 & TYPE_GOLD) || (_filterData1.word0 & TYPE_GOLD))
	{
		if ((_filterData0.word0 & TYPE_GOLD) && (_filterData1.word0 & TYPE_GOLD))
		{
			// 머지 할 수 있도록
		}

		return physx::PxFilterFlag::eSUPPRESS;
	}

	return physx::PxFilterFlag::eSUPPRESS;

}

// physx::PxFilterFlags CustomFilter::CustomFilterShader2(
// 	physx::PxFilterObjectAttributes _attributes0, physx::PxFilterData _filterData0
// 	, physx::PxFilterObjectAttributes _attributes1, physx::PxFilterData _filterData1
// 	, physx::PxPairFlags& _pairFlags, const void* _constantBlock, physx::PxU32 _constantBlockSize)
// {
// 	// 투사체들끼리 충돌하지 않도록 설정
// 	if ((_filterData0.word0 & TYPE_PROJECTILE) && (_filterData1.word0 & TYPE_PROJECTILE))
// 	{
// 		return physx::PxFilterFlag::eSUPPRESS;
// 	}
// 
// 	if ((_filterData0.word0 & TYPE_GOLD) && (_filterData1.word0 & TYPE_GOLD))
// 	{
// 		_pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT | physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
// 		return physx::PxFilterFlag::eSUPPRESS;
// 	}
// 
// 	// 공중에서 동전과 지면 외의 다른 객체는 충돌하지 않음
// 	if (((_filterData0.word0 & TYPE_GOLD) && !(_filterData1.word0 & TYPE_GROUND)) ||
// 		((_filterData1.word0 & TYPE_GOLD) && !(_filterData0.word0 & TYPE_GROUND)))
// 	{
// 		if (_filterData0.word1 & ~ATTR_ON_GROUND || _filterData1.word1 & ~ATTR_ON_GROUND)
// 		{
// 			return physx::PxFilterFlag::eSUPPRESS;
// 		}
// 	}
// 
// 	// 같은 팀일 경우에도 유닛끼리는 충돌하도록 설정
// 	if (((_filterData0.word1 & ATTR_ALLY) && (_filterData1.word1 & ATTR_ALLY)) ||
// 		((_filterData0.word1 & ATTR_ENEMY) && (_filterData1.word1 & ATTR_ENEMY)))
// 	{
// 		// 유닛끼리 충돌 발생
// 		if ((_filterData0.word0 & TYPE_UNIT) && (_filterData1.word0 & TYPE_UNIT))
// 		{
// 			_pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;
// 			return physx::PxFilterFlag::eDEFAULT;
// 		}
// 
// 		// 유닛이 아닌 다른 경우는 충돌 억제
// 		_pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT;
// 		return physx::PxFilterFlag::eDEFAULT;
// 	}
// 
// 	// 무기와 유닛의 충돌 처리 (트리거 발동)
// 	if (((_filterData0.word0 & TYPE_WEAPON) && (_filterData1.word0 & TYPE_UNIT)) ||
// 		((_filterData0.word0 & TYPE_UNIT) && (_filterData1.word0 & TYPE_WEAPON)))
// 	{
// 		_pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT | physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
// 		return physx::PxFilterFlag::eDEFAULT;
// 	}
// 
// 	// 금화가 지면과 충돌했을 때 처리
// 	if ((_filterData0.word0 & TYPE_GOLD) && (_filterData1.word0 & TYPE_GROUND))
// 	{
// 		_filterData0.word1 |= ATTR_ON_GROUND;
// 	}
// 	else if ((_filterData1.word0 & TYPE_GOLD) && (_filterData0.word0 & TYPE_GROUND))
// 	{
// 		_filterData1.word1 |= ATTR_ON_GROUND;
// 	}
// 
// 	// 금화와 유닛의 루팅 처리
// 	if (((_filterData0.word0 & TYPE_GOLD) && (_filterData1.word0 & TYPE_UNIT)) ||
// 		((_filterData1.word0 & TYPE_GOLD) && (_filterData0.word0 & TYPE_UNIT)))
// 	{
// 		_pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT | physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
// 		return physx::PxFilterFlag::eSUPPRESS;
// 	}
// 
// 	// 투사체가 장애물 및 지면과 충돌할 때 처리
// 	if (((_filterData0.word0 & TYPE_PROJECTILE) && (_filterData1.word0 & TYPE_OBSTACLE)) ||
// 		((_filterData0.word0 & TYPE_OBSTACLE) && (_filterData1.word0 & TYPE_PROJECTILE)) ||
// 		((_filterData0.word0 & TYPE_PROJECTILE) && (_filterData1.word0 & TYPE_GROUND)) ||
// 		((_filterData0.word0 & TYPE_GROUND) && (_filterData1.word0 & TYPE_PROJECTILE)))
// 	{
// 		_pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT | physx::PxPairFlag::eNOTIFY_TOUCH_FOUND | physx::PxPairFlag::eNOTIFY_TOUCH_LOST;
// 		return physx::PxFilterFlag::eKILL;
// 	}
// 
// 	// 기본 충돌 처리
// 	_pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT | physx::PxPairFlag::eNOTIFY_TOUCH_FOUND | physx::PxPairFlag::eNOTIFY_TOUCH_LOST;
// 	return physx::PxFilterFlag::eDEFAULT;
// }
