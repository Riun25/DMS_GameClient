#pragma once
#include "Component.h"
#include "Texture2D.h"

enum class FadeInOutState
{
	FADE_IN, // 요구되는 alpha 값 : 0.0f; 점점 밝아짐
	NONE,
	FADE_OUT, // 요구되는 alpha 값 : 1.0f; 점점 어두워짐
};

/// <summary>
/// 페이드인 아웃을 담당하는 컴포넌트
/// </summary>
class FadeInOut :
	public Component
{
public:
	FadeInOut(std::shared_ptr<Entity> _owner, Texture2D* _pTexture2D = nullptr, float _alpha = 1.0f, FadeInOutState _state = FadeInOutState::NONE)
		: Component(_owner), mpTexture2D(_pTexture2D), mAlpha(_alpha), mTime(0.0f), state(_state)
	{
	}
public:
	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<FadeInOut>(*this);
	}

	// 화면이 전부 어두워지면 true값 반환[tTime, _time은 1.0일 경우 어두워지지만, 이 상태를 더 유지하고 싶으면 그 이상으로 설정하세요]
	bool IsFadingOutFin(float _dTime, float _time);

	// 화면이 전부 밝아지면 true값 반환(_dTime)
	bool IsFadingInFin(float _dTime);

public:
	Texture2D* mpTexture2D;
	float mAlpha; // 목표하는 fading 최종값
	float mTime;
	FadeInOutState state;
};

// 사용 예시
// alpha값과 state 상태를 원하는 걸로 설정한 후, 관련된 함수를 호출하면 실행이 완료된 후 FadeInOutState::NONE가 된다.
//
/// 초기화 부분
//auto ui5 = m_pEntityManager->CreateEntity("FaidInOut");
//m_pUIManager->AddUI(ui5, "Black.png", Vector2(0, 0), Vector2(1920, 1080), 2, Vector4(1.f, 1.0f, 1.0f, 1.0f), true);
//ui5->AddComponent<FadeInOut>(&ui5->GetComponent<Texture2D>(), 0.0f, FadeInOutState::FADE_IN);

/// 실행 업데이트 부분
//if (mIsExit == true)
//{
//	if (fadeInOut->IsFadingOutFin(_dTime, 1.5f) == true)
//	{
//		PostQuitMessage(0);
//	}
//}
//else
//{
//	fadeInOut->IsFadingInFin(_dTime) == true;
//	if (fadeInOut->state == FadeInOutState::NONE)
//	{
//		fadeInOut->mAlpha = 1.0f;
//		fadeInOut->state = FadeInOutState::FADE_OUT;
//	}
//}