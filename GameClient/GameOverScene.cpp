#include "DearsGameEngineAPI.h"
#include "GameOverScene.h"

GameOverScene::GameOverScene(entt::registry& _registry, const std::string& _name, 
	EventManager* _pEventManager, RenderManager* _pRenderManager, 
	PhysicsManager* _pPhysicsManager, InputManager* _pInpueManager, 
	WorldManager* _worldManager, UIManager* _UIManager, EntityManager* _pEntityManager
	, ResourceManager* _pResourceManager)
	: Scene(_registry, _name, _pEventManager, _pRenderManager, _pPhysicsManager
	, _pInpueManager, _worldManager, _UIManager, _pEntityManager, _pResourceManager)
{

}

bool GameOverScene::Initialize()
{
	/// 리소스 추가
	AddResource();

	// 카메라 엔티티 생성, 세팅
	auto cameraEntity = m_pEntityManager->CreateEntity("Camera");
	cameraEntity->AddComponent<CameraComponent>(m_pRenderManager->GetScreenWidth(), m_pRenderManager->GetScreenHeight()
		, 10.f, Vector3(0.f, 5.f, -50.f), Vector3(0.f, 0.f, 1.f), Vector3(0.f, 1.f, 0.f), Vector3(70.f, 0.01f, 1000.f), static_cast<unsigned int>(cameraEnum::WorldCamera));

	/// 왜 그런지 모르겠지만, 먼저 생성된 ui 오브젝트가 더 위에 그려짐. 주의할것
	auto ui2 = m_pEntityManager->CreateEntity("UI");
	ui2->AddComponent<Texture2D>("GameOver.png", Vector2(0, 0), Vector2(1920, 1080));

	return Scene::Initialize();
}

void GameOverScene::FixedUpdate(float _dTime)
{

}

void GameOverScene::Update(float _dTime)
{

}

void GameOverScene::LateUpdate(float _dTime)
{

}

void GameOverScene::Finalize()
{
	m_pUIManager->Erase_Textures("GameOver.png");

	Scene::Finalize();
}

void GameOverScene::AddResource()
{
	/// 리소스 추가
	m_pUIManager->AddTexture2D("../Resources/Texture/", "GameOver.png");
}
