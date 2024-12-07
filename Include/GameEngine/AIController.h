#pragma once
#include "Component.h"

struct AIController : public Component
{
public:
	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<AIController>(*this);
	}

};