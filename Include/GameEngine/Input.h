#pragma once
#include "Component.h"

struct Input : public Component
{
	Input() = default;
	~Input() = default;

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<Input>(*this);
	}

};

