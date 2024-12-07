#include "AnimationTestScene.h"
#include "DearsGameEngineAPI.h"
#include "PhysicsManager.h"
#include "PlayerComponent.h"
#include "CustomFilter.h"

AnimationTestScene::AnimationTestScene(entt::registry& _registry, const std::string& _name
	, EventManager* _pEventManager, RenderManager* _pRenderManager
	, PhysicsManager* _pPhysicsManager, InputManager* _pInpueManager
	, WorldManager* _worldManager, UIManager* _UIManager, EntityManager* _pEntityManager
	, ResourceManager* _pResourceManager)
	: Scene(_registry, _name, _pEventManager, _pRenderManager, _pPhysicsManager
		, _pInpueManager, _worldManager, _UIManager, _pEntityManager, _pResourceManager)
{
}

bool AnimationTestScene::Initialize()
{
	/// 리소스 추가
	AddResource();

	/// 오브젝트 생성
	// 카메라 엔티티 생성, 세팅
	auto camera = m_pEntityManager->CreateEntity("Camera");
	camera->AddComponent<CameraComponent>(m_pRenderManager->GetScreenWidth(), m_pRenderManager->GetScreenHeight()
		, 10.f, Vector3(0.f, 10.f, -50.f), Vector3(0.f, 0.f, 1.f), Vector3(0.f, 1.f, 0.f),Vector3(90.f, 0.01f, 1000.f), static_cast<unsigned int>(cameraEnum::WorldCamera));
	camera->GetComponent<CameraComponent>().m_pCamera->UpdateFrameMouse(0.f, 0.f);

	// 바닥
	auto plane = m_pEntityManager->CreateEntity("plane");
	plane->AddComponent<Transform>(Vector3(0.f, -10.f, 0.f), Vector3(), Vector3(100.f, 1.f, 100.f));
	plane->AddComponent<BoxCollider>();
	plane->AddComponent<Rigidbody>(100.f, 1.f, 1.f, false, true);
	plane->AddComponent<MeshRenderer>("box.fbx", "Cube.001", false, false);
	// 	plane->AddComponent<PlaneComponent>();
	plane->AddComponent<Texture2D>("rough-stone-wall-texture.jpg");

	std::shared_ptr<Entity> theBox = m_pEntityManager->CreateEntity("theBox");
	theBox->AddComponent<Transform>(Vector3(-2.f, 0.f, 5.5f));
	theBox->AddComponent<BoxCollider>();
	theBox->GetComponent<BoxCollider>().m_size = { 6.5f, 1.0f, 3.0f };
	theBox->AddComponent<Rigidbody>(1.f, 1.f, 1.f, true, false);
	theBox->AddComponent<MeshRenderer>("Debug_Box.fbx", "Box", false, false);
	m_pPhysicsManager->AddPhysicsObject(plane->GetUID());

	// 모델 본 엔티티 생성
	// 모든 본을 하나의 구조체가 가지고있어서 나중에 구조 바꿔야함
	auto boneEntity = m_pEntityManager->CreateEntity("BoneGroup");
	boneEntity->AddComponent<BoneGroupComponent>();
	auto& boneData = boneEntity->GetComponent<BoneGroupComponent>().m_boneData;

	// UI 엔티티 생성
	auto uiEntity = m_pEntityManager->CreateEntity("UI");
	uiEntity->AddComponent<Text>();
	auto& text = uiEntity->GetComponent<Text>();
	text.m_text = u8"1. IDLE : 1.0초, 31 frame, 0.5167";
	text.m_font = "B.ttf";
	text.m_position = Vector2(10, 400);

	auto uiEntity2 = m_pEntityManager->CreateEntity("UI");
	uiEntity2->AddComponent<Text>();
	auto& text2 = uiEntity2->GetComponent<Text>();
	text2.m_text = u8"2. MOVE : 0.7초, 22 frame, 0.3666";
	text2.m_font = "B.ttf";
	text2.m_position = Vector2(10, 450);

	auto uiEntity3 = m_pEntityManager->CreateEntity("UI");
	uiEntity3->AddComponent<Text>();
	auto& text3 = uiEntity3->GetComponent<Text>();
	text3.m_text = u8"3. LOOTMOVE : 0.85초, 22 frame, 0.3666";
	text3.m_font = "B.ttf";
	text3.m_position = Vector2(10, 500);

	auto uiEntity4 = m_pEntityManager->CreateEntity("UI");
	uiEntity4->AddComponent<Text>();
	auto& text4 = uiEntity4->GetComponent<Text>();
	text4.m_text = u8"4. ATTACK : 1.0초, 51 frame, 0.85";
	text4.m_font = "B.ttf";
	text4.m_position = Vector2(10, 550);

	auto uiEntity5 = m_pEntityManager->CreateEntity("UI");
	uiEntity5->AddComponent<Text>();
	auto& text5 = uiEntity5->GetComponent<Text>();
	text5.m_text = u8"5. BOW ATTACK : 0.7초, 21 frame, 0.35";
	text5.m_font = "B.ttf";
	text5.m_position = Vector2(10, 600);

	auto uiEntity6 = m_pEntityManager->CreateEntity("UI");
	uiEntity6->AddComponent<Text>();
	auto& text6 = uiEntity6->GetComponent<Text>();
	text6.m_text = u8"6. DIE : 1.5초, 51 frame, 0.85";
	text6.m_font = "B.ttf";
	text6.m_position = Vector2(10, 650);

	/// 라이트 세팅
	auto lightEntity = m_pEntityManager->CreateEntity("LightGroup");
	lightEntity->AddComponent<LightComponent>();
	//auto& lightsData = lightEntity->GetComponent<LightComponent>().m_commonConstData;

	m_pRenderManager->LightInitialize(&lightEntity->GetComponent<LightComponent>().m_commonConstData, 1);
	m_pRenderManager->SetDirLight(&lightEntity->GetComponent<LightComponent>().m_commonConstData, 0, 1.0f, Vector3(0.0f, -1.0f, 1.0f));
	m_pRenderManager->PrintLightInfo(&lightEntity->GetComponent<LightComponent>().m_commonConstData);

	Light templight = lightEntity->GetComponent<LightComponent>().m_commonConstData.light[0];
	templight.position = Vector3(-50.f, 50.0f, -50.f);
	std::shared_ptr<Entity> tempEntity = m_pEntityManager->CreateEntity("Camera");
	tempEntity->AddComponent<CameraComponent>(m_pRenderManager->GetScreenWidth(), m_pRenderManager->GetScreenHeight()
		, 0.f, Vector3(-50.f, 50.0f, -50.f), Vector3(1.f, -1.f, 1.f), Vector3(0.f, 1.f, 0.f), Vector3(90.f, 10.f, 1000.f),
		static_cast<unsigned int>(cameraEnum::LightCamera));
	tempEntity->GetComponent<CameraComponent>().setLightCameraIndex(0);	//0번째 라이트기 때문
	tempEntity->GetComponent<CameraComponent>().setViewDir(templight.direction);	//0번째 라이트기 때문
	tempEntity->GetComponent<CameraComponent>().setEyePos(templight.position);	//0번째 라이트기 때문


	///-------- test용 임시값 전체 세팅
	auto tester = m_pEntityManager->CreateEntity("tester");
	tester->AddComponent<Transform>(Vector3(0.f, 0.f, 0.f), Vector3(), Vector3(0.1f, 0.1f, 0.1f));
	auto& player_trs1 = tester->GetComponent<Transform>();
	player_trs1.m_localPosition = Vector3(0.f, 0.f, 0.f);
	player_trs1.m_localScale = Vector3(0.1f, 0.15f, 0.1f);
	tester->AddComponent<CapsuleCollider>();
	tester->AddComponent<Rigidbody>(50.f, 1.f, 1.f, true);
	tester->AddComponent<MeshRenderer>("Character 01.fbx", "Character 01", true, true);
	tester->AddComponent<Texture3D>("M-Cos-16-Blue.png");
	tester->GetComponent<MeshRenderer>().m_pModel->mAnimationPlaytime = 0;
	tester->AddComponent<AnimationComponent>();
	tester->AddComponent<AnimationComponent>().mpTargetAnimation = m_pResourceManager->Get_Animation("Character@Mutant Run2.fbx");
	tester->AddComponent<BoneGroupComponent>();

	auto player1_1 = m_pEntityManager->CreateEntity("Hat");
	player1_1->AddComponent<Transform>();
	// 	player1_1->AddComponent<BoxCollider>();
	// 	player1_1->AddComponent<Rigidbody>();
	player1_1->AddComponent<MeshRenderer>();
	player1_1->AddComponent<Texture3D>();
	player1_1->AddComponent<TargetBoneComponent>();
	auto& player_trs1_1 = player1_1->GetComponent<Transform>();
	player_trs1_1.m_pParent = &player_trs1;
	player1_1->GetComponent<MeshRenderer>().m_file = "Hat 04.FBX";
	player1_1->GetComponent<MeshRenderer>().m_meshName = "Hat 04";
	auto& player_targetBoneComponent = player1_1->GetComponent<TargetBoneComponent>();
	player_targetBoneComponent.mTargetBoneIndex = m_pRenderManager->Get_TargetModelBoneIndex("Character 01", "RigHead");
	player_targetBoneComponent.mTarGetBoneConstantBufferData = &(tester->GetComponent<BoneGroupComponent>().m_boneData);
	player_targetBoneComponent.mTargetBoneData.targrtBoneMatrix = player_targetBoneComponent.mTarGetBoneConstantBufferData->bone[player_targetBoneComponent.mTargetBoneIndex];
	//순서보장을 위해 서로가 서로를 알게끔..
	//tester->GetComponent<AnimationComponent>().myEntityVec.push_back(player1_1);
	//player1_1->GetComponent<TargetBoneComponent>().myPlayer = tester;
	/// ----임시값 세팅 끝

	return Scene::Initialize();
}

void AnimationTestScene::FixedUpdate(float _dTime)
{

}

void AnimationTestScene::Update(float _dTime)
{
	Camera* mpCamera = nullptr;
	for (auto& [uid, entity] : m_pEntityManager->GetEntityMap())
	{		// 카메라의 마우스 위치 업데이트
		if (entity->GetName() == "Camera")
		{
			mpCamera = entity->GetComponent<CameraComponent>().m_pCamera;
		}
	}

	for (auto& [uid, entity] : m_pEntityManager->GetEntityMap())
	{
		if (entity->GetName() == "tester") // 엔티티가 여러개가 된다면, 그 여러개에 따른 거 list 같은 거 만들어서 for문 돌리면 될 듯함.
		{
			interpolationTime = static_cast<float>(entity->GetComponent<MeshRenderer>().m_pModel->mNextAnimationPlaytime);
			animationPlaytime = static_cast<float>(entity->GetComponent<MeshRenderer>().m_pModel->mAnimationPlaytime);

			// 연속 재생on/Off
			if (m_pInputManager->GetKeyDown(KEY::Q))
			{
				auto& looping = entity->GetComponent<AnimationComponent>().mIsLoop;
				if (looping == true)
				{
					looping = false;
				}
				else
				{
					looping = true;
				}
			}

			Vector3 theLastPosition = {};
			if (m_pInputManager->GetKey(KEY::LBUTTON))
			{
				Vector3 tt = ScreenToWorldRay();
				entity->GetComponent<Transform>().m_localPosition = GetTargetPosition(tt, mpCamera->mViewPos, Vector3(0, -5, 0), 100);

				entity->GetComponent<AnimationComponent>().mpNextTargetAnimation = m_pResourceManager->Get_Animation("Character@Mutant Run2.fbx");
				entity->GetComponent<AnimationComponent>().mIsLoop = true;
			}
			else if (m_pInputManager->GetKeyUp(KEY::LBUTTON))
			{
				Vector3 tt = ScreenToWorldRay();
				theLastPosition = GetTargetPosition(tt, mpCamera->mViewPos, Vector3(0, -5, 0), 100);

				theLastPosition = Vector3(static_cast<int>(theLastPosition.x), 0, static_cast<int>(theLastPosition.z));

				entity->GetComponent<AnimationComponent>().mpNextTargetAnimation = m_pResourceManager->Get_Animation("Character@Idle.FBX");
				entity->GetComponent<Transform>().m_localPosition = theLastPosition;
				entity->GetComponent<AnimationComponent>().mIsLoop = false;
			}

			if (m_pInputManager->GetKeyDown(KEY::D1))
			{
				entity->GetComponent<AnimationComponent>().mpNextTargetAnimation = m_pResourceManager->Get_Animation("Character@Idle.FBX");
			}
			if (m_pInputManager->GetKeyDown(KEY::D2))
			{
				entity->GetComponent<AnimationComponent>().mpNextTargetAnimation = m_pResourceManager->Get_Animation("Character@Run Forward Bare Hands In Place.FBX");
			}
			if (m_pInputManager->GetKeyDown(KEY::D3))
			{
				entity->GetComponent<AnimationComponent>().mpNextTargetAnimation = m_pResourceManager->Get_Animation("Character@Mutant Run2.fbx");
			}
			if (m_pInputManager->GetKeyDown(KEY::D4))
			{
				entity->GetComponent<AnimationComponent>().mpNextTargetAnimation = m_pResourceManager->Get_Animation("Character@Slash Attack.FBX");
			}
			if (m_pInputManager->GetKeyDown(KEY::D5))
			{
				entity->GetComponent<AnimationComponent>().mpNextTargetAnimation = m_pResourceManager->Get_Animation("Character@Bow Shoot Attack.FBX");
			}
			if (m_pInputManager->GetKeyDown(KEY::D6))
			{
				entity->GetComponent<AnimationComponent>().mpNextTargetAnimation = m_pResourceManager->Get_Animation("Character@Take Damage.FBX");
			}
			if (m_pInputManager->GetKeyDown(KEY::D7))
			{
				entity->GetComponent<AnimationComponent>().mpNextTargetAnimation = m_pResourceManager->Get_Animation("Character@Die.FBX");
			}

		}
	}

}

void AnimationTestScene::LateUpdate(float _dTime)
{
	auto view = m_registry.view<Transform>();
	for (auto& entity : view)
	{
		if (auto transform = m_registry.try_get<Transform>(entity))
		{
			auto pos = transform->m_localPosition;
		}
	}
}

void AnimationTestScene::Finalize()
{
	Scene::Finalize();
}

void AnimationTestScene::AddResource()
{
	/// 리소스 추가
	// 모델 추가
	m_pRenderManager->AddModel("../TestAsset/", "box.fbx");
	m_pRenderManager->AddModel("../TestAsset/", "Debug_Capsule.fbx");
	m_pRenderManager->AddModel("../TestAsset/", "Debug_Box.fbx");
	m_pRenderManager->AddModel("../TestAsset/Test/", "Character 01.fbx");

	// 텍스쳐 추가
	m_pRenderManager->Add3DTexture("../TestAsset/Test/", "ss.png");
	m_pRenderManager->Add3DTexture("../TestAsset/Test/", "M-Cos-16-Blue.png");
	m_pUIManager->AddTexture2D("../Resources/Texture/", "rough-stone-wall-texture.jpg");

	// 애니메이션 추가
	m_pRenderManager->AddAnimation("../TestAsset/Test/", "Character@Idle.FBX");
	m_pRenderManager->AddAnimation("../TestAsset/Test/", "Character@Slash Attack.FBX");
	m_pRenderManager->AddAnimation("../TestAsset/Test/", "Character@Die.FBX");
	m_pRenderManager->AddAnimation("../TestAsset/Test/", "Character@Run Forward Bare Hands In Place.FBX");
	m_pRenderManager->AddAnimation("../TestAsset/Test/", "Character@Mutant Run2.fbx");
	m_pRenderManager->AddAnimation("../TestAsset/Test/", "Character@Bow Shoot Attack.FBX");
	m_pRenderManager->AddAnimation("../TestAsset/Test/", "Character@Take Damage.FBX");

	// 폰트 추가
	/*m_pUIManager->AddFont("../../Resources/Font/", "B.ttf", 40.f, true);
	m_pUIManager->AddFont("../../Resources/Font/", "L.ttf", 120.f, true);
	m_pUIManager->FontSetFinish();*/
}

Vector3 AnimationTestScene::ScreenToNDC(int mouseX, int mouseY, float screenWidth, float screenHeight)
{
	// 화면 좌표를 NDC로 변환
	float xNDC = (2.0f * mouseX) / screenWidth - 1.0f;
	float yNDC = 1.0f - (2.0f * mouseY) / screenHeight;
	return Vector3(xNDC, yNDC, 0.0f);
}

Vector4 AnimationTestScene::NDCToClip(Vector3 _NDCCoords)
{
	return Vector4(_NDCCoords.x, _NDCCoords.y, -1.0f, 1.0f);
}

Vector4 AnimationTestScene::ClipToEye(Vector4 clipCoords, Matrix invProj)
{
	Vector4 eyeCoords = Vector4::Transform(clipCoords, invProj);
	eyeCoords.z = 1.0f;  // 기본값으로 설정
	eyeCoords.w = 0.0f;  // 기본값으로 설정
	return eyeCoords;
}

Vector3 AnimationTestScene::EyeToWorld(Vector4 eyeCoords, Matrix invView)
{
	Vector4 rayWorld = Vector4::Transform(eyeCoords, invView);
	return Vector3(rayWorld.x, rayWorld.y, rayWorld.z);
}

Vector3 AnimationTestScene::ScreenToWorldRay()
{
	Camera* mpCamera = nullptr;
	for (auto& [uid, entity] : m_pEntityManager->GetEntityMap())
	{		// 카메라의 마우스 위치 업데이트
		if (entity->GetName() == "Camera")
		{
			mpCamera = entity->GetComponent<CameraComponent>().m_pCamera;
		}
	}

	Vector2 mousePos = m_pInputManager->GetMousePos();

	// 1. 화면 좌표를 NDC로 변환
	Vector3 ndcCoords = ScreenToNDC(mousePos.x, mousePos.y, m_pRenderManager->GetScreenWidth(), m_pRenderManager->GetScreenHeight());

	// 2. NDC를 클립 좌표로 변환
	Vector4 clipCoords = NDCToClip(ndcCoords);

	// 3. 클립 좌표를 뷰 좌표로 변환
	Matrix invProj = (mpCamera->GetProjRow()).Invert();
	Vector4 eyeCoords = ClipToEye(clipCoords, invProj);

	// 4. 뷰 좌표를 월드 좌표로 변환
	Matrix invView = (mpCamera->GetViewRow()).Invert();
	Vector3 rayWorld = EyeToWorld(eyeCoords, invView);

	// 레이의 원점은 카메라의 위치, 방향은 rayWorld
	return rayWorld;
}

Vector3 AnimationTestScene::GetTargetPosition(Vector3 rayWorldDirection, Vector3 cameraPosition, Vector3 pointOffset, float cameraDistance)
{
	Vector3 offsetPosition = (cameraPosition + pointOffset) + rayWorldDirection * cameraDistance;
	return offsetPosition;
	//return cameraPosition + rayWorldDirection * distance;
}
