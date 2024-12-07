#pragma once
#include "Component.h"
#include "Graphics/BufferData.h"

/// <summary>
/// 큐브맵을 그려주기 위해 필요한 컴포넌트
/// </summary>
struct CubeMapComponent
	: public Component
{
public:
	CubeMapComponent(std::shared_ptr<Entity> _owner, const std::string& _diffuse = "", const std::string& _specular = "")
		: Component(_owner), m_diffuse(_diffuse), m_specular(_specular)
	{
		m_pModel = std::make_shared<ModelBuffer>();
	}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<CubeMapComponent>(*this);
	}

public:
	std::shared_ptr<ModelBuffer> m_pModel;
	std::string m_diffuse;
	std::string m_specular;

	VSConstantBufferData m_VSCBD;
};