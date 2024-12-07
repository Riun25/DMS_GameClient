#pragma once
#include "pch.h"
#include "Transform.h"
#include "Entity.h"
#include "Singleton.h"

// class Entity;
// class World;
// class Object;
// class Component;
// 
// // Factory 클래스
// template <typename T>
// class Factory
// {
// public:
// 	void RegisterClass(std::string _key, std::function<std::shared_ptr<T>()> _creator)
// 	{
// 		if (m_registry.find(_key) != m_registry.end())
// 		{
// 			DLog("Class already registered with key: " + _key);
// 		}
// 		m_registry[_key] = _creator;
// 	}
// 
// 	std::shared_ptr<T> Create(std::string _key, uint16_t _id)
// 	{
// 		if (m_registry.find(_key) == m_registry.end())
// 		{
// 			DLog("Class not registered with key: " + _key);
// 			break;
// 		}
// 		return m_registry[_key]();
// 	}
// 
// private:
// 	static std::map<std::string, std::function<std::shared_ptr<T>()>> m_registry;
// 
// };
// 
// using ObjectFactory = Factory<Object>;
// using ComponentFactory = Factory<Component>;
// 
// // 클래스 등록 매크로
// #define REGISTER_OBJECT_CLASS(CLASS_NAME, ID, NAME) \
// 	Object* Create_##CLASS_NAME() { return std::make_shared<CLASS_NAME>(ID, NAME); } \
// 	const bool Registered_##CLASS_NAME = []() \
// 	{ \
// 		ObjectFactory::RegisterClass(#CLASS_NAME, create_##CLASS_NAME); \
// 		return true; \
// 	}(); \
// 
// #define REGISTER_COMPONENT_CLASS(CLASS_NAME) \
// 	Component* Create_##CLASS_NAME() { return std::make_shared<CLASS_NAME>(); } \
// 	const bool Registered_##CLASS_NAME = []() \
// 	{ \
// 		ComponentFactory::RegisterClass(#CLASS_NAME, create_##CLASS_NAME); \
// 		return true; \
// 	}(); \
