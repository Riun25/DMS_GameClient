#pragma once
#include "Component.h"
#include "Graphics/BufferData.h"

/// <summary>
/// 발광체 오브젝트에 사용되는 컴포넌트입니다.
/// </summary>
struct LightComponent 
	: public Component
{
public:
	LightComponent(std::shared_ptr<Entity> _owner)
		: Component(_owner), m_commonConstData()/*, m_isShadow(false), m_targetCamera(nullptr)*/
	{}
	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<LightComponent>(*this);
	}

	//bool m_isShadow;	//그림자를 그리게 할 경우

public:
	CommonConstantBufferData m_commonConstData;		//#sung 안에 전역적으로 쓸 컴포넌트 버퍼 데이터를 만들어 두었다. -> 그 안에  Light를 가진 배열이 존재한다.

public:
	CommonConstantBufferData* GetCommonConstantBufferData() 
	{
		return &m_commonConstData;
	}


};