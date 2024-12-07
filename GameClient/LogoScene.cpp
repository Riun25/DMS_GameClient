#include "LogoScene.h"
#include "LevelManager.h"
#include "DearsGameEngineAPI.h"
#include "FadeInOut.h"

LogoScene::LogoScene(entt::registry& _registry, const std::string& _name, EventManager* _pEventManager
	, RenderManager* _pRenderManager, PhysicsManager* _pPhysicsManager, InputManager* _pInpueManager
	, WorldManager* _worldManager, UIManager* _pUIManager, EntityManager* _pEntityManager
	, ResourceManager* _pResourceManager, SoundManager* _pSoundManager)
	:Scene(_registry, _name, _pEventManager, _pRenderManager
		, _pPhysicsManager, _pInpueManager, _worldManager
		, _pUIManager, _pEntityManager, _pResourceManager, _pSoundManager)
	, mpLevelManager(nullptr)
{

}

bool LogoScene::Initialize()
{
	AddResource();

	mpLevelManager = new LevelManager(m_registry, m_pRenderManager, m_pInputManager, m_pUIManager, m_pEntityManager
		, m_pResourceManager, m_pSoundManager);
	mpLevelManager->InitializeforNoneFbx(GetUID());

	// 카메라 엔티티 생성, 세팅
	mpLevelManager->AddCamera(Vector3(0.f, 5.f, -50.f), Vector3(0.f, 0.f, 1.f));

	auto ui1 = m_pEntityManager->CreateEntity("UI_titleLogo");
	m_pUIManager->AddUI(ui1, "TitleLogo.png", Vector2(0, 0), Vector2(1920, 1080), 2, Vector4(1.f));

	mpLevelManager->AddBasicUI();
	mpLevelManager->AddVerifyPopUpUI();

	return Scene::Initialize();
}

void LogoScene::FixedUpdate(float _dTime)
{
	bool change = false;
	auto view = m_registry.view<FadeInOut>();
	for (auto& entity : view)
	{
		auto& texture = m_registry.get<Texture2D>(entity);
		auto& fadeInOut = m_registry.get<FadeInOut>(entity);
		if (mFadeIn)
		{
			if (fadeInOut.IsFadingInFin(_dTime / 2) == true)
			{
				mFadeIn = false;
			}
			if (fadeInOut.state == FadeInOutState::NONE)
			{
				fadeInOut.mAlpha = 1.0f;
				fadeInOut.state = FadeInOutState::FADE_OUT;
			}

		}
		else
		{
			if (fadeInOut.IsFadingOutFin(_dTime / 2, 1.f))
			{
				mFadeIn = true;
				fadeInOut.state = FadeInOutState::FADE_IN;
				change = true;
			}
		}

	}

	if (change)
	{
		auto view = m_registry.view<Texture2D>();
		for (auto& entity : view)
		{
			auto& texture = m_registry.get<Texture2D>(entity);
			auto& name = m_registry.get<Name>(entity);
			if (name.m_name == "UI_titleLogo")
			{
				texture.m_isVisible = false;
				change = false;
				m_pWorldManager->GetCurrentWorld()->SetScene(1);
				break;
			}
		}
	}
}

void LogoScene::Update(float _dTime)
{

}

void LogoScene::LateUpdate(float _dTime)
{

}

void LogoScene::Finalize()
{
	if (mpLevelManager)
	{
		delete mpLevelManager;
	}
	Scene::Finalize();
}

void LogoScene::AddResource()
{
	m_pResourceManager->AddFilesInDir2D("Texture2D");

}
