#pragma once
#include "pch.h"
#include "Utility.h"
#include "EventListener.h"
#include "IData.h"

// 임시로 쓰는 static
static bool sIsMain = true;

class Entity;
class System;
class RenderManager;
class PhysicsManager;
class InputManager;
class WorldManager;
class UIManager;
class EntityManager;
class SoundManager;
class ResourceManager;

/// <summary>
/// 월드 내의 개별 씬을 나타내는 클래스입니다. 씬은 엔티티들의 배열로 구성됩니다.
/// </summary>
class Scene
	:public EventListener
{
public:
	Scene() = delete;
	Scene(entt::registry& _registry, const std::string& _name
		, EventManager* _pEventManager, RenderManager* _pRenderManager
		, InputManager* _pInpueManager, WorldManager* _pWorldManager);
	Scene(entt::registry& _registry, const std::string& _name
		, EventManager* _pEventManager, RenderManager* _pRenderManager
		, PhysicsManager* _pPhysicsManager, InputManager* _pInpueManager
		, WorldManager* _pWorldManager, UIManager* _pUIManager
		, EntityManager* _pEntityManager, ResourceManager* _pResourceManager
		, SoundManager* _pSoundManager = nullptr);
	virtual ~Scene() = default;

	/// <summary>
	/// 씬을 초기화합니다.
	/// </summary>
	/// <returns>초기화 실패시 false 반환</returns>
	virtual bool Initialize();

	/// <summary>
	/// 고정된 시간 간격으로 물리 업데이트를 수행합니다.
	/// </summary>
	/// <param name="_dTime">고정된 델타타임</param>
	virtual void FixedUpdate(float _dTime);

	/// <summary>
	/// 매 프레임 업데이트를 수행합니다.
	/// </summary>
	/// <param name="_dTime">프레임 시간</param>
	virtual void Update(float _dTime);

	/// <summary>
	/// 후처리 업데이트를 수행합니다.
	/// </summary>
	/// <param name="_dTime">프레임 시간</param>
	virtual void LateUpdate(float _dTime);

	/// <summary>
	/// 씬을 종료하고 정리합니다.
	/// </summary>
	virtual void Finalize();

	///// <summary>
	///// 엔티티를 반환합니다.
	///// </summary>
	///// <param name="_uid">반환할 엔티티의 UID</param>
	///// <returns>엔티티 객체</returns>
	//std::shared_ptr<Entity> GetEntity(const UID& _uid);

	///// <summary>
	///// 씬의 엔티티 맵을 반환합니다.
	///// </summary>
	///// <returns>엔티티 맵</returns>
	//std::unordered_map<UID, std::shared_ptr<Entity>> GetEntityMap() const;

	/*/// <summary>
	/// 새로운 엔티티를 생성합니다.
	/// </summary>
	/// <param name="_name">엔티티 객체의 이름</param>
	/// <returns>생성된 엔티티 객체</returns>
	std::shared_ptr<Entity> CreateEntity(const std::string& _name);*/

	///// <summary>
	///// 이 씬에 엔티티 추가
	///// </summary>
	///// <param name="_entity">엔티티 객체</param>
	//void AddEntity(std::shared_ptr<Entity> _entity);

	///// <summary>
	///// 엔티티를 씬에서 제거합니다.
	///// </summary>
	///// <param name="_uid">제거할 엔티티의 UID</param>
	//void RemoveEntity(const UID& _uid);

	///// <summary>
	///// 모든 엔티티를 씬에서 제거합니다.
	///// </summary>
	//void RemoveAllEntities();

	///// <summary>
	///// 씬에 시스템을 추가합니다.
	///// </summary>
	///// <param name="_pSystem">추가할 시스템 객체</param>
	////void AddSystem(const std::shared_ptr<System>& _pSystem);

	/// <summary>
	/// 씬에 리소스를 추가합니다.
	/// </summary>
	virtual void AddResource();

	/// <summary>
	/// 씬의 UID를 반환합니다.
	/// </summary>
	/// <returns>씬 UID</returns>
	UID GetUID() const;

	/// <summary>
	/// 씬의 이름을 반환합니다.
	/// </summary>
	/// <returns>씬 이름</returns>
	std::string GetName() const;

	IData* GetSceneData() const;

protected:
	UID mUid;														// 씬의 고유 식별자
	std::string mSceneName;										// 씬의 이름
	//std::unordered_map<UID, std::shared_ptr<Entity>> m_pEntities;	// 씬에 존재하는 엔티티 맵핑
	entt::registry& mRegistry;										// 엔티티 레지스트리
	// 	std::vector<std::shared_ptr<System>> m_pSystems;				// 씬에 존재하는 시스템 목록

	RenderManager* mpRenderManager;
	PhysicsManager* mpPhysicsManager;
	InputManager* mpInputManager;
	WorldManager* mpWorldManager;
	UIManager* mpUIManager;
	SoundManager* mpSoundManager;
	EntityManager* mpEntityManager;
	ResourceManager* mpResourceManager;

	IData* mpSceneData;
};

