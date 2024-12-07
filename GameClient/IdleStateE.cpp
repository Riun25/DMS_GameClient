#include "IdleStateE.h"
#include "Transform.h"

void IdleStateE::Initialize()
{
}

void IdleStateE::Enter(std::shared_ptr<Entity> _entity)
{
	// 각 유닛의 해당 (보간)애니메이션 경과 시간 초기화 및 세팅
	_entity->GetComponent<AnimationComponent>().mpTargetAnimation = mpResourceManager->Get_Animation("Character@Idle.fbx");

	///디버깅용
	//std::cout << "Enter Idle State" << std::endl;
}

void IdleStateE::Update(float _dTime, std::shared_ptr<Entity> _entity)
{
	// 엔티티의 위치를 고정
	mpPhysicsManager->UpdatePosition(_entity, _entity->GetComponent<Transform>().m_localPosition);
}

void IdleStateE::Exit(std::shared_ptr<Entity> _entity)
{
	///디버깅용
	//std::cout << "Exit Idle State" << std::endl;
}
