#pragma once
#include "Component.h"
#include <string.h>

/// <summary>
/// 해당 엔티티가 인디케이터임을 정의하기 위한 컴포넌트
/// </summary>
class IndicatorComponent :
	public Component
{
public:
	IndicatorComponent(std::shared_ptr<Entity> _owner, const std::string& _original = "", const std::string& _changed = "")
		: Component(_owner), mOriginalColor(_original), mChangedColor(_changed)
	{
	}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<IndicatorComponent>(*this);
	}

public:
	// 인디케이터의 색상 텍스처 파일을 저장해둔다.
	std::string mOriginalColor;
	std::string mChangedColor;
};

