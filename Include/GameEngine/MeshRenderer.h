#pragma once
#include "Component.h"
#include "GraphicsDefine.h"

/// <summary>
/// 오브젝트의 모델이나 텍스쳐를 등록하고 렌더링할수 있는 컴포넌트입니다.
/// <para>
/// MeshRenderer("파일명", "메시 이름", 본 여부, 애니메이션 여부)
/// </para>
/// </summary>
struct MeshRenderer
	: public Component
{
public:
	/// <summary>
	/// MeshRenderer 생성자입니다.
	/// </summary>
	/// <param name="_file">모델 파일명</param>
	/// <param name="_meshName">메시 이름</param>
	/// <param name="_hasBone">본 여부</param>
	/// <param name="_hasAnimation">애니메이션 여부</param>
	MeshRenderer(std::shared_ptr<Entity> _owner, const std::string& _file = "", const std::string& _meshName = ""
		, bool _hasBone = false, bool _hasAnimation = false, bool _flag = false)
		: Component(_owner), m_file(_file), m_meshName(_meshName)
		, m_hasBone(_hasBone), m_flag(_flag)
		, m_pModel(nullptr), mIs_VSconstant(false), mIs_VSBoneConstant(false), mIs_VSTargetBoneconstant(false), mIs_PSconstant(false), mIs_CubeMap(false)
		, mVSConstantBufferData()
	{
		m_pModel = std::make_shared<ModelBuffer>();
	}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<MeshRenderer>(*this);
	}

	~MeshRenderer()
	{
// 		if (m_pModel != nullptr)
// 		{
// 			delete m_pModel;
// 		}
	}

public:
	std::string m_file;
	std::string m_meshName;
	std::shared_ptr<ModelBuffer> m_pModel;
	bool m_hasBone;
	bool m_flag;	// 임시 부울 변수, 게임 플레이 중에 오브젝트 생성오류 해결하기 위해 임시로 넣음

	bool mIs_VSconstant = true;						 // VSConstantBuffer가 있는가?
	bool mIs_VSBoneConstant;					 // VSBoneConstanatBuffer가 있는가?
	bool mIs_VSTargetBoneconstant;				 // VsTargetBoneConstantBuffer가 있는가?
	bool mIs_PSconstant;						 // PsConstnatBuffer가 있는가?
	bool mIs_CubeMap;							 // 이 메쉬를 큐브맵으로 쓸것인가?
	bool mIsWeapon = false;
	//---------------------------------ConstantData---------------------------------------나중에 shared_ptr로 바궈도 될듯 하다. 
	/// <summary>
	/// model, view, projection, invTranspose
	/// </summary>
	VSConstantBufferData mVSConstantBufferData;	// 얘가 다른 멤버변수들 메모리 오염시켰었음
	//#sung : 원래는 이곳에 PSConstantBufferData도 있어야한다.
};