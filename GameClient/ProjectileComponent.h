#pragma once
#include "Component.h"

struct ProjectileComponent
	: public Component
{
public:
	ProjectileComponent(std::shared_ptr<Entity> _owner, float _lifeTime = -1, float _height = -1
		, const Vector3& _targetPosition = Vector3(), float _speed = -1)
		: Component(_owner), mLifeTime(_lifeTime), mHeight(_height), mTargetPosition(_targetPosition)
		, mSpeed(_speed)
	{}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<ProjectileComponent>(*this);
	}

public:
	uint8_t mDamage;
	float mLifeTime;
	float mHeight;
	float mSpeed;
	bool mIsThrown = false;
	bool mIsTriggered = false;	// ĳ���� �� ��ֹ� ��� �浹 ���� ��� true�� ��ȯ
	Vector3 mTargetPosition;
	Vector3 mControl1 = Vector3();		// ������
	Vector3 mControl2 = Vector3();		// ������
};