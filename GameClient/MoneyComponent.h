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
		:Component(_owner), mAmount(_amount), mIsOnGround(_onGround), mIsInVec(false)
	{
	}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<MoneyComponent>(*this);
	}

public:
	int mAmount;	// 금액
	bool mIsOnGround;
	uint16_t mNum;
	static uint16_t mCurrentNum;
	bool mIsInVec;
	std::shared_ptr<Entity> mpTarget;
};