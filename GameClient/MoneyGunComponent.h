#pragma once
#include "Component.h"

/// <summary>
/// �� 
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
	uint8_t mMaxLevel = 2;			// ��� ������ �Ӵϰ��� �ִ� �ܰ�
	uint8_t mCurrentLevel = 0;		// ���� ���� ���� �Ӵϰ��� �ܰ�
	uint8_t mGrade;			// �Ӵϰ��� ��ȭ �ܰ�
	uint8_t mThrowAmount;		// ��ô 1ȸ�� �������� �ݾ�
	float mThrowCoolTime;		// ��ô �� ���� ��ô���� ��� �ð�

};

