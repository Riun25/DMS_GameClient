#pragma once
#include "pch.h"
#include "Entity.h"
#include "MeshRenderer.h"
#include "AnimationComponent.h"
#include "Graphics/DearsGraphicsEngine.h"

/// <summary>
/// 매니저보다는 헬퍼에 가까운 클래스입니다.
/// [Riun] 이 매니저는 랜더 매니저에 종속되어 있기 때문에 상속을 받지 않는다.
/// </summary>
class AnimationManager
{
public:
	AnimationManager(entt::registry& _registry, std::shared_ptr<DearsGraphicsEngine> _graphicsEngine);
	~AnimationManager();

	void AnimationUpdate(std::shared_ptr<Entity> _entity, float _dTime);

	/// 애니메이션이 존재하고, 모델 버퍼가 있다는 전제하에만 사용할 것---
	// 초기화, 즉 첫 시작의 애니메이션 세팅에 사용하는 함수입니다.
	void SetInitialAnimation(std::shared_ptr<Entity> _entity);

	// 보간이 발생했을 때 자동으로 다음 애니메이션을 이전 애니메이션으로 바꾸고, 다음 애니메이션을 nullptr로 바꿉니다.
	void InterPolationAnimation(std::shared_ptr<Entity> _entity);
	///--------------------------------------------------------------

private:
	entt::registry& mRegistry;
	std::shared_ptr<DearsGraphicsEngine> mpGraphicsEngine;	// 그래픽 엔진 객체

	void SettingLoopAnimation(std::shared_ptr<Entity> _entity);

	float GetCoolTime(std::shared_ptr<Entity> _entity);
};

