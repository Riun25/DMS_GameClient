#pragma once
#include "Component.h"

/// <summary>
/// 직사각형 ui를 위한 컴포넌트 클래스
/// <para>
/// Tile(Vector2 위치, Vector2 사이즈, int 레이어, Vector4 RGBA, , float 두께, Vector4 테두리 색상, bool 보임 여부, bool 3D깊이감 여부);
/// </para>
/// </summary>
struct Tile
	: public Component
{
public:
	Tile(std::shared_ptr<Entity> _owner, Vector2 _position = Vector2(), Vector2 _sizeWH = Vector2(), int _layer = 0,
		Vector4 _rgba = Vector4(0.0f, 0.0f, 0.0f, 1.0f), float _thickness = 0.0f, Vector4 _BorderRgba = Vector4(), const bool& _isVisible = true, const bool& _hasDepth = false)
		: Component(_owner), mPosition(_position), mSize(_sizeWH), mLayer(_layer),
		mRgba(_rgba), mThickness(_thickness), mBorderRgba(_BorderRgba), mIsVisible(_isVisible), mHasDepth(_hasDepth)
	{}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<Tile>(*this);
	}

public:

	Vector2 mPosition;
	Vector2 mSize;
	int mLayer; // 숫자가 클수록 뒤에 그려집니다.
	Vector4 mRgba;
	float mThickness;
	Vector4 mBorderRgba;
	bool mIsVisible; // 렌더러에서만 사용할 변수로, true일 때만 그린다.
	bool mHasDepth;	// 3D 공간에서 z축으로 깊이감을 줘야 하면 사용한다.
};