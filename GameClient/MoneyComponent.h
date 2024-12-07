#pragma once
#include "Component.h"

/// <summary>
/// [Riun] 8/14 변수 수정
/// </summary>
struct MoneyComponent
	: public Component
{
public:
	MoneyComponent(std::shared_ptr<Entity> _owner, int _amount = 0, bool _onGround = false)
		:Component(_owner), m_amount(_amount), m_onGround(_onGround), m_IsInVec(false)
	{
	}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<MoneyComponent>(*this);
	}

public:
	int m_amount;	// 금액
	bool m_onGround;
	uint16_t m_num;
	static uint16_t m_currentNum;
	bool m_IsInVec;
	std::shared_ptr<Entity> m_pTarget;
};