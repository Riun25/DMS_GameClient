#pragma once
#include "Component.h"

/// <summary>
/// 임시 컴포넌트 클래스
/// </summary>
struct TempComponent
	: public Component
{
public:
	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<TempComponent>(*this);
	}


public:
	bool m_onGround;

};