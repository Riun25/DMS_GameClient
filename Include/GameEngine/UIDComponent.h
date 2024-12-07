#pragma once
#include "Component.h"
#include "Utility.h"

class UIDComponent :
	public Component
{
public:
	UIDComponent(std::shared_ptr<Entity> _owner)
		: Component(_owner)
		, m_uid(UIDGenerator::GenerateUID<Entity>())
	{}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<UIDComponent>(*this);
	}
public:
	UID m_uid;
};

