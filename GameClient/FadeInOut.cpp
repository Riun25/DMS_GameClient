#include "FadeInOut.h"

bool FadeInOut::IsFadingOutFin(float _dTime, float _time)
{
	// ���̵� �� �۾��� �������� true�� ��ȯ�Ѵ�.
	if (state != FadeInOutState::FADE_OUT)
	{
		mTime = 0.0f;
		return true;
	}

	mpTexture2D->mIsVisible = true;
	mTime += _dTime;

	// ���� ���� ��ǥ�� alpha������ �۴ٸ� mTime ���� ���� ���� ����
	if (mAlpha > mTime)
	{
		mpTexture2D->mRgba.w = mTime;
	}
	else // ��ǥ�� ���İ��̶�� alpha ������ ����
	{
		mpTexture2D->mRgba.w = mAlpha;
	}

	// ��ο����� ������ Ư�� �ð����� ��ο��� �����ϰ� �ʹٸ�, _time�� 1.0 �̻����� �����Ѵ�.
	if (mTime > _time)
	{
		state = FadeInOutState::NONE;
	}
	return false;
}

bool FadeInOut::IsFadingInFin(float _dTime)
{
	// ���̵� �� �۾��� �������� true�� ��ȯ�Ѵ�.
	if (state != FadeInOutState::FADE_IN)
	{
		mpTexture2D->mIsVisible = false;
		return true;
	}

	mpTexture2D->mIsVisible = true;
	mTime += _dTime;

	// ���� ���� ��ǥ�� alpha������ �۴ٸ� mTime ���� ���� ���� ����
	if (mAlpha < 1.0f - mTime)
	{
		mpTexture2D->mRgba.w = 1.0f - mTime;
	}
	else // ��ǥ�� ���İ��̶�� alpha ������ ����
	{
		mpTexture2D->mRgba.w = mAlpha;
		state = FadeInOutState::NONE;
		mTime = 0.0f;
	}
	return false;
}

