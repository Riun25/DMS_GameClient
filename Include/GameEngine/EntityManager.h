#pragma once
#include "Entity.h"
#include "Rigidbody.h"
#include "Transform.h"

class EntityManager
{
public:
	EntityManager(entt::registry& _registry);
	~EntityManager() {};

	/// <summary>
	/// 엔티티를 반환합니다.
	/// </summary>
	/// <param name="_uid">반환할 엔티티의 UID</param>
	/// <returns>엔티티 객체</returns>
	std::shared_ptr<Entity> GetEntity(const UID& _uid);

	std::shared_ptr<Entity> GetEntity(const entt::entity& _uid);

	/// <summary>
	/// 씬의 엔티티 맵을 반환합니다.
	/// </summary>
	/// <returns>엔티티 맵</returns>
	std::unordered_map<UID, std::shared_ptr<Entity>> GetEntityMap() const;

	/// <summary>
	/// 새로운 엔티티를 생성합니다.
	/// </summary>
	/// <param name="_name">엔티티 객체의 이름</param>
	/// <returns>생성된 엔티티 객체</returns>
	std::shared_ptr<Entity> CreateEntity(const std::string& _name);

	/// <summary>
	/// 이 씬에 엔티티 추가
	/// </summary>
	/// <param name="_entity">엔티티 객체</param>
	void AddEntity(std::shared_ptr<Entity> _entity);

	/// <summary>
	/// 엔티티를 씬에서 제거합니다.
	/// </summary>
	/// <param name="_uid">제거할 엔티티의 UID</param>
	void RemoveEntity(const UID& _uid);

	/// <summary>
	/// 모든 엔티티를 씬에서 제거합니다.
	/// </summary>
	void RemoveAllEntities();

//	std::shared_ptr<Entity> CreateEntity(entt::registry& _registry, const std::string& _name)
//	{
//		auto entity = std::make_shared<Entity>(_registry, _name);
//		m_entities[entity->GetUID()] = entity;
//		return entity;
//	}
//
//	void DestroyEntity(const UID& id)
//	{
//		auto it = m_entities.find(id);
//		if (it != m_entities.end())
//		{
//// 			it->second->Finalize();
//			m_entities.erase(it);
//		}
//	}
//
//	std::shared_ptr<Entity> GetEntity(const UID& id)
//	{
//		auto it = m_entities.find(id);
//		if (it != m_entities.end())
//		{
//			return it->second;
//		}
//		return nullptr;
//	}
//
//	void Update()
//	{
//		for (auto& [id, entity] : m_entities)
//		{
//// 			entity->Update();
//		}
//	}
//
//	void Clear()
//	{
//		for (auto& [id, entity] : m_entities)
//		{
//// 			entity->Finalize();
//		}
//		m_entities.clear();
//	}

private:
	//std::unordered_map<UID, std::shared_ptr<Entity>> m_entities;
	entt::registry& mRegistry;										// 엔티티 레지스트리
	std::unordered_map<UID, std::shared_ptr<Entity>> mpEntities;	// 씬에 존재하는 엔티티 맵핑

};