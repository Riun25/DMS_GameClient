#pragma once
#include <iostream>
#include "Entity.h"
#include "ResourceManager.h"
#include "EntityManager.h"
#include "PhysicsManager.h"
#include "EventManager.h"
#include "AnimationComponent.h"
#include "MeshRenderer.h"
#include "Event.h"
#include "SoundManager.h"

class Entity;
class IState
{
public:
	IState(EntityManager* _pEntityManager, PhysicsManager* _pPhysicsManager
		, ResourceManager* _pResourceManager, EventManager* _pEventManager
		, SoundManager* _pSoundManager)
		:mpEntityManager(_pEntityManager), mpPhysicsManager(_pPhysicsManager)
		, mpResourceManager(_pResourceManager), mpEventManager(_pEventManager)
		, mpSoundManager(_pSoundManager)
	{
	};
	virtual ~IState() {};

	virtual void Initialize() abstract;
	virtual void Enter(std::shared_ptr<Entity> _entity) abstract;
	virtual void Update(float _dTime, std::shared_ptr<Entity> _entity) abstract;
	virtual void FixedUpdate(float _fixedTime, std::shared_ptr<Entity> _entity) abstract;
	virtual void Exit(std::shared_ptr<Entity> _entity) abstract;

	///엔티티와 연결할 변수 필요 -> 값을 직접수정할수 있게 ex)entity*
protected:
	EntityManager* mpEntityManager;
	PhysicsManager* mpPhysicsManager;
	ResourceManager* mpResourceManager;
	EventManager* mpEventManager;
	SoundManager* mpSoundManager;
};

