#include "DearsGameEngineAPI.h"
#include "MainMenuScene.h"
#include "ResourceManager.h"
#include "Button.h"
#include "FadeInOut.h"
#include "Name.h"
#include "LayerEnum.h"
#include "SceneNumEnum.h"

MainMenuScene::MainMenuScene(entt::registry& _registry, const std::string& _name,
	EventManager* _pEventManager, RenderManager* _pRenderManager,
	PhysicsManager* _pPhysicsManager, InputManager* _pInpueManager,
	WorldManager* _worldManager, UIManager* _pUIManager, EntityManager* _pEntityManager
	, ResourceManager* _pResourceManager, SoundManager* _pSoundManager)
	: Scene(_registry, _name, _pEventManager, _pRenderManager, _pPhysicsManager
		, _pInpueManager, _worldManager, _pUIManager, _pEntityManager, _pResourceManager
		, _pSoundManager)
	, tempEasing(nullptr)
{
	mpLevelManager = nullptr;
}

bool MainMenuScene::Initialize()
{
	/// 리소스 추가
	AddResource();

	mpLevelManager = new LevelManager(m_registry, m_pRenderManager, m_pInputManager, m_pUIManager, m_pEntityManager
		, m_pResourceManager, m_pSoundManager);
	mpLevelManager->InitializeforNoneFbx(GetUID());

	// 카메라 엔티티 생성, 세팅
	mpLevelManager->AddCamera(Vector3(0.f, 5.f, -50.f), Vector3(0.f, 0.f, 1.f));

	// 배경
	auto ui1 = m_pEntityManager->CreateEntity("UI_Main_FrontBG");
	m_pUIManager->AddUI(ui1, "BG_Main.png", Vector2(0, 0), Vector2(1920, 1080), static_cast<int>(Layer::BG), Vector4(1.f));

	auto ui2 = m_pEntityManager->CreateEntity("UI_Main_Logo");
	m_pUIManager->AddUI(ui2, "UI_img_Logo.png", Vector2(1920, -450), Vector2(600, 530), static_cast<int>(Layer::BG_P), Vector4(1.f));

	// 시작하기 버튼
	auto ui3 = m_pEntityManager->CreateEntity("UI_Main_StartButton");
	m_pUIManager->AddUI(ui3, "UI_btn_MainToMap.png", Vector2(310, 1080), Vector2(262, 93), static_cast<int>(Layer::BG_B), Vector4(1.f));
	m_pUIManager->AddButtonHoveredUI(ui3, "UI_btn_MainToMapHover.png", Vector2(169, 608), Vector2(540, 93));
	ui3->GetComponent<Button>().mUIHoveringSize = Vector2(540, 93);

	// 게임 설명 버튼
	auto ui4 = m_pEntityManager->CreateEntity("UI_Main_Instructions");
	m_pUIManager->AddUI(ui4, "UI_btn_MainToManual.png", Vector2(310, 1080), Vector2(262, 93), static_cast<int>(Layer::BG_B), Vector4(1.f));
	m_pUIManager->AddButtonHoveredUI(ui4, "UI_btn_MainToManualHover.png", Vector2(169, 701), Vector2(540, 93));
	ui4->GetComponent<Button>().mUIHoveringSize = Vector2(540, 93);

	// 크레딧 버튼
	auto ui5 = m_pEntityManager->CreateEntity("UI_Main_Credit");
	m_pUIManager->AddUI(ui5, "UI_btn_MainToCredit.png", Vector2(349, 1080), Vector2(185, 93), static_cast<int>(Layer::BG_B), Vector4(1.f));
	m_pUIManager->AddButtonHoveredUI(ui5, "UI_btn_MainToCreditHover.png", Vector2(169, 794), Vector2(540, 93));
	ui5->GetComponent<Button>().mUIHoveringSize = Vector2(540, 93);

	// 종료하기 버튼
	auto ui6 = m_pEntityManager->CreateEntity("UI_Main_ExitButton");
	m_pUIManager->AddUI(ui6, "UI_btn_MainToExit.png", Vector2(369, 1080), Vector2(135, 93), static_cast<int>(Layer::BG_B), Vector4(1.f));
	m_pUIManager->AddButtonHoveredUI(ui6, "UI_btn_MainToExitHover.png", Vector2(169, 887), Vector2(540, 93));
	ui6->GetComponent<Button>().mUIHoveringSize = Vector2(540, 93);

	/// UI
	mpLevelManager->AddBasicUI();
	AddExplainUI();
	mpLevelManager->AddVerifyPopUpUI();
	mpLevelManager->AddAnimationUI(false, static_cast<int>(m_pWorldManager->GetCurrentWorld()->GetPreviousScene()));
	mpLevelManager->SetUIAnimationState(UIAnimationState::NORMAL);

	tempEasing = new EasingFunc();
	mtime = 0;
	mtime2 = 0;
	mTimeExit = 0.0f;
	mIsPressedExplainButton = false;

	m_pSoundManager->StopBGM();
	m_pSoundManager->PlayBGM("Snd_bgm_Title");

	return Scene::Initialize();
}

void MainMenuScene::FixedUpdate(float _dTime)
{

}

void MainMenuScene::Update(float _dTime)
{
	mpLevelManager->BasicUIUpdate();
	mpLevelManager->PopUpUIMainUpdate();

	if (mpLevelManager->GetUIAnimationState() == UIAnimationState::NORMAL)
	{
		mtime += _dTime * 0.5f;
		auto view = m_registry.view<Texture2D>();
		for (auto& entity : view)
		{
			auto& name = m_registry.get<Name>(entity).m_name;
			auto& texture = m_registry.get<Texture2D>(entity);
			auto button = m_registry.try_get<Button>(entity);
			auto pEntity = m_pEntityManager->GetEntity(entity);

			if (name == "UI_Main_Logo")
			{
				tempEasing->EasingFuncMap["easeOutQuint"](1920, 130, mtime, &texture.m_position.x);
				tempEasing->EasingFuncMap["easeOutBounce"](-450, 74, mtime/* * 0.7*/, &texture.m_position.y);
			}

			/// 설명서 관련 코드
			if (mIsPressedExplainButton == true)
			{
				if (name.find("UI_I") != std::string::npos)
				{
					texture.m_isVisible = true;
					if (button)
					{
						button->mIsEnable = true;
					}

					if (name == "UI_I_Close" && (m_pUIManager->GetButtonState(pEntity) == ButtonState::PRESSED 
						|| m_pInputManager->GetKeyDown(KEY::ESCAPE)))
					{
						mIsPressedExplainButton = false;
						button->mButtonState = ButtonState::NORMAL;
// 						m_pSoundManager->PlaySFX("Snd_sfx_ClickBtn")
						//break;
					}
				}
				if (name.find("UI_Main") != std::string::npos)
				{
					if (button)
					{
						button->mIsEnable = false;
					}
				}
			}
			else
			{
				if (name.find("UI_I") != std::string::npos)
				{
					texture.m_isVisible = false;
					if (button)
					{
						button->mIsEnable = false;
					}
				}

				if (name.find("UI_Main") != std::string::npos)
				{
					if (button)
					{
						button->mIsEnable = true;
					}
				}
			}

			/// dotween과 관련된 코드
			if (button)
			{
				if (name == "UI_Main_StartButton")
				{
					mtime2 += _dTime * 0.5f;
					tempEasing->EasingFuncMap["easeInOutSine"](1080, 608, mtime2, &texture.m_position.y);

					if (m_pUIManager->GetButtonState(pEntity) == ButtonState::NORMAL)
					{
						button->mUIPosition = texture.m_position;
					}
					else
					{
						if (m_pUIManager->GetButtonState(pEntity) == ButtonState::PRESSED)
						{
// 							m_pSoundManager->PlaySFX("Snd_sfx_ClickBtn");
							mpLevelManager->PushAllUI();
							mpLevelManager->SetUIAnimationState(UIAnimationState::POSTGAMESTART);
							return;
						}
					}
				}
				if (name == "UI_Main_Instructions")
				{
					mtime2 += _dTime * 0.5f;
					tempEasing->EasingFuncMap["easeInOutSine"](1080, 701, mtime2, &texture.m_position.y);

					if (m_pUIManager->GetButtonState(pEntity) == ButtonState::NORMAL)
					{
						button->mUIPosition = texture.m_position;
					}
					else
					{
						if (m_pUIManager->GetButtonState(pEntity) == ButtonState::PRESSED
							/*&& m_pInputManager->GetKeyUp(KEY::LBUTTON)*/)
						{
// 							m_pSoundManager->PlaySFX("Snd_sfx_ClickBtn");
							mIsPressedExplainButton = true;
							break;
						}
					}
				}

				if (name == "UI_Main_Credit")
				{
					mtime2 += _dTime * 0.5f;
					tempEasing->EasingFuncMap["easeInOutSine"](1080, 794, mtime2, &texture.m_position.y);

					if (m_pUIManager->GetButtonState(pEntity) == ButtonState::NORMAL)
					{
						button->mUIPosition = texture.m_position;
					}
					else
					{
						if (m_pUIManager->GetButtonState(pEntity) == ButtonState::PRESSED)
						{
// 							m_pSoundManager->PlaySFX("Snd_sfx_ClickBtn");
							m_pWorldManager->GetCurrentWorld()->SetScene(static_cast<uint32_t>(SceneName::CREDIT));
							return;
						}
					}
				}

				if (name == "UI_Main_ExitButton")
				{
					mtime2 += _dTime * 0.5f;
					tempEasing->EasingFuncMap["easeInOutSine"](1080, 887, mtime2, &texture.m_position.y);

					if (m_pUIManager->GetButtonState(pEntity) == ButtonState::NORMAL)
					{
						button->mUIPosition = texture.m_position;
					}
					else
					{
						if (m_pUIManager->GetButtonState(pEntity) == ButtonState::PRESSED)
						{
							mpLevelManager->SetPopUpState(PopUpState::OPEN_EXITGAME);
							break;
						}
					}
				}
			}

			// 페이드 인 아웃 기능
			if (auto fadeInOut = m_registry.try_get<FadeInOut>(entity))
			{
				if (mpLevelManager->mIsExit == true)
				{
					// 게임 종료
	// 				m_pSoundManager->PlaySFX("Snd_sfx_ClickBtn");
					if (fadeInOut->IsFadingOutFin(_dTime, 1.5f) == true)
					{
						m_pWorldManager->RequestQuit();
						return;
					}
				}
				else
				{
					fadeInOut->IsFadingInFin(_dTime) == true;
					if (fadeInOut->state == FadeInOutState::NONE)
					{
						fadeInOut->mAlpha = 1.0f;
						fadeInOut->state = FadeInOutState::FADE_OUT;
					}
				}
			}
		}
	}
	else if (mpLevelManager->GetUIAnimationState() == UIAnimationState::NEXTSCENE)
	{
		m_pWorldManager->GetCurrentWorld()->SetScene(static_cast<uint32_t>(SceneName::LOADING));
		return;
	}

	mpLevelManager->UIAniUpdate(_dTime);
}

void MainMenuScene::LateUpdate(float _dTime)
{

}

void MainMenuScene::Finalize()
{
	//m_pRenderManager->Erase_Textures("startButton.png");
	//m_pRenderManager->Erase_Textures("tempTitle.png");
// 	mpLevelManager->Finalize();
	if (mpLevelManager)
	{
		delete mpLevelManager;
	}
	if (tempEasing)
	{
		delete tempEasing;
	}

	Scene::Finalize();
}

void MainMenuScene::AddResource()
{
	/// 리소스 추가
	// 2D 텍스처 추가
	m_pResourceManager->AddFilesInDir2D("Texture2D");
}

void MainMenuScene::AddExplainUI()
{
	// 게임 설명 패널
	auto ui1 = m_pEntityManager->CreateEntity("UI_I_Panel");
	m_pUIManager->AddUI(ui1, "UI_pnl_ExplainPanel.png", Vector2(960, 100), Vector2(860, 880), static_cast<int>(Layer::COM_P), Vector4(1.f), false);

	// 게임 설명 닫기 버튼
	auto ui2 = m_pEntityManager->CreateEntity("UI_I_Close");
	m_pUIManager->AddUI(ui2, "UI_btn_Close.png", Vector2(1759, 123), Vector2(38, 38), static_cast<int>(Layer::COM_B), Vector4(1.f), false);
	m_pUIManager->AddButtonAllColor(ui2, Vector4(0.8f, 0.8f, 0.8f, 0.8f), Vector4(0.5f, 0.5, 0.5, 1.0f));

	// 게임 설명
	auto ui3 = m_pEntityManager->CreateEntity("UI_I_Explain1");
	m_pUIManager->AddUI(ui3, "UI_img_ExplainImage.png", Vector2(990, 132), Vector2(806, 808), static_cast<int>(Layer::COM_B), Vector4(1.f), false);
}
