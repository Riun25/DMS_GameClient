#pragma once
#include "Component.h"

struct ShrinkEffectComponent
	: public Component
{
public:
	ShrinkEffectComponent(std::shared_ptr<Entity> _owner)
		:Component(_owner)
	{

	}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<ShrinkEffectComponent>(*this);
	}

public:

};