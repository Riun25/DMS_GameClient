#pragma once
#include "Component.h"
#include "Box2D.h"

/// <summary>
/// [Riun] 8/14 변수 수정
/// </summary>
struct ShrinkRectComponent
	: public Component
{
public:
	ShrinkRectComponent(std::shared_ptr<Entity> _owner, Vector2 _origineSize, Vector2 _targetSize = Vector2(), int _shrinkAmount = 1)
		:Component(_owner), mTargetSize(_targetSize)
	{
		if (_shrinkAmount <= 0)
		{
			std::cout << "_shrinkAmount는 1보다 크거나 같은 정수여야 합니다." << std::endl;
			return;
		}

		mShrinkAmount = (_origineSize - mTargetSize) / _shrinkAmount;
		mShrinkAlpha = 1.0f / _shrinkAmount;
	}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<ShrinkRectComponent>(*this);
	}

	void Update(Box2D& _box2D, float _dTime)
	{
		if (_box2D.mSize.x > mTargetSize.x && _box2D.mIsVisible == true)
		{
			_box2D.mPosition.x += mShrinkAmount.x * 0.5f;
			_box2D.mPosition.y += mShrinkAmount.y * 0.5f;
			_box2D.mSize.x -= mShrinkAmount.x;
			_box2D.mSize.y -= mShrinkAmount.y;
			_box2D.mBorderRgba.w += mShrinkAlpha;
		}
	}

	void Resetting(Box2D& _box2D, Vector2 _newBoxPos, Vector2 _newOrigineSize, Vector2 _newTargetSize, int _shrinkAmount)
	{
		_box2D.mPosition = _newBoxPos;
		_box2D.mSize = _newOrigineSize;
		_box2D.mBorderRgba.w = 0.0f;
		mTargetSize = _newTargetSize;
		mShrinkAmount = (_newOrigineSize - _newTargetSize) / _shrinkAmount;
	}

public:
	Vector2 mTargetSize;
	Vector2 mShrinkAmount;
	float mShrinkAlpha;
};