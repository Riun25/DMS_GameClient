#pragma once
#include "Component.h"

/// <summary>
/// 돈 
/// </summary>
struct MoneyGunComponent
	: public Component
{
public:
	MoneyGunComponent(std::shared_ptr<Entity> _owner, uint8_t _maxLevel, uint8_t _grade)
		: Component(_owner), mMaxLevel(_maxLevel), mGrade(_grade)
		, mCurrentLevel(1), mThrowAmount(1), mThrowCoolTime(1)
	{
	}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<MoneyGunComponent>(*this);
	}

public:
	uint8_t mMaxLevel = 2;			// 사용 가능한 머니건의 최대 단계
	uint8_t mCurrentLevel = 0;		// 현재 적용 중인 머니건의 단계
	uint8_t mGrade;			// 머니건의 강화 단계
	uint8_t mThrowAmount;		// 투척 1회당 던져지는 금액
	float mThrowCoolTime;		// 투척 후 다음 투척까지 대기 시간

};

