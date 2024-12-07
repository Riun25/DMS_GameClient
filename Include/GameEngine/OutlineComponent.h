#pragma once
#include "Component.h"
#include "Graphics/BufferData.h"

/// <summary>
/// 모델의 외곽선을 그리기 위해 필요한 컴포넌트
/// <para >
/// OutlineComponent(float 두께, vector3 색상);
/// </para>
/// </summary>
struct OutlineComponent
	:public Component
{
public:
	OutlineComponent(std::shared_ptr<Entity> _owner, float _thickness = 1.01, const Vector3& _color = Vector3(1.f))
		: Component(_owner), mThickness(_thickness), mColor(_color)
	{}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<OutlineComponent>(*this);
	}

public:
	float mThickness;	// 두께
	Vector3 mColor;	// 색

	VSEdgeConstantBufferData mVSEdgeCBD;
	PSEdgeConstantBufferData mPSEdgeCBD;

};