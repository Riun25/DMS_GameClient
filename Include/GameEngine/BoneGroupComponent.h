#pragma once
#include "Component.h"
#include "Graphics/BufferData.h"

/// <summary>
/// 모델들의 본들의 데이터가 있는 컴포넌트입니다.
/// </summary>
struct BoneGroupComponent
	: public Component
{
public:
	BoneGroupComponent(std::shared_ptr<Entity> _owner)
		: Component(_owner), mBoneData()
	{}
	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<BoneGroupComponent>(*this);
	}


public:
// 	std::string m_file;
	VSBoneConstantBufferData mBoneData;
};