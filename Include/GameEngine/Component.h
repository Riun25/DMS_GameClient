#pragma once
#include "pch.h"
#include "MetaData.hpp"

class Entity;

/// <summary>
/// 컴포넌트의 인터페이스 클래스입니다.
/// </summary>
class Component
{
public:
	//Component() = default;
	virtual ~Component() = default;

	/// <summary>
	/// 컴포넌트를 소유중인 엔티티의 이름/uid등을 알기 위해 생성자
	/// </summary>
	/// <param name="_owner"></param>
	Component(std::shared_ptr<Entity> _owner) :m_pOwner(_owner) {}

	/// <summary>
	/// 컴포넌트 복사 메서드	// 추후 툴에서 프리펩같은
	/// std::shared_ptr<Component> Clone() const override {	return std::make_shared<T>(*this);	}	// 클론 매서드를 정의 해줘야함
	/// </summary>
	/// <returns></returns>
	virtual std::shared_ptr<Component> Clone() const = 0;

	std::string GetOwnerName() const;

 	std::shared_ptr<Entity> m_pOwner;
};



/// <summary>
/// 컴포넌트의 컨셉트 정의, 베이스 클래스가 Component인지 확인합니다.
/// </summary>
template <typename T>
concept IsComponent = std::is_base_of<Component, T>::value;

