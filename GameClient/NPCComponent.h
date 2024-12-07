#pragma once
#include "Entity.h"
#include "Component.h"

/// <summary>
/// NPC 컴포넌트
/// </summary>
struct NPCComponent
	: public Component
{
public:
	NPCComponent(std::shared_ptr<Entity> _owner, std::string _npcName = std::string())
		: Component(_owner), mNpcName(_npcName), mIsPlayerInteracted(false), mElapsTime(0.0f)
	{
	}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<NPCComponent>(*this);
	}

public:
	std::string mNpcName;
	bool mIsPlayerInteracted; // 플레이어와 상호작용을 했는지 여부 : 텍스트 완료 여부나 특정 행동의 트리거를 담당할 예정이다.
	float mElapsTime; // 얼마나 시간이 경과했는지 필요할 때 쓰는 변수
};