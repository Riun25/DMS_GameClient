#include "TutorialScene2.h"
#include "DearsGameEngineAPI.h"
#include "PhysicsManager.h"

#include "PlayerComponent.h"
#include "EnemyComponent.h"
#include "ProjectileComponent.h"
#include "CustomFilter.h"

TutorialScene2::TutorialScene2(entt::registry& _registry, const std::string& _name
	, EventManager* _pEventManager, RenderManager* _pRenderManager
	, PhysicsManager* _pPhysicsManager, InputManager* _pInpueManager, WorldManager* _worldManager
	, UIManager* _UIManager, EntityManager* _pEntityManager, ResourceManager* _pResourceManager
	, SoundManager* _pSoundManager)
	: Scene(_registry, _name, _pEventManager, _pRenderManager, _pPhysicsManager
		, _pInpueManager, _worldManager, _UIManager, _pEntityManager, _pResourceManager, _pSoundManager)
{
	mpFSM = nullptr;
}

bool TutorialScene2::Initialize()
{
	/// 리소스 추가
	AddResource();

	/// 오브젝트 생성
	// 카메라 엔티티 생성, 세팅
	auto camera = mpEntityManager->CreateEntity("Camera");
	// 	camera->AddComponent<CameraComponent>(mpRenderManager->m_width, mpRenderManager->m_height
	// 		, 10.f, Vector3(0.f, 5.f, -50.f), Vector3(0.f, 0.f, 1.f), Vector3(0.f, 1.f, 0.f), Vector3(), Vector3(70.f, 0.01f, 1000.f));
	camera->AddComponent<CameraComponent>(mpRenderManager->GetScreenWidth(), mpRenderManager->GetScreenHeight()
		// 		, 10.f, Vector3(0.f, 10.f, -50.f), Vector3(0.f, 0.f, -1.f), Vector3(0.f, 1.f, 0.f), Vector3(), Vector3(70.f, 0.01f, 1000.f));
		, 10.f, Vector3(-64.f, 137.f, -58.f), Vector3(0.f, 0.f, 1.f), Vector3(0.f, 1.f, 0.f), Vector3(70.f, 0.01f, 1000.f), static_cast<unsigned int>(cameraEnum::WorldCamera));
	camera->GetComponent<CameraComponent>().mpCamera->mIsFirstPersonMode = false;


	// 바닥
	auto plane = mpEntityManager->CreateEntity("plane");
	plane->AddComponent<Transform>(Vector3(0.f, -10.f, 0.f), Vector3(), Vector3(4.f));
	plane->AddComponent<BoxCollider>(true, Vector3(0, 0, 0), Vector3(100, 1, 100));
	plane->AddComponent<Rigidbody>(100.f, 1.f, 1.f, false, true, 0, 1, 1);
	plane->AddComponent<MeshRenderer>("BG_GroundGrass.fbx", "BG_GroundGrass_Mesh", false, false);
	plane->AddComponent<Texture3D>("Ground-Grass.png");
	mpPhysicsManager->AddPhysicsObject(plane->GetUID(), TYPE_GROUND);

	// 캐릭터
	auto gs1 = mpEntityManager->CreateEntity("char");
	gs1->AddComponent<Transform>(Vector3(5.f, 5.f, 0.f), Vector3(), Vector3(0.1f));
	gs1->AddComponent<CapsuleCollider>();
	gs1->AddComponent<Rigidbody>(1.f, 1.f, 1.f, true, false);
	gs1->AddComponent<MeshRenderer>("Character 01.fbx", "Character 01", true, true);
	gs1->AddComponent<Texture3D>("M-Cos-16-Blue.png");
	gs1->AddComponent<AnimationComponent>();
	gs1->AddComponent<BoneGroupComponent>();
	gs1->GetComponent<AnimationComponent>().mpTargetAnimation = mpResourceManager->Get_Animation("Character@Run Forward Bare Hands In Place.FBX");
	gs1->GetComponent<MeshRenderer>().mpModel->pTargetAnimation
		= mpRenderManager->Get_Animation("Character@Run Forward Bare Hands In Place.FBX");
	mpPhysicsManager->AddPhysicsObject(gs1->GetUID(), TYPE_UNIT, ATTR_ALLY);

	auto weapon1 = mpEntityManager->CreateEntity("weapon");
	weapon1->AddComponent<Transform>(Vector3(), Vector3(), Vector3(0.1f));
	weapon1->GetComponent<Transform>().mpParent = &(gs1->GetComponent<Transform>());
	// 	weapon1->AddComponent<BoxCollider>();
	// 	weapon1->AddComponent<Rigidbody>();
	// 	weapon1->GetComponent<Rigidbody>().mIsKinematic = true;
	weapon1->AddComponent<MeshRenderer>("Axe 01.FBX", "Axe 01");
	weapon1->AddComponent<Texture3D>("M-Cos-16-Blue.png");
	weapon1->AddComponent<TargetBoneComponent>();
	weapon1->GetComponent<TargetBoneComponent>().mTargetBoneAboveMatrix
		= mpRenderManager->GetTargetBoneAboveMatrix("Character 01.FBX", "RigRPalm", 1.f);
	mpPhysicsManager->AddPhysicsObject(weapon1->GetUID(), TYPE_WEAPON, ATTR_ALLY);

	// 	auto gs2 = CreateEntity("char");
	// 	gs2->AddComponent<Transform>(Vector3(-5.f, 5.f, 0.f), Quaternion(), Vector3(0.1, 0.1, 0.1));
	// 	gs2->AddComponent<CapsuleCollider>();
	// 	gs2->AddComponent<Rigidbody>(50.f, 1.f, 1.f, true);
	// 	gs2->AddComponent<MeshRenderer>("Character 01.fbx", "Character 01", true, true);
	// 	gs2->AddComponent<Texture3D>("M-Cos-16-Blue.png");
	// 	gs2->AddComponent<AnimationComponent>();
	// 	gs2->GetComponent<AnimationComponent>().m_animationList.push_back("Character@Run Forward Bare Hands In Place.FBX");
	// 	gs2->GetComponent<MeshRenderer>().mpModel->mpTargetAnimation
	// 		= mpRenderManager->Get_Animation("Character@Run Forward Bare Hands In Place.FBX");
	// 	 	mpPhysicsManager->AddPhysicsObject(gs2, FILTER_GROUP_MINION, FILTER_DATA_IS_ALLY);


	auto coinSpawner = mpEntityManager->CreateEntity("Coin Spawner");
	coinSpawner->AddComponent<Transform>(Vector3(-10.f, 0, 0));


	// 모델 본 엔티티 생성
	// 모든 본을 하나의 구조체가 가지고있어서 나중에 구조 바꿔야함
	auto boneEntity = mpEntityManager->CreateEntity("BoneGroup");
	boneEntity->AddComponent<BoneGroupComponent>();
	auto& boneData = boneEntity->GetComponent<BoneGroupComponent>().mBoneData;

	// 타이머 ui
// 	auto ui = mpEntityManager->CreateEntity("timer");
// 	ui->AddComponent<Text>(u8"00:00", "B.ttf", Vector2(910, 10));

	/// 라이트 세팅
	auto lightEntity = mpEntityManager->CreateEntity("LightGroup");
	lightEntity->AddComponent<LightComponent>();
	//auto& lightsData = lightEntity->GetComponent<LightComponent>().m_commonConstData;

	mpRenderManager->LightInitialize(&lightEntity->GetComponent<LightComponent>().mCommonConstData, 1);

	mpRenderManager->SetDirLight(&lightEntity->GetComponent<LightComponent>().mCommonConstData, 0, 1.0f, Vector3(0.0f, -1.0f, 0.0f));
	// mpRenderManager->m_pGraphicsEngine->SetPointLight(&lightEntity->GetComponent<LightComponent>().m_constData, 1, 1.0f, 1.0f, 10.0f, Vector3(0.0f, 10.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
	// mpRenderManager->m_pGraphicsEngine->SetSpotLight(&lightEntity->GetComponent<LightComponent>().m_constData, 2, 1.0f, 10.0f, 50.0f, Vector3(0.0f, -20.0f, 1.0f), Vector3(0.0f, 0.0f, -4.0f), 1.0f, Vector3(1.0f, 0.0f, 0.0f));

	mpRenderManager->PrintLightInfo(&lightEntity->GetComponent<LightComponent>().mCommonConstData);

	Light templight = lightEntity->GetComponent<LightComponent>().mCommonConstData.light[0];
	templight.position = Vector3(-100.f, 100.0f, -100.f);
	std::shared_ptr<Entity> tempEntity = mpEntityManager->CreateEntity("Camera");
	tempEntity->AddComponent<CameraComponent>(mpRenderManager->GetScreenWidth(), mpRenderManager->GetScreenHeight()
		, 0.f, Vector3(-100.f, 100.0f, -100.f), Vector3(1.f, -1.f, 1.f), Vector3(0.f, 1.f, 0.f), Vector3(70.f, 100.f, 1000.f),
		static_cast<unsigned int>(cameraEnum::LightCamera));
	tempEntity->GetComponent<CameraComponent>().setLightCameraIndex(0);	//0번째 라이트기 때문
	tempEntity->GetComponent<CameraComponent>().setViewDir(templight.direction);	//0번째 라이트기 때문
	tempEntity->GetComponent<CameraComponent>().setEyePos(templight.position);	//0번째 라이트기 때문

	REGISTER_IMMEDIATE_EVENT("Move Test", Move);
	REGISTER_IMMEDIATE_EVENT("Create Money", CreateMoney);
	REGISTER_IMMEDIATE_EVENT("Throw", ThrowCoin);

	return Scene::Initialize();
}

void TutorialScene2::FixedUpdate(float _dTime)
{

}

void TutorialScene2::Update(float _dTime)
{
	for (auto& [uid, entity] : mpEntityManager->GetEntityMap())
	{
		// 		if (entity->GetName() == "plane")
		// 		{
		// 			const auto& actor = mpPhysicsManager->GetActor(entity);
		// 			int a = 0;
		// 		}
		// 		
		// 		if (entity->GetName() == "coin")
		// 		{
		// 			const auto& actor = mpPhysicsManager->GetActor(entity);
		// 			int a = 0;
		// 		}

	}

	if (mpInputManager->GetKeyUp(KEY::RBUTTON))
	{
		Vector3 start = Vector3();
		Vector3 pick = mpPhysicsManager->PickObejct();

		if (Vector3::Distance(start, pick) >= 0.1)
		{
			mpEventManager->TriggerEvent(Event("Create Money"));

			mpEventManager->TriggerEvent(Event("Throw", pick));
		}
	}

	// 동전의 움직임을 모니터링하고 목표 지점에 도달하면 멈추게 함
// 	for (auto& [uid, entity] : m_pEntities)
// 	{
// 		if (entity->GetName() == "coin")
// 		{
// 			auto actor = mpPhysicsManager->GetActor(entity);
// 			auto num = actor->getNbShapes();
// 
// 			physx::PxShape* shape[1];
// 			mpPhysicsManager->GetActor(entity)->getShapes(shape, 1);
// 			physx::PxFilterData filterData = shape[0]->getSimulationFilterData();
// 
// 			if (filterData.word1 & ATTR_ON_GROUND)
// 			{
// 				entity->GetComponent<MoneyComponent>().mIsOnGround = true;
// 				mpPhysicsManager->SetVelocity(entity, Vector3());
// 			}
// 		}
// 	}

	for (auto& [uid, entity] : mpEntityManager->GetEntityMap())
	{
		if (entity->GetName() == "timer")
		{
			static float time = 180.f;
			time -= _dTime;

			int minutes = static_cast<int>(time) / 60;
			int seconds = static_cast<int>(time) % 60;

			std::ostringstream oss;
			oss << std::setfill('0') << std::setw(2) << minutes << ":"
				<< std::setfill('0') << std::setw(2) << seconds;

			// std::u8string으로 변환
			std::string str = oss.str();
			entity->GetComponent<Text>().mText = std::u8string(str.begin(), str.end());

			if (time <= 0 && time >= -0.01)
			{
				time = 180.f;
				DLOG(LOG_INFO, "Game Over");
				mpWorldManager->GetCurrentWorld()->SetScene("game over");
				return;
			}

		}
	}

}

void TutorialScene2::LateUpdate(float _dTime)
{
	auto view = mRegistry.view<Transform>();
	for (auto& entity : view)
	{
		if (auto transform = mRegistry.try_get<Transform>(entity))
		{
			auto pos = transform->mLocalPosition;
			// 			DLOG(LOG_INFO, std::to_string(pos.x) + "/" + std::to_string(pos.y) + "/" + std::to_string(pos.z));
		}
	}

	// 	if (mpInputManager->GetKeyUp(KEY::RBUTTON))
	// 	{
	// 		mpEventManager->TriggerEvent(Event("Throw"));
	// 	}

	for (auto& [uid, entity] : mpEntityManager->GetEntityMap())
	{
		if (entity->GetName() == "Camera")
		{
			entity->GetComponent<CameraComponent>().mpCamera->OnMouseMove(static_cast<int>(mpInputManager->GetMousePos().x), static_cast<int>(mpInputManager->GetMousePos().y));
		}
	}

	if (mpInputManager->GetKey(KEY::F))
	{
		for (auto& [uid, entity] : mpEntityManager->GetEntityMap())
		{
			if (entity->GetName() == "Camera")
			{
				if (entity->GetComponent<CameraComponent>().mpCamera->mIsFirstPersonMode)
				{
					entity->GetComponent<CameraComponent>().mpCamera->mIsFirstPersonMode = false;
				}
				else
				{
					entity->GetComponent<CameraComponent>().mpCamera->mIsFirstPersonMode = true;
					entity->GetComponent<CameraComponent>().mpCamera->PrintCamInfo();
				}
			}
		}
	}
}

void TutorialScene2::Finalize()
{
	UNREGISTER_EVENT("Move Test");
	UNREGISTER_EVENT("Create Money");

	Scene::Finalize();
	delete mpFSM;
}

void TutorialScene2::AddResource()
{
	/// 리소스 추가
	// 모델 추가
	mpRenderManager->AddModel("../TestAsset/", "box.fbx");
	mpRenderManager->AddModel("../TestAsset/", "Debug_Capsule.fbx");
	mpRenderManager->AddModel("../TestAsset/", "Debug_Box.fbx");
	mpRenderManager->AddModel("../TestAsset/Test/", "Character 01.fbx");
	mpRenderManager->AddModel("../TestAsset/Test/", "Ground_01.fbx");
	mpRenderManager->AddModel("../TestAsset/Test/", "Wand 01.fbx");
	mpRenderManager->AddModel("../TestAsset/Test/", "Axe 01.FBX");
	mpRenderManager->AddModel("../TestAsset/money/", "Money1.fbx");
	mpRenderManager->AddModel("../TestAsset/money/", "Money2.fbx");
	mpRenderManager->AddModel("../TestAsset/Test/", "BG_GroundGrass.fbx");

	// 텍스쳐 추가
	mpUIManager->AddTexture2D("../Resources/Texture/", "blue.png");
	mpUIManager->AddTexture2D("../Resources/Texture/", "green.png");
	mpUIManager->AddTexture2D("../Resources/Texture/", "Grid.png");

	mpRenderManager->Add3DTexture("../Resources/Texture/", "rough-stone-wall-texture.jpg");
	mpRenderManager->Add3DTexture("../TestAsset/Test/", "M-Cos-16-Blue.png");
	mpRenderManager->Add3DTexture("../TestAsset/Test/", "NaturePackLite_Texture_01.png");
	mpRenderManager->Add3DTexture("../TestAsset/Test/", "ss.png");
	mpRenderManager->Add3DTexture("../TestAsset/money/", "Texture_Money.png");
	mpRenderManager->Add3DTexture("../TestAsset/money/", "Texture_Money2.png");
	mpRenderManager->Add3DTexture("../TestAsset/", "Ground-Grass.png");

	// 애니메이션 추가
	mpRenderManager->AddAnimation("../TestAsset/Test/", "Character@Run Forward Bare Hands In Place.FBX");
	mpRenderManager->AddAnimation("../TestAsset/Test/", "Character@Jump In Place.FBX");

}

void TutorialScene2::Move(const Event& _event)
{
	auto chara = _event.GetDataAs<std::shared_ptr<Entity>>().value();

	mpPhysicsManager->ApplyForce(chara, Vector3(5.f, 0.f, 0.f));
}


void TutorialScene2::ThrowCoin(const Event& _event)
{
	Vector3 target = _event.GetDataAs<Vector3>().value();

	for (auto& [uid, entity] : mpEntityManager->GetEntityMap())
	{
		if (entity->GetName() == "coin")
		{
			if (entity->GetComponent<ProjectileComponent>().mIsThrown == false)
			{
				entity->GetComponent<ProjectileComponent>().mIsThrown = true;
				Vector3 start = entity->GetComponent<Transform>().mLocalPosition;
				auto time = entity->GetComponent<ProjectileComponent>().mLifeTime;
				auto mass = entity->GetComponent<Rigidbody>().mMass;
				entity->GetComponent<ProjectileComponent>().mTargetPosition = target;

				/// 
				Vector3 force;
				force.x = (target.x - start.x) / time;
				force.y = (target.y - start.y + 0.5f * time * time * 9.81f) / time;
				force.z = (target.z - start.z) / time;

				mpPhysicsManager->ApplyForce(entity, force);
			}
		}
	}

	DLOG(LOG_INFO, "Coin");

}

void TutorialScene2::CreateMoney(const Event& _event)
{
	auto coin = mpEntityManager->CreateEntity("coin");
	coin->AddComponent<Transform>(Vector3(-100, 0, -50), Vector3(), Vector3(0.01f));
	coin->AddComponent<BoxCollider>(true, Vector3());
	coin->AddComponent<Rigidbody>(1.f, 0.f, 0.f, true, false, 0, 1, 1);
	coin->AddComponent<MeshRenderer>("Money1.fbx", "Money", false, false, true);
	coin->AddComponent<Texture3D>("Texture_Money.png");
	coin->AddComponent<ProjectileComponent>(2, 10, Vector3(0, 0, 0));
	coin->AddComponent<MoneyComponent>();
	mpRenderManager->InitailizeEntity(coin);
	mpPhysicsManager->AddPhysicsObject(coin->GetUID(), TYPE_GOLD);
}

