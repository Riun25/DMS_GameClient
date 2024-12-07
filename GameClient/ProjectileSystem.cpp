#include "ProjectileSystem.h"
#include "DearsGameEngineAPI.h"
#include "PhysicsManager.h"
#include "RenderManager.h"
#include "FilterEnum.h"
#include "MoneyGunComponent.h"
#include "MoneyComponent.h"
#include "ProjectileComponent.h"
#include "SceneData.h"
#include "UserData.h"
#include "IndicatorComponent.h"

ProjectileSystem::ProjectileSystem(entt::registry& _registry, EventManager* _pEventManager
	, EntityManager* _pEntityManager, PhysicsManager* _pPhysicsManager
	, RenderManager* _pRenderManager, SoundManager* _pSoundManager
	, InputManager* _pInputManager, WorldManager* _pWorldManager)
	: m_registry(_registry), EventListener(_pEventManager)
	, m_pEntityManager(_pEntityManager), m_pPhysicsManager(_pPhysicsManager)
	, m_pRenderManager(_pRenderManager), m_pSoundManager(_pSoundManager)
	, m_pInputManager(_pInputManager), m_pWorldManager(_pWorldManager)
{
	mAstarMap = nullptr;
	mMapwidth = 31;
}

bool ProjectileSystem::Initialize()
{
	m_pWorldData = dynamic_pointer_cast<UserData>(m_pWorldManager->GetWorldData("config"));
	m_pSceneData = dynamic_cast<SceneData*>(m_pWorldManager->GetSceneData());
	m_coinSpeed = m_pWorldData->gameplay.moneySpeed;
	m_arrowSpeed = m_pWorldData->gameplay.projectileSpeed;
	auto moneyGun = m_pEntityManager->CreateEntity("MoneyGun");
	moneyGun->AddComponent<Transform>(Vector3(-20, 5, 0));
	moneyGun->AddComponent<MoneyGunComponent>(1, 1);
	moneyGun->GetComponent<MoneyGunComponent>().m_currentLevel = 1;
	moneyGun->GetComponent<MoneyGunComponent>().m_throwAmount = m_pWorldData->gameplay.moneyAmount;
	moneyGun->GetComponent<MoneyGunComponent>().m_throwCoolTime = 0.5;

	REGISTER_IMMEDIATE_EVENT("Create Money", CreateMoney);
	REGISTER_IMMEDIATE_EVENT("Throw", ThrowCoin);
	REGISTER_IMMEDIATE_EVENT("Create Arrow", CreateProjectile);
	// 	REGISTER_IMMEDIATE_EVENT("Shoot", ShootProjectile);

	mAstarMap = nullptr;
	return true;
}

void ProjectileSystem::FixedUpdate(float _fixedDTime)
{
	auto view = m_registry.view<MoneyComponent>();
	for (auto entity : view)
	{
		auto& transform = m_registry.get<Transform>(entity);
		auto& rot = transform.m_localRotation;
		rot.y += 0.1f;
	}
}

void ProjectileSystem::Update(float _dTime)
{
	// 머니건 쿨타임
	spawnTime += _dTime;

	if (spawnTime >= 0.5f)
	{
		if (m_pInputManager->GetKeyDown(KEY::LBUTTON))
		{
			Vector3 start = Vector3();
			Vector3 pick = m_pPhysicsManager->PickObejct("plane");
			pick += Vector3(0, 0.55838f, 0);
			if (pick.x > 0)pick.x += 0.5;
			else pick.x -= 0.5;
			if (pick.z < 0) pick.z -= 0.5;

			// 			pick = Vector3(astarPos.x, pick.y, astarPos.y);
			if (pick.x >= -15 && pick.x <= 15 && pick.z >= -15 && pick.z <= 15)
			{
				pick.x = static_cast<int>(pick.x);
				pick.z = static_cast<int>(pick.z);
				Vector2 astarPos = {};
				mpAstar->AdjustToTileCenter(pick, astarPos);
				if ((*mpAstar->GetAstarMap())[static_cast<int>(astarPos.y + abs(mStartPoint.y))][static_cast<int>(astarPos.x + abs(mStartPoint.x))] == 0)
				{
					auto indView = m_registry.view<IndicatorComponent>();
					for (auto entity : indView)
					{
						auto& name = m_registry.get<Name>(entity).m_name;
						auto& trsComp = m_registry.get<Transform>(entity);
						auto& pos = trsComp.m_localPosition;
						if (name == "selectedSquare")
						{
							pick.x = pos.x;
							pick.z = pos.z;
						}
					}

					//m_pEventManager->TriggerEvent(Event("Create Money"));
					auto view = m_registry.view<MoneyGunComponent>();
					for (auto entity : view)
					{
						auto& moneyGun = m_registry.get<MoneyGunComponent>(entity);
						auto& moneyGunPosition = m_registry.get<Transform>(entity).m_localPosition;
						start = moneyGunPosition;
						if (m_pSceneData->m_heldMoney >= moneyGun.m_throwAmount)
						{
							auto coin = m_pEntityManager->CreateEntity("coin");
							coin->AddComponent<Transform>(moneyGunPosition, Vector3(), Vector3(0.4f));
							coin->AddComponent<BoxCollider>(false, Vector3(), Vector3(1.5f, 0.56f, 1.5f));
							coin->AddComponent<Rigidbody>(50.f, 0.f, 0.f, true, false, 0, 1, 1);
							coin->AddComponent<MeshRenderer>("Coin.fbx", "Coin_Mesh.001", false, false, true);
							coin->AddComponent<Texture3D>("Voxel_Level_Blocks_Set.png");
							coin->AddComponent<ProjectileComponent>(0, 10, pick);
							coin->AddComponent<MoneyComponent>().m_amount = moneyGun.m_throwAmount;
							coin->AddComponent<ParticleComponent>(moneyGunPosition, 0, Vector3(1.f), 0.25f, Vector3(), 0, Vector3(), 1, Vector3(0.1f), 0
								, Vector3(), 0, Vector3(-0.05f), Vector3(0.05), Vector3(1), Vector3(), Vector3(), 0, Vector3());

							// 테스트를 위한 추가 코드임//
							/*coin->AddComponent<Text>(u8" ", "NotoSansKR-Regular(24).ttf", Vector2(), 2, true, Vector4(1.0f, 1.0f, 0.0f, 1.0f));
							coin->GetComponent<Text>().m_hasDepth = true;
							coin->GetComponent<Text>().m_idx = TEXT_IDX::FLOAT2;
							coin->GetComponent<Text>().m_offset = Vector3(0, 5, 0);*/
							//-------------------------

							const auto& uid = coin->GetUID();
							m_pPhysicsManager->AddPhysicsObject(uid, TYPE_GOLD);
							m_pRenderManager->InitailizeEntity(coin);

							//m_pEventManager->TriggerEvent(Event("Throw", coin));
							auto& projComp = coin->GetComponent<ProjectileComponent>();
							Vector3 target = projComp.m_targetPosition;
							// 방향벡터
							Vector3 dir = target - start;
							// 제어점1
							//projComp.m_control1 = start + RandomUtil::RandomFloat(0.3, 0.5) * dir + Vector3(0, RandomUtil::RandomFloat(1.f, 5.f), 0);
							projComp.m_control1 = start + 0.5 * dir + Vector3(0, RandomUtil::RandomFloat(1.f, 5.f), 0);
							// 제어점2																		 									  
							projComp.m_control2 = start + RandomUtil::RandomFloat(0.6, 0.7) * dir + Vector3(0, RandomUtil::RandomFloat(1.f, 5.f), 0);

							auto& projectile = coin->GetComponent<ProjectileComponent>();
							auto& rigidbody = coin->GetComponent<Rigidbody>();
							auto& transform = coin->GetComponent<Transform>();
							//auto& money = entity->GetComponent<MoneyComponent>();
							//if (projectile.m_isThrown == false)
							{
								//projectile.m_isThrown = true;
								//Vector3 start = transform.m_localPosition;
								//auto time = projectile.m_lifeTime;
								//time += _dTime;
								//auto mass = rigidbody.m_mass;
								//projectile.m_targetPosition = target;
								///// 
								//Vector3 force;
								//force.x = (target.x - start.x) / time;
								//force.y = (target.y - start.y + 0.5f * time * time * 9.81f) / time;
								//force.z = (target.z - start.z) / time;

								//m_pPhysicsManager->SetVelocity(projectile.m_pOwner, force);

								// 씬정보 수정
								//SceneData* data = dynamic_cast<SceneData*>(m_pWorldManager->GetCurrentWorld()->GetCurrentScene()->GetSceneData());
								//data->m_heldMoney -= money.m_amount;

								m_pSoundManager->PlaySFX("Snd_sfx_MoneyFire");
							}

							m_pSceneData->m_usedAmount += moneyGun.m_throwAmount;
							m_pSceneData->m_heldMoney -= moneyGun.m_throwAmount;
						}
					}
					mIsThrow = true;
					// 이벤트가 발생했으므로 spawnTime을 초기화합니다.
					spawnTime = 0;
				}
			}
		}
	}

	// 코인
	auto view = m_registry.view<MoneyComponent>();
	for (auto& entity : view)
	{
		auto& transform = m_registry.get<Transform>(entity);
		auto& money = m_registry.get<MoneyComponent>(entity);
		auto& porjComp = m_registry.get<ProjectileComponent>(entity);
		auto& pos = transform.m_localPosition;
		auto& rot = transform.m_localRotation;
		auto& target = porjComp.m_targetPosition;
		auto& time = porjComp.m_lifeTime;
		auto particle = m_registry.try_get<ParticleComponent>(entity);

		if (!money.m_onGround)
		{
			time += _dTime * m_coinSpeed;
		}
		else
		{
			int a = 0;
		}
		//auto rigid = m_registry.try_get<Rigidbody>(entity)->m_pRigidActor;
// 		const auto& pose = rigid->getGlobalPose();
// 		rot.y += 0.1f;
		Vector3 start = Vector3(-20, 5, 0);

		if (!money.m_onGround)
		{
			/// 곡선 방정식 계산
			// 곡선 방정식으로 계산한 시간에 따른 위치
			// Vector3 pos = std::pow((1.f - time), 3) * start + 3 * (1.f - time) * time * porjComp.m_control1
			// 	+ 3 * (1.f - time) * std::pow(time, 2) * porjComp.m_control2 + std::pow(time, 3) * target;

			// 곡선 방정식으로 계산한 시간에 따른 위치
			Vector3 pos = std::pow((1.f - time), 2) * start + 2 * (1.f - time) * time * porjComp.m_control1
				+ std::pow(time, 2) * target;

			// 파티클 위치 계산(동전과 겹치치 않도록 조금 이전의 위치에 오도록 계산)
			if (particle)
			{
				particle->m_pParticleData->position
					= std::pow((1.f - (time - 5 * _dTime)), 2) * start + 2 * (1.f - (time - 5 * _dTime)) * time * porjComp.m_control1 + std::pow(time - 5 * _dTime, 2) * target;
				//if (particle->m_pParticleData->playTime <= 2.f)
				{
					m_pRenderManager->AddParticle(2, *particle->m_pParticleData);
				}
			}
			m_pPhysicsManager->UpdatePosition(m_pEntityManager->GetEntity(entity), pos);

		}
		else
		{
			m_pPhysicsManager->SetVelocity(transform.m_pOwner, Vector3());
			m_pPhysicsManager->UpdatePosition(transform.m_pOwner, target);

		}

		if (m_pPhysicsManager->GetFilterData(transform.m_pOwner).word1 & ATTR_ON_GROUND)
		{
			money.m_onGround = true;
			if (money.m_IsInVec == false)
			{
				mMoneyPosVec.emplace_back(m_pEntityManager->GetEntity(entity), transform.m_localPosition);
				money.m_IsInVec = true;
			}
			m_pPhysicsManager->SetVelocity(transform.m_pOwner, Vector3());
			m_pPhysicsManager->UpdatePosition(transform.m_pOwner, target);
		}

		// 테스트를 위한 추가 코드임//
		/*auto text = m_registry.try_get<Text>(entity);
		text->m_pOwner->GetComponent<Text>().m_text = u8"coin \n(%.2f, %.2f)";
		Vector3& MoneyPos = text->m_pOwner->GetComponent<Transform>().m_localPosition;
		text->m_pOwner->GetComponent<Text>().m_worldPosition = MoneyPos;
		text->m_pOwner->GetComponent<Text>().m_num3 = MoneyPos.x;
		text->m_pOwner->GetComponent<Text>().m_num4 = MoneyPos.z;*/

		//-------------------------

	}

	// 충돌한 투사체 삭제
	auto proView = m_registry.view<ProjectileComponent>();
	for (auto entity : proView)
	{
		if (!m_registry.try_get<MoneyComponent>(entity))
		{
			auto& pro = m_registry.get<ProjectileComponent>(entity);
			if (pro.m_isTriggered)
			{
				m_pEntityManager->RemoveEntity(pro.m_pOwner->GetUID());
			}
		}
	}

}

void ProjectileSystem::LateUpdate(float _dTime)
{

}

void ProjectileSystem::Finalize()
{
	m_pPhysicsManager->ClearFilterUpdate();
	auto view = m_registry.view<ProjectileComponent>();
	for (auto entity : view)
	{
		m_pEntityManager->RemoveEntity(static_cast<UID>(entity));
	}

	UNREGISTER_EVENT("Create Money");
	UNREGISTER_EVENT("Throw");
	UNREGISTER_EVENT("Create Arrow");
	// 	UNREGISTER_EVENT("Shoot");

}

void ProjectileSystem::ThrowCoin(const Event& _event)
{
	auto entity = _event.GetDataAs<std::shared_ptr<Entity>>().value();
	Vector3 target = entity->GetComponent<ProjectileComponent>().m_targetPosition;

	auto& projectile = entity->GetComponent<ProjectileComponent>();
	auto& rigidbody = entity->GetComponent<Rigidbody>();
	auto& transform = entity->GetComponent<Transform>();
	//auto& money = entity->GetComponent<MoneyComponent>();
	if (projectile.m_isThrown == false)
	{
		projectile.m_isThrown = true;
		Vector3 start = transform.m_localPosition;
		auto time = projectile.m_lifeTime;
		auto mass = rigidbody.m_mass;
		projectile.m_targetPosition = target;
		/// 
		Vector3 force;
		force.x = (target.x - start.x) / time;
		force.y = (target.y - start.y + 0.5f * time * time * 9.81f) / time;
		force.z = (target.z - start.z) / time;

		m_pPhysicsManager->SetVelocity(projectile.m_pOwner, force);

		// 씬정보 수정
		//SceneData* data = dynamic_cast<SceneData*>(m_pWorldManager->GetCurrentWorld()->GetCurrentScene()->GetSceneData());
		//data->m_heldMoney -= money.m_amount;

		m_pSoundManager->PlaySFX("Snd_sfx_MoneyFire");
	}
	// 	DLOG(LOG_INFO, "Coin");
}

void ProjectileSystem::CreateMoney(const Event& _event)
{
	auto view = m_registry.view<MoneyGunComponent>();
	for (auto entity : view)
	{
		auto& moneyGun = m_registry.get<MoneyGunComponent>(entity);
		auto& moneyGunPosition = m_registry.get<Transform>(entity).m_localPosition;

		auto coin = m_pEntityManager->CreateEntity("coin");
		coin->AddComponent<Transform>(moneyGunPosition, Vector3(), Vector3(1.f));
		coin->AddComponent<SphereCollider>(false, Vector3(), 1.4f);
		coin->AddComponent<Rigidbody>(50.f, 0.f, 0.f, true, false, 0, 1, 1);
		coin->AddComponent<MeshRenderer>("Coin.fbx", "Coin_Mesh.001", false, false, true);
		coin->AddComponent<Texture3D>("Texture_Money.png");
		coin->AddComponent<ProjectileComponent>(2, 10, Vector3(0, 0, 0));
		coin->AddComponent<MoneyComponent>().m_amount = moneyGun.m_throwAmount;
		const auto& uid = coin->GetUID();
		m_pPhysicsManager->AddPhysicsObject(uid, TYPE_GOLD);
		m_pRenderManager->InitailizeEntity(coin);
	}

}
// void ProjectileSystem::ShootProjectile(const Event& _event)
// {
// 	Vector3 target = entity->GetComponent<Transform>().m_localPosition;
// 
// 	for (auto& [uid, entity] : m_pEntityManager->GetEntityMap())
// 	{
// 		if (entity->GetName() == "arrow")
// 		{
// 			if (entity->GetComponent<ProjectileComponent>().m_isThrown == false)
// 			{
// 				entity->GetComponent<ProjectileComponent>().m_isThrown = true;
// 				Vector3 start = entity->GetComponent<Transform>().m_localPosition;
// 				auto time = entity->GetComponent<ProjectileComponent>().m_lifeTime;
// 				auto mass = entity->GetComponent<Rigidbody>().m_mass;
// 				entity->GetComponent<ProjectileComponent>().m_targetPosition = target;
// 				/// 
// 				Vector3 force;
// 				force.x = target.x - start.x;
// 				force.z = target.z - start.z;
// 				force.Normalize();
// 				force *= entity->GetComponent<ProjectileComponent>().m_speed;
// 
// 				m_pPhysicsManager->SetVelocity(entity, force);
// 				auto& pos = entity->GetComponent<Transform>().m_localPosition;
// 				//m_pSoundManager->Play3DSound("bow", pos.x, pos.y, pos.z);
// 				m_pSoundManager->PlaySFX("bow");
// 			}
// 		}
// 	}
// }

void ProjectileSystem::CreateProjectile(const Event& _event)
{
	auto entity = m_pEntityManager->GetEntity(_event.GetDataAs<UID>().value());
	entity->AddComponent<BoxCollider>(true, Vector3(), Vector3(0.15f, 0.15f, 1.5f));
	entity->AddComponent<Rigidbody>(50.f, 0.f, 0.f, false, false, 0, 1, 1);
	entity->AddComponent<MeshRenderer>("Arrow 01.FBX", "Arrow 01_Mesh", false, false, true);
	entity->AddComponent<Texture3D>("Weapon Brown.png");
	entity->AddComponent<ProjectileComponent>(0.5, 10, Vector3(0, 0, 0), 10);
	m_pPhysicsManager->AddPhysicsObject(entity->GetUID(), TYPE_PROJECTILE, ATTR_ALLY);
	m_pRenderManager->InitailizeEntity(entity);
}

void ProjectileSystem::TutirialUpdateThrow(float _dTime)
{
	if (m_pInputManager->GetKey(KEY::LBUTTON))
	{
		if (mTutoSpawnTime >= 0.5f)
		{
			Vector3 start = Vector3();
			Vector3 pick = m_pPhysicsManager->PickObejct("plane");
			pick += Vector3(0, 0.55838f, 0);
			Vector2 astarPos = {};
			mpAstar->AdjustToTileCenter(pick, astarPos);
			if (pick.x >= -15 && pick.x <= 15 && pick.z >= -15 && pick.z <= 15 &&
				(*mAstarMap)[static_cast<int>(astarPos.y + abs(mStartPoint.y))][static_cast<int>(astarPos.x + abs(mStartPoint.x))] == 0)
			{
				//m_pEventManager->TriggerEvent(Event("Create Money"));
				auto view = m_registry.view<MoneyGunComponent>();
				for (auto entity : view)
				{
					auto& moneyGun = m_registry.get<MoneyGunComponent>(entity);
					auto& moneyGunPosition = m_registry.get<Transform>(entity).m_localPosition;
					start = moneyGunPosition;
					if (m_pSceneData->m_heldMoney >= moneyGun.m_throwAmount)
					{
						auto coin = m_pEntityManager->CreateEntity("coin");
						coin->AddComponent<Transform>(moneyGunPosition, Vector3(), Vector3(0.4f));
						coin->AddComponent<BoxCollider>(false, Vector3(), Vector3(1.5f, 0.56f, 1.5f));
						coin->AddComponent<Rigidbody>(50.f, 0.f, 0.f, true, false, 0, 1, 1);
						coin->AddComponent<MeshRenderer>("Coin.fbx", "Coin_Mesh.001", false, false, true);
						coin->AddComponent<Texture3D>("Voxel_Level_Blocks_Set.png");
						coin->AddComponent<ProjectileComponent>(0, 10, pick);
						coin->AddComponent<MoneyComponent>().m_amount = moneyGun.m_throwAmount;

						// 테스트를 위한 추가 코드임//
						/*coin->AddComponent<Text>(u8" ", "NotoSansKR-Regular(24).ttf", Vector2(), 2, true, Vector4(1.0f, 1.0f, 0.0f, 1.0f));
						coin->GetComponent<Text>().m_hasDepth = true;
						coin->GetComponent<Text>().m_idx = TEXT_IDX::FLOAT2;
						coin->GetComponent<Text>().m_offset = Vector3(0, 5, 0);*/
						//-------------------------

						const auto& uid = coin->GetUID();
						m_pPhysicsManager->AddPhysicsObject(uid, TYPE_GOLD);
						m_pRenderManager->InitailizeEntity(coin);

						//m_pEventManager->TriggerEvent(Event("Throw", coin));
						auto& projComp = coin->GetComponent<ProjectileComponent>();
						Vector3 target = projComp.m_targetPosition;
						// 방향벡터
						Vector3 dir = target - start;
						// 제어점1
						//projComp.m_control1 = start + RandomUtil::RandomFloat(0.3, 0.5) * dir + Vector3(0, RandomUtil::RandomFloat(1.f, 5.f), 0);
						projComp.m_control1 = start + 0.5 * dir + Vector3(0, RandomUtil::RandomFloat(1.f, 5.f), 0);
						// 제어점2																		 									  
						projComp.m_control2 = start + RandomUtil::RandomFloat(0.6, 0.7) * dir + Vector3(0, RandomUtil::RandomFloat(1.f, 5.f), 0);

						//auto& projectile = coin->GetComponent<ProjectileComponent>();
						auto& rigidbody = coin->GetComponent<Rigidbody>();
						auto& transform = coin->GetComponent<Transform>();
						//auto& money = entity->GetComponent<MoneyComponent>();
						//if (projectile.m_isThrown == false)
						{
							//projectile.m_isThrown = true;
							//Vector3 start = transform.m_localPosition;
							//auto time = projectile.m_lifeTime;
							//time += _dTime;
							//auto mass = rigidbody.m_mass;
							//projectile.m_targetPosition = target;
							///// 
							//Vector3 force;
							//force.x = (target.x - start.x) / time;
							//force.y = (target.y - start.y + 0.5f * time * time * 9.81f) / time;
							//force.z = (target.z - start.z) / time;

							//m_pPhysicsManager->SetVelocity(projectile.m_pOwner, force);

							// 씬정보 수정
							//SceneData* data = dynamic_cast<SceneData*>(m_pWorldManager->GetCurrentWorld()->GetCurrentScene()->GetSceneData());
							//data->m_heldMoney -= money.m_amount;

							m_pSoundManager->PlaySFX("Snd_sfx_MoneyFire");
						}

						m_pSceneData->m_usedAmount += moneyGun.m_throwAmount;
						m_pSceneData->m_heldMoney -= moneyGun.m_throwAmount;
					}
				}
				mIsThrow = true;
				// 이벤트가 발생했으므로 mTutoSpawnTime을 초기화합니다.
				mTutoSpawnTime = 0;
			}
		}
	}

	// 충돌한 투사체 삭제
	auto proView = m_registry.view<ProjectileComponent>();
	for (auto entity : proView)
	{
		if (!m_registry.try_get<MoneyComponent>(entity))
		{
			auto& pro = m_registry.get<ProjectileComponent>(entity);
			if (pro.m_isTriggered)
			{
				m_pEntityManager->RemoveEntity(pro.m_pOwner->GetUID());
			}
		}
	}
}

void ProjectileSystem::TutirialUpdateMoney(float _dTime)
{
	// 머니건 쿨타임
	mTutoSpawnTime += _dTime;

	// 코인
	auto view = m_registry.view<MoneyComponent>();
	for (auto entity : view)
	{
		auto& transform = m_registry.get<Transform>(entity);
		auto& money = m_registry.get<MoneyComponent>(entity);
		auto& porjComp = m_registry.get<ProjectileComponent>(entity);
		auto& pos = transform.m_localPosition;
		auto& rot = transform.m_localRotation;
		auto& target = porjComp.m_targetPosition;
		auto& time = porjComp.m_lifeTime;
		if (!money.m_onGround)
		{
			time += _dTime * m_coinSpeed;
		}

		Vector3 start = Vector3(-20, 5, 0);

		if (!money.m_onGround)
		{
			/// 곡선 방정식 계산
			// 곡선 방정식으로 계산한 시간에 따른 위치
			// Vector3 pos = std::pow((1.f - time), 3) * start + 3 * (1.f - time) * time * porjComp.m_control1
			// 	+ 3 * (1.f - time) * std::pow(time, 2) * porjComp.m_control2 + std::pow(time, 3) * target;

			Vector3 pos = std::pow((1.f - time), 2) * start + 2 * (1.f - time) * time * porjComp.m_control1
				+ std::pow(time, 2) * target;

			m_pPhysicsManager->UpdatePosition(m_pEntityManager->GetEntity(entity), pos);
		}
		else
		{
			m_pPhysicsManager->SetVelocity(transform.m_pOwner, Vector3());
			m_pPhysicsManager->UpdatePosition(transform.m_pOwner, target);
		}

		if (m_pPhysicsManager->GetFilterData(transform.m_pOwner).word1 & ATTR_ON_GROUND)
		{
			money.m_onGround = true;
			if (money.m_IsInVec == false)
			{
				mMoneyPosVec.emplace_back(m_pEntityManager->GetEntity(entity), transform.m_localPosition);
				money.m_IsInVec = true;
			}
			m_pPhysicsManager->SetVelocity(transform.m_pOwner, Vector3());
			m_pPhysicsManager->UpdatePosition(transform.m_pOwner, target);
		}
	}

}

void ProjectileSystem::SetAstar(AStar* _pAstar)
{
	mpAstar = _pAstar;
	mAstarMap = mpAstar->GetAstarMap();
	mStartPoint = mpAstar->GetMapStartPoint();
}

std::vector<std::pair<std::shared_ptr<Entity>, Vector3>>* ProjectileSystem::GetMoneyPosVec()
{
	return &mMoneyPosVec;
}
