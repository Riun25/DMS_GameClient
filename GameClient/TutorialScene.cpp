#include "TutorialScene.h"
#include "DearsGameEngineAPI.h"
#include "PhysicsManager.h"
#include "ResourceManager.h"
#include "PlayerComponent.h"
#include "EnemyComponent.h"
#include "WeaponComponent.h"
#include "ProjectileComponent.h"
#include "CustomFilter.h"
#include "MoneyGunComponent.h"
#include "IndicatorComponent.h"
#include "NPCComponent.h"
#include "SceneNumEnum.h"
#include "SceneTable.h"

TutorialScene::TutorialScene(entt::registry& _registry, const std::string& _name
	, EventManager* _pEventManager, RenderManager* _pRenderManager
	, PhysicsManager* _pPhysicsManager, InputManager* _pInputManager, WorldManager* _worldManager
	, UIManager* _UIManager, EntityManager* _pEntityManager, ResourceManager* _pResourceManager
	, SoundManager* _pSoundManager)
	: Scene(_registry, _name, _pEventManager, _pRenderManager, _pPhysicsManager
		, _pInputManager, _worldManager, _UIManager, _pEntityManager, _pResourceManager, _pSoundManager)
{
	mpUnitSystem = nullptr;
	mpLevelManager = nullptr;
	mpAstar = nullptr;
}

bool TutorialScene::Initialize()
{
	/// 리소스 추가
	AddResource();
	// 	mpRenderManager->SetCubeMap("Atrium_diffuseIBL.dds", "Atrium_specularIBL.dds");

		// 씬데이터 생성
	mpSceneData = new SceneData();
	SceneData* pSceneData = dynamic_cast<SceneData*>(mpSceneData);
	auto sceneTable = mpResourceManager->ParseCSV<SceneTable>("StageVariables.csv");
	for (auto& table : sceneTable)
	{
		if (m_mapNum == table.sceneNum)
		{
			pSceneData->m_heldMoney = table.sceneMoney;
			pSceneData->m_totalAmount = table.sceneMoney;
			pSceneData->m_rankA = table.standardRankA;
			pSceneData->m_rankB = table.standardRankB;
			pSceneData->m_time = table.time;
		}
	}
	/// mpAstar 초기화 : 원래는 맵 매니저 안에 있는 게 좋지만 현재 레벨 매니저가 상당 부분 처리하고 있어서 밖으로 따로 빼둔 상태다.
	// 추후 시간이 남을 때 예쁜 구조로 바꿔보자.
	mpAstar = new AStar();
	std::vector<std::vector<int>>* astarMap = new std::vector<std::vector<int>>();

	*astarMap = mpResourceManager->ParseMapCSV(m_stageName + std::string(".csv"));

	mpAstar->Initialize(Vector2(-15.0f), Vector2(15.0f), astarMap);

	/// 매니저 및 시스템 초기화
	// 유닛 시스템 설정(투사체, 플레이어, NPC 시스템 및 장비 매니저 초기화)
	mpUnitSystem = new UnitSystem(mRegistry, mpEntityManager, mpPhysicsManager, mpInputManager, mpWorldManager
		, mpRenderManager, mpResourceManager, mpEventManager, mpUIManager, mpSoundManager, mpAstar);
	mpUnitSystem->Initialize();

	mpLevelManager = new LevelManager(mRegistry, mpRenderManager, mpPhysicsManager, mpInputManager, mpUIManager, mpEntityManager
		, mpResourceManager, mpSoundManager, mpUnitSystem, mpAstar);
	mpLevelManager->Initialize(GetUID());
	/// 기본적인 환경 설정
	// 카메라 엔티티 생성, 세팅
	//mpLevelManager->AddCamera(Vector3(-16.46, 18.06, -16.62), Vector3(-1.f, -0.2f, 0.0f));
	mpLevelManager->AddCamera(mpLevelManager->GetCameraPosInfo().startPos, mpLevelManager->GetCameraPosInfo().startRot);

	// 라이트 세팅
	mpLevelManager->AddLight();
	// 큐브맵 세팅
	mpLevelManager->AddCubeMap(m_mapNum);

	/// 오브젝트 생성
	//mpLevelManager->AddCubeMap();
	// 투명 벽 배치(원거리 무기 삭제)
	mpLevelManager->AddPlaneNWall();
	// 바닥 좌표계 배치
	// mpLevelManager->DrawCoordinatesXZ(true);
	mpLevelManager->DrawCoordinatesXZ(false);
	// 인디케이터 타일 생성
	mpLevelManager->AddSquareAtPointer("blue.png", "red.png");
	mpLevelManager->AddPlaceIndicator();

	// 맵 데이터 추가[기획의 씬 넘버]
	mpLevelManager->AddMapData(m_mapNum);
	// 유닛세팅 --------------------------------------------------------
	// 적군 유닛 세팅
	mpUnitSystem->GetPlayerSystem()->AddSceneEnemyUnits(m_mapNum);
	// 	auto enemy = mpEntityManager->CreateEntity("enemy");
	// 	mpUnitSystem->GetPlayerSystem()->AddUnit(enemy, "RVC_0", Vector3(10.f, 0.f, -10.f));
		//mpUnitSystem->GetPlayerSystem()->AddUnit(enemy, "RVC_R1", Vector3(10.f, 0.f, -10.f));
		// NPC 세팅
	mpUnitSystem->GetNPCSystem()->AddNPC("Chief"); // 아군 용병왕
	mpUnitSystem->GetNPCSystem()->AddNPC("RVC_C"); // 적군 용병왕

	// 폭죽 생성
	mpLevelManager->AddFireCracker();

	if (m_mapNum == 4 || m_mapNum == 5)
	{
		mpLevelManager->AddSmog();
	}

	// 리스타트 확인
	if (mpWorldManager->GetCurrentWorld()->GetCurrentScene()->GetUID() == mpWorldManager->GetCurrentWorld()->GetPreviousScene())
	{
		mpLevelManager->mIsRestart = true;
	}

	/// UI
	mpLevelManager->AddBasicUI();
	mpLevelManager->AddBattleUI(pSceneData);
	mpLevelManager->AddStageNumAndObjects(m_u8StageName);
	mpLevelManager->AddClassUI(2, false); // UI 클래스 버튼
	mpLevelManager->AddVerifyPopUpUI();
	mpLevelManager->AddAnimationUI(true, static_cast<int>(mpWorldManager->GetCurrentWorld()->GetPreviousScene()));
	mpLevelManager->SetUIAnimationState(UIAnimationState::PREBATTLE); // UI 애니메이션 상태 설정

	/// Sound
	mpSoundManager->StopBGM();
	mpSoundManager->PlayBGM("Snd_bgm_BeforeBattle");
	return Scene::Initialize();
}

void TutorialScene::FixedUpdate(float _dTime)
{
	if (mpLevelManager->GetGameState() == GameState::NORMAL || mpLevelManager->GetGameState() == GameState::RESULT)
	{
		mpLevelManager->UIAniUpdate(_dTime);

		switch (mpLevelManager->GetUIAnimationState())
		{
		case UIAnimationState::VISIBLE:
		{
			auto view = mRegistry.view<Texture2D>();
			for (auto& entity : view)
			{
				auto& name = mRegistry.get<Name>(entity).mName;
				auto& texture = mRegistry.get<Texture2D>(entity);
				if (name.find("UI_A_Cloud") != std::string::npos)
				{
					texture.mIsVisible = true;
				}
			}
			mpLevelManager->SetUIAnimationState(UIAnimationState::POSTBATTLE);
		}
		break;
		default:
			break;
		}
	}
	else if (mpLevelManager->GetGameState() == GameState::START_ANIMATION)
	{
		mpLevelManager->StartAnimationFixedUpdate(_dTime);
	}
	else if (mpLevelManager->GetGameState() == GameState::PLAY)
	{
		mpLevelManager->PlayFixedUpdate(_dTime);
	}
	else if (mpLevelManager->GetGameState() == GameState::POST_PLAY_ANIMATION)
	{
		mpLevelManager->PostAnimationFixedUpdate(_dTime);
	}
}

void TutorialScene::Update(float _dTime)
{
	SceneData* pSceneData = dynamic_cast<SceneData*>(mpSceneData);

	mpLevelManager->BasicUIUpdate();
	mpLevelManager->PopUpUIUpdate(pSceneData);

	if (mpLevelManager->mIsGoMain == true)
	{
		mpWorldManager->GetCurrentWorld()->SetScene(static_cast<uint32_t>(SceneName::MAIN));
		return;
	}

	/// 플레이어 시스템 업데이트
	switch (mpLevelManager->GetGameState())
	{
	case GameState::NORMAL:
	{
		mpLevelManager->StartNoneUI();
	}
	break;
	case GameState::PRE_PLACEMENT:
	{
		mpLevelManager->PrePlacementUpdate(pSceneData);
	}
	break;
	case GameState::PLACEMENT:
	{
		mpLevelManager->PlacementUpdate(pSceneData, _dTime);
	}
	break;
	case GameState::PRE_PLAY:
	{
		mpLevelManager->PreplayUpdate(pSceneData);
	}
	break;
	case GameState::PLAY:
	{
		mpUnitSystem->GetProjectileSystem()->Update(_dTime);
		mpLevelManager->PlayUpdate(pSceneData, _dTime);
		mpLevelManager->UpdateParticle(_dTime);
	}
	break;
	case GameState::NONE_UI:
	{
		mpLevelManager->NoneUI();
		mpUnitSystem->GetProjectileSystem()->Finalize();
		int a = mpWorldManager->GetCurrentWorld()->GetCurrentScene()->GetUID();
		int b = static_cast<uint32_t>(SceneName::LAST);
		if (static_cast<uint32_t>(mpWorldManager->GetCurrentWorld()->GetCurrentScene()->GetUID()) == static_cast<uint32_t>(SceneName::LAST) - 1)
		{
			mpLevelManager->mIsFinalStage = true;
		}
	}
	break;
	/*case GameState::POST_PLAY_ANIMATION:
	{
		mpLevelManager->PostAnimationUpdate(_dTime);
	}
	break;*/
	case GameState::POST_PLAY:
	{
		mpLevelManager->PostplayUpdate(pSceneData);
		mpUnitSystem->GetProjectileSystem()->Finalize();
	}
	break;
	case GameState::RESULT:
	{
		mpLevelManager->ResultUpdate(pSceneData, _dTime);

		if (mpLevelManager->mIsGoMain == true)
		{
			mpWorldManager->GetCurrentWorld()->SetScene(static_cast<uint32_t>(SceneName::MAIN));
			return;
		}

		if (mpLevelManager->GetUIAnimationState() == UIAnimationState::NEXTSCENE)
		{
			mpWorldManager->GetCurrentWorld()->SetScene(static_cast<uint32_t>(SceneName::LOADING));
			return;
		}

		if (mpLevelManager->mIsGameEnding == true)
		{
			mpWorldManager->GetCurrentWorld()->SetScene(static_cast<uint32_t>(SceneName::CREDIT));
			return;
		}
	}
	break;
	case GameState::RESTART:
	{
		mpLevelManager->SetUIAnimationState(UIAnimationState::UNVISIBLE);
		mpWorldManager->GetCurrentWorld()->SetScene(mpWorldManager->GetCurrentWorld()->GetCurrentScene()->GetUID());
	}
	break;
	default:
		break;
	}

}

void TutorialScene::LateUpdate(float _dTime)
{
	auto cameraView = mRegistry.view<CameraComponent>();
	for (auto entity : cameraView)
	{
		auto& camera = mRegistry.get<CameraComponent>(entity);
		// 카메라의 마우스 위치 업데이트
		if (camera.mCameraEnum == 0)
		{
			// 			camera.mpCamera->OnMouseMove(static_cast<int>(mpInputManager->GetMousePos().x), static_cast<int>(mpInputManager->GetMousePos().y));
			// 			if (mpInputManager->GetKey(KEY::F))
			// 			{
			// 				if (camera.mpCamera->mIsFirstPersonMode)
			// 				{
			// 					camera.mpCamera->mIsFirstPersonMode = false;
			// 				}
			// 				else
			// 				{
			// 					camera.mpCamera->mIsFirstPersonMode = true;
			// 					camera.mpCamera->PrintCamInfo();
			// 				}
			// 			}
		}
	}

	if (mpInputManager->GetKeyUp(KEY::M))
	{
		auto camera = mpLevelManager->GetWorldCamera();
		camera->SetEyePos(Vector3(0, 17, -17.5));
		camera->SetDirection(Vector3(0, -0.8, 0.6));

		mpRenderManager->CameraSetPerspective();
	}
	if (mpInputManager->GetKeyUp(KEY::N))
	{
		auto camera = mpLevelManager->GetWorldCamera();
		camera->SetEyePos(Vector3(-15.42, 18.06, -17.72));
		camera->SetDirection(Vector3(0.57735, -0.57735, 0.57735));

		mpRenderManager->CameraSetOrthographic(0.03);
	}

	/// 임시로 적용시켜놓은 버프 상수값 변경 버튼
	if (mpInputManager->GetKeyUp(KEY::RIGHT))
	{
		mpUnitSystem->GetPlayerSystem()->mpStatusManager->mMoneyBuffCoef += 0.05f;
	}
	if (mpInputManager->GetKeyUp(KEY::LEFT))
	{
		mpUnitSystem->GetPlayerSystem()->mpStatusManager->mMoneyBuffCoef -= 0.05f;
	}

}

void TutorialScene::Finalize()
{
	mpUnitSystem->GetPlayerSystem()->Finalize();
	mpLevelManager->Finalize();
	Scene::Finalize();
	if (mpLevelManager)
	{
		delete mpLevelManager;
	}
	if (mpUnitSystem)
	{
		delete mpUnitSystem;
	}
	if (mpAstar)
	{
		delete mpAstar;
	}
	if (mpSceneData)
	{
		delete mpSceneData;
	}
}

void TutorialScene::AddResource()
{
	//mpResourceManager->GetFileManager()->PrintAll(false);
	/// 리소스 추가
	// 모델 추가
	//mpRenderManager->AddModel("../TestAsset/", "box.fbx");

	/////UI
	//mpUIManager->AddTexture2D("../Resources/Texture/", "blue.png");

	//// 애니메이션 추가
	//mpResourceManager->AddFilesInDirAni("Animation");

	//// 3D 텍스처 추가
	//mpResourceManager->AddFilesInDir3D("Texture3D");

	//// 큐브맵 텍스처 추가
	//mpResourceManager->AddFilesInDirDDS("CubeMap");

	//// Fbx 추가 -> 추후 클래스 별로 쪼갤 것
	//mpResourceManager->AddFilesInDirModel("Mercenary");
	//mpResourceManager->AddFilesInDirModel("Environment");
	//mpResourceManager->AddFilesInDirModel("Archer");
	//mpResourceManager->AddFilesInDirModel("Enemy");
	//mpResourceManager->AddFilesInDirModel("money");
	//mpResourceManager->AddFilesInDirModel("Chief");
}

void TutorialScene::SetMapInfo(uint8_t _num, string _stageName)
{
	m_mapNum = _num;
	m_stageName = _stageName;
	m_u8StageName = u8string(m_stageName.begin(), m_stageName.end());
}
