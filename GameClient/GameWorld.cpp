#include "DearsGameEngineAPI.h"
#include "GameWorld.h"
#include "MainMenuScene.h"
#include "LoadingScene.h"
#include "ExampleScene.h"
#include "TutorialScene.h"
#include "StageScene1_2.h"
#include "StageScene1_3.h"
#include "TutorialScene2.h"
#include "AnimationTestScene.h"
#include "CreditScene.h"
#include "BattleScene.h"
#include "PhysicsManager.h"
#include "CustomCallback.h"
#include "CustomFilter.h"
#include "LogoScene.h"
#include "CreditScene.h"
#include "SceneTable.h"
#include "UserData.h"
#include "PlayerData.h"

GameWorld::GameWorld(entt::registry& _registry, GameEngine* _pGameEngine, EventManager* _pEventManager)
	: World(_registry, _pEventManager), m_pEngine(_pGameEngine)
{

}

bool GameWorld::Initialize()
{
	if (!World::Initialize())
	{
		return false;
	}

	// 추가 월드 초기화 로직 

	/// 물리 씬 초기화
	auto callback = new CustomCallback(m_pEngine->GetPhysicsManager(), m_pEngine->GetEventManager(), m_pEngine->GetEntityManager());
	physx::PxSimulationFilterShader filter = CustomFilter::CustomFilterShader;
	auto desc = m_pEngine->GetPhysicsManager()->CreateSceneDesc(callback, filter);
	m_pEngine->GetPhysicsManager()->CreatePhysicsScene(desc);
	//m_pEngine->GetRenderManager()->SetRenderMode(true, false);

	/// 씬 추가
	//0
	auto logoScene = std::make_shared<LogoScene>(m_registry, "logo"
		, m_pEngine->GetEventManager(), m_pEngine->GetRenderManager()
		, m_pEngine->GetPhysicsManager(), m_pEngine->GetInputManager()
		, m_pEngine->GetWorldManager(), m_pEngine->GetUIManager()
		, m_pEngine->GetEntityManager(), m_pEngine->GetResourceManager()
		, m_pEngine->GetSoundManager());
	m_pEngine->GetWorldManager()->GetCurrentWorld()->AddScene(logoScene);

	//1
	auto mainScene = std::make_shared<MainMenuScene>(m_registry, "main menu"
		, m_pEngine->GetEventManager(), m_pEngine->GetRenderManager()
		, m_pEngine->GetPhysicsManager(), m_pEngine->GetInputManager()
		, m_pEngine->GetWorldManager(), m_pEngine->GetUIManager()
		, m_pEngine->GetEntityManager(), m_pEngine->GetResourceManager()
		, m_pEngine->GetSoundManager());
	m_pEngine->GetWorldManager()->GetCurrentWorld()->AddScene(mainScene);

	//2
	auto loadingScene = std::make_shared<LoadingScene>(m_registry, "loading"
		, m_pEngine->GetEventManager(), m_pEngine->GetRenderManager()
		, m_pEngine->GetPhysicsManager(), m_pEngine->GetInputManager()
		, m_pEngine->GetWorldManager(), m_pEngine->GetUIManager()
		, m_pEngine->GetEntityManager(), m_pEngine->GetResourceManager()
		, m_pEngine->GetSoundManager());
	m_pEngine->GetWorldManager()->GetCurrentWorld()->AddScene(loadingScene);

	// 	//3
	// 	auto stageScene1 = std::make_shared<TutorialScene>(m_registry, "stage1"
	// 		, m_pEngine->GetEventManager(), m_pEngine->GetRenderManager()
	// 		, m_pEngine->GetPhysicsManager(), m_pEngine->GetInputManager()
	// 		, m_pEngine->GetWorldManager(), m_pEngine->GetUIManager()
	// 		, m_pEngine->GetEntityManager(), m_pEngine->GetResourceManager()
	// 		, m_pEngine->GetSoundManager());
	// 	stageScene1->SetMapInfo(1, "Stage1");
	// 	m_pEngine->GetWorldManager()->GetCurrentWorld()->AddScene(stageScene1);
	// 
	// 	//4
	// 	auto stageScene2 = std::make_shared<TutorialScene>(m_registry, "stage2"
	// 		, m_pEngine->GetEventManager(), m_pEngine->GetRenderManager()
	// 		, m_pEngine->GetPhysicsManager(), m_pEngine->GetInputManager()
	// 		, m_pEngine->GetWorldManager(), m_pEngine->GetUIManager()
	// 		, m_pEngine->GetEntityManager(), m_pEngine->GetResourceManager()
	// 		, m_pEngine->GetSoundManager());
	// 	stageScene2->SetMapInfo(2, "Stage2");
	// 	m_pEngine->GetWorldManager()->GetCurrentWorld()->AddScene(stageScene2);
	// 
	// 	//5
	// 	auto stageScene3 = std::make_shared<StageScene1_3>(m_registry, "stage3"
	// 		, m_pEngine->GetEventManager(), m_pEngine->GetRenderManager()
	// 		, m_pEngine->GetPhysicsManager(), m_pEngine->GetInputManager()
	// 		, m_pEngine->GetWorldManager(), m_pEngine->GetUIManager()
	// 		, m_pEngine->GetEntityManager(), m_pEngine->GetResourceManager()
	// 		, m_pEngine->GetSoundManager());
	// 	m_pEngine->GetWorldManager()->GetCurrentWorld()->AddScene(stageScene3);

		//6 //3
	auto test1 = std::make_shared<TutorialScene2>(m_registry, "test1"
		, m_pEngine->GetEventManager(), m_pEngine->GetRenderManager()
		, m_pEngine->GetPhysicsManager(), m_pEngine->GetInputManager()
		, m_pEngine->GetWorldManager(), m_pEngine->GetUIManager()
		, m_pEngine->GetEntityManager(), m_pEngine->GetResourceManager()
		, m_pEngine->GetSoundManager());
	m_pEngine->GetWorldManager()->GetCurrentWorld()->AddScene(test1);

	//7	//4
	auto animationTestScene = std::make_shared<AnimationTestScene>(m_registry, "AniTest"
		, m_pEngine->GetEventManager(), m_pEngine->GetRenderManager()
		, m_pEngine->GetPhysicsManager(), m_pEngine->GetInputManager()
		, m_pEngine->GetWorldManager(), m_pEngine->GetUIManager()
		, m_pEngine->GetEntityManager(), m_pEngine->GetResourceManager());
	m_pEngine->GetWorldManager()->GetCurrentWorld()->AddScene(animationTestScene);

	//8 //5
	auto TestScene = std::make_shared<BattleScene>(m_registry, "Test2"
		, m_pEngine->GetEventManager(), m_pEngine->GetRenderManager()
		, m_pEngine->GetPhysicsManager(), m_pEngine->GetInputManager()
		, m_pEngine->GetWorldManager(), m_pEngine->GetUIManager()
		, m_pEngine->GetEntityManager(), m_pEngine->GetResourceManager());
	m_pEngine->GetWorldManager()->GetCurrentWorld()->AddScene(TestScene);

	//9 //6
	auto creditScene = std::make_shared<CreditScene>(m_registry, "credit"
		, m_pEngine->GetEventManager(), m_pEngine->GetRenderManager()
		, m_pEngine->GetPhysicsManager(), m_pEngine->GetInputManager()
		, m_pEngine->GetWorldManager(), m_pEngine->GetUIManager()
		, m_pEngine->GetEntityManager(), m_pEngine->GetResourceManager()
		, m_pEngine->GetSoundManager());
	m_pEngine->GetWorldManager()->GetCurrentWorld()->AddScene(creditScene);

	//10 //7
	auto tutorialScene = std::make_shared<ExampleScene>(m_registry, "tutorial"
		, m_pEngine->GetEventManager(), m_pEngine->GetRenderManager()
		, m_pEngine->GetPhysicsManager(), m_pEngine->GetInputManager()
		, m_pEngine->GetWorldManager(), m_pEngine->GetUIManager()
		, m_pEngine->GetEntityManager(), m_pEngine->GetResourceManager()
		, m_pEngine->GetSoundManager());
	m_pEngine->GetWorldManager()->GetCurrentWorld()->AddScene(tutorialScene);

	/// 씬 테이블로 씬 생성
	CreateSceneFromTable();

	// 전역적으로 사용하는 리소스 추가
	AddResource();

	m_pEngine->GetSoundManager()->StopBGM();
	m_pEngine->GetSoundManager()->SetMasterVolume(0.5);
	m_pEngine->GetSoundManager()->SetBGMVolume(0.5);

	/// 이벤트 추가
	REGISTER_IMMEDIATE_EVENT("Change Scene", ChangeScene);
	REGISTER_IMMEDIATE_EVENT("Exit", ExitGameProcess);

	return true;
}

void GameWorld::FixedUpdate(float _fixedDTime)
{
	World::FixedUpdate(_fixedDTime);

	// 추가 업데이트 로직

}

void GameWorld::Update(float _dTime)
{
	World::Update(_dTime);

	// 씬 전환
	if (m_inputSceneChange)
	{
		if (m_pEngine->GetInputManager()->GetKeyUp(KEY::F1))
		{
			// 		m_pEventManager->TriggerEvent(Event("Change Scene", UID(0)));
			SetScene(SceneName::LOGO);
		}
		else if (m_pEngine->GetInputManager()->GetKeyUp(KEY::F2))
		{
			// 		m_pEventManager->TriggerEvent(Event("Change Scene", UID(1)));
			SetScene(SceneName::MAIN);
		}
		else if (m_pEngine->GetInputManager()->GetKeyUp(KEY::F3))
		{
			// 		m_pEventManager->TriggerEvent(Event("Change Scene", UID(2)));
			SetScene(SceneName::LOADING);
		}
		// 		else if (m_pEngine->GetInputManager()->GetKeyUp(KEY::F4))
		// 		{
		// 			// 		m_pEventManager->TriggerEvent(Event("Change Scene", UID(3)));
		// 			SetScene(SceneName::SCENE1);
		// 		}
		// 		else if (m_pEngine->GetInputManager()->GetKeyUp(KEY::F5))
		// 		{
		// 			//m_pEventManager->TriggerEvent(Event("Change Scene", UID(4)));
		// 			SetScene(SceneName::SCENE2);
		// 		}
		// 		else if (m_pEngine->GetInputManager()->GetKeyUp(KEY::F6))
		// 		{
		// 			//m_pEventManager->TriggerEvent(Event("Change Scene", UID(5)));
		// 			SetScene(SceneName::SCENE3);
		// 		}
		else if (m_pEngine->GetInputManager()->GetKeyUp(KEY::F7))
		{
			//m_pEventManager->TriggerEvent(Event("Change Scene", UID(5)));
			SetScene(SceneName::TUTORIAL);
		}
		// 	else if (m_pEngine->GetInputManager()->GetKeyUp(KEY::F8))
		// 	{
		// 		//m_pEventManager->TriggerEvent(Event("Change Scene", UID(5)));
		// 		SetScene(SceneName::ANITEST);
		// 	}
		// 	else if (m_pEngine->GetInputManager()->GetKeyUp(KEY::F9))
		// 	{
		// 		//m_pEventManager->TriggerEvent(Event("Change Scene", UID(5)));
		// 		SetScene(SceneName::TEST2);
		// 	}
			//else if (m_pEngine->GetInputManager()->GetKeyUp(KEY::F10))
		else if (m_pEngine->GetInputManager()->GetKeyUp(KEY::F8))
		{
			//m_pEventManager->TriggerEvent(Event("Change Scene", UID(5)));
			SetScene(SceneName::CREDIT);
		}

		// 	if (m_pEngine->GetInputManager()->GetKeyUp(KEY::F))
		// 	{
		// 		std::cout << m_pEngine->GetTimeManager()->FPS() << std::endl;
		// 	}

		static int sceneIndex = 8;

		// CTRL 키가 눌려있는 상태
		if (m_pEngine->GetInputManager()->GetKey(KEY::CTRL))
		{
			for (int i = static_cast<int>(KEY::D1); i <= static_cast<int>(KEY::D8); ++i) // 0 또는 1 확인
			{
				KEY firstKey = static_cast<KEY>(i);

				if (m_pEngine->GetInputManager()->GetKeyDown(firstKey)) // 첫 번째 키가 눌려져 있는 상태
				{
					// 					for (int j = static_cast<int>(KEY::D0); j <= static_cast<int>(KEY::D9); ++j) // 두 번째 키 확인
// 					{
						// 						KEY secondKey = static_cast<KEY>(j);

						// 						if (m_pEngine->GetInputManager()->GetKeyDown(secondKey)) // 두 번째 키가 방금 눌림
// 						{
							// 숫자 값을 sceneIndexBuffer에 추가
					sceneIndex += /*10 **/ (i - static_cast<int>(KEY::D1)); // D0부터 시작
					// 							sceneIndex += (j - static_cast<int>(KEY::D0)); // D0부터 시작
					SetScene(static_cast<SceneName>(sceneIndex)); // n번째 씬으로 이동
					// 						}
					// 					}
				}
			}

		}
		else
		{
			sceneIndex = 8;
		}
	}

}

void GameWorld::LateUpdate(float _dTime)
{
	World::LateUpdate(_dTime);

	// 추가 업데이트 로직 
// 	static int mode = 0;
// 	if (m_inputSceneChange)
// 	{
// 		if (m_pEngine->GetInputManager()->GetKeyUp(KEY::D0))
// 		{
// 			mode++;
// 		}
// 		switch (mode)
// 		{
// 		case 0:
// 			m_pEngine->GetRenderManager()->SetRenderMode(true, false, true);
// 			break;
// 		case 1:
// 			m_pEngine->GetRenderManager()->SetRenderMode(false, true, false);
// 			break;
// 		case 2:
// 			m_pEngine->GetRenderManager()->SetRenderMode(true, true, true);
// 			break;
// 		default:
// 			mode = 0;
// 			break;
// 
// 		}
// 	}

	if (m_flyingCamera)
	{
		static bool moveCameraMode = 0;		// 0: world, 1: light
		if (m_pEngine->GetInputManager()->GetKeyUp(KEY::P))
		{
			moveCameraMode = !moveCameraMode;
		}
		// 플라잉 카메라
		auto view = m_registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto camera = m_registry.try_get<CameraComponent>(entity)->m_pCamera;
			if (m_registry.try_get<CameraComponent>(entity)->m_cameraEnum == 0 && !moveCameraMode)
			{
				if (camera)
				{
					if (m_pEngine->GetInputManager()->GetKeyState(KEY::Z) == KEY_STATE::TAP)
					{
						if (camera->mIsFirstPersonMode)
						{
							camera->mIsFirstPersonMode = false;
						}
						else
						{
							camera->mIsFirstPersonMode = true;
						}
					}

					if (m_pEngine->GetInputManager()->GetKeyState(KEY::W) == KEY_STATE::HOLD)
					{
						camera->MoveForward(_dTime * 1);
					}
					if (m_pEngine->GetInputManager()->GetKeyState(KEY::S) == KEY_STATE::HOLD)
					{
						camera->MoveForward(-_dTime * 1);
					}
					if (m_pEngine->GetInputManager()->GetKeyState(KEY::A) == KEY_STATE::HOLD)
					{
						camera->MoveRight(-_dTime * 1);
					}
					if (m_pEngine->GetInputManager()->GetKeyState(KEY::D) == KEY_STATE::HOLD)
					{
						camera->MoveRight(_dTime * 1);
					}
					if (m_pEngine->GetInputManager()->GetKeyState(KEY::Q) == KEY_STATE::HOLD)
					{
						camera->MoveUp(_dTime * 1);
					}
					if (m_pEngine->GetInputManager()->GetKeyState(KEY::E) == KEY_STATE::HOLD)
					{
						camera->MoveUp(-_dTime * 1);
					}

					Vector2 dMousePos = m_pEngine->GetInputManager()->GetMousePos();
					// DLog(LOG_INFO, std::to_string(dMousePos.x) + std::to_string(dMousePos.y));
					camera->OnMouseMove(static_cast<int>(dMousePos.x), static_cast<int>(dMousePos.y));
					//camera->PrintCamInfo();

				}
			}
			if (m_registry.try_get<CameraComponent>(entity)->m_cameraEnum == 1 && moveCameraMode)
			{
				if (camera)
				{
					if (m_pEngine->GetInputManager()->GetKeyState(KEY::Z) == KEY_STATE::TAP)
					{
						if (camera->mIsFirstPersonMode)
						{
							camera->mIsFirstPersonMode = false;
						}
						else
						{
							camera->mIsFirstPersonMode = true;
						}
					}

					if (m_pEngine->GetInputManager()->GetKeyState(KEY::W) == KEY_STATE::HOLD)
					{
						camera->MoveForward(_dTime * 1);
					}
					if (m_pEngine->GetInputManager()->GetKeyState(KEY::S) == KEY_STATE::HOLD)
					{
						camera->MoveForward(-_dTime * 1);
					}
					if (m_pEngine->GetInputManager()->GetKeyState(KEY::A) == KEY_STATE::HOLD)
					{
						camera->MoveRight(-_dTime * 1);
					}
					if (m_pEngine->GetInputManager()->GetKeyState(KEY::D) == KEY_STATE::HOLD)
					{
						camera->MoveRight(_dTime * 1);
					}
					if (m_pEngine->GetInputManager()->GetKeyState(KEY::Q) == KEY_STATE::HOLD)
					{
						camera->MoveUp(_dTime * 1);
					}
					if (m_pEngine->GetInputManager()->GetKeyState(KEY::E) == KEY_STATE::HOLD)
					{
						camera->MoveUp(-_dTime * 1);
					}

					Vector2 dMousePos = m_pEngine->GetInputManager()->GetMousePos();
					// DLog(LOG_INFO, std::to_string(dMousePos.x) + std::to_string(dMousePos.y));
					camera->OnMouseMove(static_cast<int>(dMousePos.x), static_cast<int>(dMousePos.y));
					//camera->PrintCamInfo();

				}
			}
		}
	}
}

void GameWorld::Finalize()
{
	auto wData = dynamic_pointer_cast<UserData>(m_pWorldData["config"]);
	wData->Save();
	World::Finalize();
	UNREGISTER_EVENT("Change Scene");
}

void GameWorld::SetScene(const SceneName& _sceneName)
{
	World::SetScene(static_cast<UID>(_sceneName));
}

void GameWorld::ChangeScene(const Event& _event)
{
	auto uid = _event.GetDataAs<UID>().value();
	World::SetScene(uid);
}

void GameWorld::ExitGameProcess(const Event& _event)
{
	//m_pEngine->Finalize();
	ExitProcess(0);
}

void GameWorld::AddResource()
{
	//m_pEngine->GetResourceManager()->GetFileManager()->PrintAll(false);

	/// 사운드 리소스 추가
	m_pEngine->GetSoundManager()->SetListenerPosition(0.f, 0.f, -10.f);
	m_pEngine->GetSoundManager()->SetBGMVolume(0.5);
	m_pEngine->GetSoundManager()->LoadBGM("../../Resources/Sound/BGM/Snd_bgm_Title.wav", "Snd_bgm_Title");
	m_pEngine->GetSoundManager()->LoadBGM("../../Resources/Sound/BGM/Snd_bgm_Credit.wav", "Snd_bgm_Credit");
	m_pEngine->GetSoundManager()->LoadSFX("../../Resources/Sound/SFX/Snd_sfx_ClickBtn.wav", "Snd_sfx_ClickBtn");

	// 마우스 커서 추가
	m_pEngine->GetInputManager()->AddMouseCursor("../../Resources/Texture/Texture2D/Cursor_Attack.ico");
	m_pEngine->GetInputManager()->AddMouseCursor("../../Resources/Texture/Texture2D/ICON_X.ico");
	m_pEngine->GetInputManager()->AddMouseCursor("../../Resources/Texture/Texture2D/ICON_V.ico");

	m_pEngine->GetInputManager()->SetMouseCursor(0);

	// 폰트 추가
	//m_pEngine->GetResourceManager()->AddFont("NotoSansKR-Regular(24).ttf", 24.f, true);	// 36		// 3/5
	//m_pEngine->GetRenderManager()->m_pGraphicsEngine->AddFont("../../Resources/Font/", "NotoSansKR-Regular(24).ttf", 24.f, true);	// 36		// 3/5
	//m_pEngine->GetResourceManager()->AddFont("NotoSansKR-Regular(32).ttf", 32.f, true);	// 48	// 4/5
	//m_pEngine->GetResourceManager()->AddFont("NotoSansKR-Regular(36).ttf", 36.f, true);	// 54	// 9/10
	//m_pEngine->GetResourceManager()->AddFont("NotoSansKR-Regular(40).ttf", 40.f, true);	// 60	// 1
	//m_pEngine->GetResourceManager()->AddFont("NotoSansKR-Regular(48).ttf", 48.f, true);	// 72	// 6/5
	//m_pEngine->GetResourceManager()->AddFont("NotoSansKR-Regular(60).ttf", 60.f, true);		// 90	// 3/2
	m_pEngine->GetRenderManager()->m_pGraphicsEngine->AddFont("../../Resources/Font/", "KIMM_Bold(60).ttf", 60.f, true);		// 90	// 3/2
	m_pEngine->GetUIManager()->FontSetFinish();

	m_pEngine->GetRenderManager()->AddDDSTexture("../../Resources/Texture/CubeMap/", "skybox_diffuse.dds");
	m_pEngine->GetRenderManager()->AddDDSTexture("../../Resources/Texture/CubeMap/", "skybox_specular.dds");

	// 월드 세팅
	LoadWorldSetting();
}

void GameWorld::LoadWorldSetting()
{
	/// 월드 데이터 로드
	std::shared_ptr<IData> worldData = std::make_shared<UserData>("config.json");
	m_pWorldData["config"] = worldData;
	auto wData = dynamic_pointer_cast<UserData>(worldData);
	wData->Load();

	m_pEngine->GetTimeManager()->SetTargetFPS(wData->config.fps);
	m_pEngine->GetRenderManager()->SetRenderMode(true, wData->debug.drawCollider, wData->graphics.drawShadow);
	m_pEngine->GetUIManager()->SetRenderDebugInfo(wData->debug.drawFps, wData->debug.drawCameraInfo);
	m_flyingCamera = wData->debug.flyingCamera;
	m_inputSceneChange = wData->debug.inputSceneChange;
	if (!wData->debug.logging)
	{
		SET_LOG_LEVEL(LOG_OFF);
	}
	m_pEngine->GetSoundManager()->SetMasterVolume(wData->audio.masterVolume * 0.2f);
	m_pEngine->GetSoundManager()->SetBGMVolume(wData->audio.bgmVolume * 0.2f);
	m_pEngine->GetSoundManager()->SetSFXVolume(wData->audio.sfxVolume * 0.2f);

}

void GameWorld::CreateSceneFromTable()
{
	auto sceneTable = m_pEngine->GetResourceManager()->ParseCSV<SceneTable>("StageVariables.csv");

	for (auto& scene : sceneTable)
	{
		auto stageScene = std::make_shared<TutorialScene>(m_registry, scene.sceneName
			, m_pEngine->GetEventManager(), m_pEngine->GetRenderManager()
			, m_pEngine->GetPhysicsManager(), m_pEngine->GetInputManager()
			, m_pEngine->GetWorldManager(), m_pEngine->GetUIManager()
			, m_pEngine->GetEntityManager(), m_pEngine->GetResourceManager()
			, m_pEngine->GetSoundManager());
		stageScene->SetMapInfo(scene.sceneNum, scene.sceneName);
		AddScene(stageScene);
	}

}
