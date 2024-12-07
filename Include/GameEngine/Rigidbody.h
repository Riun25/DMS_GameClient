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
		: Component(_owner), mMass(_mass), mDrag(_drag), mAngularDrag(_angularDrag), mUseGravity(_useGravity), mIsKinematic(_isKinematic)
		, mRestitution(_restitution), mStaticFriction(_staticFriction), mDynamicFriction(_dynamicFriction), mIsStatic(_static)
		, mFreezePosition(_freezePosition), mFreezeRotation(_freezeRotation), mpRigidActor(nullptr)
	{
		const std::string& ownerName = _owner->GetName();
		mName = new char[ownerName.size() + 1];  // 문자열 길이 + null terminator
		strcpy_s(mName, ownerName.size() + 1, ownerName.c_str());  // 안전한 문자열 복사
	}
	
	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<Rigidbody>(*this);
	}

public:

	Vector3 mFreezePosition;	// 위치 고정 플래그
	Quaternion mFreezeRotation;	// 회전 고정 플래그
	float mMass;				// 질량
	float mDrag;				// 선형 감쇠
	float mAngularDrag;		// 각 감쇠
	float mRestitution;		// 반발 계수			// 0 ~ 1
	float mStaticFriction;		// 정적 마찰 계수	// 0 ~ 1
	float mDynamicFriction;	// 동적 마찰 계수	// 0 ~ 1
	bool mUseGravity;			// 중력 사용 여부
	bool mIsKinematic;			// 키네마틱 여부
	bool mIsStatic;			// 정적 여부
	physx::PxRigidActor* mpRigidActor;
	char* mName;
};