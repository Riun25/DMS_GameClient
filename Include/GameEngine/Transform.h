#pragma once
#include "Component.h"

/// <summary>
/// 오브젝트의 위치, 회전, 크기를 조절할 수 있는 컴포넌트입니다.
/// <para>
/// Transform(Vector3 위치, Vector3 회전각, Vector3 크기, Vector3 부모 위치, Quaternion 부모 회전, Vector3 부모 크기);
/// </para>
/// </summary>
struct Transform
	: public Component
{
public:
	Transform(std::shared_ptr<Entity> _owner, Vector3 _localPosition = Vector3(), Vector3 _localRotation = Vector3(), Vector3 _localScale = Vector3(1.f)
		, Vector3 _parentPosition = Vector3(), Vector3 _parentRotation = Vector3(), Vector3 _parentScale = Vector3(1.f))
		: Component(_owner), mLocalPosition(_localPosition), mLocalRotation(_localRotation), mLocalScale(_localScale)
		, mWorldPosition(_parentPosition), mWorldRotation(_parentRotation), mWorldScale(_parentScale)
		, mpParent(nullptr)
	{
	}

	virtual std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<Transform>(*this);
	}

	Matrix GetTransformMatrix();

public:
	Transform* mpParent;

	// 월드 객체 내에서의 위치, 회전, 크기를 나타냅니다.
	Vector3 mLocalPosition;
	Vector3 mLocalRotation;
	Vector3 mLocalScale;

	// 부모 객체의 위치, 회전, 크기를 나타냅니다.
	Vector3 mWorldPosition;
	Vector3 mWorldRotation;
	Vector3 mWorldScale;

	Matrix mLocalMatrix;
};
