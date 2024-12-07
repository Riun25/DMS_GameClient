#pragma once
#include "Component.h"
#include "GraphicsDefine.h"

/// <summary>
/// 캐릭터가 장착하는 장비 하나에 해당하는 객체
/// </summary>
struct Equipment
{
	std::string m_modelFileName;	// 모델 파일 이름
	std::string m_meshName;			// 메쉬 이름
	std::string m_diffuseTexture;	// 디퓨즈 텍스쳐 파일명
	std::string m_normalTexture;	// 노말 텍스쳐 파일명
	std::string m_targetModelName;	// 장착시킬 모델의 이름
	std::string m_boneName;			// 본의 이름
	std::shared_ptr<ModelBuffer> m_pModelBuffer;	// 모델 버퍼
	int m_targetBoneIndex;			// 본 인덱스
	VSTargetBoneConstantBufferData m_targetBoneConstantBufferData;	// 본 매트릭스
};

/// <summary>
/// 장비들의 벡터를 가지고있는 컴포넌트
/// 장비 렌더링을 할때 사용됨
/// 충돌체가 필요한 경우는 별개의 엔티티와 컴포넌트를 이용해 생성
/// </summary>
class EquipmentComponent :
	public Component
{
public:
	EquipmentComponent(std::shared_ptr<Entity> _owner)
		: Component(_owner)
	{}

	~EquipmentComponent();

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<EquipmentComponent>(*this);
	}

	void AddEquipment(const std::string& _fileName = "", const std::string& _meshName = ""
		, const std::string& _targetName = "", const std::string& _boneName = ""
		, const std::string& _diffuse = "", const std::string& _normal = "");

public:
	std::vector<std::shared_ptr<Equipment>> m_pEquipments;	// 장비들의 벡터

};

// 캐릭터의 meshrenderer컴포넌트의 객체를 가져다 쓰면 됨
// 월드, 월드 역행렬을 가지고 있는 객체
// VSConstantBufferData 

// 캐릭터의 bonegroup컴포넌트의 객체를 가져다 쓰면 됨
// 본 매트릭스들의 행렬
// VSBoneConstantBufferData* 
