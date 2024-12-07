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
		: Component(_owner), m_maxLevel(_maxLevel), m_grade(_grade)
		, m_currentLevel(1), m_throwAmount(1), m_throwCoolTime(1)
	{
	}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<MoneyGunComponent>(*this);
	}

public:
	uint8_t m_maxLevel = 2;			// 사용 가능한 머니건의 최대 단계
	uint8_t m_currentLevel = 0;		// 현재 적용 중인 머니건의 단계
	uint8_t m_grade;			// 머니건의 강화 단계
	uint8_t m_throwAmount;		// 투척 1회당 던져지는 금액
	float m_throwCoolTime;		// 투척 후 다음 투척까지 대기 시간

};

