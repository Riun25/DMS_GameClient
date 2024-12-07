#pragma once
#include "Component.h"
#include "Graphics/ModelInfo.h"

struct WeaponCollider
	: public Component
{
public:
	WeaponCollider(std::shared_ptr<Entity> _owner)
		:Component(_owner)
	{
	}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<WeaponCollider>(*this);
	}

public:
	AABB m_weaponAABB;
};