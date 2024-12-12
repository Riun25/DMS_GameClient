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
	// �� �߰�
	mpRenderManager->AddModel("../TestAsset/Test/", "Character 01.fbx");
	mpRenderManager->AddModel("../TestAsset/Test/", "Ground 01.fbx");
	mpRenderManager->AddModel("../TestAsset/", "box.fbx");

	// �ؽ��� �߰�
	mpRenderManager->Add3DTexture("../TestAsset/Test/", "M-Cos-16-Blue.png");
	mpRenderManager->Add3DTexture("../Resources/Texture/", "M-Cos-03-Purple.png");
	mpRenderManager->Add3DTexture("../TestAsset/Test/", "Ground-Grass.png");

	// UI ���ҽ� �߰�
	mpUIManager->AddTexture2D("../../Resources/Dummy/", "UI_img_MercNum.png");

	// ��Ʈ �߰�
	mpUIManager->AddFont("../../Resources/Font/", "B.ttf", 40.f, true);
	mpUIManager->AddFont("../../Resources/Font/", "L.ttf", 120.f, true);
	mpUIManager->FontSetFinish();

	/// ----------------------------------------�� �Ʒ��� �������� �� �� ---------------------------------------------

	/// ������Ʈ ����
	// ī�޶� ��ƼƼ ����, ����
	auto camera = mpEntityManager->CreateEntity("Camera");
	camera->AddComponent<CameraComponent>(mpRenderManager->GetScreenWidth(), mpRenderManager->GetScreenHeight()
		//	, 10.f, Vector3(0.f, 15.f, -17.f), Vector3(0.f, 0.f, 1.f), Vector3(0.f, 1.f, 0.f), Vector3(0.0f, 55.0f, 0.0f), Vector3(70.f, 0.01f, 1000.f));
		, 10.f, Vector3(-2.f, 4.f, -50.f), Vector3(0.f, 0.f, 1.f), Vector3(0.f, 1.f, 0.f), Vector3(70.f, 0.01f, 1000.f), static_cast<unsigned int>(cameraEnum::WorldCamera)); // ������ �� ��
	camera->GetComponent<CameraComponent>().mpCamera->mIsFirstPersonMode = false;

	// �ٴ�
	auto plane = mpEntityManager->CreateEntity("plane");
	plane->AddComponent<Transform>(Vector3(0.f, 0.0f, 0.f), Vector3(0.f, 0.0f, 0.f), Vector3(0.775f));
	plane->AddComponent<MeshRenderer>("Ground 01.fbx", "Ground 01_Mesh", false, false);
	plane->AddComponent<Texture3D>("Ground-Grass.png");

	for (int i = -15; i <= 16; i++)
	{
		int j = 0;
		auto tile = mpEntityManager->CreateEntity("tile");
		tile->AddComponent<Transform>(Vector3(0, j, -0.5f + i), Vector3(0.f), Vector3(20.0f, 0.005f, 0.005f));
		tile->AddComponent<MeshRenderer>("box.fbx", "Cube.001", false, false);
		tile->AddComponent<Texture3D>("M-Cos-03-Purple.png");

		auto tile1 = mpEntityManager->CreateEntity("tile");
		tile1->AddComponent<Transform>(Vector3(-0.5f + i, j, 0.f), Vector3(0.f), Vector3(0.005f, 0.005f, 20.f));
		tile1->AddComponent<MeshRenderer>("box.fbx", "Cube.001", false, false);
		tile1->AddComponent<Texture3D>("M-Cos-03-Purple.png");
	}

	/// ����Ʈ ����
	auto lightEntity = mpEntityManager->CreateEntity("LightGroup");
	lightEntity->AddComponent<LightComponent>();
	//auto& lightsData = lightEntity->GetComponent<LightComponent>().m_constData;

	mpRenderManager->LightInitialize(&lightEntity->GetComponent<LightComponent>().mCommonConstData, 3);
// 	mpRenderManager->SetDirLight(&lightEntity->GetComponent<LightComponent>().m_commonConstData, 0, 1.0f, Vector3(0.0f, -1.0f, 1.0f));
// 	mpRenderManager->PrintLightInfo(&lightEntity->GetComponent<LightComponent>().m_commonConstData);

	Light templight = lightEntity->GetComponent<LightComponent>().mCommonConstData.light[0];
	templight.position = Vector3(-100.f, 100.0f, -100.f);
	std::shared_ptr<Entity> tempEntity = mpEntityManager->CreateEntity("Camera");
	tempEntity->AddComponent<CameraComponent>(mpRenderManager->GetScreenWidth(), mpRenderManager->GetScreenHeight()
		, 0.f, Vector3(-100.f, 100.0f, -100.f), Vector3(1.f, -1.f, 1.f), Vector3(0.f, 1.f, 0.f), Vector3(70.f, 100.f, 1000.f),
		static_cast<unsigned int>(cameraEnum::LightCamera));
	tempEntity->GetComponent<CameraComponent>().setLightCameraIndex(0);	//0��° ����Ʈ�� ����
	tempEntity->GetComponent<CameraComponent>().setViewDir(templight.direction);	//0��° ����Ʈ�� ����
	tempEntity->GetComponent<CameraComponent>().setEyePos(templight.position);	//0��° ����Ʈ�� ����

	/// ----------------------------------------�� ���� �������� �� ��-------------------------------------------------

	// ��ġ�� Vector3(), ������ Vector3(), �� ���ϸ�, �� �޽��̸�, �ؽ��� ���ϸ�
	std::tuple tup1 = std::make_tuple(Vector3(0, 0, 0), Vector3(0.1, 0.1, 0.1)
		, std::string("Character 01.fbx"), std::string("Character 01"), std::string("M-Cos-16-Blue.png"));
	mpEventManager->TriggerEvent(Event("CreateSomething", tup1));


	// UI ���� (�̹���) [�̹��� �̸�, ��ġXY, ������WH, ���̾�, RGBA[0~1]) RGB 1 �� �⺻, 0�� �������� ������ // A�� ���� 
	auto ui1 = mpEntityManager->CreateEntity("UI");
	mpUIManager->AddUI(ui1, "UI_img_MercNum.png", Vector2(620.f, 980.f), Vector2(80.f, 80.f), 2, Vector4(0.8f, 0.8f, 0.8f, 1.0f));

	// UI ���� (�ؽ�Ʈ) [�ؽ�Ʈ, ��Ʈ �����̸�, ��ġ, ���̾�])
	auto text1 = mpEntityManager->CreateEntity("Text");
	mpUIManager->AddText(text1, u8"1234 �����ٶ� ABCD", "B.ttf", Vector2(620.f, 980.f), 0);

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
	auto obj = mpEntityManager->CreateEntity("obj");
	obj->AddComponent<Transform>(std::get<0>(tup), Vector3(), std::get<1>(tup));
	obj->AddComponent<MeshRenderer>(std::get<2>(tup), std::get<3>(tup));
	obj->AddComponent<Texture3D>(std::get<4>(tup));
	mpRenderManager->InitailizeEntity(obj);
}
