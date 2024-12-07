#pragma once
#include "Component.h"
#include "Graphics/BufferData.h"

/// <summary>
/// 모델들의 본들의 데이터가 있는 컴포넌트입니다.
/// </summary>
struct TargetBoneComponent
	: public Component
{
public:
	TargetBoneComponent(std::shared_ptr<Entity> _owner)
		: Component(_owner), mTargetBoneData(), mTarGetBoneConstantBufferData(nullptr), mTargetBoneAboveMatrix(Matrix())
	{}
	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<TargetBoneComponent>(*this);
	}


public:
	// 	std::string m_file;
	Matrix mTargetBoneAboveMatrix;
	int mTargetBoneIndex;			//장비의 경우 타겟본의 인덱스를 알고 있어야한다. -> 똑같은 계산을 두번 안해도 된다.
	VSBoneConstantBufferData* mTarGetBoneConstantBufferData;
	VSTargetBoneConstantBufferData mTargetBoneData;

	std::weak_ptr<Entity> myPlayer;
};