#pragma once
#include "pch.h"

/// <summary>
/// 매니저 클래스들의 인터페이스 클래스입니다.
/// 상속받을 클래스에서 엔티티들의 컴포넌트 정보들이 필요한 경우
/// </summary>
class IManager
{
public:
	IManager() = default;
	virtual ~IManager() = default;

	virtual bool Initialize() = 0;
	virtual void FixedUpdate(float _fixedDTime) = 0;
	virtual void Update(float _dTime) = 0;
	virtual void LateUpdate(float _dTime) = 0;
	virtual void Finalize() = 0;

};