#pragma once
#include <string>
#include <memory>
#include "Entity.h"
#include "EntityManager.h"
#include "AnimationComponent.h"
#include "MeshRenderer.h"
#include "PhysicsManager.h"

class PhysicsManager;

/// <summary>
/// 기본 속도를 조정하기 위한 변수를 추가
/// 2024.8.13.
/// </summary>
class ClientAnimator 
{
public:
	ClientAnimator(EntityManager* _pEntityManager, PhysicsManager* _pPhysicsManager)
		:mpEntityManager(_pEntityManager), mpPhysicsManager(_pPhysicsManager)
	{};
	~ClientAnimator() {};

public:
	EntityManager* mpEntityManager;
	PhysicsManager* mpPhysicsManager;
};
