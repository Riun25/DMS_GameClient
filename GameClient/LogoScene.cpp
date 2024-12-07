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

	mpLevelManager = new LevelManager(mRegistry, mpRenderManager, mpInputManager, mpUIManager, mpEntityManager
		, mpResourceManager, mpSoundManager);
	mpLevelManager->InitializeforNoneFbx(GetUID());

	// 카메라 엔티티 생성, 세팅
	mpLevelManager->AddCamera(Vector3(0.f, 5.f, -50.f), Vector3(0.f, 0.f, 1.f));

	auto ui1 = mpEntityManager->CreateEntity("UI_titleLogo");
	mpUIManager->AddUI(ui1, "TitleLogo.png", Vector2(0, 0), Vector2(1920, 1080), 2, Vector4(1.f));

	mpLevelManager->AddBasicUI();
	mpLevelManager->AddVerifyPopUpUI();

	return Scene::Initialize();
}

void LogoScene::FixedUpdate(float _dTime)
{
	bool change = false;
	auto view = mRegistry.view<FadeInOut>();
	for (auto& entity : view)
	{
		auto& texture = mRegistry.get<Texture2D>(entity);
		auto& fadeInOut = mRegistry.get<FadeInOut>(entity);
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
		auto view = mRegistry.view<Texture2D>();
		for (auto& entity : view)
		{
			auto& texture = mRegistry.get<Texture2D>(entity);
			auto& name = mRegistry.get<Name>(entity);
			if (name.mName == "UI_titleLogo")
			{
				texture.mIsVisible = false;
				change = false;
				mpWorldManager->GetCurrentWorld()->SetScene(1);
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
	mpResourceManager->AddFilesInDir2D("Texture2D");

}
