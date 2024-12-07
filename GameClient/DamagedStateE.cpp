#include "DamagedStateE.h"

void DamagedStateE::Initialize()
{
}

void DamagedStateE::Enter(std::shared_ptr<Entity> _entity)
{
	// 각 유닛의 해당 (보간)애니메이션 경과 시간 초기화 및 세팅
	_entity->GetComponent<AnimationComponent>().mpTargetAnimation = mpResourceManager->Get_Animation("Character@Damaged.fbx");

	// 정지하도록 위치를 고정하고 무브 상태에서 혹시나 있을 속도 변수를 다 제거한다.
	mpPhysicsManager->SetFixPosition(_entity, true);
}

void DamagedStateE::Update(float _dTime, std::shared_ptr<Entity> _entity)
{
	/// 엔티티의 위치를 고정
	//mpPhysicsManager->UpdatePosition(_entity, _entity->GetComponent<Transform>().m_localPosition);
	if (_entity->HasComponent<Rigidbody>())
	{
		mpPhysicsManager->SetVelocity(_entity, Vector3());
	}
}

void DamagedStateE::Exit(std::shared_ptr<Entity> _entity)
{
	// 위치 고정을 해제한다.
	mpPhysicsManager->SetFixPosition(_entity, false);
}
