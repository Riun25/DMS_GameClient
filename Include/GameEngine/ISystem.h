#pragma once
#include "pch.h"

class ISystem
{
public:
	ISystem() = delete;
	ISystem(entt::registry& _registry) 
		:m_registry(_registry)
	{}

	virtual ~ISystem() {}

	virtual bool Initialize() = 0;
	virtual void FixedUpdate(float _fixedDTime) = 0;
	virtual void Update(float _dTime) = 0;
	virtual void LateUpdate(float _dTime) = 0;
	virtual void Finalize() = 0;

protected:
	entt::registry& m_registry;
};