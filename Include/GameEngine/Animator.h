#pragma once
#include "Entity.h"
#include "MeshRenderer.h"
#include "Graphics/DearsGraphicsEngine.h"

/// <summary>
/// [Riun] 2024.8.31.
/// 이 클래스는 클라이언트에서 애니메이션 매니저의 모든 내용을 알게 하는 것을 제어하고자 만든 클래스입니다.
/// 클라이언트에서는 이 애니메이터의 내용만 쓸 수 있고, 그 외에는 접근하지 못하게 합니다.
/// </summary>
/// 
class Animator
{
public:
	Animator(std::shared_ptr<DearsGraphicsEngine> _graphicsEngine);
	~Animator();

	float GetCoolTime(std::shared_ptr<Entity> _entity);

	bool IsAnimatonFinished(std::shared_ptr<Entity> _entity);


private:
	std::shared_ptr<DearsGraphicsEngine> mpGraphicsEngine;	// 그래픽 엔진 객체


};

