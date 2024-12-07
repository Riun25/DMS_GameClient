#pragma once
#include "Component.h"

class Collider :
	public Component
{
public:
	//Collider() {}

	Collider(std::shared_ptr<Entity> _owner, bool _isTrigger = false, const Vector3& _center = Vector3(), const Vector3& _size = Vector3(1.f))
		: Component(_owner), mIsTrigger(_isTrigger), mCenter(_center), mSize(_size)
	{}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<Collider>(*this);
	}
public:
	Vector3 mCenter;	// 
	Vector3 mSize;
	bool mIsTrigger;	// 

};

