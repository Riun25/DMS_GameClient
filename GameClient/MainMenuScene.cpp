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

	mpLevelManager = new LevelManager(mRegistry, mpRenderManager, mpInputManager, mpUIManager, mpEntityManager
		, mpResourceManager, mpSoundManager);
	mpLevelManager->InitializeforNoneFbx(GetUID());

	// 카메라 엔티티 생성, 세팅
	mpLevelManager->AddCamera(Vector3(0.f, 5.f, -50.f), Vector3(0.f, 0.f, 1.f));

	// 배경
	auto ui1 = mpEntityManager->CreateEntity("UI_Main_FrontBG");
	mpUIManager->AddUI(ui1, "BG_Main.png", Vector2(0, 0), Vector2(1920, 1080), static_cast<int>(Layer::BG), Vector4(1.f));

	auto ui2 = mpEntityManager->CreateEntity("UI_Main_Logo");
	mpUIManager->AddUI(ui2, "UI_img_Logo.png", Vector2(1920, -450), Vector2(600, 530), static_cast<int>(Layer::BG_P), Vector4(1.f));

	// 시작하기 버튼
	auto ui3 = mpEntityManager->CreateEntity("UI_Main_StartButton");
	mpUIManager->AddUI(ui3, "UI_btn_MainToMap.png", Vector2(310, 1080), Vector2(262, 93), static_cast<int>(Layer::BG_B), Vector4(1.f));
	mpUIManager->AddButtonHoveredUI(ui3, "UI_btn_MainToMapHover.png", Vector2(169, 608), Vector2(540, 93));
	ui3->GetComponent<Button>().mUIHoveringSize = Vector2(540, 93);

	// 게임 설명 버튼
	auto ui4 = mpEntityManager->CreateEntity("UI_Main_Instructions");
	mpUIManager->AddUI(ui4, "UI_btn_MainToManual.png", Vector2(310, 1080), Vector2(262, 93), static_cast<int>(Layer::BG_B), Vector4(1.f));
	mpUIManager->AddButtonHoveredUI(ui4, "UI_btn_MainToManualHover.png", Vector2(169, 701), Vector2(540, 93));
	ui4->GetComponent<Button>().mUIHoveringSize = Vector2(540, 93);

	// 크레딧 버튼
	auto ui5 = mpEntityManager->CreateEntity("UI_Main_Credit");
	mpUIManager->AddUI(ui5, "UI_btn_MainToCredit.png", Vector2(349, 1080), Vector2(185, 93), static_cast<int>(Layer::BG_B), Vector4(1.f));
	mpUIManager->AddButtonHoveredUI(ui5, "UI_btn_MainToCreditHover.png", Vector2(169, 794), Vector2(540, 93));
	ui5->GetComponent<Button>().mUIHoveringSize = Vector2(540, 93);

	// 종료하기 버튼
	auto ui6 = mpEntityManager->CreateEntity("UI_Main_ExitButton");
	mpUIManager->AddUI(ui6, "UI_btn_MainToExit.png", Vector2(369, 1080), Vector2(135, 93), static_cast<int>(Layer::BG_B), Vector4(1.f));
	mpUIManager->AddButtonHoveredUI(ui6, "UI_btn_MainToExitHover.png", Vector2(169, 887), Vector2(540, 93));
	ui6->GetComponent<Button>().mUIHoveringSize = Vector2(540, 93);

	/// UI
	mpLevelManager->AddBasicUI();
	AddExplainUI();
	mpLevelManager->AddVerifyPopUpUI();
	mpLevelManager->AddAnimationUI(false, static_cast<int>(mpWorldManager->GetCurrentWorld()->GetPreviousScene()));
	mpLevelManager->SetUIAnimationState(UIAnimationState::NORMAL);

	tempEasing = new EasingFunc();
	mtime = 0;
	mtime2 = 0;
	mTimeExit = 0.0f;
	mIsPressedExplainButton = false;

	mpSoundManager->StopBGM();
	mpSoundManager->PlayBGM("Snd_bgm_Title");

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
		auto view = mRegistry.view<Texture2D>();
		for (auto& entity : view)
		{
			auto& name = mRegistry.get<Name>(entity).mName;
			auto& texture = mRegistry.get<Texture2D>(entity);
			auto button = mRegistry.try_get<Button>(entity);
			auto pEntity = mpEntityManager->GetEntity(entity);

			if (name == "UI_Main_Logo")
			{
				tempEasing->EasingFuncMap["easeOutQuint"](1920, 130, mtime, &texture.mPosition.x);
				tempEasing->EasingFuncMap["easeOutBounce"](-450, 74, mtime/* * 0.7*/, &texture.mPosition.y);
			}

			/// 설명서 관련 코드
			if (mIsPressedExplainButton == true)
			{
				if (name.find("UI_I") != std::string::npos)
				{
					texture.mIsVisible = true;
					if (button)
					{
						button->mIsEnable = true;
					}

					if (name == "UI_I_Close" && (mpUIManager->GetButtonState(pEntity) == ButtonState::PRESSED 
						|| mpInputManager->GetKeyDown(KEY::ESCAPE)))
					{
						mIsPressedExplainButton = false;
						button->mButtonState = ButtonState::NORMAL;
// 						mpSoundManager->PlaySFX("Snd_sfx_ClickBtn")
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
					texture.mIsVisible = false;
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
					tempEasing->EasingFuncMap["easeInOutSine"](1080, 608, mtime2, &texture.mPosition.y);

					if (mpUIManager->GetButtonState(pEntity) == ButtonState::NORMAL)
					{
						button->mUIPosition = texture.mPosition;
					}
					else
					{
						if (mpUIManager->GetButtonState(pEntity) == ButtonState::PRESSED)
						{
// 							mpSoundManager->PlaySFX("Snd_sfx_ClickBtn");
							mpLevelManager->PushAllUI();
							mpLevelManager->SetUIAnimationState(UIAnimationState::POSTGAMESTART);
							return;
						}
					}
				}
				if (name == "UI_Main_Instructions")
				{
					mtime2 += _dTime * 0.5f;
					tempEasing->EasingFuncMap["easeInOutSine"](1080, 701, mtime2, &texture.mPosition.y);

					if (mpUIManager->GetButtonState(pEntity) == ButtonState::NORMAL)
					{
						button->mUIPosition = texture.mPosition;
					}
					else
					{
						if (mpUIManager->GetButtonState(pEntity) == ButtonState::PRESSED
							/*&& mpInputManager->GetKeyUp(KEY::LBUTTON)*/)
						{
// 							mpSoundManager->PlaySFX("Snd_sfx_ClickBtn");
							mIsPressedExplainButton = true;
							break;
						}
					}
				}

				if (name == "UI_Main_Credit")
				{
					mtime2 += _dTime * 0.5f;
					tempEasing->EasingFuncMap["easeInOutSine"](1080, 794, mtime2, &texture.mPosition.y);

					if (mpUIManager->GetButtonState(pEntity) == ButtonState::NORMAL)
					{
						button->mUIPosition = texture.mPosition;
					}
					else
					{
						if (mpUIManager->GetButtonState(pEntity) == ButtonState::PRESSED)
						{
// 							mpSoundManager->PlaySFX("Snd_sfx_ClickBtn");
							mpWorldManager->GetCurrentWorld()->SetScene(static_cast<uint32_t>(SceneName::CREDIT));
							return;
						}
					}
				}

				if (name == "UI_Main_ExitButton")
				{
					mtime2 += _dTime * 0.5f;
					tempEasing->EasingFuncMap["easeInOutSine"](1080, 887, mtime2, &texture.mPosition.y);

					if (mpUIManager->GetButtonState(pEntity) == ButtonState::NORMAL)
					{
						button->mUIPosition = texture.mPosition;
					}
					else
					{
						if (mpUIManager->GetButtonState(pEntity) == ButtonState::PRESSED)
						{
							mpLevelManager->SetPopUpState(PopUpState::OPEN_EXITGAME);
							break;
						}
					}
				}
			}

			// 페이드 인 아웃 기능
			if (auto fadeInOut = mRegistry.try_get<FadeInOut>(entity))
			{
				if (mpLevelManager->mIsExit == true)
				{
					// 게임 종료
	// 				mpSoundManager->PlaySFX("Snd_sfx_ClickBtn");
					if (fadeInOut->IsFadingOutFin(_dTime, 1.5f) == true)
					{
						mpWorldManager->RequestQuit();
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
		mpWorldManager->GetCurrentWorld()->SetScene(static_cast<uint32_t>(SceneName::LOADING));
		return;
	}

	mpLevelManager->UIAniUpdate(_dTime);
}

void MainMenuScene::LateUpdate(float _dTime)
{

}

void MainMenuScene::Finalize()
{
	//mpRenderManager->Erase_Textures("startButton.png");
	//mpRenderManager->Erase_Textures("tempTitle.png");
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
	mpResourceManager->AddFilesInDir2D("Texture2D");
}

void MainMenuScene::AddExplainUI()
{
	// 게임 설명 패널
	auto ui1 = mpEntityManager->CreateEntity("UI_I_Panel");
	mpUIManager->AddUI(ui1, "UI_pnl_ExplainPanel.png", Vector2(960, 100), Vector2(860, 880), static_cast<int>(Layer::COM_P), Vector4(1.f), false);

	// 게임 설명 닫기 버튼
	auto ui2 = mpEntityManager->CreateEntity("UI_I_Close");
	mpUIManager->AddUI(ui2, "UI_btn_Close.png", Vector2(1759, 123), Vector2(38, 38), static_cast<int>(Layer::COM_B), Vector4(1.f), false);
	mpUIManager->AddButtonAllColor(ui2, Vector4(0.8f, 0.8f, 0.8f, 0.8f), Vector4(0.5f, 0.5, 0.5, 1.0f));

	// 게임 설명
	auto ui3 = mpEntityManager->CreateEntity("UI_I_Explain1");
	mpUIManager->AddUI(ui3, "UI_img_ExplainImage.png", Vector2(990, 132), Vector2(806, 808), static_cast<int>(Layer::COM_B), Vector4(1.f), false);
}
