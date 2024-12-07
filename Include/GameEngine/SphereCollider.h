#pragma once
#include "Component.h"
#include "Collider.h"

/// <summary>
/// 구체 모양을 정의하는 컴포넌트입니다.
/// <para>
/// SphereCollider(bool 트리거 여부, Vector3 센터, float 반지름);
/// </para>
/// </summary>
struct SphereCollider
	: public Collider
{
public:
	SphereCollider(std::shared_ptr<Entity> _owner, bool _isTrigger, const Vector3& _center, float _radius)
		:Collider(_owner, _isTrigger, _center, Vector3(1.f)), mRadius(_radius)
	{}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<SphereCollider>(*this);
	}

public:
	float mRadius;		// 
};