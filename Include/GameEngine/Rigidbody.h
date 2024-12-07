#pragma once
#include "Component.h"
#include "physx/PxRigidActor.h"
#include "Entity.h"

/// <summary>
/// 오브젝트에 물리 법칙을 적용하여 실제처럼 움직이거나 반응하게 하는 컴포넌트입니다.
/// <para>
/// Rigidbody(float 질량, float 선형 감쇠(0~1), float 각 감쇠, bool 중력, bool 키네마틱, float 반발 계수(0~1), float 정적 마찰 계수(0~1)
/// , float 동적 마찰 계수(0~1), 정적인 객체인지, Vector3 위치 고정 플래그, Quaternion 회전 고정 플래그)
/// </para>
/// </summary>
struct Rigidbody
	: public Component
{
public:
	Rigidbody(std::shared_ptr<Entity> _owner, float _mass = 1.f, float _drag = 0.f, float _angularDrag = 0.f, bool _useGravity = true, bool _isKinematic = false
		, float _restitution = 0.f, float _staticFriction = 1.f, float _dynamicFriction = 1.f, bool _static = false
		, const Vector3& _freezePosition = Vector3(1000.f), const Quaternion& _freezeRotation = Quaternion(360.f, 360.f, 360.f, 1.f))
		: Component(_owner), m_mass(_mass), m_drag(_drag), m_angularDrag(_angularDrag), m_useGravity(_useGravity), m_isKinematic(_isKinematic)
		, m_restitution(_restitution), m_staticFriction(_staticFriction), m_dynamicFriction(_dynamicFriction), m_isStatic(_static)
		, m_freezePosition(_freezePosition), m_freezeRotation(_freezeRotation), m_pRigidActor(nullptr)
	{
		const std::string& ownerName = _owner->GetName();
		m_name = new char[ownerName.size() + 1];  // 문자열 길이 + null terminator
		strcpy_s(m_name, ownerName.size() + 1, ownerName.c_str());  // 안전한 문자열 복사
	}
	
	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<Rigidbody>(*this);
	}

public:

	Vector3 m_freezePosition;	// 위치 고정 플래그
	Quaternion m_freezeRotation;	// 회전 고정 플래그
	float m_mass;				// 질량
	float m_drag;				// 선형 감쇠
	float m_angularDrag;		// 각 감쇠
	float m_restitution;		// 반발 계수			// 0 ~ 1
	float m_staticFriction;		// 정적 마찰 계수	// 0 ~ 1
	float m_dynamicFriction;	// 동적 마찰 계수	// 0 ~ 1
	bool m_useGravity;			// 중력 사용 여부
	bool m_isKinematic;			// 키네마틱 여부
	bool m_isStatic;			// 정적 여부
	physx::PxRigidActor* m_pRigidActor;
	char* m_name;
};