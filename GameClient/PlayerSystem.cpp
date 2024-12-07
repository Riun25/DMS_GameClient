#include "PlayerSystem.h"
#include "WeaponComponent.h"
#include "AnimationComponent.h"
#include "ParticleComponent.h"
// 임시로 추가하는 헤더
#include "Text.h"

PlayerSystem::PlayerSystem(entt::registry& _registry, EntityManager* _pEntityManager, PhysicsManager* _pPhysicsManager
	, RenderManager* _pRenderManager, ResourceManager* _pResourceManager, EventManager* _pEventManager
	, SoundManager* _pSoundManager, EquipmentManager* _pEquipmentManager)
	: mRegistry(_registry)
{
	mpStatusManager = new StatusManager(_pEntityManager, _pResourceManager);
	mpEquipmentManager = _pEquipmentManager;
	mpEntityManager = _pEntityManager;
	mpPhysicsManager = _pPhysicsManager;
	mpResourceManager = _pResourceManager;
	mpSoundManager = _pSoundManager;
	mpFSMHelper = new FSMHelper(_pResourceManager);
	mpAllyFSM = new AllyFSM(_pEntityManager, _pPhysicsManager, _pResourceManager, _pEventManager, _pSoundManager, _pRenderManager, mpFSMHelper);
	mpEnemyFSM = new EnemyFSM(_pEntityManager, _pPhysicsManager, _pResourceManager, _pEventManager, _pSoundManager, _pRenderManager, mpFSMHelper);
	mpEnemyPosVec = {};

	// 기획에서 고정해준 값으로, 소지금의 값이다.
	mMaxSetUnitNum = 20;  // 최대 배치 가능한 유닛 수
	mBuffLv1_Limit = 20;
	mBuffLv2_Limit = 50;

	unitDeployPosVec.resize(mMaxSetUnitNum); // 배치할 유닛 수의 최대 사이즈로 메모리 할당(재할당을 막기 위함)
	// 삭제할 것임
	mpRenderManager = _pRenderManager;
}

PlayerSystem::~PlayerSystem()
{
	delete mpStatusManager;
}

void PlayerSystem::Initialize(std::vector<std::pair<std::shared_ptr<Entity>, Vector3>>* _pMoneyPosVec, AStar* _pAstar)
{
	// 여기서 플레이어 시스템에 종속된 모든 매니저를 초기화한다.
	mpStatusManager->Initialize();
	mpEnemyPosVec = mpResourceManager->ParseCSV<EnemyData>("Enemy.csv");

	// FSM 초기화
	mpFSMHelper->Initialize();
	// 투사체 시스템의 moneyPosVec* 전달하기(Loot에서 Vector삭제)
	mpFSMHelper->SetMoneyVec(_pMoneyPosVec);
	mpFSMHelper->SetAstar(_pAstar);

	mpAllyFSM->Initialize();
	mpEnemyFSM->Initialize();


}

void PlayerSystem::PlayerUnitUpdate(std::shared_ptr<Entity> _entity, float _dTime)
{
	/// 배치 후 시작됐을 때
	auto& playerComp = _entity->GetComponent<PlayerComponent>();

	// 죽은 유닛은 업데이트 하지 않는다.
	if (playerComp.mIsDead == true)
	{
		return;
	}

	// 각 FSM이 실행됩니다.
	mpAllyFSM->Update(_dTime, _entity);

	// 타겟이 없는 것들은 타겟을 재설정합니다.(PickBestTarget은 위치가 변경될 가능성이 큰 함수입니다.)
	if (playerComp.mpTarget == nullptr)
	{
		//playerComp.mpTarget = EnemyTargetInRange(_entity);
		playerComp.mpTarget = TargetNearestEnemy(_entity);
	}

	// 소지금을 확인해서 버프 레벨을 갱신합니다.(이건 매프레임 진행되어야 한다.)
	int mTempBuffLv = 0;
	if (playerComp.mMoney >= playerComp.mBuffMoney1) // 30이상이면 버프 변경 -> 버프 금액 이상이면 변경
	{
		mTempBuffLv = 1;
		if (playerComp.mMoney >= playerComp.mBuffMoney1 + playerComp.mBuffMoney2)
		{
			mTempBuffLv = 2;
		}
	}

	if (playerComp.mbuffLv != mTempBuffLv) // 이건 버프가 바꼈을 때만 진행하는 게 좋다. (리소스를 바꾸는 거니...최대한 효율적으로..)
	{
		// status와 장비 파츠를 갈아낀다.
		std::string stringClasstype = mpStatusManager->ConvertClasstypeToString(playerComp.mClassType);

		mpStatusManager->UpdateAllyStatus(_entity, stringClasstype, mTempBuffLv);
		mpEquipmentManager->UpdateEquipment(_entity, stringClasstype, mTempBuffLv);
		playerComp.mbuffLv = mTempBuffLv;
		mpSoundManager->PlaySFX("Snd_sfx_CharaBuff");
	}
}

void PlayerSystem::EnemyUnitUpdate(std::shared_ptr<Entity> _entity, float _dTime)
{
	/// 배치 후 시작됐을 때
	auto& playerComp = _entity->GetComponent<EnemyComponent>();

	// 죽은 유닛은 업데이트 하지 않는다.
	if (playerComp.mIsDead == true)
	{
		return;
	}

	// 각 FSM이 실행됩니다.
	mpEnemyFSM->Update(_dTime, _entity);

	// 타겟이 없는 것들은 타겟을 재설정합니다.(PickBestTarget은 위치가 변경될 가능성이 큰 함수입니다.)
	if (playerComp.mpTarget == nullptr)
	{
		//playerComp.mpTarget = PlayerTargetInRange(_entity);
		playerComp.mpTarget = TargetNearestPlayer(_entity);
	}
}

void PlayerSystem::FixedUpdate(std::shared_ptr<Entity> _entity, float _fixedDTime)
{
	if (_entity->HasComponent<PlayerComponent>())
	{
		mpAllyFSM->FixedUpdate(_fixedDTime, _entity);
	}
	else if (_entity->HasComponent<EnemyComponent>())
	{
		mpEnemyFSM->FixedUpdate(_fixedDTime, _entity);
	}
}

void PlayerSystem::Finalize()
{
}

void PlayerSystem::AddUnit(std::shared_ptr<Entity> _entity, const std::string& _classType, Vector3 _pos)
{
	// 테이블로부터 기본 스텟과 메쉬를 추가한다.
	mpStatusManager->AddInitStatus(_entity, _classType);
	_entity->GetComponent<Transform>().m_localPosition = _pos;

	// 장비를 추가한다.
	mpEquipmentManager->AddInitUnitEquipment(_entity, _classType);

	// 먼지 파티클을 추가한다.
	_entity->AddComponent<ParticleComponent>(Vector3(), 0, Vector3(0.78, 0.55, 0.3), 0.5, Vector3(0, 1, 0), 0, Vector3(), 1
		, Vector3(0.1), -0.5, Vector3(), 0.1, Vector3(0.1), Vector3(0.2), Vector3(), Vector3(1, 0.3, 1), Vector3(), 0, Vector3());

	// 물리 추가
	AddPhysics(_entity);
}

void PlayerSystem::AddDeplyUnit(std::shared_ptr<Entity> _entity, const std::string& _classType)
{
	// 테이블로부터 기본 스텟과 메쉬를 추가한다.
	mpStatusManager->AddStatus(_entity, _classType, 0);

	// 먼지 파티클을 추가한다.
	_entity->AddComponent<ParticleComponent>(Vector3(), 0, Vector3(0.78, 0.55, 0.3), 0.5, Vector3(0,1,0), 0, Vector3(), 1
		, Vector3(0.1), -0.5, Vector3(), 0.1, Vector3(0.1), Vector3(0.2), Vector3(), Vector3(1, 0.3, 1), Vector3(), 0, Vector3());

	// 장비를 추가한다.
	mpEquipmentManager->AddUnitEquip(_entity, _classType, 0);
	mpRenderManager->InitailizeEntity(_entity);
}

void PlayerSystem::ChangeUnit(std::shared_ptr<Entity> _entity, const std::string& _classType)
{
	// 같은 클래스가 아닐 경우만 변경함
	if (_classType != mpStatusManager->ConvertClasstypeToString(_entity->GetComponent<PlayerComponent>().mClassType))
	{
		// 테이블로부터 기본 스텟과 메쉬를 변경한다.
		mpStatusManager->UpdateStautsNClass(_entity, _classType);

		// 장비를 변경한다.
		mpEquipmentManager->UpdateEquipment(_entity, _classType, 0);
	}
}

void PlayerSystem::RemoveUnit(UID _entityUID)
{
	auto unitEntity = mpEntityManager->GetEntity(_entityUID);
	// 	auto& myEntityVec = unitEntity->GetComponent<AnimationComponent>().myEntityVec;

	// 	for (auto& equipments : myEntityVec)
	// 	{
	// 		mpEntityManager->RemoveEntity(equipments->GetUID());
	// 	}
	mpEntityManager->RemoveEntity(_entityUID);
}

void PlayerSystem::AddPhysics(std::shared_ptr<Entity> _entity)
{
	// 피직스 정보를 추가한다.
	_entity->AddComponent<CapsuleCollider>(false, Vector3(0, 0.78f, 0), 0.3f, 0.38f);
	//_entity->GetComponent<CapsuleCollider>().m_radius = 30 * _entity->GetComponent<Transform>().m_localScale.x/*0.4 * 100 * pow(mBuffUnitSizeCoef, _buffLv)*/;
	_entity->AddComponent<Rigidbody>(50.f, 0.f, 0.f, true, false, 0.5, 0.8, 0.8);

	// 부족한 정보 업데이트
	if (_entity->HasComponent<PlayerComponent>())
	{
		//_entity->GetComponent<Tr
		//_entity->GetComponent<PlayerComponent>().mAstarPos = Vector2(_entity->GetComponent<Transform>().m_localPosition.x, _entity->GetComponent<Transform>().m_localPosition.z);
		mpPhysicsManager->AddPhysicsObject(_entity->GetUID(), TYPE_UNIT, ATTR_ALLY);
	}
	else if (_entity->HasComponent<EnemyComponent>())
	{
		mpPhysicsManager->AddPhysicsObject(_entity->GetUID(), TYPE_UNIT, ATTR_ENEMY);
	}
	//ShowState(_entity);

	mpPhysicsManager->SetFixPosition(_entity, false);
}

void PlayerSystem::AddSceneEnemyUnits(UID _sceneUID)
{
	for (const auto& enemyData : mpEnemyPosVec)
	{
		if (_sceneUID == UID(enemyData.sceneNum))
		{
			auto enemy = mpEntityManager->CreateEntity("enemy");
			AddUnit(enemy, enemyData.className, Vector3(enemyData.posX, 0.0f, enemyData.posZ));
		}
	}
}

void PlayerSystem::SetInitializeState(std::shared_ptr<Entity> _entity, State _state)
{
	// 각 플레이어의 FSM 상태 초기화
	if (_entity->HasComponent<PlayerComponent>())
	{
		auto& playerComponent = _entity->GetComponent<PlayerComponent>();
		playerComponent.mCurrentSM = mpAllyFSM->GetState(_state);
		playerComponent.mCurrentSM->Enter(playerComponent.m_pOwner);
	}
	else if (_entity->HasComponent<EnemyComponent>())
	{
		auto& enemyComponent = _entity->GetComponent<EnemyComponent>();
		enemyComponent.mCurrentSM = mpEnemyFSM->GetState(_state);
		enemyComponent.mCurrentSM->Enter(enemyComponent.m_pOwner);
	}
}

bool PlayerSystem::IsAttackable(const Vector2& _myPos, const Vector2& _otherPos, float _attackRange)
{
	// x의 거리와 y의 거리가 attackRange의 안쪽이라면 공격 가능 상태다.
	if (abs(_otherPos.x - _myPos.x) <= _attackRange &&
		abs(_otherPos.y - _myPos.y) <= _attackRange)
	{
		return true;
	}
	return false;
}

void PlayerSystem::ShowState(std::shared_ptr<Entity> _entity)
{
	// 폰트 추가
	auto& textComp = _entity->AddComponent<Text>(u8" ", "KIMM_Bold(60).ttf", Vector2(), 2, true, Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	textComp.m_hasDepth = true;
	textComp.m_idx = TEXT_IDX::FLOAT2;
	textComp.m_offset = Vector3(0, 5, 0);
	textComp.m_scale = 0.4f;
}

void PlayerSystem::UpdateStateText(std::shared_ptr<Entity> _entity)
{
	std::u8string stateText = u8"";
	State currentState;

	if (_entity->HasComponent<PlayerComponent>())
	{
		currentState = _entity->GetComponent<PlayerComponent>().mOwnState;
	}
	else
	{
		currentState = _entity->GetComponent<EnemyComponent>().mOwnState;
	}

	switch (currentState)
	{
	case State::IDLE:
		stateText = u8"IDLE\n(%.2f, %.2f)";
		break;
	case State::MOVE:
		stateText = u8"MOVE\n(%.2f, %.2f)";
		break;
	case State::ATTACK:
		stateText = u8"ATTACK\n(%.2f, %.2f)";
		break;
	case State::LOOT:
		stateText = u8"LOOT\n(%.2f, %.2f)";
		break;
	case State::DAMAGED:
		stateText = u8"DAMAGED\n(%.2f, %.2f)";
		break;
	case State::DEAD:
		stateText = u8"DEAD\n(%.2f, %.2f)";
		break;
	default:
		break;
	}

	// std::string을 std::u8string으로 변환
	auto& textComp = _entity->GetComponent<Text>();
	textComp.m_text = stateText;
	Vector3& playerPos = _entity->GetComponent<Transform>().m_localPosition;
	textComp.m_worldPosition = playerPos;
	textComp.m_num3 = playerPos.x;
	textComp.m_num4 = playerPos.z;
}

void PlayerSystem::DetectGroundMoney()
{
	auto& moneyVec = *mpFSMHelper->GetMoneyVec(); // 돈 벡터 참조 얻기
	//auto& astarObsPosVec = *mpFSMHelper->GetAstarObstacles(); // 장애물 벡터 참조 얻기

	// 만약 돈 위치를 저장한 벡터의 사이즈가 0이 아니고, 혹은 사이즈가 바뀐다면
	if (moneyVec.size() != tempMoneyVecSize || moneyVec.size() > 0)
	{
		//for (auto it = moneyVec.begin(); it != moneyVec.end(); )
		//{
		//	if (it->first.x == astarObsPosVec.x) // 특정 타겟과 일치하는 요소를 찾음
		//	{
		//		it = moneyVec.erase(it); // 해당 요소 삭제 후, 다음 요소로 이동
		//	}
		//	else
		//	{
		//		++it; // 일치하지 않으면 다음 요소로 이동
		//	}
		//}



		// 사이즈를 변경해준다.
		tempMoneyVecSize = mpFSMHelper->GetMoneyVec()->size();

		auto playerView = mRegistry.view<Transform, PlayerComponent>();
		for (auto& entity : playerView)
		{
			auto& playerComp = mRegistry.get<PlayerComponent>(entity);
			auto& trsComp = mRegistry.get<Transform>(entity);
			if (playerComp.mOwnState != State::DEAD) // 타겟이 죽었으면 행동하지 않는다.
			{
				// 현재 유닛과 가장 가까운 돈의 엔티티와 거리값을 우선순위 큐로 뽑아온다.
				std::pair<std::shared_ptr<Entity>, float> nearlistMoney = FindClosestEntity(trsComp.m_localPosition);

				// 돈의 인식범위 이내이고, 돈이 nullptr이 아니면 타겟으로 지정한다. 이 부분에서 ground를 계산하는 것은, 우선순위 큐에서 벡터마다 Get을 하는 것보다 여기서 한 번 하는 게 낫기 때문이다..
				if (playerComp.mRecogRange > nearlistMoney.second && nearlistMoney.first != nullptr)
				{
					playerComp.mpTarget = nearlistMoney.first;
				}
				else
				{
					// 만약 돈 위치를 저장한 벡터의 사이즈가 0이라면 타겟이 적군이 아닌 것들은 전부 타깃을 nullptr로 만든다.
					if (tempMoneyVecSize == 0 && playerComp.mpTarget && playerComp.mpTarget->HasComponent<EnemyComponent>() == false)
					{
						playerComp.mpTarget = nullptr;
					}
				}
			}
		}

		auto enemyView = mRegistry.view<Transform, EnemyComponent>();
		for (auto& entity : enemyView)
		{
			//auto& enemyComp = mRegistry.get<EnemyComponent>(entity);
			auto enemyComp = mRegistry.try_get<EnemyComponent>(entity);
			auto& trsComp = mRegistry.get<Transform>(entity);
			//if (enemyComp.mOwnState != State::DEAD) // 타겟이 죽었으면 행동하지 않는다.
			//{
			//	// 현재 유닛과 가장 가까운 돈의 엔티티와 거리값을 우선순위 큐로 뽑아온다.
			//	std::pair<std::shared_ptr<Entity>, float> nearlistMoney = FindClosestEntity(trsComp.m_localPosition);

			//	// 돈의 인식범위 이내이고, 돈이 nullptr이 아니면 타겟으로 지정한다.
			//	if (enemyComp.mRecogRange > nearlistMoney.second && nearlistMoney.first != nullptr)
			//	{
			//		enemyComp.mpTarget = nearlistMoney.first;
			//	}
			//	else
			//	{
			//		// 만약 돈 위치를 저장한 벡터의 사이즈가 0이라면 타겟이 적군이 아닌 것들은 전부 타깃을 nullptr로 만든다.
			//		if (tempMoneyVecSize == 0 && enemyComp.mpTarget && enemyComp.mpTarget->HasComponent<PlayerComponent>() == false)
			//		{
			//			enemyComp.mpTarget = nullptr;
			//		}
			//	}
			//}
			if (enemyComp->mOwnState != State::DEAD) // 타겟이 죽었으면 행동하지 않는다.
			{
				// 현재 유닛과 가장 가까운 돈의 엔티티와 거리값을 우선순위 큐로 뽑아온다.
				std::pair<std::shared_ptr<Entity>, float> nearlistMoney = FindClosestEntity(trsComp.m_localPosition);

				// 돈의 인식범위 이내이고, 돈이 nullptr이 아니면 타겟으로 지정한다.
				if (enemyComp->mRecogRange > nearlistMoney.second && nearlistMoney.first != nullptr)
				{
					enemyComp->mpTarget = nearlistMoney.first;
				}
				else
				{
					// 만약 돈 위치를 저장한 벡터의 사이즈가 0이라면 타겟이 적군이 아닌 것들은 전부 타깃을 nullptr로 만든다.
					if (tempMoneyVecSize == 0 && enemyComp->mpTarget && enemyComp->mpTarget->HasComponent<PlayerComponent>() == false)
					{
						enemyComp->mpTarget = nullptr;
					}
				}
			}
		}
	}
}

std::shared_ptr<Entity> PlayerSystem::TargetNearestEnemy(std::shared_ptr<Entity> _entity)
{
	//		 돈과의 거리 <= 인식 범위
	//			|				\
	//			|				 \
	//		  아군인가?         타겟 설정 : 가장 가까운 적
	//		|		  \					  ↑
	//		|		   \				  |
	//	소지금<상한선?	타겟 설정 : 돈	  |
	//		|	\______________↑__________|
	//		|__________________|

	auto& position = _entity->GetComponent<Transform>().m_localPosition;

	// 거리 순으로 정렬하기 위한 주머니 선언
	std::vector<std::pair<std::shared_ptr<Entity>, float>> entities;

	/// 가까운 적(아군, 적군 포함)을 타겟으로 지정합니다.
	auto PlayerView = mRegistry.view<EnemyComponent>();
	for (auto& entity : PlayerView)
	{
		auto& enemyComponent = mRegistry.get<EnemyComponent>(entity);
		// 죽은 유닛은 업데이트 하지 않는다.
		if (enemyComponent.mIsDead == true)
		{
			continue;
		}

		// 가장 가까운 타깃을 계산한다.
		Vector3& notAllyUnit = mRegistry.get<Transform>(entity).m_localPosition; // 타깃 유닛의 위치
		float distance = (notAllyUnit - position).Length();
		entities.emplace_back(enemyComponent.m_pOwner, distance);
	}

	// 가장 가까운 엔티티 반환
	if (!entities.empty())
	{
		std::sort(entities.begin(), entities.end(), [](const auto& lhs, const auto& rhs)
			{return lhs.second < rhs.second; });

		return entities.front().first;
	}

	/// 아무것도 없으면 nullptr을 반환
	return nullptr;
}

std::shared_ptr<Entity> PlayerSystem::EnemyTargetInRange(std::shared_ptr<Entity> _entity)
{
	auto& playerComp = _entity->GetComponent<PlayerComponent>();

	/// 범위 내에 적군이 존재하는지 확인합니다.
	auto PlayerView = mRegistry.view<EnemyComponent>();
	for (auto& entity : PlayerView)
	{
		auto& enemyComponent = mRegistry.get<EnemyComponent>(entity);
		// 죽은 유닛은 타겟에서 제외한다.
		if (enemyComponent.mIsDead == true)
		{
			continue;
		}

		//	// 각 값을 올림 처리하여 인트로 처리한다.
		int attackRange = static_cast<int>(ceil(playerComp.mAttackRange));

		// 범위 안에 들어온 타깃을 선택한다.
		if (IsAttackable(playerComp.mAstarPos, enemyComponent.mAstarPos, attackRange) == true)
		{
			return mpEntityManager->GetEntity(entity);
		}
	}

	// 못 찾았으면 nullptr임
	return nullptr;
}

std::shared_ptr<Entity> PlayerSystem::TargetNearestPlayer(std::shared_ptr<Entity> _entity)// _entity가 나고, entity가 남이야
{
	auto& position = _entity->GetComponent<Transform>().m_localPosition;

	// 거리 순으로 정렬하기 위한 주머니 선언
	std::vector<std::pair<std::shared_ptr<Entity>, float>> entities;

	/// 가까운 적(아군)을 타겟으로 지정합니다.
	auto PlayerView = mRegistry.view<PlayerComponent>();
	for (auto& entity : PlayerView)
	{
		auto& playerComponent = mRegistry.get<PlayerComponent>(entity);
		{
			// 죽은 유닛은 업데이트 하지 않는다.
			if (playerComponent.mIsDead == true)
			{
				continue;
			}

			// 가장 가까운 타깃을 계산한다.
			Vector3& notAllyUnit = mRegistry.get<Transform>(entity).m_localPosition; // 타깃 유닛의 위치
			float distance = (notAllyUnit - position).Length();
			entities.emplace_back(playerComponent.m_pOwner, distance);
		}
	}

	// 가장 가까운 엔티티 반환
	if (!entities.empty())
	{
		std::sort(entities.begin(), entities.end(), [](const auto& lhs, const auto& rhs)
			{return lhs.second < rhs.second; });

		return entities.front().first;
	}

	/// 아무것도 없으면 nullptr을 반환
	return nullptr;
}

std::shared_ptr<Entity> PlayerSystem::PlayerTargetInRange(std::shared_ptr<Entity> _entity)
{
	auto& enemyComp = _entity->GetComponent<EnemyComponent>();

	/// 범위 내에 플레이어가 존재하는지 확인합니다.
	auto PlayerView = mRegistry.view<PlayerComponent>();
	for (auto& entity : PlayerView)
	{
		auto& playerComp = mRegistry.get<PlayerComponent>(entity);
		// 죽은 유닛은 타겟에서 제외한다.
		if (playerComp.mIsDead == true)
		{
			continue;
		}

		//	// 각 값을 올림 처리하여 인트로 처리한다.
		int attackRange = static_cast<int>(ceil(enemyComp.mAttackRange));

		// 범위 안에 들어온 타깃을 선택한다.
		if (IsAttackable(enemyComp.mAstarPos, playerComp.mAstarPos, attackRange) == true)
		{
			return mpEntityManager->GetEntity(entity);
		}
	}

	// 못 찾았으면 nullptr임
	return nullptr;
}

std::vector<Vector2>* PlayerSystem::GetUnitDeployPosVec()
{
	return &unitDeployPosVec;
}

std::pair<std::shared_ptr<Entity>, float> PlayerSystem::FindClosestEntity(const Vector3& _pos)
{
	// 우선순위 큐 선언
	std::priority_queue<std::pair<std::shared_ptr<Entity>, float>,
		std::vector<std::pair<std::shared_ptr<Entity>, float>>, DistanceCompare> closetList;

	// mpMoneyPosVec에 있는 엔티티들과 _pos 간의 거리 계산 및 우선순위 큐에 추가
	for (const auto& e : *mpFSMHelper->GetMoneyVec())
	{
		float distance = (_pos - e.second).Length(); // _pos와 e의 위치 차이 계산
		closetList.emplace(e.first, distance);		 // 엔티티와 거리를 큐에 추가
	}

	// 큐가 비어있지 않다면 가장 가까운 엔티티와 거리 반환
	if (!closetList.empty())
	{
		return closetList.top(); // 엔티티와 거리 값 모두 반환
	}

	// 큐가 비어있을 때는 기본 값(혹은 nullptr)을 반환
	return { nullptr, 30.0f };
}
