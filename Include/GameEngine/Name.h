#pragma once
#include "Component.h"

class Name :
	public Component
{
public:
	Name(std::shared_ptr<Entity> _owner, const std::string& _name)
		: Component(_owner)
		, m_name(_name)
	{}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<Name>(*this);
	}

public:
	std::string m_name;
};

