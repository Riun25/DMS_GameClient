#pragma once
#include "pch.h"
#include "Component.h"
#include "Utility.h"
#include <mutex>
#include <system_error>

class Scene;

/// <summary>
/// 
/// </summary>
class Entity
	: public std::enable_shared_from_this<Entity>
{
public:
	// 기본 생성자 삭제
	Entity() = delete;

	Entity(entt::registry& _registry);

	// 복사 생성자와 복사 할당 연산자를 삭제합니다.
	Entity(const Entity&) = delete;
	Entity& operator=(const Entity&) = delete;

	// 이동 생성자와 이동 할당 연산자를 추가합니다.
	Entity(Entity&&) noexcept = delete;
	Entity& operator=(Entity&&) noexcept = delete;

	~Entity();

	/// <summary>
	/// 엔티티를 초기화합니다.
	/// </summary>
	/// <returns>초기화 실패시 false 반환</returns>
	bool Initialize(const std::string& _name);

	/// <summary>
	/// 엔티티를 파괴합니다.
	/// </summary>
	void Destroy();

	/// <summary>
	/// 엔티티에 컴포넌트를 추가합니다.
	/// </summary>
	/// <typeparam name="T">컴포넌트 타입</typeparam>
	/// <param name="...args">컴포넌트 생성 인자</param>
	/// <returns>추가된 컴포넌트 반환</returns>
	template <IsComponent T, typename... Args>
	T& AddComponent(Args&&... args)
	{
		if (HasComponent<T>())
		{
			DLOG(LOG_ERROR, "Component already Added.");
			return GetComponent<T>();
		}
		auto self = shared_from_this();
		return m_registry.emplace<T>(m_entity, self, std::forward<Args>(args)...);
	}

	/// <summary>
	/// 엔티티가 해당 컴포넌트를 가지고있는지 확인합니다.
	/// </summary>
	/// <typeparam name="T">컴포넌트 타입</typeparam>
	/// <returns>컴포넌트 보유 여부</returns>
	template <IsComponent T>
	bool HasComponent() const
	{
		return m_registry.all_of<T>(m_entity);
	}

	/// <summary>
	/// 엔티티의 컴포넌트를 반환합니다.
	/// </summary>
	/// <typeparam name="T">컴포넌트 타입</typeparam>
	/// <returns>컴포넌트 참조</returns>
	template <IsComponent T>
	T& GetComponent()
	{
		if (HasComponent<T>())
		{
			return m_registry.get<T>(m_entity);
		}
		throw std::runtime_error("Component not found");
	}

	/// <summary>
	/// 엔티티의 컴포넌트들의 벡터를 반환합니다.
	/// </summary>
	/// <typeparam name="T">컴포넌트 타입</typeparam>
	/// <returns>컴포넌트 벡터</returns>
// 	template<typename T>
// 	std::vector<std::shared_ptr<T>> GetAllComponents()
// 	{
// 		std::vector<std::shared_ptr<T>> components;
// 		m_registry.view<T>().each([&components](auto entity, T& component) {components.push_back(std::make_shared<T>(component)); });
// 		return components;
// 	}

	/// <summary>
	/// 엔티티의 컴포넌트를 제거합니다.
	/// </summary>
	/// <typeparam name="T">컴포넌트 타입</typeparam>
	template <IsComponent T>
	void RemoveComponent()
	{
		m_registry.remove<T>(m_entity);
	}

	/// <summary>
	/// 엔티티를 복사합니다.
	/// 같은 형태의 엔티티들을 복사할때 사용합니다.
	/// </summary>
	/// <returns>복사된 엔티티 객체</returns>
	std::shared_ptr<Entity> Clone() const;

	/// <summary>
	/// 이 엔티티를 소유중인 씬을 반환합니다.
	/// </summary>
	/// <returns>소유중인 씬 객체</returns>
	std::shared_ptr<Scene> GetOwner() const;

	/// <summary>
	/// 고유 식별자를 반환합니다.
	/// </summary>
	/// <returns>엔티티 UID</returns>
	UID GetUID() const;

	/// <summary>
	/// 엔티티의 이름을 반환합니다.
	/// </summary>
	/// <returns></returns>
	std::string GetName() const;

private:
public:
	// 	UID m_uid;						// 객체 고유식별자, 엔티티 생성시에 자동으로 부여됩니다.
	// 	std::string m_name;				// 객체 이름
	std::weak_ptr<Scene> m_owner;	// 이 오브젝트를 보유하고있는 씬
	entt::registry& m_registry;		// 엔티티와 컴포넌트를 관리하는 엔티티 레지스트리
	entt::entity m_entity;			// 엔티티 핸들

};

