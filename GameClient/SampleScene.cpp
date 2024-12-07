#include "DearsGameEngineAPI.h"
#include "SampleScene.h"

SampleScene::SampleScene(entt::registry& _registry, const std::string& _name
	, EventManager* _pEventManager, RenderManager* _pRenderManager
	, PhysicsManager* _pPhysicsManager, InputManager* _pInpueManager
	, WorldManager* _pWorldManager, UIManager* _UIManager, EntityManager* _pEntityManager
	, ResourceManager* _pResourceManage)
	: Scene(_registry, _name, _pEventManager, _pRenderManager
		, _pPhysicsManager, _pInpueManager, _pWorldManager, _UIManager, _pEntityManager, _pResourceManage)
{

}

bool SampleScene::Initialize()
{
	/// 이 씬에서 사용할 이벤트 등록
	REGISTER_IMMEDIATE_EVENT("test event", DoSomething);

	/// 이 씬에서 사용할 리소스 추가 // 모델, 애니메이션, 3D 텍스쳐, 2D 텍스쳐, 폰트
	// 모델 추가
	m_pRenderManager->m_pGraphicsEngine->AddModel("../TestAsset/", "box.fbx");
	// m_pRenderManager->m_pGraphicsEngine->AddModel("", "");

	// 애니메이션 추가
	// m_pRenderManager->m_pGraphicsEngine->AddAnimation("", "");

	// 3D 텍스쳐 추가
	// m_pRenderManager->m_pGraphicsEngine->Add3DTexture("", "");

	// 2D 텍스쳐 추가
	m_pRenderManager->m_pGraphicsEngine->Add2DTexture("../Resources/Texture/", "blue.png");

	// 폰트 추가
	m_pRenderManager->m_pGraphicsEngine->AddFont("../Resources/Font/", "B.ttf", 10.f, true);
	m_pRenderManager->m_pGraphicsEngine->AddFont("../Resources/Font/", "L.ttf", 10.f, true);
	m_pRenderManager->m_pGraphicsEngine->FontSetFinish();


	/// 카메라 생성 및 설정
	auto cameraEntity = m_pEntityManager->CreateEntity("Camera");
	cameraEntity->AddComponent<CameraComponent>(m_pRenderManager->m_pGraphicsEngine->GetScreenWidth(), m_pRenderManager->m_pGraphicsEngine->GetScreenHeight()
		, 10.f, Vector3(0.f, 5.f, -50.f), Vector3(0.f, 0.f, 1.f), Vector3(0.f, 1.f, 0.f),  Vector3(70.f, 0.01f, 1000.f), static_cast<unsigned int>(cameraEnum::WorldCamera));
	m_pEntityManager->AddEntity(cameraEntity);

	/// 라이트 생성 및 설정
	auto lightEntity = m_pEntityManager->CreateEntity("LightGroup");
	lightEntity->AddComponent<LightComponent>();

	Light templight = lightEntity->GetComponent<LightComponent>().m_commonConstData.light[0];
	templight.position = Vector3(-100.f, 100.0f, -100.f);
	std::shared_ptr<Entity> tempEntity = m_pEntityManager->CreateEntity("Camera");
	tempEntity->AddComponent<CameraComponent>(m_pRenderManager->GetScreenWidth(), m_pRenderManager->GetScreenHeight()
		, 0.f, Vector3(-100.f, 100.0f, -100.f), Vector3(1.f, -1.f, 1.f), Vector3(0.f, 1.f, 0.f), Vector3(70.f, 100.f, 1000.f),
		static_cast<unsigned int>(cameraEnum::LightCamera));
	tempEntity->GetComponent<CameraComponent>().setLightCameraIndex(0);	//0번째 라이트기 때문
	tempEntity->GetComponent<CameraComponent>().setViewDir(templight.direction);	//0번째 라이트기 때문
	tempEntity->GetComponent<CameraComponent>().setEyePos(templight.position);	//0번째 라이트기 때문

	m_pRenderManager->m_pGraphicsEngine->LightInitialize(
		&lightEntity->GetComponent<LightComponent>().m_commonConstData, 3);

	m_pRenderManager->m_pGraphicsEngine->SetDirLight(
		&lightEntity->GetComponent<LightComponent>().m_commonConstData
		, 0, 1.0f, Vector3(0.0f, -1.0f, 0.0f));

	m_pRenderManager->m_pGraphicsEngine->SetPointLight(
		&lightEntity->GetComponent<LightComponent>().m_commonConstData
		, 1, 1.0f, 1.0f, 10.0f, Vector3(3.0f, 3.0f, -4.0f)
		, Vector3(0.0f, 1.0f, 0.0f));

	m_pRenderManager->m_pGraphicsEngine->SetSpotLight(
		&lightEntity->GetComponent<LightComponent>().m_commonConstData
		, 2, 1.f, 10.f, 50.f, Vector3(0.f, 0.f, 1.f)
		, Vector3(0.f, 0.f, -4.f), 1.f, Vector3(1.f, 0.f, 0.f));

	// m_pRenderManager->m_pGraphicsEngine->PrintLightInfo(&lightEntity->GetComponent<LightComponent>().m_constData);

	/// 오브젝트들 생성	// 나중에 옮겨야함
	auto entity1 = m_pEntityManager->CreateEntity("box");
	entity1->AddComponent<Transform>();
	entity1->AddComponent<BoxCollider>();
	entity1->AddComponent<Rigidbody>();
	entity1->AddComponent<MeshRenderer>();
	entity1->AddComponent<Texture2D>();

	/// UI 생성
	auto ui = m_pEntityManager->CreateEntity("UI");
	ui->AddComponent<Texture2D>("blue.png", Vector2(0.f, 0.f), Vector2(10.f, 10.f));
	ui->AddComponent<Text>(u8"출력할 텍스트 입력", "B.ttf", Vector2(0.f, 0.f));	// Text가 있을 경우 추가


	return Scene::Initialize();
}

void SampleScene::FixedUpdate(float _dTime)
{

}

void SampleScene::Update(float _dTime)
{
	m_pEventManager->TriggerEvent(Event("test event"));
}

void SampleScene::LateUpdate(float _dTime)
{

}

void SampleScene::Finalize()
{
	// 카메라 메모리 할당 해제
	auto cameraView = m_registry.view<CameraComponent>();
	for (auto entity : cameraView)
	{
		auto camera = m_registry.try_get<CameraComponent>(entity);
		delete camera->m_pCamera;
	}

	/// 리소스 할당 해제
	// 모델
	m_pRenderManager->m_pGraphicsEngine->Erase_ModelInfo("box.fbx");
	// m_pRenderManager->m_pGraphicsEngine->Erase_ModelInfo("");

	// 애니메이션 
	// m_pRenderManager->m_pGraphicsEngine->Erase_Animation("");

	// 3D 텍스쳐 
	// m_pRenderManager->m_pGraphicsEngine->Erase_Textures("");

	// 2D 텍스쳐 
	m_pRenderManager->m_pGraphicsEngine->Erase_Textures("blue.png");

	/// 이 씬에서 사용할 이벤트 등록 해제
	UNREGISTER_EVENT("test event");
}


void SampleScene::DoSomething(const Event& _event)
{
	DLOG(LOG_DEBUG, "Do Somegthing");
}
