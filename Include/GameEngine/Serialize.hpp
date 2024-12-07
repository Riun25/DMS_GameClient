#pragma once
#include "pch.h"
#include "Transform.h"
#include "Entity.h"
#include "Scene.h"
#include "World.h"

using namespace entt::literals;

// 컴포넌트 시리얼라이즈
template <IsComponent T>
json SerializeComponent(const T& _component)
{
	json j;
	auto type = entt::resolve<T>();
	for (auto data : type.data())
	{
		j[data.name().data()] = data.get(_component).template cast<float>();
	}
	return j;
}

// 컴포넌트 디시리얼라이즈
template <IsComponent T>
T DeserializeComponent(const json& j)
{
	T component;
	auto type = entt::resolve<T>();
	for (auto data : type.data())
	{
		auto it = j.find(data.name().data());
		if (it != j.end())
		{
			data.set(component, it->get<float>());
		}
	}
	return component;
}

// 엔티티 시리얼라이즈
json SerializeEntity(const std::shared_ptr<Entity>& _entity)
{
	json j;
	j["id"] = _entity->GetUID();
	if (_entity->HasComponent<Transform>())
	{
		j["Transform"] = SerializeComponent(_entity->GetComponent<Transform>());
	}
	// 다른 컴포넌트들도 이곳에 추가

	return j;
}

// 엔티티 디시리얼라이즈
void DeserializeEntity(const json& j, std::shared_ptr<Entity>& _entity)
{
	_entity->SetUID(j["id"]);
	if (j.contains("Transform"))
	{
		auto transform = DeserializeComponent<Transform>(j.at("Transform"));
		_entity->AddComponent<Transform>(transform);
	}
	// 다른 컴포넌트들도 이곳에 추가
}

// 씬 시리얼라이즈
json SerializeScene(const std::shared_ptr<Scene>& _scene)
{
	json j;
	j["id"] = _scene->GetUID();
	j["entities"] = json::array();
	for (auto& [uid, entity] : _scene->GetEntityMap())
	{
		j["entities"].push_back(SerializeEntity(entity));
	}
	return j;
}

// 씬 디시리얼라이즈
void DeserializeScene(const json& j, std::shared_ptr<Scene>& _scene)
{
	_scene->SetUID(j["id"]);
	for (const auto& entityJson : j.at("entities"))
	{
		auto entity = _scene->CreateEntity(entityJson["name"]);
		DeserializeEntity(entityJson, entity);
	}
}

// 월드 시리얼라이즈
json SerializeWorld(const std::shared_ptr<World>& _world)
{
	json j;
	j["id"] = _world->GetUID();
	j["scenes"] = json::array();
	for (auto& [uid, scene] : _world->GetSceneMap())
	{
		j["scenes"].push_back(SerializeScene(scene));
	}
	return j;
}

// 월드 시리얼라이즈
void DeserializeWorld(const json& j, std::shared_ptr<World>& _world)
{
	_world->SetUID(j["id"]);
	for (const auto& sceneJson : j.at("scenes"))
	{
		auto scene = _world->CreateScene(sceneJson["name"]);
		DeserializeScene(sceneJson, scene);
	}
}
