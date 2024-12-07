#include "LootState.h"
#include "Scene.h"
#include "WorldManager.h"
#include "Rigidbody.h"

void LootState::Initialize()
{
}

void LootState::Enter(std::shared_ptr<Entity> _entity)
{
	// 각 유닛의 해당 (보간)애니메이션 경과 시간 초기화 및 세팅
	auto& animationComp = _entity->GetComponent<AnimationComponent>();
	animationComp.mpTargetAnimation = mpResourceManager->Get_Animation("Character@Clapping.fbx");

	// 루팅 상태에선 정지하도록 위치를 고정한다.
	mpPhysicsManager->SetFixPosition(_entity, true);

	///디버깅용
	//std::cout << "Enter Loot State" << std::endl;
}

void LootState::Update(float _dTime, std::shared_ptr<Entity> _entity)
{
	/// 0.타깃이 없으면 끝낸다.
	if (!_entity->GetComponent<PlayerComponent>().mpTarget)
	{
		return;
	}

	/// 1. 엔티티의 위치를 고정
	mpPhysicsManager->UpdatePosition(_entity, _entity->GetComponent<Transform>().m_localPosition);

	/// 2. 플레이어의 소지금에 돈을 추가하고, MoneyComponent 엔티티를 삭제한 후 타겟을 nullptr로 변경
	auto& animationComp = _entity->GetComponent<AnimationComponent>();
	auto& playerComp = _entity->GetComponent<PlayerComponent>();

	float mAnimationCoolTime = animationComp.mpTargetAnimation->mDuration / animationComp.mpTargetAnimation->mTicksPerSecond;

	if (mAnimationCoolTime - animationComp.mAnimationPlaytime < 0.05f)
	{
		if (playerComp.mpTarget->HasComponent<MoneyComponent>())
		{
			if (!playerComp.mpTarget->GetComponent<ProjectileComponent>().m_isTriggered
				/*|| _entity != playerComp.mpTarget->GetComponent<MoneyComponent>().m_pTarget*/)
			{
				return;
			}
			// 플레이어의 소지금에 돈을 추가한다.
			playerComp.mMoney += playerComp.mpTarget->GetComponent<MoneyComponent>().m_amount;
			std::shared_ptr<Entity>& pTarget = playerComp.mpTarget;

			/// moneyVec에서 해당 엔티티를 지운다.
			auto& moneyVec = *mpFSMHelper->GetMoneyVec(); // 벡터 참조 얻기
			for (auto it = moneyVec.begin(); it != moneyVec.end(); )
			{
				if (it->first == pTarget) // 특정 타겟과 일치하는 요소를 찾음
				{
					it = moneyVec.erase(it); // 해당 요소 삭제 후, 다음 요소로 이동
				}
				else
				{
					++it; // 일치하지 않으면 다음 요소로 이동
				}
			}

			// 충돌한 moneyComponent 엔티티를 삭제하고, 타겟을 nullptr로 변경한다.
			mpEntityManager->RemoveEntity(playerComp.mpTarget->GetUID());
			auto uid = _entity->GetUID();
			auto uid2 = playerComp.mpTarget->GetUID();
			mpSoundManager->PlaySFX("Snd_sfx_CharaLoot");
		}
	}
}

void LootState::Exit(std::shared_ptr<Entity> _entity)
{
	mpPhysicsManager->SetFixPosition(_entity, false);
	_entity->GetComponent<PlayerComponent>().mpTarget = nullptr;
}
