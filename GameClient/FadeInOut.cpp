#include "FadeInOut.h"

bool FadeInOut::IsFadingOutFin(float _dTime, float _time)
{
	// 페이드 인 작업이 끝났으면 true를 반환한다.
	if (state != FadeInOutState::FADE_OUT)
	{
		mTime = 0.0f;
		return true;
	}

	mpTexture2D->m_isVisible = true;
	mTime += _dTime;

	// 현재 값이 목표한 alpha값보다 작다면 mTime 값을 투명도 값에 대입
	if (mAlpha > mTime)
	{
		mpTexture2D->m_rgba.w = mTime;
	}
	else // 목표한 알파값이라면 alpha 값으로 대입
	{
		mpTexture2D->m_rgba.w = mAlpha;
	}

	// 어두워지고 나서도 특정 시간동안 어두움을 유지하고 싶다면, _time을 1.0 이상으로 설정한다.
	if (mTime > _time)
	{
		state = FadeInOutState::NONE;
	}
	return false;
}

bool FadeInOut::IsFadingInFin(float _dTime)
{
	// 페이드 인 작업이 끝났으면 true를 반환한다.
	if (state != FadeInOutState::FADE_IN)
	{
		mpTexture2D->m_isVisible = false;
		return true;
	}

	mpTexture2D->m_isVisible = true;
	mTime += _dTime;

	// 현재 값이 목표한 alpha값보다 작다면 mTime 값을 투명도 값에 대입
	if (mAlpha < 1.0f - mTime)
	{
		mpTexture2D->m_rgba.w = 1.0f - mTime;
	}
	else // 목표한 알파값이라면 alpha 값으로 대입
	{
		mpTexture2D->m_rgba.w = mAlpha;
		state = FadeInOutState::NONE;
		mTime = 0.0f;
	}
	return false;
}

