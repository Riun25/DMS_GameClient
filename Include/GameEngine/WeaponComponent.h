#pragma once
#include "Component.h"
#include "Graphics/ModelInfo.h"

struct WeaponComponent
	: public Component
{
public:
	WeaponComponent(std::shared_ptr<Entity> _owner
	/*, const std::string& _targetModel = "", const std::string& _targetBone = "", float _scale = 1.f*/)
		:Component(_owner), m_pAttachedEntity(nullptr)/*, m_targetModel(_targetModel), m_targetBone(_targetBone), m_scale(_scale)*/
	{
		m_pModel = std::make_shared<ModelBuffer>();
	}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<WeaponComponent>(*this);
	}

public:
	Entity* m_pAttachedEntity;	// 무기의 엔티티
	std::shared_ptr<ModelBuffer> m_pModel; // 무기의 모델 버퍼
	AABB m_weaponAABB;
	//std::string m_targetModel;
	//std::string m_targetBone;
	std::string m_fileName;
	std::string m_meshName;
	float m_scale = 1.f;
	int m_boneIndex;	//본의 인덱스를 가지고 있게끔.
	Matrix m_weaponBoneMatrix;				// 콜라이더의 매트릭스
	VSConstantBufferData m_weaponConstantBufferData;	// 무기 메쉬의 컨스턴트 버퍼 데이터
};