#pragma once
#include "Component.h"

/// <summary>
/// 2D 텍스쳐 컴포넌트입니다.
/// UI 출력시에 사용됩니다.
/// 기본 도형은 따로 구현하지 않겠습니다.
/// <para>
/// Texture2D(string "파일명", Vector2 위치, Vector2 사이즈, int 레이어, bool 그리는 여부, Vector4 RGBA);
/// </para>
/// </summary>
struct Texture2D
	:public Component
{
public:
	Texture2D(std::shared_ptr<Entity> _owner, const std::string& _file = "", const Vector2& _position = Vector2()
		, const Vector2& _size = Vector2(1.f), const int& _layer = 0, const Vector4& _rgba = Vector4(1.0f, 1.0f, 1.0f, 1.0f)
		, const bool& _isVisible = true, const bool& _hasDepth = false)
		: Component(_owner), mFile(_file), mPosition(_position), mSize(_size), mLayer(_layer), 
		mIsVisible(_isVisible), mRgba(_rgba), mHasDepth(_hasDepth)
	{}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<Texture2D>(*this);
	}

public:
	std::string mFile; // 파일명
	Vector2 mPosition; // 위치값은 좌측 상당 기준이다.
	Vector2 mSize;		// 가로, 세로
	int mLayer;		// 숫자가 클수록 뒤에 그려집니다.
	Vector4 mRgba;		// RGBA 값, 이미지는 RGB가 기본 1이어야 하고, A는 알파값이다.
	bool mIsVisible;	// 렌더러에서만 사용할 변수로, true일 때만 그린다.
	bool mHasDepth;	// 3D 공간에서 z축으로 깊이감을 줘야 하면 사용한다.
};