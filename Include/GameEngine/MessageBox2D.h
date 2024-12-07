#pragma once
#include "Component.h"
#include "TextIdxEnum.h"

/// <summary>
/// 메세지 박스입니다. 기존에 텍스처와 텍스트를 가졌더라도 그림을 덧대어서 그릴 수 있게 따로 컴포넌트화합니다.
/// </summary>
struct MessageBox2D
	: public Component
{
public:
	MessageBox2D(std::shared_ptr<Entity> _owner)
		: Component(_owner), mFile(""), mTexturePosition(Vector2()), mSize(Vector2()), mImgRgba(Vector4()), mIsImgVisible(true)
		, mText(u8""), mTextPosition(Vector2()), mScale(1.0f), mFont(""), mTextRgba(Vector4()), mIsTextVisible(true)
		, mLayer(0), mIdx(TEXT_IDX::NONE), mNum1(0)
	{}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<MessageBox2D>(*this);
	}

public:
	std::string mFile; // 파일명
	Vector2 mTexturePosition; // 위치값은 좌측 상당 기준이다.
	Vector2 mSize;		// 가로, 세로
	Vector4 mImgRgba;		// RGBA 값, 이미지는 RGB가 기본 1이어야 하고, A는 알파값이다.
	bool mIsImgVisible;	// 렌더러에서만 사용할 변수로, true일 때만 그린다.

	std::u8string mText;
	Vector2 mTextPosition;
	float mScale;	// 폰트의 크기
	std::string mFont;
	Vector4 mTextRgba;		// RGBA 값, 이미지는 RGB가 기본 1이어야 하고, A는 알파값이다.
	bool mIsTextVisible; // 렌더러에서만 사용할 변수로, true일 때만 그린다.

	int mLayer;		// 숫자가 클수록 뒤에 그려집니다.

	/// <summary>
	///  임시로 이렇게 사용하겠습니다. 추후 정돈하겠습니다.
	/// </summary>
	TEXT_IDX mIdx;
	int mNum1; //AddTextwithInt() 함수의 인수입니다.
};