#pragma once
#include "Component.h"
#include "Collider.h"

/// <summary>
/// 캡슐 모양을 정의하는 컴포넌트입니다.
/// <para>
/// CapsuleCollider(bool 트리거 여부, Vector3 센터, float 반지름, float 높이, 캡슐 방향);
/// </para>
/// </summary>
struct CapsuleCollider
	:public Collider
{
public:
	CapsuleCollider(std::shared_ptr<Entity> _owner, bool _isTrigger = false, const Vector3& _center = Vector3()
		, float _radius = 1.f, float _height = 1.f, int _direction = 1)
		: Collider(_owner, _isTrigger, _center, Vector3(1.f))
		, mRadius(_radius), mHeight(_height), mDirection(_direction)
	{}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<CapsuleCollider>(*this);
	}

public:
	float mRadius;        // 캡슐의 반지름
	float mHeight;        // 캡슐의 높이
	int mDirection;       // 캡슐의 방향 (0: X축, 1: Y축, 2: Z축)	// Vector3로 수정할것
};