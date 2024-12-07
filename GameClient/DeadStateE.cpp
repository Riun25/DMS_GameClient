#include "DeadStateE.h"

void DeadStateE::Initialize()
{
}

void DeadStateE::Enter(std::shared_ptr<Entity> _entity)
{
	// 애니메이션을 변경한다.
	auto& aniComp = _entity->GetComponent<AnimationComponent>();
	aniComp.mpTargetAnimation = mpResourceManager->Get_Animation("Character@Death.fbx");

	// 타겟을 없앤다.
	_entity->GetComponent<EnemyComponent>().mpTarget = nullptr;

	// 정지하도록 위치를 고정하고 무브 상태에서 혹시나 있을 속도 변수를 다 제거한다.
	mpPhysicsManager->SetFixPosition(_entity, true);
	aniComp.mIsLoop = false;

	// 사운드
	mpSoundManager->PlaySFX("Snd_sfx_CharaDead1");

	/// 디버깅용
	//std::cout << "Enter Enemy Dead State" << std::endl;
}

void DeadStateE::Update(float _dTime, std::shared_ptr<Entity> _entity)
{
	// 엔티티의 위치를 고정
//mpPhysicsManager->UpdatePosition(_entity, _entity->GetComponent<Transform>().m_localPosition);
	if (_entity->HasComponent<Rigidbody>())
	{
		mpPhysicsManager->SetVelocity(_entity, Vector3());
	}

	// 다시 반복될 때 애니메이션을 마지막 프레임으로 고정시키고 루프를 종료한다.
	auto& aniComp = _entity->GetComponent<AnimationComponent>();
	if (aniComp.mpNextTargetAnimation == nullptr/* && _entity->GetComponent<AnimationComponent>().mAnimationPlaytime >= _entity->GetComponent<AnimationComponent>().mAnimationCoolTime - 0.1f*/)
	{
		// 아군이 죽은 경우
		if (_entity->HasComponent<EnemyComponent>())
		{
			// 타겟에서 제외하기 위해 PlayerComponent의 죽음 플래그를 활성화한다.
			_entity->GetComponent<EnemyComponent>().mIsDead = true;
		}
		return;
	}

	/// 디버깅용
	//std::cout << "Update Enemy Dead State" << std::endl;
}

void DeadStateE::Exit(std::shared_ptr<Entity> _entity)
{
	/// 디버깅용
	//std::cout << "Exit Enemy Dead State" << std::endl;
}
