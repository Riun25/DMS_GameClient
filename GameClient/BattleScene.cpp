#include "BattleScene.h"
#include "DearsGameEngineAPI.h"

BattleScene::BattleScene(entt::registry& _registry, const std::string& _name
	, EventManager* _pEventManager, RenderManager* _pRenderManager
	, PhysicsManager* _pPhysicsManager, InputManager* _pInpueManager
	, WorldManager* _worldManager, UIManager* _UIManager, EntityManager* _pEntityManager
	, ResourceManager* _pResourceManager)
	: Scene(_registry, _name, _pEventManager, _pRenderManager, _pPhysicsManager
		, _pInpueManager, _worldManager, _UIManager, _pEntityManager, _pResourceManager)
{

}

bool BattleScene::Initialize()
{
	// 모델 추가
	m_pRenderManager->AddModel("../TestAsset/Test/", "Character 01.fbx");
	m_pRenderManager->AddModel("../TestAsset/Test/", "Ground 01.fbx");
	m_pRenderManager->AddModel("../TestAsset/", "box.fbx");

	// 텍스쳐 추가
	m_pRenderManager->Add3DTexture("../TestAsset/Test/", "M-Cos-16-Blue.png");
	m_pRenderManager->Add3DTexture("../Resources/Texture/", "M-Cos-03-Purple.png");
	m_pRenderManager->Add3DTexture("../TestAsset/Test/", "Ground-Grass.png");

	// UI 리소스 추가
	m_pUIManager->AddTexture2D("../../Resources/Dummy/", "UI_img_MercNum.png");

	// 폰트 추가
	m_pUIManager->AddFont("../../Resources/Font/", "B.ttf", 40.f, true);
	m_pUIManager->AddFont("../../Resources/Font/", "L.ttf", 120.f, true);
	m_pUIManager->FontSetFinish();

	/// ----------------------------------------이 아래는 편집하지 말 것 ---------------------------------------------

	/// 오브젝트 생성
	// 카메라 엔티티 생성, 세팅
	auto camera = m_pEntityManager->CreateEntity("Camera");
	camera->AddComponent<CameraComponent>(m_pRenderManager->GetScreenWidth(), m_pRenderManager->GetScreenHeight()
		//	, 10.f, Vector3(0.f, 15.f, -17.f), Vector3(0.f, 0.f, 1.f), Vector3(0.f, 1.f, 0.f), Vector3(0.0f, 55.0f, 0.0f), Vector3(70.f, 0.01f, 1000.f));
		, 10.f, Vector3(-2.f, 4.f, -50.f), Vector3(0.f, 0.f, 1.f), Vector3(0.f, 1.f, 0.f), Vector3(70.f, 0.01f, 1000.f), static_cast<unsigned int>(cameraEnum::WorldCamera)); // 지우지 말 것
	camera->GetComponent<CameraComponent>().m_pCamera->mIsFirstPersonMode = false;

	// 바닥
	auto plane = m_pEntityManager->CreateEntity("plane");
	plane->AddComponent<Transform>(Vector3(0.f, 0.0f, 0.f), Vector3(0.f, 0.0f, 0.f), Vector3(0.775f));
	plane->AddComponent<MeshRenderer>("Ground 01.fbx", "Ground 01_Mesh", false, false);
	plane->AddComponent<Texture3D>("Ground-Grass.png");

	for (int i = -15; i <= 16; i++)
	{
		int j = 0;
		auto tile = m_pEntityManager->CreateEntity("tile");
		tile->AddComponent<Transform>(Vector3(0, j, -0.5f + i), Vector3(0.f), Vector3(20.0f, 0.005f, 0.005f));
		tile->AddComponent<MeshRenderer>("box.fbx", "Cube.001", false, false);
		tile->AddComponent<Texture3D>("M-Cos-03-Purple.png");

		auto tile1 = m_pEntityManager->CreateEntity("tile");
		tile1->AddComponent<Transform>(Vector3(-0.5f + i, j, 0.f), Vector3(0.f), Vector3(0.005f, 0.005f, 20.f));
		tile1->AddComponent<MeshRenderer>("box.fbx", "Cube.001", false, false);
		tile1->AddComponent<Texture3D>("M-Cos-03-Purple.png");
	}

	/// 라이트 세팅
	auto lightEntity = m_pEntityManager->CreateEntity("LightGroup");
	lightEntity->AddComponent<LightComponent>();
	//auto& lightsData = lightEntity->GetComponent<LightComponent>().m_constData;

	m_pRenderManager->LightInitialize(&lightEntity->GetComponent<LightComponent>().m_commonConstData, 3);
// 	m_pRenderManager->SetDirLight(&lightEntity->GetComponent<LightComponent>().m_commonConstData, 0, 1.0f, Vector3(0.0f, -1.0f, 1.0f));
// 	m_pRenderManager->PrintLightInfo(&lightEntity->GetComponent<LightComponent>().m_commonConstData);

	Light templight = lightEntity->GetComponent<LightComponent>().m_commonConstData.light[0];
	templight.position = Vector3(-100.f, 100.0f, -100.f);
	std::shared_ptr<Entity> tempEntity = m_pEntityManager->CreateEntity("Camera");
	tempEntity->AddComponent<CameraComponent>(m_pRenderManager->GetScreenWidth(), m_pRenderManager->GetScreenHeight()
		, 0.f, Vector3(-100.f, 100.0f, -100.f), Vector3(1.f, -1.f, 1.f), Vector3(0.f, 1.f, 0.f), Vector3(70.f, 100.f, 1000.f),
		static_cast<unsigned int>(cameraEnum::LightCamera));
	tempEntity->GetComponent<CameraComponent>().setLightCameraIndex(0);	//0번째 라이트기 때문
	tempEntity->GetComponent<CameraComponent>().setViewDir(templight.direction);	//0번째 라이트기 때문
	tempEntity->GetComponent<CameraComponent>().setEyePos(templight.position);	//0번째 라이트기 때문

	/// ----------------------------------------이 위는 편집하지 말 것-------------------------------------------------

	// 위치값 Vector3(), 스케일 Vector3(), 모델 파일명, 모델 메쉬이름, 텍스쳐 파일명
	std::tuple tup1 = std::make_tuple(Vector3(0, 0, 0), Vector3(0.1, 0.1, 0.1)
		, std::string("Character 01.fbx"), std::string("Character 01"), std::string("M-Cos-16-Blue.png"));
	m_pEventManager->TriggerEvent(Event("CreateSomething", tup1));


	// UI 셋팅 (이미지) [이미지 이름, 위치XY, 사이즈WH, 레이어, RGBA[0~1]) RGB 1 이 기본, 0에 가까울수록 검은색 // A는 투명도 
	auto ui1 = m_pEntityManager->CreateEntity("UI");
	m_pUIManager->AddUI(ui1, "UI_img_MercNum.png", Vector2(620.f, 980.f), Vector2(80.f, 80.f), 2, Vector4(0.8f, 0.8f, 0.8f, 1.0f));

	// UI 셋팅 (텍스트) [텍스트, 폰트 파일이름, 위치, 레이어])
	auto text1 = m_pEntityManager->CreateEntity("Text");
	m_pUIManager->AddText(text1, u8"1234 가나다라 ABCD", "B.ttf", Vector2(620.f, 980.f), 0);

	REGISTER_IMMEDIATE_EVENT("CreateSomething", CreateSomething);

	return Scene::Initialize();
}

void BattleScene::FixedUpdate(float _dTime)
{

}

void BattleScene::Update(float _dTime)
{

}

void BattleScene::LateUpdate(float _dTime)
{

}

void BattleScene::Finalize()
{
	UNREGISTER_EVENT("CreateSomething");

}

void BattleScene::CreateSomething(const Event& _event)
{
	auto tup = _event.GetDataAs<std::tuple<Vector3, Vector3, std::string, std::string, std::string>>().value();
	auto obj = m_pEntityManager->CreateEntity("obj");
	obj->AddComponent<Transform>(std::get<0>(tup), Vector3(), std::get<1>(tup));
	obj->AddComponent<MeshRenderer>(std::get<2>(tup), std::get<3>(tup));
	obj->AddComponent<Texture3D>(std::get<4>(tup));
	m_pRenderManager->InitailizeEntity(obj);
}
