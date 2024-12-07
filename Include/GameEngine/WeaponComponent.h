#pragma once
#include "Component.h"
#include "Graphics/ModelInfo.h"

struct WeaponComponent
	: public Component
{
public:
	WeaponComponent(std::shared_ptr<Entity> _owner
	/*, const std::string& _targetModel = "", const std::string& _targetBone = "", float _scale = 1.f*/)
		:Component(_owner), mpAttachedEntity(nullptr)/*, m_targetModel(_targetModel), m_targetBone(_targetBone), m_scale(_scale)*/
	{
		mpModel = std::make_shared<ModelBuffer>();
	}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<WeaponComponent>(*this);
	}

public:
	Entity* mpAttachedEntity;	// 무기의 엔티티
	std::shared_ptr<ModelBuffer> mpModel; // 무기의 모델 버퍼
	AABB mWeaponAABB;
	//std::string m_targetModel;
	//std::string m_targetBone;
	std::string mFileName;
	std::string mMeshName;
	float mScale = 1.f;
	int mBoneIndex;	//본의 인덱스를 가지고 있게끔.
	Matrix mWeaponBoneMatrix;				// 콜라이더의 매트릭스
	VSConstantBufferData mWeaponConstantBufferData;	// 무기 메쉬의 컨스턴트 버퍼 데이터
};