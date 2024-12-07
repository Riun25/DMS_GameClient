#pragma once
#include "Component.h"

/// <summary>
/// 자유형 사각형 ui를 위한 컴포넌트 클래스
/// <para>
/// FreeBox2D( Vector2 위치, );
/// </para>
/// </summary>
struct FreeBox2D
	: public Component
{
public:
	FreeBox2D(std::shared_ptr<Entity> _owner, Vector2 _position = Vector2(), int _layer = 0, const bool& _isVisible = true,
		Vector4 _rgba = Vector4(0.0f, 0.0f, 0.0f, 1.0f))
		: Component(_owner), m_position(_position), m_layer(_layer),
		m_isVisible(_isVisible), m_rgba(_rgba), m_hasDepth(false)
	{}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<FreeBox2D>(*this);
	}

public:

	Vector2 m_position;
	int m_layer; // 숫자가 클수록 뒤에 그려집니다.
	bool m_isVisible; // 렌더러에서만 사용할 변수로, true일 때만 그린다.
	Vector4 m_rgba;
	bool m_hasDepth;	// 3D 공간에서 z축으로 깊이감을 줘야 하면 사용한다.
};