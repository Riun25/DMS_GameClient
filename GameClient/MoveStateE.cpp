#include "MoveStateE.h"
#include "Transform.h"
#include "EnemyComponent.h"
#include "Rigidbody.h"
#include "MoneyComponent.h"
#include "ProjectileComponent.h"

void MoveStateE::Initialize()
{
}

void MoveStateE::Enter(std::shared_ptr<Entity> _entity)
{
	/// 0. 공통 변수 선언
	auto& enemyComp = _entity->GetComponent<EnemyComponent>();
	auto& animationComp = _entity->GetComponent<AnimationComponent>();

	/// 2. 애니메이션 설정
	if (enemyComp.mpTarget)
	{
		auto& tempTarget = enemyComp.mpTarget;
		if (tempTarget->HasComponent<MoneyComponent>())
		{
			animationComp.mpTargetAnimation = mpResourceManager->Get_Animation("Character@MutantMove.fbx");
		}
		else
		{
			animationComp.mpTargetAnimation = mpResourceManager->Get_Animation("Character@Move.fbx");
		}
	}
	// 애니메이션 스피드 조정
	animationComp.mPlaySpeed = enemyComp.mMoveSpeed / mAnimationRation;

	/// 디버깅용
	//std::cout << "Enter Move State" << std::endl;
}

void MoveStateE::Update(float _dTime, std::shared_ptr<Entity> _entity)
{

}

void MoveStateE::Exit(std::shared_ptr<Entity> _entity)
{
	/// 0. 공통 변수 선언
	auto& enemyComp = _entity->GetComponent<EnemyComponent>();
	auto& trsComp = _entity->GetComponent<Transform>();

	/// 에니메이션 스피드 원래로 조정
	_entity->GetComponent<AnimationComponent>().mPlaySpeed = 1.0f;

	if (enemyComp.mpPath != nullptr)
	{
		enemyComp.mpPath->clear(); // 전에 있던 경로 지우기 : 추후에 exit에도 엔티티를 넣으면 거기에 넣게 될 것..
	}
	enemyComp.mPathIndex = 0;

	auto& tempTarget = enemyComp.mpTarget;
	if (tempTarget == nullptr)
	{
		return;
	}
	if (tempTarget->HasComponent<Transform>())
	{
		// 캐릭터가 보고 있는 타깃을 보도록 한 번 더 점검
		mpFSMHelper->LookAtTarget(trsComp.m_localPosition, tempTarget->GetComponent<Transform>().m_localPosition, enemyComp.tempDirection, trsComp.m_localRotation.y);
	}

	/// 디버깅용
	//std::cout << "Exit Move State" << std::endl;
}

void MoveStateE::FixedUpdate(float _fixedTime, std::shared_ptr<Entity> _entity)
{
	/// 0. 공통 변수 선언
	auto& enemyComp = _entity->GetComponent<EnemyComponent>();
	auto& animationComp = _entity->GetComponent<AnimationComponent>();
	auto& trsComp = _entity->GetComponent<Transform>();

	/// 타깃이 없다면 종료
	if (!_entity || enemyComp.mpTarget == nullptr)
	{
		return;
	}

	auto& tempTarget = enemyComp.mpTarget;
	if (tempTarget->HasComponent<Transform>())
	{
		auto& previousTarget = enemyComp.mPreviousTarget;				// 이전 타겟 : 현재 지정된 타겟과 동일한지 확인하여, 다르면 경로를 재계산하기 위해 선언함
		auto& previousTargetPos = enemyComp.mPreviousTargetPos;			// 이전 타깃의 위치
		auto& tempTarget = enemyComp.mpTarget;							// 현재 타겟 : 현재 지정된 타겟
		auto& position = trsComp.m_localPosition;						// 유닛의 위치
		auto& speed = enemyComp.mMoveSpeed;								// 유닛의 속도
		auto& direction = enemyComp.tempDirection;						// 유닛의 방향
		auto& pathdirection = enemyComp.pathDirection;		 // 유닛의 방향(경로 방향)
		size_t& currentPathIndex = enemyComp.mPathIndex;					// path 인덱스
		Vector2& currentPos = enemyComp.mAstarPos;						// xz평면의 위치
		Vector3& targetPos = tempTarget->GetComponent<Transform>().m_localPosition;
		Vector2 goalXZPos = Vector2();

		/// 현재의 좌표를 타일 내에 존재하는 중점으로 조정한다. (Astar에서는 타일 중점으로 계산을 하기 때문이다.)
		mpFSMHelper->AdjustToTileCenter(position, currentPos);
		mpFSMHelper->AdjustToTileCenter(targetPos, goalXZPos);

		/// 이전 타깃이 존재하지 않는다면, 현재 타깃과 현재 위치를 이전 정보로 넣어준다.
		if (!previousTarget)
		{
			previousTarget = tempTarget;
			previousTargetPos = targetPos;
		}

		/// 타깃이 변경되면 재계산
		if ((tempTarget != previousTarget))
		{
			previousTarget = tempTarget;

			// 전에 있던 경로 지우기
			if (enemyComp.mpPath != nullptr)
			{
				enemyComp.mpPath->clear();
			}

			// 경로 재계산
			mpFSMHelper->CalculatePathforEnemy(enemyComp.mpPath, currentPos, goalXZPos);
			currentPathIndex = 0;

			// 캐릭터가 바라보고 있는 방향을 업데이트
			mpFSMHelper->LookAtTarget(position, targetPos, direction, trsComp.m_localRotation.y);

			// 타깃이 달라지면 같은 상태에서도 애니메이션이 바뀌어야 한다. 타깃이 바뀌는 특수 상황에서만 애니메이션이 변경된다.
			if (tempTarget->HasComponent<EnemyComponent>())
			{
				animationComp.mpTargetAnimation = mpResourceManager->Get_Animation("Character@Move.fbx");
			}
			else
			{
				animationComp.mpTargetAnimation = mpResourceManager->Get_Animation("Character@MutantMove.fbx");
			}
		}
		else
		{
			// 타깃이 변경되지 않았을 경우 중, 위치가 변경되면 재계산(기존 타깃의 위치가 노드 1개 이상으로 변경되면 재계산)
			if ((targetPos - previousTargetPos).Length() > mThreshold)
			{
				previousTargetPos = targetPos;
				// 경로 재계산
				mpFSMHelper->CalculatePathforEnemy(enemyComp.mpPath, currentPos, goalXZPos);
				currentPathIndex = 0;

				// 캐릭터가 바라보고 있는 방향을 업데이트
				mpFSMHelper->LookAtPathDir2(position, targetPos, enemyComp.mpPath, currentPathIndex, pathdirection, trsComp.m_localRotation.y);
				return;
			}
		}


		/// path의 사이즈가 0보다 작거나 인덱스가 더 클 수 없으므로 경로를 재계산한다. (이전 타깃이 없을 경우 size가 0이라 이때 계산한다.)
		if (enemyComp.mpPath == nullptr || enemyComp.mpPath->size() <= 0 || currentPathIndex > enemyComp.mpPath->size())
		{
			// 경로 재계산
			mpFSMHelper->CalculatePathforEnemy(enemyComp.mpPath, currentPos, goalXZPos);
			currentPathIndex = 0;

			// 캐릭터가 바라보고 있는 방향을 업데이트
			mpFSMHelper->LookAtPathDir2(position, targetPos, enemyComp.mpPath, currentPathIndex, pathdirection, trsComp.m_localRotation.y);
		}
		else
		{
			if (currentPos == (*enemyComp.mpPath)[currentPathIndex])
			{
				// 방어 코드 // 혹시라도 아래의 currentPathIndex+1이 사이즈를 넘어서면 안 되기 때문에 시도한다.
				if (enemyComp.mpPath->size() == currentPathIndex + 1)
				{
					mpFSMHelper->CalculatePathforEnemy(enemyComp.mpPath, currentPos, goalXZPos);
					currentPathIndex = 0;
					return;
				}
				currentPathIndex++;
				mpFSMHelper->LookAtPathDir2(position, targetPos, enemyComp.mpPath, currentPathIndex, pathdirection, trsComp.m_localRotation.y);
			}
			pathdirection.Normalize();
		}

		// 이동
		position += pathdirection * speed * testSpeedCoef;
		if (position.y > 0.5f)
		{
			position.y = 0.0f;
		}
		mpPhysicsManager->UpdatePosition(_entity, position);

		/// 물리충돌에 의해 피치 못하게 경로를 이탈했을 경우를 위해 경로를 재계산한다.
		// 둘 중 하나를 채택하려고 하고 있다. 아래의 것이 더 효율적인 구조는 맞으나 오류가 생길 수도 있다고 판단 중이다.
		Vector2 xzUnitPos = Vector2(position.x, position.z);
		if (enemyComp.mpPath != nullptr && enemyComp.mpPath->size() > currentPathIndex)
		{
			if (abs(currentPos.x - (*enemyComp.mpPath)[currentPathIndex].x) > 0.5f || abs(xzUnitPos.y - (*enemyComp.mpPath)[currentPathIndex].y) > 0.5f)
			{
				// 경로 재계산
				mpFSMHelper->CalculatePathforEnemy(enemyComp.mpPath, currentPos, goalXZPos);
				currentPathIndex = 0;
				mpFSMHelper->LookAtPathDir2(position, targetPos, enemyComp.mpPath, currentPathIndex, pathdirection, trsComp.m_localRotation.y);
				return;
			}
		}
	}
	else
	{
		tempTarget = nullptr;
	}
}
