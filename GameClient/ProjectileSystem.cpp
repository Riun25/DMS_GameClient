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
	: mRegistry(_registry), EventListener(_pEventManager)
	, mpEntityManager(_pEntityManager), mpPhysicsManager(_pPhysicsManager)
	, mpRenderManager(_pRenderManager), mpSoundManager(_pSoundManager)
	, mpInputManager(_pInputManager), mpWorldManager(_pWorldManager)
{
	mAstarMap = nullptr;
	mMapwidth = 31;
}

bool ProjectileSystem::Initialize()
{
	m_pWorldData = dynamic_pointer_cast<UserData>(mpWorldManager->GetWorldData("config"));
	mpSceneData = dynamic_cast<SceneData*>(mpWorldManager->GetSceneData());
	m_coinSpeed = m_pWorldData->gameplay.moneySpeed;
	m_arrowSpeed = m_pWorldData->gameplay.projectileSpeed;
	auto moneyGun = mpEntityManager->CreateEntity("MoneyGun");
	moneyGun->AddComponent<Transform>(Vector3(-20, 5, 0));
	moneyGun->AddComponent<MoneyGunComponent>(1, 1);
	moneyGun->GetComponent<MoneyGunComponent>().mCurrentLevel = 1;
	moneyGun->GetComponent<MoneyGunComponent>().mThrowAmount = m_pWorldData->gameplay.moneyAmount;
	moneyGun->GetComponent<MoneyGunComponent>().mThrowCoolTime = 0.5;

	REGISTER_IMMEDIATE_EVENT("Create Money", CreateMoney);
	REGISTER_IMMEDIATE_EVENT("Throw", ThrowCoin);
	REGISTER_IMMEDIATE_EVENT("Create Arrow", CreateProjectile);
	// 	REGISTER_IMMEDIATE_EVENT("Shoot", ShootProjectile);

	mAstarMap = nullptr;
	return true;
}

void ProjectileSystem::FixedUpdate(float _fixedDTime)
{
	auto view = mRegistry.view<MoneyComponent>();
	for (auto entity : view)
	{
		auto& transform = mRegistry.get<Transform>(entity);
		auto& rot = transform.mLocalRotation;
		rot.y += 0.1f;
	}
}

void ProjectileSystem::Update(float _dTime)
{
	// 머니건 쿨타임
	spawnTime += _dTime;

	if (spawnTime >= 0.5f)
	{
		if (mpInputManager->GetKeyDown(KEY::LBUTTON))
		{
			Vector3 start = Vector3();
			Vector3 pick = mpPhysicsManager->PickObejct("plane");
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
					auto indView = mRegistry.view<IndicatorComponent>();
					for (auto entity : indView)
					{
						auto& name = mRegistry.get<Name>(entity).mName;
						auto& trsComp = mRegistry.get<Transform>(entity);
						auto& pos = trsComp.mLocalPosition;
						if (name == "selectedSquare")
						{
							pick.x = pos.x;
							pick.z = pos.z;
						}
					}

					//mpEventManager->TriggerEvent(Event("Create Money"));
					auto view = mRegistry.view<MoneyGunComponent>();
					for (auto entity : view)
					{
						auto& moneyGun = mRegistry.get<MoneyGunComponent>(entity);
						auto& moneyGunPosition = mRegistry.get<Transform>(entity).mLocalPosition;
						start = moneyGunPosition;
						if (mpSceneData->m_heldMoney >= moneyGun.mThrowAmount)
						{
							auto coin = mpEntityManager->CreateEntity("coin");
							coin->AddComponent<Transform>(moneyGunPosition, Vector3(), Vector3(0.4f));
							coin->AddComponent<BoxCollider>(false, Vector3(), Vector3(1.5f, 0.56f, 1.5f));
							coin->AddComponent<Rigidbody>(50.f, 0.f, 0.f, true, false, 0, 1, 1);
							coin->AddComponent<MeshRenderer>("Coin.fbx", "Coin_Mesh.001", false, false, true);
							coin->AddComponent<Texture3D>("Voxel_Level_Blocks_Set.png");
							coin->AddComponent<ProjectileComponent>(0, 10, pick);
							coin->AddComponent<MoneyComponent>().mAmount = moneyGun.mThrowAmount;
							coin->AddComponent<ParticleComponent>(moneyGunPosition, 0, Vector3(1.f), 0.25f, Vector3(), 0, Vector3(), 1, Vector3(0.1f), 0
								, Vector3(), 0, Vector3(-0.05f), Vector3(0.05), Vector3(1), Vector3(), Vector3(), 0, Vector3());

							// 테스트를 위한 추가 코드임//
							/*coin->AddComponent<Text>(u8" ", "NotoSansKR-Regular(24).ttf", Vector2(), 2, true, Vector4(1.0f, 1.0f, 0.0f, 1.0f));
							coin->GetComponent<Text>().mHasDepth = true;
							coin->GetComponent<Text>().mIdx = TEXT_IDX::FLOAT2;
							coin->GetComponent<Text>().mOffset = Vector3(0, 5, 0);*/
							//-------------------------

							const auto& uid = coin->GetUID();
							mpPhysicsManager->AddPhysicsObject(uid, TYPE_GOLD);
							mpRenderManager->InitailizeEntity(coin);

							//mpEventManager->TriggerEvent(Event("Throw", coin));
							auto& projComp = coin->GetComponent<ProjectileComponent>();
							Vector3 target = projComp.mTargetPosition;
							// 방향벡터
							Vector3 dir = target - start;
							// 제어점1
							//projComp.mControl1 = start + RandomUtil::RandomFloat(0.3, 0.5) * dir + Vector3(0, RandomUtil::RandomFloat(1.f, 5.f), 0);
							projComp.mControl1 = start + 0.5 * dir + Vector3(0, RandomUtil::RandomFloat(1.f, 5.f), 0);
							// 제어점2																		 									  
							projComp.mControl2 = start + RandomUtil::RandomFloat(0.6, 0.7) * dir + Vector3(0, RandomUtil::RandomFloat(1.f, 5.f), 0);

							auto& projectile = coin->GetComponent<ProjectileComponent>();
							auto& rigidbody = coin->GetComponent<Rigidbody>();
							auto& transform = coin->GetComponent<Transform>();
							//auto& money = entity->GetComponent<MoneyComponent>();
							//if (projectile.mIsThrown == false)
							{
								//projectile.mIsThrown = true;
								//Vector3 start = transform.mLocalPosition;
								//auto time = projectile.mLifeTime;
								//time += _dTime;
								//auto mass = rigidbody.mMass;
								//projectile.mTargetPosition = target;
								///// 
								//Vector3 force;
								//force.x = (target.x - start.x) / time;
								//force.y = (target.y - start.y + 0.5f * time * time * 9.81f) / time;
								//force.z = (target.z - start.z) / time;

								//mpPhysicsManager->SetVelocity(projectile.mpOwner, force);

								// 씬정보 수정
								//SceneData* data = dynamic_cast<SceneData*>(mpWorldManager->GetCurrentWorld()->GetCurrentScene()->GetSceneData());
								//data->m_heldMoney -= money.mAmount;

								mpSoundManager->PlaySFX("Snd_sfx_MoneyFire");
							}

							mpSceneData->m_usedAmount += moneyGun.mThrowAmount;
							mpSceneData->m_heldMoney -= moneyGun.mThrowAmount;
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
	auto view = mRegistry.view<MoneyComponent>();
	for (auto& entity : view)
	{
		auto& transform = mRegistry.get<Transform>(entity);
		auto& money = mRegistry.get<MoneyComponent>(entity);
		auto& porjComp = mRegistry.get<ProjectileComponent>(entity);
		auto& pos = transform.mLocalPosition;
		auto& rot = transform.mLocalRotation;
		auto& target = porjComp.mTargetPosition;
		auto& time = porjComp.mLifeTime;
		auto particle = mRegistry.try_get<ParticleComponent>(entity);

		if (!money.mIsOnGround)
		{
			time += _dTime * m_coinSpeed;
		}
		else
		{
			int a = 0;
		}
		//auto rigid = mRegistry.try_get<Rigidbody>(entity)->m_pRigidActor;
// 		const auto& pose = rigid->getGlobalPose();
// 		rot.y += 0.1f;
		Vector3 start = Vector3(-20, 5, 0);

		if (!money.mIsOnGround)
		{
			/// 곡선 방정식 계산
			// 곡선 방정식으로 계산한 시간에 따른 위치
			// Vector3 pos = std::pow((1.f - time), 3) * start + 3 * (1.f - time) * time * porjComp.mControl1
			// 	+ 3 * (1.f - time) * std::pow(time, 2) * porjComp.mControl2 + std::pow(time, 3) * target;

			// 곡선 방정식으로 계산한 시간에 따른 위치
			Vector3 pos = std::pow((1.f - time), 2) * start + 2 * (1.f - time) * time * porjComp.mControl1
				+ std::pow(time, 2) * target;

			// 파티클 위치 계산(동전과 겹치치 않도록 조금 이전의 위치에 오도록 계산)
			if (particle)
			{
				particle->mpParticleData->position
					= std::pow((1.f - (time - 5 * _dTime)), 2) * start + 2 * (1.f - (time - 5 * _dTime)) * time * porjComp.mControl1 + std::pow(time - 5 * _dTime, 2) * target;
				//if (particle->mpParticleData->playTime <= 2.f)
				{
					mpRenderManager->AddParticle(2, *particle->mpParticleData);
				}
			}
			mpPhysicsManager->UpdatePosition(mpEntityManager->GetEntity(entity), pos);

		}
		else
		{
			mpPhysicsManager->SetVelocity(transform.mpOwner, Vector3());
			mpPhysicsManager->UpdatePosition(transform.mpOwner, target);

		}

		if (mpPhysicsManager->GetFilterData(transform.mpOwner).word1 & ATTR_ON_GROUND)
		{
			money.mIsOnGround = true;
			if (money.mIsInVec == false)
			{
				mMoneyPosVec.emplace_back(mpEntityManager->GetEntity(entity), transform.mLocalPosition);
				money.mIsInVec = true;
			}
			mpPhysicsManager->SetVelocity(transform.mpOwner, Vector3());
			mpPhysicsManager->UpdatePosition(transform.mpOwner, target);
		}

		// 테스트를 위한 추가 코드임//
		/*auto text = mRegistry.try_get<Text>(entity);
		text->mpOwner->GetComponent<Text>().mText = u8"coin \n(%.2f, %.2f)";
		Vector3& MoneyPos = text->mpOwner->GetComponent<Transform>().mLocalPosition;
		text->mpOwner->GetComponent<Text>().mWorldPosition = MoneyPos;
		text->mpOwner->GetComponent<Text>().mNum3 = MoneyPos.x;
		text->mpOwner->GetComponent<Text>().mNum4 = MoneyPos.z;*/

		//-------------------------

	}

	// 충돌한 투사체 삭제
	auto proView = mRegistry.view<ProjectileComponent>();
	for (auto entity : proView)
	{
		if (!mRegistry.try_get<MoneyComponent>(entity))
		{
			auto& pro = mRegistry.get<ProjectileComponent>(entity);
			if (pro.mIsTriggered)
			{
				mpEntityManager->RemoveEntity(pro.mpOwner->GetUID());
			}
		}
	}

}

void ProjectileSystem::LateUpdate(float _dTime)
{

}

void ProjectileSystem::Finalize()
{
	mpPhysicsManager->ClearFilterUpdate();
	auto view = mRegistry.view<ProjectileComponent>();
	for (auto entity : view)
	{
		mpEntityManager->RemoveEntity(static_cast<UID>(entity));
	}

	UNREGISTER_EVENT("Create Money");
	UNREGISTER_EVENT("Throw");
	UNREGISTER_EVENT("Create Arrow");
	// 	UNREGISTER_EVENT("Shoot");

}

void ProjectileSystem::ThrowCoin(const Event& _event)
{
	auto entity = _event.GetDataAs<std::shared_ptr<Entity>>().value();
	Vector3 target = entity->GetComponent<ProjectileComponent>().mTargetPosition;

	auto& projectile = entity->GetComponent<ProjectileComponent>();
	auto& rigidbody = entity->GetComponent<Rigidbody>();
	auto& transform = entity->GetComponent<Transform>();
	//auto& money = entity->GetComponent<MoneyComponent>();
	if (projectile.mIsThrown == false)
	{
		projectile.mIsThrown = true;
		Vector3 start = transform.mLocalPosition;
		auto time = projectile.mLifeTime;
		auto mass = rigidbody.mMass;
		projectile.mTargetPosition = target;
		/// 
		Vector3 force;
		force.x = (target.x - start.x) / time;
		force.y = (target.y - start.y + 0.5f * time * time * 9.81f) / time;
		force.z = (target.z - start.z) / time;

		mpPhysicsManager->SetVelocity(projectile.mpOwner, force);

		// 씬정보 수정
		//SceneData* data = dynamic_cast<SceneData*>(mpWorldManager->GetCurrentWorld()->GetCurrentScene()->GetSceneData());
		//data->m_heldMoney -= money.mAmount;

		mpSoundManager->PlaySFX("Snd_sfx_MoneyFire");
	}
	// 	DLOG(LOG_INFO, "Coin");
}

void ProjectileSystem::CreateMoney(const Event& _event)
{
	auto view = mRegistry.view<MoneyGunComponent>();
	for (auto entity : view)
	{
		auto& moneyGun = mRegistry.get<MoneyGunComponent>(entity);
		auto& moneyGunPosition = mRegistry.get<Transform>(entity).mLocalPosition;

		auto coin = mpEntityManager->CreateEntity("coin");
		coin->AddComponent<Transform>(moneyGunPosition, Vector3(), Vector3(1.f));
		coin->AddComponent<SphereCollider>(false, Vector3(), 1.4f);
		coin->AddComponent<Rigidbody>(50.f, 0.f, 0.f, true, false, 0, 1, 1);
		coin->AddComponent<MeshRenderer>("Coin.fbx", "Coin_Mesh.001", false, false, true);
		coin->AddComponent<Texture3D>("Texture_Money.png");
		coin->AddComponent<ProjectileComponent>(2, 10, Vector3(0, 0, 0));
		coin->AddComponent<MoneyComponent>().mAmount = moneyGun.mThrowAmount;
		const auto& uid = coin->GetUID();
		mpPhysicsManager->AddPhysicsObject(uid, TYPE_GOLD);
		mpRenderManager->InitailizeEntity(coin);
	}

}
// void ProjectileSystem::ShootProjectile(const Event& _event)
// {
// 	Vector3 target = entity->GetComponent<Transform>().mLocalPosition;
// 
// 	for (auto& [uid, entity] : mpEntityManager->GetEntityMap())
// 	{
// 		if (entity->GetName() == "arrow")
// 		{
// 			if (entity->GetComponent<ProjectileComponent>().mIsThrown == false)
// 			{
// 				entity->GetComponent<ProjectileComponent>().mIsThrown = true;
// 				Vector3 start = entity->GetComponent<Transform>().mLocalPosition;
// 				auto time = entity->GetComponent<ProjectileComponent>().mLifeTime;
// 				auto mass = entity->GetComponent<Rigidbody>().mMass;
// 				entity->GetComponent<ProjectileComponent>().mTargetPosition = target;
// 				/// 
// 				Vector3 force;
// 				force.x = target.x - start.x;
// 				force.z = target.z - start.z;
// 				force.Normalize();
// 				force *= entity->GetComponent<ProjectileComponent>().mSpeed;
// 
// 				mpPhysicsManager->SetVelocity(entity, force);
// 				auto& pos = entity->GetComponent<Transform>().mLocalPosition;
// 				//mpSoundManager->Play3DSound("bow", pos.x, pos.y, pos.z);
// 				mpSoundManager->PlaySFX("bow");
// 			}
// 		}
// 	}
// }

void ProjectileSystem::CreateProjectile(const Event& _event)
{
	auto entity = mpEntityManager->GetEntity(_event.GetDataAs<UID>().value());
	entity->AddComponent<BoxCollider>(true, Vector3(), Vector3(0.15f, 0.15f, 1.5f));
	entity->AddComponent<Rigidbody>(50.f, 0.f, 0.f, false, false, 0, 1, 1);
	entity->AddComponent<MeshRenderer>("Arrow 01.FBX", "Arrow 01_Mesh", false, false, true);
	entity->AddComponent<Texture3D>("Weapon Brown.png");
	entity->AddComponent<ProjectileComponent>(0.5, 10, Vector3(0, 0, 0), 10);
	mpPhysicsManager->AddPhysicsObject(entity->GetUID(), TYPE_PROJECTILE, ATTR_ALLY);
	mpRenderManager->InitailizeEntity(entity);
}

void ProjectileSystem::TutirialUpdateThrow(float _dTime)
{
	if (mpInputManager->GetKey(KEY::LBUTTON))
	{
		if (mTutoSpawnTime >= 0.5f)
		{
			Vector3 start = Vector3();
			Vector3 pick = mpPhysicsManager->PickObejct("plane");
			pick += Vector3(0, 0.55838f, 0);
			Vector2 astarPos = {};
			mpAstar->AdjustToTileCenter(pick, astarPos);
			if (pick.x >= -15 && pick.x <= 15 && pick.z >= -15 && pick.z <= 15 &&
				(*mAstarMap)[static_cast<int>(astarPos.y + abs(mStartPoint.y))][static_cast<int>(astarPos.x + abs(mStartPoint.x))] == 0)
			{
				//mpEventManager->TriggerEvent(Event("Create Money"));
				auto view = mRegistry.view<MoneyGunComponent>();
				for (auto entity : view)
				{
					auto& moneyGun = mRegistry.get<MoneyGunComponent>(entity);
					auto& moneyGunPosition = mRegistry.get<Transform>(entity).mLocalPosition;
					start = moneyGunPosition;
					if (mpSceneData->m_heldMoney >= moneyGun.mThrowAmount)
					{
						auto coin = mpEntityManager->CreateEntity("coin");
						coin->AddComponent<Transform>(moneyGunPosition, Vector3(), Vector3(0.4f));
						coin->AddComponent<BoxCollider>(false, Vector3(), Vector3(1.5f, 0.56f, 1.5f));
						coin->AddComponent<Rigidbody>(50.f, 0.f, 0.f, true, false, 0, 1, 1);
						coin->AddComponent<MeshRenderer>("Coin.fbx", "Coin_Mesh.001", false, false, true);
						coin->AddComponent<Texture3D>("Voxel_Level_Blocks_Set.png");
						coin->AddComponent<ProjectileComponent>(0, 10, pick);
						coin->AddComponent<MoneyComponent>().mAmount = moneyGun.mThrowAmount;

						// 테스트를 위한 추가 코드임//
						/*coin->AddComponent<Text>(u8" ", "NotoSansKR-Regular(24).ttf", Vector2(), 2, true, Vector4(1.0f, 1.0f, 0.0f, 1.0f));
						coin->GetComponent<Text>().mHasDepth = true;
						coin->GetComponent<Text>().mIdx = TEXT_IDX::FLOAT2;
						coin->GetComponent<Text>().mOffset = Vector3(0, 5, 0);*/
						//-------------------------

						const auto& uid = coin->GetUID();
						mpPhysicsManager->AddPhysicsObject(uid, TYPE_GOLD);
						mpRenderManager->InitailizeEntity(coin);

						//mpEventManager->TriggerEvent(Event("Throw", coin));
						auto& projComp = coin->GetComponent<ProjectileComponent>();
						Vector3 target = projComp.mTargetPosition;
						// 방향벡터
						Vector3 dir = target - start;
						// 제어점1
						//projComp.mControl1 = start + RandomUtil::RandomFloat(0.3, 0.5) * dir + Vector3(0, RandomUtil::RandomFloat(1.f, 5.f), 0);
						projComp.mControl1 = start + 0.5 * dir + Vector3(0, RandomUtil::RandomFloat(1.f, 5.f), 0);
						// 제어점2																		 									  
						projComp.mControl2 = start + RandomUtil::RandomFloat(0.6, 0.7) * dir + Vector3(0, RandomUtil::RandomFloat(1.f, 5.f), 0);

						//auto& projectile = coin->GetComponent<ProjectileComponent>();
						auto& rigidbody = coin->GetComponent<Rigidbody>();
						auto& transform = coin->GetComponent<Transform>();
						//auto& money = entity->GetComponent<MoneyComponent>();
						//if (projectile.mIsThrown == false)
						{
							//projectile.mIsThrown = true;
							//Vector3 start = transform.mLocalPosition;
							//auto time = projectile.mLifeTime;
							//time += _dTime;
							//auto mass = rigidbody.mMass;
							//projectile.mTargetPosition = target;
							///// 
							//Vector3 force;
							//force.x = (target.x - start.x) / time;
							//force.y = (target.y - start.y + 0.5f * time * time * 9.81f) / time;
							//force.z = (target.z - start.z) / time;

							//mpPhysicsManager->SetVelocity(projectile.mpOwner, force);

							// 씬정보 수정
							//SceneData* data = dynamic_cast<SceneData*>(mpWorldManager->GetCurrentWorld()->GetCurrentScene()->GetSceneData());
							//data->m_heldMoney -= money.mAmount;

							mpSoundManager->PlaySFX("Snd_sfx_MoneyFire");
						}

						mpSceneData->m_usedAmount += moneyGun.mThrowAmount;
						mpSceneData->m_heldMoney -= moneyGun.mThrowAmount;
					}
				}
				mIsThrow = true;
				// 이벤트가 발생했으므로 mTutoSpawnTime을 초기화합니다.
				mTutoSpawnTime = 0;
			}
		}
	}

	// 충돌한 투사체 삭제
	auto proView = mRegistry.view<ProjectileComponent>();
	for (auto entity : proView)
	{
		if (!mRegistry.try_get<MoneyComponent>(entity))
		{
			auto& pro = mRegistry.get<ProjectileComponent>(entity);
			if (pro.mIsTriggered)
			{
				mpEntityManager->RemoveEntity(pro.mpOwner->GetUID());
			}
		}
	}
}

void ProjectileSystem::TutirialUpdateMoney(float _dTime)
{
	// 머니건 쿨타임
	mTutoSpawnTime += _dTime;

	// 코인
	auto view = mRegistry.view<MoneyComponent>();
	for (auto entity : view)
	{
		auto& transform = mRegistry.get<Transform>(entity);
		auto& money = mRegistry.get<MoneyComponent>(entity);
		auto& porjComp = mRegistry.get<ProjectileComponent>(entity);
		auto& pos = transform.mLocalPosition;
		auto& rot = transform.mLocalRotation;
		auto& target = porjComp.mTargetPosition;
		auto& time = porjComp.mLifeTime;
		if (!money.mIsOnGround)
		{
			time += _dTime * m_coinSpeed;
		}

		Vector3 start = Vector3(-20, 5, 0);

		if (!money.mIsOnGround)
		{
			/// 곡선 방정식 계산
			// 곡선 방정식으로 계산한 시간에 따른 위치
			// Vector3 pos = std::pow((1.f - time), 3) * start + 3 * (1.f - time) * time * porjComp.mControl1
			// 	+ 3 * (1.f - time) * std::pow(time, 2) * porjComp.mControl2 + std::pow(time, 3) * target;

			Vector3 pos = std::pow((1.f - time), 2) * start + 2 * (1.f - time) * time * porjComp.mControl1
				+ std::pow(time, 2) * target;

			mpPhysicsManager->UpdatePosition(mpEntityManager->GetEntity(entity), pos);
		}
		else
		{
			mpPhysicsManager->SetVelocity(transform.mpOwner, Vector3());
			mpPhysicsManager->UpdatePosition(transform.mpOwner, target);
		}

		if (mpPhysicsManager->GetFilterData(transform.mpOwner).word1 & ATTR_ON_GROUND)
		{
			money.mIsOnGround = true;
			if (money.mIsInVec == false)
			{
				mMoneyPosVec.emplace_back(mpEntityManager->GetEntity(entity), transform.mLocalPosition);
				money.mIsInVec = true;
			}
			mpPhysicsManager->SetVelocity(transform.mpOwner, Vector3());
			mpPhysicsManager->UpdatePosition(transform.mpOwner, target);
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
