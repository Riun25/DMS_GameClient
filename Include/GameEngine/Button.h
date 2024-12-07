#pragma once
#include "Component.h"
#include "Text.h"
#include "Texture2D.h"

enum class ButtonState
{
	NORMAL,
	HOVERED,
	PRESSED
};

/// <summary>
/// UI 버튼 컴포넌트 클래스
/// 클라이언트에서 직접적으로 수정하지 않고, UI매니저를 통해 수정하도록 합니다.
/// 현재 이미지와 폰트만 버튼 사용 가능합니다. (다른 Box2D 같은 거는 안 됨)
// 드래그 때는 Press의 이미지를 사용합니다.
// 호버링과 클릭 함수를 동시에 사용하지 마세요. 한 틱 튕깁니다.
/// <para>
/// Button(u8string "출력할 문자열", string "사용할 폰트", Vector2 위치, int 레이어, Vector4 RGBA);
/// </para>
/// </summary>
struct Button
	: public Component
{
public:
	Button(std::shared_ptr<Entity> _owner)
		: Component(_owner), mUIPosition(Vector2()), mTextPosition(Vector2()), mUIPressedPosition(Vector2()), mTextPressedPosition(Vector2())
		, mNormalUIName(""), mNormalTextFont(""), mNormalText(u8""), mNormalRGBA(Vector4(1.0f)), mUIHoveringPosition(Vector2()), mTextHoveringPosition(Vector2())
		, mHoveredUIName(""), mHoveredTextFont(""), mHoveredText(u8""), mHoveredRGBA(Vector4(0.0f)), mPressedUIName(""), mPressedTextFont(""), mPressedText(u8"")
		, mPressedRGBA(Vector4(0.0f)), mButtonState(ButtonState::NORMAL), mIsDrag(false), mOrder(0), mIsEnable(true), mCanDrag(false)
		
	{}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<Button>(*this);
	}

public:
	Vector2 mUIPosition;		// 기존 UI의 정보를 복사해둔다.
	Vector2 mUISize;		// 기존 UI의 정보를 복사해둔다.
	Vector2 mTextPosition;		// 기존 Text의 정보를 복사해둔다.

	std::string mNormalUIName;	// 
	std::string mNormalTextFont;
	std::u8string mNormalText;
	Vector4 mNormalRGBA;

	Vector2 mUIHoveringPosition;
	Vector2 mUIHoveringSize;
	Vector2 mTextHoveringPosition;

	Vector2 mUIPressedPosition;
	Vector2 mUIPressedSize;
	Vector2 mTextPressedPosition;

	std::string mHoveredUIName;
	std::string mHoveredTextFont;
	std::u8string mHoveredText;
	Vector4 mHoveredRGBA;

	std::string mPressedUIName;
	std::string mPressedTextFont;
	std::u8string mPressedText;
	Vector4 mPressedRGBA;

	ButtonState mButtonState;
	bool mIsDrag; // 드래그를 기능으로 쓰려면 활성화할 것

	int mOrder; // 버튼의 순서를 나타내는 변수 (레이어와 비슷하게, 순서를 강제해서 다른 버튼이 동시에 눌리지 않게 한다.)
	bool mIsEnable; // 버튼의 활성화 여부를 나타내는 플래그 : 클릭 불가 및 시각적으로도 보이지 않음
	bool mCanDrag; // UI 매니저 내부에서 사용하는 플래그이므로 건들지 말 것 
};