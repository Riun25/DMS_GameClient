#pragma once
#include "Component.h"
#include "Graphics/ModelInfo.h"
struct AnimationComponent
	: public Component
{
public:
	AnimationComponent(std::shared_ptr<Entity> _owner)
		: Component(_owner)
	{}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<AnimationComponent>(*this);
	}

public:
	// 현재 재생 중인 애니메이션의 타임라인에서 경과한 시간
	float mAnimationPlaytime = 0.0f;

	float mNextAnimationPlayTime = 0.0f;

	// 현재 재생 중인 애니메이션의 전체시간
	float mAnimationCoolTime = 0.0f;

	// 현재 재생 중인 애니메이션 
	Animation* mpTargetAnimation = nullptr;

	// 다음에 재생할 애니메이션 (전환할 경우에 사용)
	Animation* mpNextTargetAnimation = nullptr;

	// 애니메이션이 루프하는지 여부
	bool mIsLoop = false;

	// 애니메이션 재생 속도 (1.0 = 기본 속도, 0.5 = 절반 속도)
	float mPlaySpeed = 1.f;

	// 애니메이션을 중지하는 플래그 (중지하면 현재 프레임에서 멈춤)
	bool mPaused = false;

	void SetTargetAnimation(Animation* _targetAnimaion)
	{
		mpTargetAnimation = _targetAnimaion;
	}
	
	void SetNextTargetAnimation(Animation* _nextTargetAnimaion)
	{
		mpNextTargetAnimation = _nextTargetAnimaion;
	}

	//std::vector<std::shared_ptr<Entity>> myEntityVec;
};
//
//void AnimationComponent::SetTargetAnimation(Animation* _targetAnimaion)
//{
//	mpTargetAnimation = _targetAnimaion;
//}
//
//void AnimationComponent::SetNextTargetAnimation(Animation* _nextTargetAnimaion)
//{
//	mpNextTargetAnimation = _nextTargetAnimaion;
//}
