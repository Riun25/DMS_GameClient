#pragma once
#include "pch.h"

/// <summary>
/// 각 씬에서 루프를 도는 시스템 클래스들의 인터페이스입니다.
/// 2024.06.12 _Doyo;
/// </summary>
class System
{
public:
	System(entt::registry& _registry) :m_registry(_registry) {}
	virtual ~System() {}
	
	virtual bool Initialize() = 0;
	virtual void FixedUpdate(float _dTime) = 0;
	virtual void Update(float _dTime) = 0;
	virtual void LateUpdate(float _dTime) = 0;
	virtual void Finalize() = 0;

protected:
	entt::registry& m_registry;
};

