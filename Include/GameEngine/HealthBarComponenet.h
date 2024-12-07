#pragma once
#include "Component.h"

/// <summary>
/// 체력바 ui를 위한 컴포넌트 클래스
/// <para>
/// HealthBarComponenet(Vector2 위치, Vector2 사이즈, int 레이어, Vector4 배경색, Vector4 체력색, float 체력비율, bool 보임 여부, bool 3D깊이감 여부);
// 테두리는 없습니다. 있게 하려면 ui 매니저 랜더에 추가할 것
/// </para>
/// </summary>
struct HealthBarComponenet
	: public Component
{
public:
	HealthBarComponenet(std::shared_ptr<Entity> _owner, Vector2 _position = {}, Vector3 _offSet = {}, Vector2 _sizeWH = {}, int _layer = 0,
		Vector4 _backgroundRgba = Vector4(0.0f, 0.0f, 0.0f, 1.0f), Vector4 _foregroundRgba = Vector4(0.0f, 0.0f, 0.0f, 1.0f), float _HealthPercentage = 1.0f,
		const bool& _isVisible = true, const bool& _hasDepth = false)
		: Component(_owner), mPosition(_position), mOffSet(_offSet), mSize(_sizeWH), mLayer(_layer), mBackgroundColor(_backgroundRgba),
		 mForegroundColor(_foregroundRgba), mHealthPercentage(_HealthPercentage), mIsVisible(_isVisible), mHasDepth(_hasDepth)
	{}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<HealthBarComponenet>(*this);
	}

public:
	Vector2 mPosition;
	Vector3 mOffSet;
	Vector2 mSize;
	int mLayer; // 숫자가 클수록 뒤에 그려집니다.
	Vector4 mBackgroundColor;
	Vector4 mForegroundColor;
	float mHealthPercentage;
	bool mIsVisible; // 렌더러에서만 사용할 변수로, true일 때만 그린다.
	bool mHasDepth;	// 3D 공간에서 z축으로 깊이감을 줘야 하면 사용한다.
};