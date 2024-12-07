#pragma once
#include "Component.h"
#include "Graphics/BufferData.h"

/// <summary>
/// FlowTextureComponent(float 시간, float 속도)
/// </summary>
struct FlowTextureComponent
	:public Component
{
public:
	FlowTextureComponent(std::shared_ptr<Entity> _owner, float _time = 0.f, float _speed = 0.1f)
		: Component(_owner), mTime(_time), mSpeed(_speed)
	{}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<FlowTextureComponent>(*this);
	}

public:
	float mTime;
	float mSpeed;
	VSWaterConstantBufferData mVSWCSD;
};