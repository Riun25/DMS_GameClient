#include "DearsGameEngineAPI.h"
#include "CreditScene.h"
#include "SceneNumEnum.h"
#include "FadeInOut.h"
#include "Name.h"


CreditScene::CreditScene(entt::registry& _registry, const std::string& _name,
	EventManager* _pEventManager, RenderManager* _pRenderManager,
	PhysicsManager* _pPhysicsManager, InputManager* _pInpueManager,
	WorldManager* _worldManager, UIManager* _pUIManager, EntityManager* _pEntityManager
	, ResourceManager* _pResourceManager, SoundManager* _pSoundManager)
	: Scene(_registry, _name, _pEventManager, _pRenderManager, _pPhysicsManager
		, _pInpueManager, _worldManager, _pUIManager, _pEntityManager, _pResourceManager
		, _pSoundManager)
{
}

bool CreditScene::Initialize()
{
	accumulateTime = 0;
	mIsCreditFinish = false;

	mpLevelManager = new LevelManager(m_registry, m_pRenderManager, m_pInputManager, m_pUIManager, m_pEntityManager
		, m_pResourceManager, m_pSoundManager);
	mpLevelManager->InitializeforNoneFbx(GetUID());

	// 카메라 엔티티 생성, 세팅
	mpLevelManager->AddCamera(Vector3(0.f, 5.f, -50.f), Vector3(0.f, 0.f, 1.f));

	auto ui1 = m_pEntityManager->CreateEntity("UI_BG");
	m_pUIManager->AddUI(ui1, "Black.png", Vector2(0, 0), Vector2(1920, 1080), 2, Vector4(1.f));
	m_pUIManager->AddText(ui1, u8"\t[마우스 좌클릭] 2배속\n\t\t [Space Bar] 스킵", "KIMM_Bold(60).ttf", Vector2(1710, 1020), 0, true, Vector4(0.8f, 0.8f, 0.8f, 1.0f));
	ui1->GetComponent<Text>().m_scale = 0.3f;

	auto ui2 = m_pEntityManager->CreateEntity("endingCredit");
	m_pUIManager->AddUI(ui2, "UI_img_EndingDeco.png", Vector2(0, 274), Vector2(1920, 14580), 1, Vector4(1.f));

	auto ui3 = m_pEntityManager->CreateEntity("endingCreditText");
	m_pUIManager->AddUI(ui3, "UI_img_EndingFinale.png", Vector2(305, 462), Vector2(1310, 156), 1, Vector4(1.0f, 1.0f, 1.0f, 0.0f));
	ui3->AddComponent<FadeInOut>(&ui3->GetComponent<Texture2D>(), 1.0f, FadeInOutState::FADE_OUT);

	m_pSoundManager->StopBGM();
	m_pSoundManager->PlayBGM("Snd_bgm_Credit");

	return Scene::Initialize();
}

void CreditScene::FixedUpdate(float _dTime)
{
}

void CreditScene::Update(float _dTime)
{
	auto uiView = m_registry.view<Texture2D>();
	for (auto& textureEntity : uiView)
	{
		auto& texture = m_registry.get<Texture2D>(textureEntity);
		auto& name = m_registry.get<Name>(textureEntity).m_name;

		if (name == "endingCredit")
		{
			if (m_pInputManager->GetKey(KEY::LBUTTON))
			{
				texture.m_position.y -= _dTime * 300.0f;
			}
			else
			{
				texture.m_position.y -= _dTime * 100.0f;
			}

			if (texture.m_position.y < -14580.f)
			{
				mIsCreditFinish = true;
			}

			if (texture.m_position.y < -14500.f)
			{
				mBGMFadeOutVolume -= _dTime * 0.2f;
				if (mBGMFadeOutVolume >= 0.f)
				{
					m_pSoundManager->SetBGMVolume(mBGMFadeOutVolume);
				}
			}

			/*if (m_pInputManager->GetKey(KEY::RBUTTON))
			{
				texture.m_position.y = -14000.f;
			}*/
		}

		// -14580이 되면 끝남
		if (mIsCreditFinish == true && name == "endingCreditText")
		{
			// 페이드 인 아웃 기능
			if (auto fadeInOut = m_registry.try_get<FadeInOut>(textureEntity))
			{
				fadeInOut->IsFadingOutFin(_dTime * 0.5f, 1.0f) == true;

				if (fadeInOut->state == FadeInOutState::NONE)
				{
					fadeInOut->mAlpha = 1.0f;
					fadeInOut->state = FadeInOutState::FADE_OUT;
					m_pWorldManager->GetCurrentWorld()->SetScene(static_cast<uint32_t>(SceneName::MAIN));
				}
			}
		}

		if (m_pInputManager->GetKeyDown(KEY::SPACE))
		{
			m_pWorldManager->GetCurrentWorld()->SetScene(static_cast<uint32_t>(SceneName::MAIN));
		}

	}
}

void CreditScene::LateUpdate(float _dTime)
{
}

void CreditScene::Finalize()
{
	Scene::Finalize();
}

void CreditScene::AddResource()
{
}
