#pragma once
#include "Component.h"

class Collider :
	public Component
{
public:
	//Collider() {}

	Collider(std::shared_ptr<Entity> _owner, bool _isTrigger = false, const Vector3& _center = Vector3(), const Vector3& _size = Vector3(1.f))
		: Component(_owner), m_isTrigger(_isTrigger), m_center(_center), m_size(_size)
	{}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<Collider>(*this);
	}
public:
	Vector3 m_center;	// 
	Vector3 m_size;
	bool m_isTrigger;	// 

};

