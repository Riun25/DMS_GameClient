#pragma once
#include <any>
#include <optional>
#include <functional>
#include <unordered_map>
#include <string>
#include <memory>
#include <Entity.h>
#include "PlayerComponent.h"
#include "enemyComponent.h"
#include "MoneyComponent.h"
#include "CapsuleCollider.h"
#include "ProjectileComponent.h"
#include "Transform.h"

/// FSM의 State를 변경하기 위한 함수를 관리하는 클래스
class CustomFunctions
{
public:
	CustomFunctions();
	~CustomFunctions() {};

public:
	float mTileScale = 1.0f;
	std::unordered_map<std::string, std::function<bool(std::shared_ptr<Entity> _temp)>> mFuncMap; // 함수를 담는 그릇이다. 이것은 최종적으로 아예 바깥으로 빠져서 돌려쓸 가능성이 있다.
};

