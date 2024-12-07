#include "DearsGameEngineAPI.h"
#include "Name.h"
#include "LevelManager.h"
#include "ResourceManager.h"
#include "Texture3D.h"
#include "PlaneComponent.h"
#include "Texture2D.h"
#include "IndicatorComponent.h"
#include "LightComponent.h"
#include "NPCComponent.h"
#include "WeaponComponent.h"
#include "CameraComponent.h"
#include "MessageBox2D.h"
#include "FadeInOut.h"
#include "LayerEnum.h"
#include "SceneNumEnum.h"
#include "EasingComponent.h"
#include "ParticleComponent.h"

LevelManager::LevelManager(entt::registry& _registry, RenderManager* _pRenderManager, PhysicsManager* _pPhysicsManager
	, InputManager* _pInputManager, UIManager* _UIManager, EntityManager* _pEntityManager
	, ResourceManager* _pResourceManager, SoundManager* _pSoundManager, UnitSystem* _pUnitSystem, AStar* _pAstar)
	: mRegistry(_registry)
{
	mpRenderManager = _pRenderManager;
	mpPhysicsManager = _pPhysicsManager;
	mpInputManager = _pInputManager;
	mpUIManager = _UIManager;
	mpEntityManager = _pEntityManager;
	mpResourceManager = _pResourceManager;
	mpSoundManager = _pSoundManager;
	mpUnitSystem = _pUnitSystem;
	mpAstar = _pAstar;

	mMapDataVec = mpResourceManager->ParseCSV<MapData>("Stage_Background.csv");
	mEnemyDataVec = mpResourceManager->ParseCSV<EnemyData>("Enemy.csv");
}

LevelManager::LevelManager(entt::registry& _registry, RenderManager* _pRenderManager, InputManager* _pInputManager
	, UIManager* _UIManager, EntityManager* _pEntityManager, ResourceManager* _pResourceManager, SoundManager* _pSoundManager)
	: mRegistry(_registry)
{
	mpRenderManager = _pRenderManager;
	mpInputManager = _pInputManager;
	mpUIManager = _UIManager;
	mpEntityManager = _pEntityManager;
	mpResourceManager = _pResourceManager;
	mpSoundManager = _pSoundManager;
}

LevelManager::~LevelManager()
{
}

void LevelManager::InitializeforNoneFbx(const UID& _sceneUID)
{
	mCurrentSceneUID = _sceneUID;
}

void LevelManager::Initialize(const UID& _sceneUID)
{
	mCurrentSceneUID = _sceneUID;
	currentLevelState = GameState::NORMAL;
	currentSettingState = SettingState::NORMAL;
	currentPopUpState = PopUpState::NORMAL;
	currentUIAniState = UIAnimationState::NORMAL;

	mIsGameStart = false;
	mIsOneDeploySet = false;
	mTempUnitUID = -1;
	mIsClassButtonPressed = false;
	mIsNext = false;
	mIsExit = false;
	mIsFinalStage = false;
	mIsGameStart = false;
	mIsSkip = false;
	mIsRestart = false;
	mClassName = "";
	tempTime = 0.0f;
	mStartPoint = mpAstar->GetMapStartPoint();
	mEndPoint = mpAstar->GetMapEndPoint();
	mTutorialFlag = false;
}

void LevelManager::SetSceneUID(const UID& _sceneUID)
{
	mCurrentSceneUID = _sceneUID;
}

void LevelManager::AddPlaneNWall()
{
	// 바닥
	auto plane = mpEntityManager->CreateEntity("plane");
	float testScale = 0.775f;
	plane->AddComponent<Transform>(Vector3(0.f, 0.0f, 0.f), Vector3(0.f, 0.0f, 0.f), Vector3(testScale));
	plane->AddComponent<PlaneComponent>(Vector3(0, 1, 0), -0.28);
	plane->AddComponent<Rigidbody>(100.f, 0.f, 0.f, false, false, 0, 1.5, 1.5, true);
	mpPhysicsManager->AddPhysicsObject(plane->GetUID(), TYPE_GROUND);

	// 맵 외곽 투명 벽
	// 후면
	auto wall1 = mpEntityManager->CreateEntity("wall");
	wall1->AddComponent<Transform>();
	wall1->AddComponent<PlaneComponent>(Vector3(0, 0, -1), 15);
	wall1->AddComponent<Rigidbody>(100.f, 0.f, 0.f, false, false, 0, 1.5, 1.5, true);
	mpPhysicsManager->AddPhysicsObject(wall1->GetUID(), TYPE_GROUND);

	// 정면
	auto wall2 = mpEntityManager->CreateEntity("wall");
	wall2->AddComponent<Transform>();
	wall2->AddComponent<PlaneComponent>(Vector3(0, 0, 1), 15);
	wall2->AddComponent<Rigidbody>(100.f, 0.f, 0.f, false, false, 0, 1.5, 1.5, true);
	mpPhysicsManager->AddPhysicsObject(wall2->GetUID(), TYPE_GROUND);

	// 우측
	auto wall4 = mpEntityManager->CreateEntity("wall");
	wall4->AddComponent<Transform>();
	wall4->AddComponent<PlaneComponent>(Vector3(-1, 0, 0), 15);
	wall4->AddComponent<Rigidbody>(100.f, 0.f, 0.f, false, false, 0, 1.5, 1.5, true);
	mpPhysicsManager->AddPhysicsObject(wall4->GetUID(), TYPE_GROUND);
}

void LevelManager::AddSquareAtPointer(const std::string& _originalTexture, const std::string& _changedTexture)
{
	auto indicator = mpEntityManager->CreateEntity("selectedSquare");
	indicator->AddComponent<Transform>(Vector3(0.f, 0.1f, 0.f), Vector3(0.f), /*Vector3(1.f)*/Vector3(0.5f, 1.f, 0.5f));
	indicator->AddComponent<MeshRenderer>("", "MySquare");
	// 	indicator->AddComponent<Texture3D>("White.png");
	indicator->AddComponent<OutlineComponent>(1.1, Vector3(0, 0, 1));
	indicator->AddComponent<AlphaBlendComponent>(0.4);

	// 	indicator->AddComponent<MeshRenderer>("box.fbx", "Cube.001", false, false);
	indicator->AddComponent<Texture3D>(_originalTexture);
	indicator->AddComponent<IndicatorComponent>(_originalTexture, _changedTexture);
}

void LevelManager::AddPlaceIndicator()
{
	auto indicator = mpEntityManager->CreateEntity("placeIndicator");
	indicator->AddComponent<Transform>(Vector3(-12.f, 0.05f, 0.f), Vector3(0.f), /*Vector3(1.f)*/Vector3(2.5f, 1.f, 9.5f));
	indicator->AddComponent<MeshRenderer>("", "MySquare");
	// 	indicator->AddComponent<Texture3D>("White.png");
	indicator->AddComponent<OutlineComponent>(1.05, Vector3(0, 0, 1));
	indicator->AddComponent<AlphaBlendComponent>(0.4);

	// 	indicator->AddComponent<MeshRenderer>("box.fbx", "Cube.001", false, false);
	indicator->AddComponent<Texture3D>("blue.png");
	indicator->AddComponent<IndicatorComponent>("blue.png", "blue.png");
}

void LevelManager::DisablePlaceIndicator()
{
	auto view = mRegistry.view<IndicatorComponent, OutlineComponent>();
	for (auto& entity : view)
	{
		auto& name = mRegistry.get<Name>(entity).m_name;
		if (name == "placeIndicator")
		{
			mpEntityManager->RemoveEntity(static_cast<UID>(entity));
		}
	}
}

void LevelManager::AddLight()
{
	auto lightEntity = mpEntityManager->CreateEntity("LightGroup");
	auto& lightComp = lightEntity->AddComponent<LightComponent>();

	mpRenderManager->LightInitialize(&lightComp.m_commonConstData, 1);
	mpRenderManager->SetDirLight(&lightComp.m_commonConstData, 0, 1.0f, Vector3(0.488983, -0.718127, 0.495166));
	mpRenderManager->PrintLightInfo(&lightComp.m_commonConstData);

	Light templight = lightComp.m_commonConstData.light[0];
	templight.position = Vector3(25.47, 85.77, 1.56);
	templight.direction = Vector3(-0.4, -0.92, 0.01);
	std::shared_ptr<Entity> tempEntity = mpEntityManager->CreateEntity("Camera");
	auto& cameraComp = tempEntity->AddComponent<CameraComponent>(1280, 1280
		, 10.f, Vector3(-20.f, 30.f, -20.f), Vector3(1.f, -1.f, 1.f), Vector3(0.f, 1.f, 0.f), Vector3(70.f, 10.f, 900.f),
		static_cast<unsigned int>(cameraEnum::LightCamera));
	cameraComp.setLightCameraIndex(0);	//0번째 라이트기 때문
	cameraComp.setViewDir(templight.direction);	//0번째 라이트기 때문
	cameraComp.setEyePos(templight.position);	//0번째 라이트기 때문
}

void LevelManager::AddCubeMap(uint8_t _mapNum)
{
	std::shared_ptr<Entity> tempCubeMap = mpEntityManager->CreateEntity("CubeMap");
	auto& trsComp = tempCubeMap->AddComponent<Transform>();

	switch (_mapNum)
	{
	case 1:
	case 2:
		tempCubeMap->AddComponent<CubeMapComponent>("skybox_diffuse.dds", "Blue_specular.dds");
		break;
	case 3:
	case 4:
	case 5:
		tempCubeMap->AddComponent<CubeMapComponent>("skybox_diffuse.dds", "Red_specular.dds");
		break;
	case 6:
	case 7:
	case 8:
		tempCubeMap->AddComponent<CubeMapComponent>("skybox_diffuse.dds", "Ocean_specular.dds");
		break;
	default:
		tempCubeMap->AddComponent<CubeMapComponent>("skybox_diffuse.dds", "Blue_specular.dds");
		break;
	}
}

void LevelManager::AddMapData(const int& _sceneNum)
{
	for (const auto& mapData : mMapDataVec)
	{
		//if (mCurrentSceneUID == UID(mapData.sceneNum))
		if (mapData.sceneNum == _sceneNum)
		{
			std::shared_ptr<Entity> map = mpEntityManager->CreateEntity("map");
			map->AddComponent<Transform>(Vector3(mapData.posX, mapData.posY, mapData.posZ),
				Vector3(DirectX::XMConvertToRadians(mapData.rotX), DirectX::XMConvertToRadians(mapData.rotY), 0.0f),
				Vector3(mapData.scaleX, mapData.scaleY, mapData.scaleZ));
			map->AddComponent<MeshRenderer>(mapData.FBXname, mapData.MeshName, false, false);
			map->AddComponent<Texture3D>(mapData.textureName);

			if (mapData.hasCollider)
			{
				map->AddComponent<BoxCollider>().m_size
					= mpRenderManager->Get_AABB(mapData.FBXname).mMax
					- mpRenderManager->Get_AABB(mapData.FBXname).mMin;
				map->GetComponent<BoxCollider>().m_size.y = 10.f;
				map->AddComponent<Rigidbody>().m_isStatic = true;
				mpPhysicsManager->AddPhysicsObject(map->GetUID(), TYPE_OBSTACLE);
			}
			if (mapData.alphaBlend)
			{
				map->AddComponent<AlphaBlendComponent>();
			}
			if (mapData.textureFlow)
			{
				map->AddComponent<FlowTextureComponent>(0, 0.1f);
			}
		}
	}
}

void LevelManager::AddBasicUI()
{
	// 세팅 버튼
	auto uiEntity = mpEntityManager->CreateEntity("UI_C_OutSetting");
	mpUIManager->AddUI(uiEntity, "UI_btn_Option.png", Vector2(25.f, 25.f), Vector2(50.f, 50.f), static_cast<int>(Layer::BG_B), Vector4(0.8f, 0.8f, 0.8f, 1.0f));
	mpUIManager->AddButtonAllColor(uiEntity, Vector4(0.0f, 0.0f, 0.0f, 1.0f), Vector4(1.0f));

	// 세팅 배경
	auto uiEntity0 = mpEntityManager->CreateEntity("UI_C_Setting");
	mpUIManager->AddUI(uiEntity0, "White.png", Vector2(0.f, 0.f), Vector2(1920.f, 1080.f), static_cast<int>(Layer::SET), Vector4(0.0f, 0.0f, 0.0f, 0.5f), false);

	// 판넬
	auto uiEntity1 = mpEntityManager->CreateEntity("UI_C_Setting");
	mpUIManager->AddUI(uiEntity1, "UI_pnl_OptionPanel.png", Vector2(720.f, 365.f), Vector2(480.f, 350.f), static_cast<int>(Layer::SET_P), Vector4(0.8f, 0.8f, 0.8f, 1.0f), false);

	// 제목
	/*auto uiEntity1 = mpEntityManager->CreateEntity("UI_C_Setting");
	mpUIManager->AddUI(uiEntity1, "UI_img_SettingBG.png", Vector2(690.f, 150.f), Vector2(600.f, 100.f), 1, Vector4(0.8f, 0.8f, 0.8f, 1.0f), false);
	mpUIManager->AddText(uiEntity1, u8"설정", "KIMM_Bold(60).ttf", Vector2(950, 160), 1, false, Vector4(1.0f));*/

	// 배경음 ---
	// 왼쪽 버튼
	auto uiEntity2 = mpEntityManager->CreateEntity("UI_C_SettingBG");
	mpUIManager->AddUI(uiEntity2, "UI_btn_VolumeDown.png", Vector2(981.f, 464.f), Vector2(30.f, 30.f), static_cast<int>(Layer::SET_B), Vector4(0.8f, 0.8f, 0.8f, 1.0f), false);
	mpUIManager->AddButtonAllColor(uiEntity2, Vector4(0.5f, 0.5f, 0.5f, 1.0f), Vector4(1.0f));

	// 오른쪽 버튼
	auto uiEntity3 = mpEntityManager->CreateEntity("UI_C_SettingBG");
	mpUIManager->AddUI(uiEntity3, "UI_btn_VolumeUp.png", Vector2(1108.f, 464.f), Vector2(30.f, 30.f), static_cast<int>(Layer::SET_B), Vector4(0.8f, 0.8f, 0.8f, 1.0f), false);
	mpUIManager->AddButtonAllColor(uiEntity3, Vector4(0.5f, 0.5f, 0.5f, 1.0f), Vector4(1.0f));
	mpUIManager->AddTextwithInt(uiEntity3, u8"%d", "KIMM_Bold(60).ttf", Vector2(1050, 469), 5, static_cast<int>(Layer::SET_B), false, Vector4(1.0f));
	uiEntity3->GetComponent<Text>().m_scale = 0.4f;

	// 효과음 --
	// 왼쪽 버튼
	auto uiEntity4 = mpEntityManager->CreateEntity("UI_C_SettingES");
	mpUIManager->AddUI(uiEntity4, "UI_btn_VolumeDown.png", Vector2(981.f, 565.f), Vector2(30.f, 30.f), static_cast<int>(Layer::SET_B), Vector4(0.8f, 0.8f, 0.8f, 1.0f), false);
	mpUIManager->AddButtonAllColor(uiEntity4, Vector4(0.5f, 0.5f, 0.5f, 1.0f), Vector4(1.0f));

	// 오른쪽 버튼
	auto uiEntity5 = mpEntityManager->CreateEntity("UI_C_SettingES");
	mpUIManager->AddUI(uiEntity5, "UI_btn_VolumeUp.png", Vector2(1108.f, 565.f), Vector2(30.f, 30.f), static_cast<int>(Layer::SET_B), Vector4(0.8f, 0.8f, 0.8f, 1.0f), false);
	mpUIManager->AddButtonAllColor(uiEntity5, Vector4(0.5f, 0.5f, 0.5f, 1.0f), Vector4(1.0f));
	mpUIManager->AddTextwithInt(uiEntity5, u8"%d", "KIMM_Bold(60).ttf", Vector2(1050, 570), 5, static_cast<int>(Layer::SET_B), false, Vector4(1.0f));
	uiEntity5->GetComponent<Text>().m_scale = 0.4f;

	// 설정 창 닫기 : 돌아가기
	auto uiEntity6 = mpEntityManager->CreateEntity("UI_C_SettingX");
	mpUIManager->AddUI(uiEntity6, "UI_btn_OptionQuit.png", Vector2(795.f, 651.f), Vector2(130.f, 40.f), static_cast<int>(Layer::SET_B), Vector4(0.8f, 0.8f, 0.8f, 1.0f), false);
	mpUIManager->AddButtonAllColor(uiEntity6, Vector4(0.5f, 0.5f, 0.5f, 1.0f), Vector4(1.0f));

	// 메인화면으로
	auto uiEntity7 = mpEntityManager->CreateEntity("UI_C_SettingGoMain");
	mpUIManager->AddUI(uiEntity7, "UI_btn_OptionToMain.png", Vector2(995.f, 651.f), Vector2(130.f, 40.f), static_cast<int>(Layer::SET_B), Vector4(0.8f, 0.8f, 0.8f, 1.0f), false);
	mpUIManager->AddButtonAllColor(uiEntity7, Vector4(0.5f, 0.5f, 0.5f, 1.0f), Vector4(1.0f));

	// 페이드 인 아웃
	auto ui5 = mpEntityManager->CreateEntity("FaidInOut");
	mpUIManager->AddUI(ui5, "Black.png", Vector2(-10, -10), Vector2(1940, 1120), static_cast<int>(Layer::Fade), Vector4(1.f, 1.0f, 1.0f, 0.0f), false);
	ui5->AddComponent<FadeInOut>(&ui5->GetComponent<Texture2D>(), 0.0f, FadeInOutState::NONE);

	mpEasingFunc = new EasingFunc();
}

void LevelManager::AddBattleUI(SceneData* _pSceneData)
{
	/// 패널들
	// 용병 클래스 패널
	auto uiPanel1 = mpEntityManager->CreateEntity("UI_D_Panel");
	mpUIManager->AddUI(uiPanel1, "UI_pnl_MercSelcPanel.png", Vector2(620, 949.f), Vector2(712, 112), static_cast<int>(Layer::COM_P), Vector4(1.0f, 1.0f, 1.0f, 0.7f), true);

	// 용병 수 패널
	//auto uiPanel2 = mpEntityManager->CreateEntity("UI_D_Panel");
	//mpUIManager->AddUI(uiPanel2, "UI_pnl_MercPanel.png", Vector2(1457, 965.f), Vector2(157, 61), static_cast<int>(Layer::COM_P), Vector4(1.0f), true);

	// 보유금
	auto uiPanel3 = mpEntityManager->CreateEntity("UI_B_Panel");
	mpUIManager->AddUI(uiPanel3, "UI_pnl_MoneyPanel.png", Vector2(200, 25.f), Vector2(266, 59), static_cast<int>(Layer::COM_P), Vector4(1.0f), true);

	// 스테이지 정보 패널
	auto uiPanel4 = mpEntityManager->CreateEntity("UI_B_Panel");
	mpUIManager->AddUI(uiPanel4, "UI_pnl_StageInfo.png", Vector2(1415, 24.f), Vector2(365, 91), static_cast<int>(Layer::COM_P), Vector4(1.0f), true);

	/// 이미지
	// 보유금 이미지
	auto uiEntity1 = mpEntityManager->CreateEntity("UI_B");
	mpUIManager->AddUI(uiEntity1, "UI_img_Money.png", Vector2(208, 32.f), Vector2(45, 45), static_cast<int>(Layer::COM_B), Vector4(1.0f), true);
	// 용병 아이콘 UI_MercenaryIcon + 용병 수
	auto uiAlive = mpEntityManager->CreateEntity("UI_D_AliveNum"); //UI_place
	mpUIManager->AddUI(uiAlive, "UI_pnl_MercPanel.png", Vector2(835.f, 13.f), Vector2(251.f, 92.f), static_cast<int>(Layer::COM_P), Vector4(1.0f), true);
	mpUIManager->AddTextwithInt(uiAlive, u8"0%d", "KIMM_Bold(60).ttf", Vector2(852.f, 28.f), 20, static_cast<int>(Layer::COM_B), true, Vector4(1.0f));
	uiAlive->GetComponent<Text>().m_scale = 1.0f;
	// 게임 시작 이펙트
	auto uiEffect = mpEntityManager->CreateEntity("UI_Effect_Start");
	mpUIManager->AddUI(uiEffect, "UI_img_StartEffect.png", Vector2(0, 0.f), Vector2(1920, 1733), static_cast<int>(Layer::EFFECT), Vector4(1.0f), false);
	uiEffect->AddComponent<FadeInOut>(&uiEffect->GetComponent<Texture2D>(), 0.0f, FadeInOutState::FADE_IN);


	/// 랭크바
	auto uiRank = mpEntityManager->CreateEntity("UI_B_Rank_Frame");
	mpUIManager->AddUI(uiRank, "UI_img_ScoreBorder.png", Vector2(193, 89.f), Vector2(268, 61), static_cast<int>(Layer::COM_M), Vector4(1.0f), false);
	//mpUIManager->AddRect(uiRank, Vector2(159, 91.f), Vector2(250.f, 16.f), static_cast<int>(Layer::COM_B), Vector4(1.0f));
	auto uiRank1 = mpEntityManager->CreateEntity("UI_B_Rank_Gauge");
	mpUIManager->AddUI(uiRank1, "UI_img_ScoreBG.png", Vector2(209, 91.f), Vector2(250, 16), static_cast<int>(Layer::COM_P), Vector4(1.0f), false);
	mpUIManager->AddMessageBox2D(uiRank1, "UI_img_ScoreBar.png", Vector2(209, 91.f), Vector2(250, 16), Vector4(0.0f, 1.0f, 0.0f, 1.0f)
		, u8"", Vector2(), 1.0f, "KIMM_Bold(60).ttf", Vector4(1.0f), static_cast<int>(Layer::COM_B), false);

	/// 버튼
	// 시작 버튼
	auto uiEntity2 = mpEntityManager->CreateEntity("UI_D_Start");
	mpUIManager->AddUI(uiEntity2, "UI_btn_StageStart.png", Vector2(310.f, 965.f), Vector2(180.f, 60.f), static_cast<int>(Layer::COM_B), Vector4(0.8f, 0.8f, 0.8f, 1.0f));
	mpUIManager->AddButtonAllColor(uiEntity2, Vector4(0.5f, 0.5f, 0.5f, 1.0f), Vector4(1.0f));

	// 배치 롤백 버튼
	auto uiReset = mpEntityManager->CreateEntity("UI_D_Reset");
	mpUIManager->AddUI(uiReset, "UI_btn_Rollback.png", Vector2(1620, 967), Vector2(58, 58), static_cast<int>(Layer::COM_B), Vector4(0.8f, 0.8f, 0.8f, 1.0f), true);
	mpUIManager->AddButtonAllColor(uiReset, Vector4(0.5f, 0.5f, 0.5f, 1.0f), Vector4(1.0f));

	/// UI TEXT
	// 타이머 ui 및 패널
	auto timerUI = mpEntityManager->CreateEntity("UI_P_Timer");
	mpUIManager->AddUI(timerUI, "UI_pnl_TimerPanel.png", Vector2(859, 13), Vector2(204, 74), static_cast<int>(Layer::COM_P), Vector4(1.0f, 1.0f, 1.0f, 1.0f), true);
	mpUIManager->AddText(timerUI, u8"01:30", "KIMM_Bold(60).ttf", Vector2(870, 19), static_cast<int>(Layer::COM_B), true, Vector4(1.0f));

	// 보유금
	auto uiText3 = mpEntityManager->CreateEntity("UI_B_Money");//UI_money
	mpUIManager->AddTextwithInt(uiText3, u8"%d", "KIMM_Bold(60).ttf", Vector2(270, 35)
		, /*dynamic_cast<SceneData*>(m_pSceneData)->m_heldMoney*/0, static_cast<int>(Layer::COM_B), true, Vector4(1.0f));
	uiText3->GetComponent<Text>().m_scale = 0.6f;
	// 배치 안내 문구
	auto guideText = mpEntityManager->CreateEntity("UI_D_Guide");
	mpUIManager->AddUI(guideText, "Frame.png", Vector2(0, 0.f), Vector2(1920, 1080), static_cast<int>(Layer::COM_P), Vector4(/*0.77f, 0.60f, 0.29f, */1.0f), false);
	mpUIManager->AddText(guideText, u8"용병 배치 모드", "KIMM_Bold(60).ttf", Vector2(730, 180), static_cast<int>(Layer::COM_B), false, Vector4(1.0f));

	/// 머니건 ui
	// 현재 머니건
	// auto ui_moneyGun1 = m_pEntityManager->CreateEntity("ui_moneyGun1");
	// m_pUIManager->AddUI(ui_moneyGun1, "green.png", Vector2(115, 940), Vector2(130, 130), 3, Vector4(0.8f, 0.8f, 0.8f, 1.0f), false);
	// 이전 머니건
	// auto ui_moneyGun2 = m_pEntityManager->CreateEntity("ui_moneyGun2");
	// m_pUIManager->AddUI(ui_moneyGun2, "green.png", Vector2(30, 990), Vector2(80, 80), 3, Vector4(0.8f, 0.8f, 0.8f, 1.0f), false);
	// m_pUIManager->AddButtonPressUI(ui_moneyGun2, "green.png");
	// // 이전 머니건 버튼
	// auto ui_moneyGun2_1 = m_pEntityManager->CreateEntity("ui_moneyGun2_1");
	// m_pUIManager->AddUI(ui_moneyGun2_1, "green.png", Vector2(55, 960), Vector2(30, 30), 3, Vector4(0.8f, 0.8f, 0.8f, 1.0f), false);
	// m_pUIManager->AddButtonPressUI(ui_moneyGun2_1, "green.png");
	// // 다음 머니건
	// auto ui_moneyGun3 = m_pEntityManager->CreateEntity("ui_moneyGun3");
	// m_pUIManager->AddUI(ui_moneyGun3, "green.png", Vector2(250, 990), Vector2(80, 80), 3, Vector4(0.8f, 0.8f, 0.8f, 1.0f), false);
	// m_pUIManager->AddButtonPressUI(ui_moneyGun3, "green.png");
	// // 다음 머니건 버튼
	// auto ui_moneyGun3_1 = m_pEntityManager->CreateEntity("ui_moneyGun3_1");
	// m_pUIManager->AddUI(ui_moneyGun3_1, "green.png", Vector2(275, 960), Vector2(30, 30), 3, Vector4(0.8f, 0.8f, 0.8f, 1.0f), false);
	// m_pUIManager->AddButtonPressUI(ui_moneyGun3_1, "green.png");

		/// 결과창 ui
	// 이미지
	// 하이라이트 UI_Res
	auto ui_res1 = mpEntityManager->CreateEntity("UI_Res1");
	mpUIManager->AddUI(ui_res1, "UI_img_Highlight.png", Vector2(0, 0), Vector2(1920, 1080), static_cast<int>(Layer::BG), Vector4(1.0f), false);
	// 결과 패널
	auto ui_res2 = mpEntityManager->CreateEntity("UI_Res2");
	mpUIManager->AddUI(ui_res2, "UI_pnl_ResultPanel.png", Vector2(1040, 128), Vector2(800, 825), static_cast<int>(Layer::COM_P), Vector4(1.0f, 1.0f, 1.0f, 1.0f), false);
	// 임무 결과 제목
// 	auto ui_res3 = mpEntityManager->CreateEntity("UI_Res3");
// 	mpUIManager->AddUI(ui_res3, "UI_img_ResultTitle.png", Vector2(1265, 200), Vector2(350, 100), static_cast<int>(Layer::COM_P), Vector4(1.0f), false);
	// 플레이어 평가 패널
	auto ui_res6 = mpEntityManager->CreateEntity("UI_Res6");
	mpUIManager->AddUI(ui_res6, "UI_pnl_RankPanel.png", Vector2(1476, 549), Vector2(237, 248), static_cast<int>(Layer::COM_P), Vector4(1.0f), false);
	// 평가 이미지
	auto ui_res6_1 = mpEntityManager->CreateEntity("UI_Res6_1");
	mpUIManager->AddUI(ui_res6_1, "UI_img_RankC.png", Vector2(1491, 573), Vector2(208, 219), static_cast<int>(Layer::COM_B), Vector4(1.0f), false);
	// 	// 사용 금액 아이콘
	// 	auto ui_res7_1 = mpEntityManager->CreateEntity("UI_Res7_1");
	// 	mpUIManager->AddUI(ui_res7_1, "UI_img_Money.png", Vector2(1363, 654), Vector2(45, 45), static_cast<int>(Layer::COM_B), Vector4(1.0f), false);
	// 	// 보수 금액 아이콘 -> 남은 금액 아이콘
	// 	auto ui_res8_1 = mpEntityManager->CreateEntity("UI_Res8_1");
	// 	mpUIManager->AddUI(ui_res8_1, "UI_img_Money.png", Vector2(1363, 755), Vector2(45, 45), static_cast<int>(Layer::COM_B), Vector4(1.0f), false);

	// 이미지와 텍스트
	// 생존 용병	-> 사용 안함
	// auto ui_res4 = mpEntityManager->CreateEntity("UI_Res4");
	// mpUIManager->AddUI(ui_res4, "UI_img_MercAlive.png", Vector2(1131, 365), Vector2(210, 60), static_cast<int>(Layer::COM_B), Vector4(1.0f), false);
	// mpUIManager->AddTextwithInt2(ui_res4, u8"%d/%d", "KIMM_Bold(60).ttf", Vector2(1395, 371)
	// 	, /*dynamic_cast<SceneData*>(m_pSceneData)->m_aliveAlly*/20, /*dynamic_cast<SceneData*>(m_pSceneData)->m_totalAlly*/20, static_cast<int>(Layer::COM_B), false, Vector4(1.0f));
	// ui_res4->GetComponent<Text>().m_scale = 1.2;
	// 남은 시간 -> 사용 안함
	// auto ui_res5 = mpEntityManager->CreateEntity("UI_Res5");
	// mpUIManager->AddUI(ui_res5, "UI_img_TimeLeft.png", Vector2(1131, 465), Vector2(210, 60), static_cast<int>(Layer::COM_B), Vector4(1.0f), false);
	// mpUIManager->AddTextwithInt(ui_res5, u8"0", "KIMM_Bold(60).ttf", Vector2(1395, 471), 0, static_cast<int>(Layer::COM_B), false, Vector4(1.0f));
	// ui_res5->GetComponent<Text>().m_scale = 1.2;
	// 사용 금액
	auto ui_res7 = mpEntityManager->CreateEntity("UI_Res7");
	//mpUIManager->AddUI(ui_res7, "UI_img_MoneyUsed.png", Vector2(1131, 646), Vector2(210, 60), static_cast<int>(Layer::COM_B), Vector4(1.0f), false);
	mpUIManager->AddText(ui_res7, u8"0", "KIMM_Bold(60).ttf", Vector2(1536, 364)
		, static_cast<int>(Layer::COM_B), false, Vector4(1.f));
	ui_res7->GetComponent<Text>().m_scale = 0.6;
	// 	ui_res7->AddComponent<EasingComponent>(mpEasingFunc);

	// 보수 금액 -> 남은 금액으로 수정
	auto ui_res8 = mpEntityManager->CreateEntity("UI_Res8");
	//mpUIManager->AddUI(ui_res8, "UI_img_MoneyEarn.png", Vector2(1131, 746), Vector2(210, 60), static_cast<int>(Layer::COM_B), Vector4(1.0f), false);
	mpUIManager->AddText(ui_res8, u8"0", "KIMM_Bold(60).ttf", Vector2(1536, 444)
		, static_cast<int>(Layer::COM_B), false, Vector4(1.f));
	ui_res8->GetComponent<Text>().m_scale = 0.6;

	// 초기 금액
	auto ui_res10 = mpEntityManager->CreateEntity("UI_Res10");
	mpUIManager->AddText(ui_res10, u8"0", "KIMM_Bold(60).ttf", Vector2(1536, 284)
		, static_cast<int>(Layer::COM_B), false, Vector4(1.f));
	ui_res10->GetComponent<Text>().m_scale = 0.6;

	// 버튼
	// 다음으로
	auto ui_res9 = mpEntityManager->CreateEntity("UI_Res9");
	mpUIManager->AddUI(ui_res9, "UI_btn_StageToNext.png", Vector2(1180, 836), Vector2(210, 60), static_cast<int>(Layer::COM_B), Vector4(1.0f), false);
	mpUIManager->AddButtonAllColor(ui_res9, Vector4(0.5f, 0.5f, 0.5f, 1.0f), Vector4(1.0f));

	// 메인으로
	auto ui_res11 = mpEntityManager->CreateEntity("UI_Res11");
	mpUIManager->AddUI(ui_res11, "UI_btn_StageToTitle.png", Vector2(1490, 836), Vector2(210, 60), static_cast<int>(Layer::COM_B), Vector4(1.0f), false);
	mpUIManager->AddButtonAllColor(ui_res11, Vector4(0.5f, 0.5f, 0.5f, 1.0f), Vector4(1.0f));
}

void LevelManager::AddStageNumAndObjects(const std::u8string& _stageNum)
{
	// 스테이지 명칭
	auto uiText = mpEntityManager->CreateEntity("UI_B_Stage");
	mpUIManager->AddText(uiText, _stageNum, "KIMM_Bold(60).ttf", Vector2(1425, 34), static_cast<int>(Layer::COM_B), true, Vector4(1.0f));
	uiText->GetComponent<Text>().m_scale = 0.7f;
	// 스테이지 목표 : 적군 숫자 표시 안내 문구
	auto uiText2 = mpEntityManager->CreateEntity("UI_B_StageGuide");
	mpUIManager->AddTextwithInt2(uiText2, u8"모든 적 처치 (%d/%d)", "KIMM_Bold(60).ttf", Vector2(1458, 78), 20, 20, static_cast<int>(Layer::COM_B), true, Vector4(1.0f));
	uiText2->GetComponent<Text>().m_scale = 0.4f;
}

void LevelManager::AddClassUI(const int& _activeClassNum, bool _isToturial)
{
	auto statusTable = mpResourceManager->ParseCSV<StatusTable>("Status.csv");
	for (int i = 0; i < 7; i++)
	{
		auto uiEntity = mpEntityManager->CreateEntity("UI_D_Class");
		std::string textureName = "UI_btn_Merc0" + std::to_string(i + 1) + ".png";
		mpUIManager->AddUI(uiEntity, textureName, Vector2(636.f + i * 100.f, 964.f), Vector2(80.f, 80.f)
			, static_cast<int>(Layer::COM_B), Vector4(0.8f, 0.8f, 0.8f, 1.0f));

		if (i < _activeClassNum)
		{
			mpUIManager->AddButtonAllColor(uiEntity, Vector4(0.5f, 0.5f, 0.5f, 1.0f), Vector4(1.0f));
			if (_isToturial == false)
			{
				std::string msgImgName = "UI_pnl_MercInfo" + std::to_string(i + 1) + ".png";
				mpUIManager->AddMessageBox2D(uiEntity, msgImgName, Vector2(578.f + i * 100.f, 734.f), Vector2(200.f, 200.f), Vector4(1.0f)
					, u8"", Vector2(), 1.0f, "KIMM_Bold(60).ttf", Vector4(1.0f), static_cast<int>(Layer::COM_M), false);
				mpUIManager->AddTextwithInt(uiEntity, u8"%d", "KIMM_Bold(60).ttf", Vector2(565.f + (i + 1) * 100.f, 790.f)
					, statusTable[i].placeMoney, static_cast<int>(Layer::COM_M), false, Vector4(1.0f));
				uiEntity->GetComponent<Text>().m_scale = 0.35f;
			}
		}
		else
		{
			uiEntity->GetComponent<Texture2D>().m_rgba = Vector4(1.0f, 0.5f, 0.5f, 1.0f);
			mpUIManager->AddButtonAllColor(uiEntity, Vector4(1.0f, 0.0f, 0.0f, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f));
			if (_isToturial == false)
			{
				mpUIManager->AddMessageBox2D(uiEntity, "UI_D_Lock.png", Vector2(636.f + i * 100.f, 964.f), Vector2(80.f, 80.f), Vector4(1.0f)
					, u8"", Vector2(), 1.0f, "KIMM_Bold(60).ttf", Vector4(1.0f), static_cast<int>(Layer::COM_M), false);
				// 				mpUIManager->AddTextwithInt(uiEntity, u8"%d", "KIMM_Bold(60).ttf", Vector2(636.f + i * 172.f, 964.f + 47.f)
				// 					, statusTable[i].placeMoney, static_cast<int>(Layer::COM_M), false, Vector4(1.0f));
				// 				uiEntity->GetComponent<Text>().m_scale = 0.25f;
			}
		}
	}
}

void LevelManager::AddVerifyPopUpUI()
{
	// 팝업 배경
	auto uiPopUp0 = mpEntityManager->CreateEntity("UI_Verify");
	mpUIManager->AddUI(uiPopUp0, "White.png", Vector2(0.f, 0.f), Vector2(1920.f, 1080.f), static_cast<int>(Layer::POP), Vector4(0.0f, 0.0f, 0.0f, 0.5f), false);

	// 가장 뒤의 패널
	auto uiPopUp = mpEntityManager->CreateEntity("UI_Verify_Main");
	mpUIManager->AddUI(uiPopUp, "UI_pnl_PopupPanel.png", Vector2(720.f, 427.f), Vector2(480.f, 225.f), static_cast<int>(Layer::POP_P), Vector4(0.8f, 0.8f, 0.8f, 1.0f), false);
	mpUIManager->AddTextwithInt2(uiPopUp, u8"정말로 게임을 종료하실 건가요?", "KIMM_Bold(60).ttf", Vector2(763.f, 470.f), 0, 0, static_cast<int>(Layer::POP_P), false, Vector4(1.0f));
	uiPopUp->GetComponent<Text>().m_scale = 0.33f;

	// Yes 버튼
	auto uiPopUp2 = mpEntityManager->CreateEntity("UI_Verify_Yes");
	mpUIManager->AddUI(uiPopUp2, "UI_btn_BtnYes.png", Vector2(795.f, 582.f), Vector2(130.f, 40.f), static_cast<int>(Layer::POP_B), Vector4(0.8f, 0.8f, 0.8f, 1.0f), false);
	//mpUIManager->AddText(uiPopUp2, u8"예", "KIMM_Bold(60).ttf", Vector2(780, 480), static_cast<int>(Layer::POP_B), false, Vector4(1.0f));
	mpUIManager->AddButtonAllColor(uiPopUp2, Vector4(0.5f, 0.5f, 0.5f, 1.0f), Vector4(1.0f));
	//uiPopUp2->GetComponent<Text>().m_scale = 0.67f;

	// No 버튼
	auto uiPopUp3 = mpEntityManager->CreateEntity("UI_Verify_No");
	mpUIManager->AddUI(uiPopUp3, "UI_btn_BtnNo.png", Vector2(995.f, 582.f), Vector2(130.f, 40.f), static_cast<int>(Layer::POP_B), Vector4(0.8f, 0.8f, 0.8f, 1.0f), false);
	//mpUIManager->AddText(uiPopUp3, u8"아니요", "KIMM_Bold(60).ttf", Vector2(1020, 480), static_cast<int>(Layer::POP_B), false, Vector4(1.0f));
	mpUIManager->AddButtonAllColor(uiPopUp3, Vector4(0.5f, 0.5f, 0.5f, 1.0f), Vector4(1.0f));
	//uiPopUp3->GetComponent<Text>().m_scale = 0.67f;
}

void LevelManager::AddAnimationUI(bool _isBattleScene, int _priviusSceneNum)
{
	///반드시 앞배경 후 넣을 것
	// 뒷배경
	auto ui0 = mpEntityManager->CreateEntity("UI_FIX");
	std::string imgBG = "UI_img_BG.png";
	if (9 < _priviusSceneNum && _priviusSceneNum <= 12)
	{
		imgBG = "UI_img_BG2.png";
	}
	else if (12 < _priviusSceneNum && _priviusSceneNum <= 15)
	{
		imgBG = "UI_img_BG3.png";
	}
	mpUIManager->AddUI(ui0, imgBG, Vector2(0, 0), Vector2(1920, 1080), static_cast<int>(Layer::BG), Vector4(1.f));

	// 구름
	auto ui1 = mpEntityManager->CreateEntity("UI_A_CloudR");
	mpUIManager->AddUI(ui1, "UI_img_CloudRight.png", Vector2(-509, -2983), Vector2(2429, 2983), static_cast<int>(Layer::EFFECT), Vector4(1.f));

	// 구름L
	auto ui2 = mpEntityManager->CreateEntity("UI_A_CloudL");
	mpUIManager->AddUI(ui2, "UI_img_CloudLeft.png", Vector2(0, -2983), Vector2(2500, 2983), static_cast<int>(Layer::EFFECT), Vector4(1.f));

	if (_isBattleScene == true)
	{
		ui0->GetComponent<Texture2D>().m_isVisible = false;
		ui1->GetComponent<Texture2D>().m_position.y = -726.f;
		ui2->GetComponent<Texture2D>().m_position.y = -726.f;
	}

	if (mIsRestart == true)
	{
		ui1->GetComponent<Texture2D>().m_position.x = 1920.f;
		ui2->GetComponent<Texture2D>().m_position.x = -2500.f;
	}
}

void LevelManager::BasicUIUpdate()
{
	if (mIsGoMain == true)
	{
		mIsGoMain == false;
	}

	auto uiButtonView = mRegistry.view<Button>();
	for (auto& uiEntity : uiButtonView)
	{
		auto& name = mRegistry.get<Name>(uiEntity).m_name;
		auto& button = mRegistry.get<Button>(uiEntity);
		//		팝업창 On?  ___________
		//			|				   \
		//			|				    \
		// 팝업창 제외 버튼 비활성화    세팅창 on?________
		//							/					\					
		//						   /					 \
		//					세팅창 제외 버튼 비활성화		모든 버튼 활성화

		if (currentPopUpState == PopUpState::NORMAL)
		{
			// 세팅창 on?
			if (currentSettingState == SettingState::NORMAL)
			{
				if (name.find("UI_C_Set") == std::string::npos)
				{
					button.mIsEnable = true; // 모든 버튼 활성화
				}
			}
			else // 세팅창 off
			{
				if (name.find("UI_C_Set") == std::string::npos)
				{
					button.mIsEnable = false; // 세팅창 버튼만 활성화
				}
				else
				{
					button.mIsEnable = true; // 세팅창 제외 버튼 비활성화	
				}
			}

		}
		else // 팝업창 off
		{
			if (name.find("UI_Ver") != std::string::npos)
			{
				button.mIsEnable = true;  // 팝업창 버튼만 활성화
			}
			else
			{
				button.mIsEnable = false; // 팝업창 제외 버튼 비활성화
			}
		}

		// 기본 설정 버튼을 누르면 설정창이 열린다.
		if (name == "UI_C_OutSetting")
		{
			if ((mpInputManager->GetKeyDown(KEY::ESCAPE) || ((mpUIManager->GetButtonState(button.m_pOwner) == ButtonState::PRESSED
				&& (mpInputManager->GetKeyUp(KEY::LBUTTON))) && currentSettingState == SettingState::NORMAL)))
			{
				// 				mpSoundManager->PlaySFX("Snd_sfx_ClickBtn");
				currentSettingState = SettingState::OPEN;
				break;
			}
		}
		// 돌아가기 버튼을 누르면 설정창이 닫힌다.
		if (name == "UI_C_SettingX")
		{
			if (currentSettingState == SettingState::ADJUST &&
				(mpUIManager->GetButtonState(button.m_pOwner) == ButtonState::PRESSED
					|| (mpInputManager->GetKeyDown(KEY::ESCAPE))))
			{
				// mpSoundManager->PlaySFX("Snd_sfx_ClickBtn");
				button.mIsEnable = false;
				currentSettingState = SettingState::CLOSED;
				break;
			}
		}
	}

	if (currentSettingState == SettingState::OPEN)
	{
		auto uiTextureView = mRegistry.view<Texture2D>();
		for (auto& uiEntity : uiTextureView)
		{
			auto& name = mRegistry.get<Name>(uiEntity).m_name;
			auto& texture = mRegistry.get<Texture2D>(uiEntity);
			auto text = mRegistry.try_get<Text>(uiEntity);
			auto button = mRegistry.try_get<Button>(uiEntity);
			if (name.find("UI_C_Set") != std::string::npos)
			{
				// 버튼은 한 번만 클릭된다.
				if (button)
				{
					button->mButtonState = ButtonState::NORMAL;
				}

				/// 씬 순서 바꾸면 무조건 수정!!! : 메인 메뉴일 때
				if (mCurrentSceneUID == static_cast<uint32_t>(SceneName::MAIN))
				{
					// 메인으로 가기를 띄우지 않는다.
					if (name == "UI_C_SettingGoMain")
					{
						continue;
					}
					// 돌아가기의 위치를 조정한다.
					if (name == "UI_C_SettingX" && button)
					{
						texture.m_position.x = 895.f;
						button->mUIPosition.x = 895.f;
						button->mUIHoveringPosition.x = 895.f;
						button->mUIPressedPosition.x = 895.f;
					}
				}
				else if (name == "UI_C_SettingX" && button)
				{
					texture.m_position.x = 795.f;
					button->mUIPosition.x = 795.f;
					button->mUIHoveringPosition.x = 795.f;
					button->mUIPressedPosition.x = 795.f;
				}
				// 그 외는 setting 창에 관련된 걸 전부 불러와서 시각화 한다.
				texture.m_isVisible = true;
				if (text)
				{
					text->m_isVisible = true;
				}
			}
		}
		currentSettingState = SettingState::ADJUST;
	}

	if (currentSettingState == SettingState::CLOSED)
	{
		auto uiTextureView = mRegistry.view<Texture2D>();
		for (auto& uiEntity : uiTextureView)
		{
			auto& name = mRegistry.get<Name>(uiEntity).m_name;
			auto& texture = mRegistry.get<Texture2D>(uiEntity);
			auto text = mRegistry.try_get<Text>(uiEntity);
			auto button = mRegistry.try_get<Button>(uiEntity);

			if (name.find("UI_C_Set") != std::string::npos)
			{
				// 버튼은 한 번만 클릭된다.
				if (button)
				{
					if (button->mButtonState == ButtonState::PRESSED)
					{
						if (!mpSoundManager->IsPlaying("Snd_sfx_ClickBtn"))
						{
							mpSoundManager->PlaySFX("Snd_sfx_ClickBtn");
						}
					}
					button->mButtonState = ButtonState::NORMAL;
					button->mIsEnable = false;
				}

				texture.m_isVisible = false;
				if (text)
				{
					text->m_isVisible = false;
				}
			}
		}
		currentSettingState = SettingState::NORMAL;
	}

	if (currentSettingState == SettingState::ADJUST)
	{
		auto uiButtonView = mRegistry.view<Button>();
		for (auto& uiEntity : uiButtonView)
		{
			auto& name = mRegistry.get<Name>(uiEntity).m_name;
			auto& texture = mRegistry.get<Texture2D>(uiEntity);
			auto text = mRegistry.try_get<Text>(uiEntity);
			auto& button = mRegistry.get<Button>(uiEntity);
			if (name == "UI_C_SettingBG")
			{
				SetVolume(texture.m_file, texture.m_pOwner, text->m_num1, BGVolume);
			}

			if (name == "UI_C_SettingES")
			{
				SetVolume(texture.m_file, texture.m_pOwner, text->m_num1, ESVolume);
			}

			mpSoundManager->SetBGMVolume(static_cast<float>(BGVolume) * (1.f / 5.f));
			mpSoundManager->SetSFXVolume(static_cast<float>(ESVolume) * (1.f / 5.f));

			if (name == "UI_C_SettingGoMain")
			{
				if (mpUIManager->GetButtonState(texture.m_pOwner) == ButtonState::PRESSED)
				{
					//mIsGoMain = true;
// 					mpSoundManager->PlaySFX("Snd_sfx_ClickBtn");
					if (!mpSoundManager->IsPlaying("Snd_sfx_ClickBtn"))
					{
						mpSoundManager->PlaySFX("Snd_sfx_ClickBtn");
					}
					button.mButtonState = ButtonState::NORMAL;
					button.mIsEnable = false;
					currentPopUpState = PopUpState::OPEN_MAINMENU;
					break;
				}
			}

		}
	}

	for (auto& entity : uiButtonView)
	{
		auto& texture = mRegistry.get<Texture2D>(entity);
		auto& button = mRegistry.get<Button>(entity);
		if (mpUIManager->GetButtonState(mpEntityManager->GetEntity(entity)) == ButtonState::PRESSED)
		{
			if (!mpSoundManager->IsPlaying("Snd_sfx_ClickBtn"))
			{
				mpSoundManager->PlaySFX("Snd_sfx_ClickBtn");
			}
		}
	}
}

void LevelManager::PopUpUIUpdate(SceneData* _pSceneData)
{
	// 보이는 것과 안 보이는 것에 대한 처리 -> 모든 리소스를 선행 처리 후 PopUpState는 일괄 처리로 한 번씩만 변경되게 한다.
	ManagePopupDisplay(PopUpState::OPEN_MAINMENU, PopUpState::OPEN_UNITDEPLOY);

	if (currentPopUpState == PopUpState::OPEN_MAINMENU)
	{
		auto imgView = mRegistry.view<Texture2D>();
		for (auto& uiEntity : imgView)
		{
			auto& name = mRegistry.get<Name>(uiEntity).m_name;
			auto text = mRegistry.try_get<Text>(uiEntity);

			if (name == "UI_Verify_Main")
			{
				// 원하는 텍스트로 교체해준다.
				text->m_text = u8"메인 메뉴로 돌아갈까요?";
				//text->m_position = Vector2(730.f, 380.f);
				currentPopUpState = PopUpState::MAINMENU;
				break;
			}
		}

	}
	else if (currentPopUpState == PopUpState::OPEN_UNITDEPLOY)
	{
		auto imgView = mRegistry.view<Texture2D>();
		for (auto& uiEntity : imgView)
		{
			auto& name = mRegistry.get<Name>(uiEntity).m_name;
			auto& texture = mRegistry.get<Texture2D>(uiEntity);
			auto text = mRegistry.try_get<Text>(uiEntity);
			auto button = mRegistry.try_get<Button>(uiEntity);

			if (name == "UI_Verify_Yes")
			{
				if (_pSceneData->m_aliveAlly == 0)
				{
					texture.m_isVisible = false;
				}
				else
				{
					texture.m_isVisible = true;
				}
			}

			if (name == "UI_Verify_No")
			{
				if (_pSceneData->m_aliveAlly == 0)
				{
					texture.m_file = "UI_btn_BtnCheck.png";
					texture.m_position.x = 895.f;
				}
				else
				{
					texture.m_file = "UI_btn_BtnNo.png";
					texture.m_position.x = 995.f;
				}
				button->mUIPosition.x = texture.m_position.x;
				button->mUIHoveringPosition.x = texture.m_position.x;
				button->mUIPressedPosition.x = texture.m_position.x;
			}

			if (name == "UI_Verify_Main")
			{
				text->m_num1 = /*mpUnitSystem->GetPlayerSystem()->mMaxSetUnitNum -*/ _pSceneData->m_aliveAlly;
				text->m_num2 = _pSceneData->m_heldMoney;

				// 원하는 텍스트로 교체해준다.
				if (_pSceneData->m_aliveAlly == 0)
				{
					text->m_text = u8"\n용병이 없으면 싸울 수 없어요!";
				}
				else
				{
					text->m_text = u8"전투를 시작할까요?\n배치된 용병 : %d명\n사용 가능한 금액 : $ %d";
					//text->m_position = Vector2(730.f, 330.f);
				}
			}
		}
		currentPopUpState = PopUpState::UNITDEPLOY;
		return;
	}
	else if (currentPopUpState == PopUpState::CLOSED)
	{
		// NORMAL 상태로 바꾼다.
		currentPopUpState = PopUpState::NORMAL;
		//break;
	}
	else if (currentPopUpState == PopUpState::MAINMENU || currentPopUpState == PopUpState::UNITDEPLOY)
	{
		auto imgView = mRegistry.view<Texture2D>();
		for (auto& uiEntity : imgView)
		{
			auto& name = mRegistry.get<Name>(uiEntity).m_name;
			auto& texture = mRegistry.get<Texture2D>(uiEntity);
			auto button = mRegistry.try_get<Button>(uiEntity);

			// 아니오를 선택할 경우 팝업 창을 끈다.
			if (name == "UI_Verify_No")
			{
				if (mpUIManager->GetButtonState(texture.m_pOwner) == ButtonState::PRESSED)
				{
					// 					mpSoundManager->PlaySFX("Snd_sfx_ClickBtn");
					currentPopUpState = PopUpState::CLOSED;
					return;
				}
			}

			// Yes를 선택할 경우, 다른 걸 시작한다.
			if (name == "UI_Verify_Yes")
			{
				if (mpUIManager->GetButtonState(texture.m_pOwner) == ButtonState::PRESSED)
				{
					// 					mpSoundManager->PlaySFX("Snd_sfx_ClickBtn");
					switch (currentPopUpState)
					{
					case PopUpState::MAINMENU:
					{
						mIsGoMain = true;
					}
					break;
					case PopUpState::UNITDEPLOY:
					{
						currentLevelState = GameState::PRE_PLAY;
						button->mIsEnable = false;
						texture.m_isVisible = false;
						mIsGameStart = true;
						if (mIsClassButtonPressed == true)
						{
							mIsOneDeploySet = true;  // 배치 완료 후 상태 변경
							auto entity = mpEntityManager->GetEntity(mTempUnitUID);
							entity->GetComponent<PlayerComponent>().mIsDeploy = false;

							mpEntityManager->RemoveEntity(mTempUnitUID);
							mTempUnitUID = -1;
							mIsClassButtonPressed == false;
						}
						mpSoundManager->PlaySFX("Snd_sfx_BattleStart");
					}
					break;
					default:
						break;
					}
					currentPopUpState = PopUpState::CLOSED;
					return;
				}
			}
		}
	}

}

void LevelManager::PopUpUIMainUpdate()
{
	// 보이는 것과 안 보이는 것에 대한 처리 -> 모든 리소스를 선행 처리 후 PopUpState는 일괄 처리로 한 번씩만 변경되게 한다.
	ManagePopupDisplay(PopUpState::OPEN_EXITGAME);

	auto uiTextureView2 = mRegistry.view<Texture2D>();
	for (auto& uiEntity : uiTextureView2)
	{
		auto& name = mRegistry.get<Name>(uiEntity).m_name;
		auto& texture = mRegistry.get<Texture2D>(uiEntity);
		auto text = mRegistry.try_get<Text>(uiEntity);
		auto button = mRegistry.try_get<Button>(uiEntity);


		if (name == "UI_Verify_Main")
		{
			if (currentPopUpState == PopUpState::OPEN_EXITGAME)
			{
				text->m_text = u8"정말로 게임을 종료하실 건가요?";
				//text->m_position = Vector2(730.f, 380.f);
				currentPopUpState = PopUpState::EXITGAME;
				break;
			}
		}

		if (currentPopUpState == PopUpState::CLOSED) // close를 일부러 가장 위에 둠
		{
			// NORMAL 상태로 바꾼다.
			currentPopUpState = PopUpState::NORMAL;
			break;
		}
		else if (currentPopUpState == PopUpState::EXITGAME)
		{
			// 아니오를 선택할 경우 팝업 창을 끈다.
			if (name == "UI_Verify_No")
			{
				if (mpUIManager->GetButtonState(texture.m_pOwner) == ButtonState::PRESSED)
				{
					// 					mpSoundManager->PlaySFX("Snd_sfx_ClickBtn");
					currentPopUpState = PopUpState::CLOSED;
					break;
				}
			}

			// Yes를 선택할 경우, 다른 걸 시작한다.
			if (name == "UI_Verify_Yes")
			{
				if (mpUIManager->GetButtonState(texture.m_pOwner) == ButtonState::PRESSED)
				{
					// 					mpSoundManager->PlaySFX("Snd_sfx_ClickBtn");
					if (currentPopUpState == PopUpState::EXITGAME)
					{
						mIsExit = true;
						currentPopUpState = PopUpState::CLOSED;
						break;
					}
				}
			}
		}
	}
}


void LevelManager::PopUPUITutorial()
{
	// 보이는 것과 안 보이는 것에 대한 처리 -> 모든 리소스를 선행 처리 후 PopUpState는 일괄 처리로 한 번씩만 변경되게 한다.
	ManagePopupDisplay(PopUpState::OPEN_MAINMENU, PopUpState::OPEN_SKIP);

	if (currentPopUpState == PopUpState::OPEN_MAINMENU)
	{
		auto imgView = mRegistry.view<Texture2D>();
		for (auto& uiEntity : imgView)
		{
			auto& name = mRegistry.get<Name>(uiEntity).m_name;
			auto text = mRegistry.try_get<Text>(uiEntity);

			if (name == "UI_Verify_Main")
			{
				// 원하는 텍스트로 교체해준다.
				text->m_text = u8"메인 메뉴로 돌아갈까요?";
				//text->m_position = Vector2(730.f, 380.f);
				currentPopUpState = PopUpState::MAINMENU;
				break;
			}
		}

	}
	else if (currentPopUpState == PopUpState::OPEN_SKIP)
	{
		auto imgView = mRegistry.view<Texture2D>();
		for (auto& uiEntity : imgView)
		{
			auto& name = mRegistry.get<Name>(uiEntity).m_name;
			auto text = mRegistry.try_get<Text>(uiEntity);

			if (name == "UI_Verify_Main")
			{
				// 원하는 텍스트로 교체해준다.
				text->m_text = u8"튜토리얼을 건너뛸까요?";
				//text->m_position = Vector2(730.f, 330.f);
				currentPopUpState = PopUpState::SKIP;
				break;
			}
		}
	}
	else if (currentPopUpState == PopUpState::CLOSED)
	{
		// NORMAL 상태로 바꾼다.
		currentPopUpState = PopUpState::NORMAL;
		//break;
	}
	else if (currentPopUpState == PopUpState::MAINMENU || currentPopUpState == PopUpState::SKIP)
	{
		auto imgView = mRegistry.view<Texture2D>();
		for (auto& uiEntity : imgView)
		{
			auto& name = mRegistry.get<Name>(uiEntity).m_name;
			auto& texture = mRegistry.get<Texture2D>(uiEntity);
			auto button = mRegistry.try_get<Button>(uiEntity);

			// 아니오를 선택할 경우 팝업 창을 끈다.
			if (name == "UI_Verify_No")
			{
				if (mpUIManager->GetButtonState(texture.m_pOwner) == ButtonState::PRESSED)
				{
					// 					mpSoundManager->PlaySFX("Snd_sfx_ClickBtn");
					currentPopUpState = PopUpState::CLOSED;
					break;
				}
			}

			// Yes를 선택할 경우, 다른 걸 시작한다.
			if (name == "UI_Verify_Yes")
			{
				if (mpUIManager->GetButtonState(texture.m_pOwner) == ButtonState::PRESSED)
				{
					// 					mpSoundManager->PlaySFX("Snd_sfx_ClickBtn");
					switch (currentPopUpState)
					{
					case PopUpState::MAINMENU:
						mIsGoMain = true;
						break;
					case PopUpState::SKIP:
						mIsSkip = true;
						break;
					default:
						break;
					}
					currentPopUpState = PopUpState::CLOSED;
					return;
				}
			}
		}
	}
}

void LevelManager::UIAniUpdate(float _dTime)
{
	switch (currentUIAniState)
	{
	case UIAnimationState::POSTGAMESTART:
	{
		mUIAnimationTime += _dTime;

		// FIX와 모든 UI_A를 제외하고 그냥 싹 다 내려버린다.
		for (const auto& e : mEasingVec)
		{
			mpEasingFunc->EasingFuncMap["easeOutSine"](e.first, e.first + 1080, mUIAnimationTime, e.second);
		}

		auto view = mRegistry.view<Texture2D>();
		for (auto& entity : view)
		{
			auto button = mRegistry.try_get<Button>(entity);
			auto& name = mRegistry.get<Name>(entity).m_name;
			auto& texture = mRegistry.get<Texture2D>(entity);
			auto pEntity = mpEntityManager->GetEntity(entity);

			if (button)
			{
				if (mpUIManager->GetButtonState(pEntity) == ButtonState::NORMAL)
				{
					button->mUIPosition = texture.m_position;
				}
			}

			if (name == "UI_Main_FrontBG" && texture.m_position.y == 1080)
			{
				currentUIAniState = UIAnimationState::NEXTSCENE;
				mEasingVec.clear();
				mUIAnimationTime = 0.0f;
				return;
			}
		}
	}
	break;
	/*case UIAnimationState::NORMAL: // 씬마다 코딩
		break;*/
	case UIAnimationState::PRELOADING:
		break;
	case UIAnimationState::POSTLOATING:
		break;
	case UIAnimationState::PREBATTLE:
	{
		if (mIsRestart == true)
		{
			currentUIAniState = UIAnimationState::NORMAL;// UI 애니메이션 상태 설정
			StartNoneUI(); // Fixed부터 업뎃되기 때문에 추가해준다.
			currentLevelState = GameState::START_ANIMATION;
			return;
		}
		else
		{
			mUIAnimationTime += _dTime;

			auto view = mRegistry.view<Texture2D>();
			for (auto& entity : view)
			{
				auto& name = mRegistry.get<Name>(entity).m_name;
				auto& texture = mRegistry.get<Texture2D>(entity);
				if (name == "UI_A_CloudR")
				{
					mpEasingFunc->EasingFuncMap["easeInSine"](-509, 1920, mUIAnimationTime, &texture.m_position.x);
				}

				if (name == "UI_A_CloudL")
				{
					mpEasingFunc->EasingFuncMap["easeInSine"](0, -2500, mUIAnimationTime, &texture.m_position.x);

					if (texture.m_position.x == -2500)
					{
						currentUIAniState = UIAnimationState::NORMAL;// UI 애니메이션 상태 설정
						currentLevelState = GameState::START_ANIMATION;
						mUIAnimationTime = 0.0f;
						return;
					}
				}
			}
		}

	}
	break;
	case UIAnimationState::POSTBATTLE:
	{
		mUIAnimationTime += _dTime;

		auto view = mRegistry.view<Texture2D>();
		for (auto& entity : view)
		{
			auto& name = mRegistry.get<Name>(entity).m_name;
			auto& texture = mRegistry.get<Texture2D>(entity);
			if (name == "UI_A_CloudR")
			{
				mpEasingFunc->EasingFuncMap["easeInSine"](1920, -509, mUIAnimationTime, &texture.m_position.x);
			}

			if (name == "UI_A_CloudL")
			{
				mpEasingFunc->EasingFuncMap["easeInSine"](-2500, 0, mUIAnimationTime, &texture.m_position.x);

				if (texture.m_position.x == 0)
				{
					currentUIAniState = UIAnimationState::NEXTSCENE;// UI 애니메이션 상태 설정
					mUIAnimationTime = 0.0f;
					return;
				}
			}
		}
	}
	break;
	case UIAnimationState::PRETUTORIAL:
	{
		mUIAnimationTime += _dTime;

		auto view = mRegistry.view<Texture2D>();
		for (auto& entity : view)
		{
			auto& name = mRegistry.get<Name>(entity).m_name;
			auto& texture = mRegistry.get<Texture2D>(entity);
			if (name == "UI_A_CloudR")
			{
				mpEasingFunc->EasingFuncMap["easeInSine"](-509, 1920, mUIAnimationTime, &texture.m_position.x);
			}

			if (name == "UI_A_CloudL")
			{
				mpEasingFunc->EasingFuncMap["easeInSine"](0, -2500, mUIAnimationTime, &texture.m_position.x);

				if (texture.m_position.x == -2500)
				{
					currentUIAniState = UIAnimationState::NORMAL;// UI 애니메이션 상태 설정
					mUIAnimationTime = 0.0f;
					return;
				}
			}
		}
	}
	break;
	case UIAnimationState::POSTTUTORIAL:
	{

	}
	break;
	/*case UIAnimationState::VISIBLE: // 씬마다 코딩
	break;
	case UIAnimationState::UNVISIBLE: // 씬마다 코딩
		break;
	case UIAnimationState::NEXTSCENE: // 씬마다 코딩
		break;
	case UIAnimationState::LAST:
		break;*/
	default:
		break;
	}
}

void LevelManager::PushAllUI()
{
	auto view = mRegistry.view<Texture2D>();
	for (auto& entity : view)
	{
		auto& name = mRegistry.get<Name>(entity).m_name;
		auto& texture = mRegistry.get<Texture2D>(entity);

		if (name.find("UI_A") == std::string::npos && name != "UI_FIX" && texture.m_isVisible == true)
		{
			mEasingVec.emplace_back(texture.m_position.y, &texture.m_position.y);
		}
	}

	auto view2 = mRegistry.view<Text>();
	for (auto& entity : view2)
	{
		auto& name = mRegistry.get<Name>(entity).m_name;
		auto& text = mRegistry.get<Text>(entity);
		if (name.find("UI_A") == std::string::npos && name != "UI_FIX" && text.m_isVisible == true)
		{
			mEasingVec.emplace_back(text.m_position.y, &text.m_position.y);
		}
	}
	mUIAnimationTime = 0.0f;
}

void LevelManager::StartNoneUI()
{
	// 모든 텍스트 가리기
	auto textView = mRegistry.view<Text>();
	for (auto entity : textView)
	{
		auto& text = mRegistry.get<Text>(entity);
		text.m_isVisible = false;
	}

	// 모든 텍스처 가리기
	auto resView = mRegistry.view<Texture2D>();
	for (auto entity : resView)
	{
		auto& img = mRegistry.get<Texture2D>(entity);
		auto& name = mRegistry.get<Name>(entity).m_name;
		if (mIsRestart == true)
		{
			if (img.m_layer <= static_cast<int>(Layer::Fade)) //페이드만 보이고 이펙트도 안 보이게
			{
				img.m_isVisible = true;
			}
			else
			{
				img.m_isVisible = false;
			}
		}
		else
		{
			if (img.m_layer <= static_cast<int>(Layer::EFFECT)) // 이펙트와 페이드만 보이게
			{
				img.m_isVisible = true;

				if (name == "UI_Effect_Start")
				{
					img.m_isVisible = false; // 게임 스타트 이펙트는 안 보이게
				}
			}
			else
			{
				img.m_isVisible = false;
			}
		}


	}

	// 애니메이션 진행중일때는 옵션 버튼 비활성화
	auto buttonView = mRegistry.view<Button>();
	for (auto& entity : buttonView)
	{
		auto& button = mRegistry.get<Button>(entity);
		auto& name = mRegistry.get<Name>(entity).m_name;
		if (name == "UI_C_OutSetting")
		{
			button.mIsEnable = false;
		}
	}
}

void LevelManager::PrePlacementUpdate(SceneData* _pSceneData)
{
	// 옵션 버튼 활성화
	auto buttonView = mRegistry.view<Button>();
	for (auto& entity : buttonView)
	{
		auto& button = mRegistry.get<Button>(entity);
		auto& name = mRegistry.get<Name>(entity).m_name;
		if (name == "UI_C_OutSetting")
		{
			button.mIsEnable = true;
		}
	}

	// 적군 최대 수 세기
	int numEnemy = 0;
	auto enemyView = mRegistry.view<EnemyComponent>();
	for (auto entity : enemyView)
	{
		numEnemy++;
	}

	/// 배치와 관련된 모든 텍스트와 텍스처 살리기
	// 배치와 관련된 텍스트 보이기
	auto textView = mRegistry.view<Text>();
	for (auto& uiEntity : textView)
	{
		auto& name = mRegistry.get<Name>(uiEntity).m_name;
		auto& text = mRegistry.get<Text>(uiEntity);

		if (name.find("UI_D") != std::string::npos || name.find("UI_B") != std::string::npos || name == "UI_C_OutSetting")
		{
			text.m_isVisible = true;

			if (name == "UI_D_Class")
			{
				text.m_isVisible = false;
			}

			if (name == "UI_B_StageGuide")
			{
				text.m_num1 = numEnemy;
				text.m_num2 = numEnemy;
			}

			if (name == "UI_B_Money")
			{
				text.m_num1 = dynamic_cast<SceneData*>(_pSceneData)->m_heldMoney;
			}
		}

	}

	// 배치와 관련된 텍스처 보이기
	auto textureView = mRegistry.view<Texture2D>();
	for (auto uiEntity : textureView)
	{
		auto& name = mRegistry.get<Name>(uiEntity).m_name;
		auto& img = mRegistry.get<Texture2D>(uiEntity);

		if (name.find("UI_D") != std::string::npos || name.find("UI_B") != std::string::npos || name == "UI_C_OutSetting")
		{
			img.m_isVisible = true;
		}
	}

	// 메세지창 부분 활성화 -> 다른 용도로 사용하는 것만 활성화
	auto msgView = mRegistry.view<MessageBox2D>();
	for (auto entity : msgView)
	{
		auto& name = mRegistry.get<Name>(entity).m_name;
		auto& msgBox = mRegistry.get<MessageBox2D>(entity);
		if (name.find("UI_B_Rank") != std::string::npos)
		{
			msgBox.mIsImgVisible = true;
		}
	}

	currentLevelState = GameState::PLACEMENT;
}

void LevelManager::PlacementUpdate(SceneData* _pSceneData, float _dTime)
{
	/// UI
	// UI 시작 버튼 - 누른 후 사라짐
	auto uiView = mRegistry.view<Button>();
	for (auto& uiEntity : uiView)
	{
		auto& name = mRegistry.get<Name>(uiEntity).m_name;
		auto& button = mRegistry.get<Button>(uiEntity);
		auto& texture = mRegistry.get<Texture2D>(uiEntity);

		if (name == "UI_D_Start")
		{
			if (mpUIManager->GetButtonState(button.m_pOwner) == ButtonState::PRESSED)
			{
				if (mpInputManager->GetKeyUp(KEY::LBUTTON))
				{
					currentPopUpState = PopUpState::OPEN_UNITDEPLOY;
					// 					mpSoundManager->PlaySFX("Snd_sfx_ClickBtn");
					return;
				}
			}
		}

		// UI 클래스 버튼
		if (name == "UI_D_Class")
		{
			auto& msg2D = mRegistry.get<MessageBox2D>(uiEntity);
			auto text = mRegistry.try_get<Text>(uiEntity);

			if (mpUnitSystem->GetPlayerSystem()->mMaxSetUnitNum > _pSceneData->m_aliveAlly)
			{
				//button.mIsEnable = true;

				if (mpUIManager->GetButtonState(button.m_pOwner) == ButtonState::PRESSED)
				{

					if (mpInputManager->GetKeyUp(KEY::LBUTTON))
					{
						// 이미 생성한 UID가 같으면 스킵!
						if (texture.m_file == "UI_btn_Merc01.png")
						{
							mClassName = "Mercenary";
							mIsClassButtonPressed = true;
							// 							mpSoundManager->PlaySFX("Snd_sfx_ClickBtn");
							return;
						}
						else if (texture.m_file == "UI_btn_Merc02.png")
						{
							mClassName = "Archer";
							mIsClassButtonPressed = true;
							// 							mpSoundManager->PlaySFX("Snd_sfx_ClickBtn");
							return;
						}
						else
						{

						}

						//mpSoundManager->PlaySFX("Snd_sfx_ClickBtn");
// 						mpSoundManager->PlaySFX("Snd_sfx_MercSelect1");
						//IsClassButtonPressed = true;
					}
				}


				if (mIsClassButtonPressed == true && mIsGameStart == false)
				{
					if (mTempUnitUID == -1)
					{
						// 유저가 배치하는 유닛을 생성한다.
						auto userPlaceUnit = mpEntityManager->CreateEntity("player");
						mTempUnitUID = userPlaceUnit->GetUID();
						mpUnitSystem->GetPlayerSystem()->AddDeplyUnit(userPlaceUnit, mClassName);
						userPlaceUnit->GetComponent<Transform>().m_localPosition = Vector3(-15.0f, 2, 0.0f);
						mpRenderManager->InitailizeEntity(userPlaceUnit);
						// 유저가 배치를 시작할 것임을 알린다.
						userPlaceUnit->GetComponent<PlayerComponent>().mIsDeploy = true;
					}
					else
					{
						mpUnitSystem->GetPlayerSystem()->ChangeUnit(mpEntityManager->GetEntity(mTempUnitUID), mClassName);
						mpEntityManager->GetEntity(mTempUnitUID)->GetComponent<Transform>().m_localPosition = Vector3(-15.0f, 2, 0.0f);
						mpRenderManager->InitailizeEntity(mpEntityManager->GetEntity(mTempUnitUID));
					}
				}
			}
			else
			{
				mIsClassButtonPressed = false; // 버튼이 눌린 상태 삭제
				texture.m_isVisible = true;
				button.mIsEnable = false;
				if (text)
				{
					text->m_isVisible = false;
				}
			}

			// 호버링 시 설명 문구가 뜨게 한다.
			if (mpUIManager->GetButtonState(button.m_pOwner) == ButtonState::HOVERED)
			{
				if (texture.m_file == "UI_btn_Merc01.png")
				{
					msg2D.mIsImgVisible = true;
					msg2D.mIsTextVisible = true;
					text->m_isVisible = true;
				}
				else if (texture.m_file == "UI_btn_Merc02.png")
				{
					msg2D.mIsImgVisible = true;
					msg2D.mIsTextVisible = true;
					text->m_isVisible = true;
				}
				else if (texture.m_file == "UI_btn_Merc03.png")
				{
					msg2D.mIsImgVisible = true;
				}
				else if (texture.m_file == "UI_btn_Merc04.png")
				{
					msg2D.mIsImgVisible = true;
				}
				else if (texture.m_file == "UI_btn_Merc05.png")
				{
					msg2D.mIsImgVisible = true;
				}
				else if (texture.m_file == "UI_btn_Merc06.png")
				{
					msg2D.mIsImgVisible = true;
				}
				else if (texture.m_file == "UI_btn_Merc07.png")
				{
					msg2D.mIsImgVisible = true;
				}
			}
			else
			{
				msg2D.mIsImgVisible = false;
				msg2D.mIsTextVisible = false;
				if (text)
				{
					text->m_isVisible = false;
				}
			}
		} //[End] : UI 클래스 버튼 관련 코드 종료
		// 배치 롤백 버튼
		if (name == "UI_D_Reset" && _pSceneData->m_aliveAlly != 0)
		{
			if (mpUIManager->GetButtonState(button.m_pOwner) == ButtonState::PRESSED)
			{
				if (mpInputManager->GetKeyUp(KEY::LBUTTON))
				{
					if (_pSceneData->m_aliveAlly != 0)
					{
						ResetPlayer(_pSceneData);
						mIsClassButtonPressed = false; // 버튼이 눌린 상태 삭제
						mTempUnitUID = -1;
						mpSoundManager->PlaySFX("Snd_sfx_RollbackAll");
					}
					else
					{
						mpSoundManager->PlaySFX("Snd_sfx_DeployX");
					}
				}
			}
		}
	}

	// 현재 좌표
	Vector3 cursorFollowPos = mpPhysicsManager->PickObejct("plane");
	if (cursorFollowPos.x > 0)cursorFollowPos.x += 0.5;
	else cursorFollowPos.x -= 0.5;
	if (cursorFollowPos.z < 0) cursorFollowPos.z -= 0.5;
	bool isPayment = false;

	auto playerView = mRegistry.view<PlayerComponent>();
	for (auto playerEntity : playerView)
	{
		auto& player = mRegistry.get<PlayerComponent>(playerEntity);
		auto& transform = mRegistry.get<Transform>(playerEntity);
		auto& owner = player.m_pOwner;

		// 1. 배치 중일 때만 마우스에 따라 위치 이동
		if (player.mIsDeploy == true)
		{
			if (cursorFollowPos.x >= mStartPoint.x && cursorFollowPos.x <= mEndPoint.x && cursorFollowPos.z >= mStartPoint.y && cursorFollowPos.z <= mEndPoint.y)
			{
				transform.m_localPosition = Vector3(static_cast<int>(cursorFollowPos.x), cursorFollowPos.y, static_cast<int>(cursorFollowPos.z));
				if (mIsOneDeploySet == true)
				{
					mIsOneDeploySet == false;
				}

				// 2. 먼저 마우스 버튼이 올라간 경우 배치함
				if (mpInputManager->GetKeyDown(KEY::LBUTTON))
				{
					if (_pSceneData->m_heldMoney < player.mPlaceMoney)
					{
						continue;
					}
					// 눌렀을 때의 마우스 좌표를 기록한다.
					Vector2 tempMousePos = mpInputManager->GetMousePos(); /// 패널 입력을 막는 부분. 아마 이 전체 시스템의 가장 상단에 위치하게 될 것이나, 현재는 여기에 둔다.
					// 눌렀을 때의 마우스 좌표가 패널 바깥일 때만 배치된다.
					if (AddBlockScreenPos(tempMousePos, Vector2(0, 955), Vector2(1920, 80)) == true) // 패널 창 바깥쪽을 입력했을 경우만 처리한다.
					{
						if (IsCanClickAreaXZ(cursorFollowPos, Vector2(-15, 10), Vector2(-9, -10), _pSceneData->m_aliveAlly) == true)
						{
							// 무기를 쥐어준다.
							mpUnitSystem->GetEquipmentManager()->AddWeapon(owner, mClassName);
							mpUnitSystem->GetPlayerSystem()->AddPhysics(owner);
							// 배치가 완료됐음을 표시한다.
							mTempUnitUID = -1;
							mIsOneDeploySet = true;  // 배치 완료 후 상태 변경
							player.mIsDeploy = false;
							player.mIsPlaced = true;
							_pSceneData->m_totalAlly++;
							(*mpUnitSystem->GetPlayerSystem()->GetUnitDeployPosVec())[_pSceneData->m_aliveAlly] = Vector2(static_cast<int>(cursorFollowPos.x), static_cast<int>(cursorFollowPos.z));
							//if (_pSceneData->m_totalAlly > 10)
							//{
							isPayment = true;
							//}
							_pSceneData->m_aliveAlly++;
							_pSceneData->m_heldMoney -= player.mPlaceMoney;
							mpSoundManager->PlaySFX("Snd_sfx_DeployO");
						}
					}
					else
					{
						break;
					}
				}
			}
		}

	}

	auto textView = mRegistry.view<Text>();
	for (auto text : textView)
	{
		auto& name = mRegistry.get<Name>(text).m_name;
		auto& textComp = mRegistry.get<Text>(text);
		if (name == "UI_B_Money")
		{
			textComp.m_num1 = _pSceneData->m_heldMoney;
		}

		// 배치한 아군의 수를 업데이트한다.
		if (name == "UI_D_AliveNum")
		{
			if (9 < textComp.m_num1 && textComp.m_num1 < 20)
			{
				textComp.m_text = u8"%d";
				textComp.m_position.x = 867.f;
			}
			else if (textComp.m_num1 == 20)
			{
				textComp.m_text = u8"%d";
				textComp.m_position.x = 852.f;
			}
			else
			{
				textComp.m_text = u8"0%d";
				textComp.m_position.x = 852.f;
			}
			textComp.m_num1 = _pSceneData->m_aliveAlly;
		}

		if (name == "UI_B_Money")
		{
			if (isPayment == true)
			{
				// 				_pSceneData->m_heldMoney -= 100;
				isPayment = false;
			}
			textComp.m_num1 = _pSceneData->m_heldMoney;
		}

		/// 배치한 용병 수가 10명이 넘어가면, 1명당 100골드가 소모된다는 것을 알린다.
	}

	auto indicatorView = mRegistry.view<IndicatorComponent>();
	for (auto indiEntity : indicatorView)
	{
		if (mRegistry.get<Name>(indiEntity).m_name == "selectedSquare")
		{
			auto& texture3dComp = mRegistry.get<Texture3D>(indiEntity);
			auto& indicatorComp = mRegistry.get<IndicatorComponent>(indiEntity);
			auto outline = mRegistry.try_get<OutlineComponent>(indiEntity);
			auto alpha = mRegistry.try_get<AlphaBlendComponent>(indiEntity);

			if (alpha->m_alpha < 0.5)
			{
				alpha->m_alpha += 0.01;
			}
			else
			{
				alpha->m_alpha = 0;
			}

			if (cursorFollowPos.x >= -15 && cursorFollowPos.x <= 15 && cursorFollowPos.z >= -15 && cursorFollowPos.z <= 15)
			{
				if (IsCanClickAreaXZ(cursorFollowPos, Vector2(-15, 10), Vector2(-9, -10), _pSceneData->m_aliveAlly) == true)
				{
					if (texture3dComp.m_diffuse != indicatorComp.mOriginalColor)
					{
						texture3dComp.m_diffuse = indicatorComp.mOriginalColor;
						outline->m_color = Vector3(0, 0, 1);
						mpRenderManager->UpdateEntityTexture(mpEntityManager->GetEntity(indiEntity));
					}
				}
				else
				{
					if (texture3dComp.m_diffuse != indicatorComp.mChangedColor)
					{
						texture3dComp.m_diffuse = indicatorComp.mChangedColor;
						outline->m_color = Vector3(1, 0, 0);
						mpRenderManager->UpdateEntityTexture(mpEntityManager->GetEntity(indiEntity));
					}
				}
				texture3dComp.m_pOwner->GetComponent<Transform>().m_localPosition = Vector3(static_cast<int>(cursorFollowPos.x), 0.1f, static_cast<int>(cursorFollowPos.z));
			}
		}
	}
	SmogUpdate(_dTime);

	// 랭크바
	UpdateRankBar(_pSceneData);
}

void LevelManager::PreplayUpdate(SceneData* _pSceneData)
{
	auto texView = mRegistry.view<Texture2D>();
	for (auto& entity : texView)
	{
		auto& name = mRegistry.get<Name>(entity).m_name;
		// 소지금
		auto& texture = mRegistry.get<Texture2D>(entity);
		//auto button = mRegistry.try_get<Button>(entity);

		//UI 활성화
		if (name == "UI_P_Timer")
		{
			texture.m_isVisible = true;
		}

		if (name.find("UI_D") != std::string::npos)
		{
			texture.m_isVisible = false;
		}

	} // End for문 : m_pEntityManager->GetEntityMap()

	auto textView = mRegistry.view<Text>();
	for (auto& entity : textView)
	{
		auto& name = mRegistry.get<Name>(entity).m_name;
		auto& text = mRegistry.get<Text>(entity);

		//UI 활성화
		if (name == "UI_P_Timer")
		{
			text.m_isVisible = true;
		}

		if (name.find("UI_D") != std::string::npos)
		{
			text.m_isVisible = false;
		}
	}

	// 유닛 관련 : 체력바를 추가하고, 초기 상태를 설정한다.
	auto playerView = mRegistry.view<PlayerComponent>();
	for (auto& entity : playerView)
	{
		mpUIManager->Add3DHPBar(mpEntityManager->GetEntity(entity), Vector3(0, 3.f, 0), Vector2(50, 5), 5, Vector4(0.2f, 0.2f, 0.2f, 1), Vector4(0.f, 0.f, 1.f, 1));
		mRegistry.get<HealthBarComponenet>(entity).mIsVisible = true;
		mpUnitSystem->GetPlayerSystem()->SetInitializeState(mpEntityManager->GetEntity(entity), State::IDLE);
		mRegistry.get<PlayerComponent>(entity).mpTarget = mpUnitSystem->GetPlayerSystem()->TargetNearestEnemy(mpEntityManager->GetEntity(entity));
	}

	auto enemyView = mRegistry.view<EnemyComponent>();
	for (auto& entity : enemyView)
	{
		mpUIManager->Add3DHPBar(mpEntityManager->GetEntity(entity), Vector3(0, 3.f, 0), Vector2(50, 5), 5, Vector4(0.2f, 0.2f, 0.2f, 1), Vector4(1.f, 0.f, 0.f, 1));
		mRegistry.get<HealthBarComponenet>(entity).mIsVisible = true;
		mpUnitSystem->GetPlayerSystem()->SetInitializeState(mpEntityManager->GetEntity(entity), State::IDLE);
		mRegistry.get<EnemyComponent>(entity).mpTarget = mpUnitSystem->GetPlayerSystem()->TargetNearestPlayer(mpEntityManager->GetEntity(entity));
	}

	DisablePlaceIndicator();
	mpSoundManager->StopBGM();
	mpSoundManager->PlayBGM("Snd_bgm_Battle");
	// 1회적인 세팅을 끝내고 Play로 넘긴다.
	SetGameState(GameState::PLAY);
}

void LevelManager::PlayUpdate(SceneData* _pSceneData, float _dTime)
{
	/// 디버깅용!!!
	if (mpInputManager->GetKey(KEY::CTRL) && mpInputManager->GetKeyDown(KEY::P))
	{
		KillAllEnemies();
	}

	uint8_t numAlly = 0;
	uint8_t numEnemy = 0;

	// 특정 조건에만 돈을 감지해서 타겟팅 하게 하는 함수다.
	mpUnitSystem->GetPlayerSystem()->DetectGroundMoney();

	// 배치 타일 색 바꾸기
	auto indicatorView = mRegistry.view<IndicatorComponent>();
	for (auto& entity : indicatorView)
	{
		if (mRegistry.get<Name>(entity).m_name == "selectedSquare")
		{
			auto& tile = mRegistry.get<Texture3D>(entity);
			auto outline = mRegistry.try_get<OutlineComponent>(entity);
			auto alpha = mRegistry.try_get<AlphaBlendComponent>(entity);

			Vector3 cursorPos = mpPhysicsManager->PickObejct("plane");
			if (cursorPos.x > 0)cursorPos.x += 0.5;
			else cursorPos.x -= 0.5;
			if (cursorPos.z < 0) cursorPos.z -= 0.5;

			if (alpha->m_alpha < 0.5)
			{
				alpha->m_alpha += 0.01;
			}
			else
			{
				alpha->m_alpha = 0;
			}

			if (cursorPos.x >= -15 && cursorPos.x <= 15 && cursorPos.z >= -15 && cursorPos.z <= 15)
			{
				cursorPos = Vector3(static_cast<int>(cursorPos.x), 0.1f, static_cast<int>(cursorPos.z));
				Vector2 astarPos = {};
				mpAstar->AdjustToTileCenter(cursorPos, astarPos);
				if ((*mpAstar->GetAstarMap())[static_cast<int>(astarPos.y + abs(mStartPoint.y))][static_cast<int>(astarPos.x + abs(mStartPoint.x))] == 0) // 장애물이 없는 곳만 클릭 가능
				{
					tile.m_diffuse = "blue.png";
					outline->m_color = Vector3(0, 0, 1);
					mpRenderManager->UpdateEntityTexture(mpEntityManager->GetEntity(entity));
				}
				else
				{
					tile.m_diffuse = "red.png";
					outline->m_color = Vector3(1, 0, 0);
					mpRenderManager->UpdateEntityTexture(mpEntityManager->GetEntity(entity));
				}
				tile.m_pOwner->GetComponent<Transform>().m_localPosition = cursorPos;
			}
		}
	}

	// 랭크바
	UpdateRankBar(_pSceneData);

	// 체력바
	auto playerView = mRegistry.view<PlayerComponent>();
	auto enemyView = mRegistry.view<EnemyComponent>();
	for (auto& entity : playerView)
	{
		auto& playerComp = mRegistry.get<PlayerComponent>(entity);
		if (auto helthBar = mRegistry.try_get<HealthBarComponenet>(entity)) // 체력바가 있는 건 다 플레이어 컴포넌트가 있을 것이다.
		{
			float healthPercent = playerComp.mHP / playerComp.mMaxHP;
			helthBar->mHealthPercentage = healthPercent;
		}

		// 플레이어 관련하여 업데이트한다.
		mpUnitSystem->GetPlayerSystem()->PlayerUnitUpdate(mpEntityManager->GetEntity(entity), _dTime); // 아군 유닛을 업데이트 합니다.
		/// 		mpUnitSystem->GetPlayerSystem()->UpdateStateText(mpEntityManager->GetEntity(entity)); // 디버깅용 코드입니다.

		if (playerComp.mIsDead == false)
		{
			numAlly++;
		}
		else
		{
			physx::PxFilterData filterData = mpPhysicsManager->GetFilterData(entity);
			filterData.word1 |= 8;
			mpPhysicsManager->SetFilterData(entity, filterData);
		}

	}

	for (auto& entity : enemyView)
	{
		auto& enemyComp = mRegistry.get<EnemyComponent>(entity);
		if (auto helthBar = mRegistry.try_get<HealthBarComponenet>(entity)) // 체력바가 있는 건 다 플레이어 컴포넌트가 있을 것이다.
		{
			float healthPercent = enemyComp.mHP / enemyComp.mMaxHP;
			helthBar->mHealthPercentage = healthPercent;
		}

		mpUnitSystem->GetPlayerSystem()->EnemyUnitUpdate(mpEntityManager->GetEntity(entity), _dTime); // 적군 유닛을 업데이트 합니다.
		/// 		mpUnitSystem->GetPlayerSystem()->UpdateStateText(mpEntityManager->GetEntity(entity)); // 디버깅용 코드입니다.

		if (enemyComp.mIsDead == false)
		{
			numEnemy++;
		}
		else
		{
			physx::PxFilterData filterData = mpPhysicsManager->GetFilterData(entity);
			filterData.word1 |= 8;
			mpPhysicsManager->SetFilterData(entity, filterData);
		}
	}

	/// 여기서 용병왕을 업데이트 한다.
	auto npcView = mRegistry.view<NPCComponent>();
	for (auto& entity : npcView)
	{
		mpUnitSystem->GetNPCSystem()->Update(mpEntityManager->GetEntity(entity), numAlly, numEnemy);
	}

	auto textView = mRegistry.view<Text>();
	for (auto& entity : textView)
	{
		auto& name = mRegistry.get<Name>(entity).m_name;
		auto& text = mRegistry.get<Text>(entity);
		// 타이머
		if (name == "UI_P_Timer")
		{
			_pSceneData->m_time -= _dTime;

			int minutes = static_cast<int>(_pSceneData->m_time) / 60;
			int seconds = static_cast<int>(_pSceneData->m_time) % 60;

			std::ostringstream oss;
			oss << std::setfill('0') << std::setw(2) << minutes << ":"
				<< std::setfill('0') << std::setw(2) << seconds;

			// std::u8string으로 변환
			std::string str = oss.str();
			text.m_text = std::u8string(str.begin(), str.end());

			// 10초 남았을때, 경고효과음 재생
			if (!mPlayWarningSound && std::fabs(_pSceneData->m_time - 10.0f) < 0.001f)
			{
				mPlayWarningSound = true;
				mpSoundManager->PlaySFX("Snd_sfx_TimeLow");
			}

			// 시간이 다 되면 자동으로 넘어감
			if (_pSceneData->m_time < std::numeric_limits<float>::epsilon())
			{
				_pSceneData->m_time = _pSceneData->m_totalTime;
				DLOG(LOG_INFO, "Game Over");
				//m_pWorldManager->GetCurrentWorld()->SetScene("game over");
				SetGameState(GameState::NONE_UI);
				return;
			}
		}

		// 플레이어의 소지금을 업데이트 한다.
		if (name == "UI_B_Money")
		{
			text.m_num1 = _pSceneData->m_heldMoney;
		}

		if (name == "UI_B_StageGuide")
		{
			text.m_num1 = numEnemy;
		}

	} // End for문 : m_pEntityManager->GetEntityMap()
	if (numAlly == 0 || numEnemy == 0)
	{
		_pSceneData->m_aliveAlly = numAlly;
		SetGameState(GameState::NONE_UI);
		mpSoundManager->StopSFX("Snd_sfx_TimeLow");
		mpSoundManager->StopBGM();
		if (numAlly == 0)
		{
			mpSoundManager->PlaySFX("Snd_bgm_BattleLose");
			mIsClear = false;
		}
		else
		{
			mpSoundManager->PlaySFX("Snd_bgm_BattleWin");
			mIsClear = true;
		}
	}

	SmogUpdate(_dTime);
}

void LevelManager::NoneUI()
{
	// UI 전체 가리기
	// 모든 체력바를 숨기고, 아군과 적군 애니메이션을 정지한다(용병왕은 제외)
	JumpingAndRotUnit();

	auto rigidView = mRegistry.view<Rigidbody>();
	for (auto& entity : rigidView)
	{
		auto& rigid = mRegistry.get<Rigidbody>(entity);
		rigid.m_isKinematic = true;
	}

	auto resTextView = mRegistry.view<Text>();
	for (auto entity : resTextView)
	{
		auto& text = mRegistry.get<Text>(entity);

		text.m_isVisible = false;
	}

	auto resView = mRegistry.view<Texture2D>();
	for (auto entity : resView)
	{
		auto& img = mRegistry.get<Texture2D>(entity);
		img.m_isVisible = false;
	}

	// 모든 메세지창 가리기
	auto msgView = mRegistry.view<MessageBox2D>();
	for (auto entity : msgView)
	{
		auto& msgBox = mRegistry.get<MessageBox2D>(entity);
		msgBox.mIsImgVisible = false;
	}

	// 배치 타일 삭제
	auto indicatorView = mRegistry.view<IndicatorComponent>();
	for (auto& entity : indicatorView)
	{
		mpEntityManager->RemoveEntity(static_cast<UID>(entity));
	}

	mpRenderManager->CameraSetPerspective();
	auto camera = GetWorldCamera();
	camera->SetEyePos(mCameraPosInfo.mainPositionPerspective);
	camera->SetDirection(mCameraPosInfo.mainRotationPerspective);

	/// 동전 전부 삭제
	DeleteMoney();

	currentLevelState = GameState::POST_PLAY_ANIMATION;
}

void LevelManager::PostAnimationFixedUpdate(float _dTime)
{
	JumpingAndRotUnit();
	auto camera = GetWorldCamera();

	tempTime += _dTime * 0.5f;
	if (tempTime <= 1)
	{
		CameraWalk(camera->mViewPos, mCameraPosInfo.mainPositionPerspective, mCameraPosInfo.pos6, tempTime);
		CameraRotate(camera->mViewDir, mCameraPosInfo.mainRotationPerspective, mCameraPosInfo.rot6, tempTime);
	}
	else if (1 < tempTime && tempTime <= 2)
	{
		CameraWalk(camera->mViewPos, mCameraPosInfo.pos6, mCameraPosInfo.pos7, (tempTime - 1.0f));
		CameraRotate(camera->mViewDir, mCameraPosInfo.rot6, mCameraPosInfo.rot7, (tempTime - 1.0f));
	}
	else if (2.f < tempTime || mpInputManager->GetKeyUp(KEY::LBUTTON))
	{
		currentLevelState = GameState::POST_PLAY;
		tempTime = 0.0f;
	}

	SmogUpdate(_dTime);

}

void LevelManager::PostplayUpdate(SceneData* _pSceneData)
{
	JumpingAndRotUnit();

	auto resTextView = mRegistry.view<Text>();
	for (auto entity : resTextView)
	{
		auto& name = mRegistry.get<Name>(entity).m_name;
		auto& text = mRegistry.get<Text>(entity);

		if (name.find("UI_Res") != std::string::npos)
		{
			text.m_isVisible = true;
		}

		// 결과창 출력에 사용할 이징함수 추가

	}

	auto resView = mRegistry.view<Texture2D>();
	for (auto entity : resView)
	{
		auto& name = mRegistry.get<Name>(entity).m_name;
		auto& texture = mRegistry.get<Texture2D>(entity);

		if (name.find("UI_Res") != std::string::npos)
		{
			texture.m_isVisible = true;
		}

		// 보상, 결과 랭크 계산
		// 결과 패널 이미지 변경, 보수 금액 설정
		if (name == "UI_Res6_1")
		{
			if (_pSceneData->m_aliveAlly == 0)
			{
				texture.m_file = "UI_img_RankF.png";
				_pSceneData->m_remainAmount = _pSceneData->m_heldMoney;
			}
			else
			{
				// A랭크
				if (_pSceneData->m_heldMoney >= _pSceneData->m_rankA)
				{
					texture.m_file = "UI_img_RankA.png";
					_pSceneData->m_remainAmount = _pSceneData->m_heldMoney;
				}
				else if (_pSceneData->m_heldMoney >= _pSceneData->m_rankB)
				{
					texture.m_file = "UI_img_RankB.png";
					_pSceneData->m_remainAmount = _pSceneData->m_heldMoney;
				}
				else
				{
					texture.m_file = "UI_img_RankC.png";
					_pSceneData->m_remainAmount = _pSceneData->m_heldMoney;
				}
			}

		}

	}

	for (auto entity : resView)
	{
		auto& name = mRegistry.get<Name>(entity).m_name;
		auto& texture = mRegistry.get<Texture2D>(entity);
		auto button = mRegistry.try_get<Button>(entity);

		if (name == "UI_Res9")
		{
			texture.m_position.x = 1180.f;
			button->mUIPosition.x = texture.m_position.x;
			button->mUIHoveringPosition.x = texture.m_position.x;
			button->mUIPressedPosition.x = texture.m_position.x;
			/// 여기 버튼 위치도 바꿔야 하는지 확인
			if (!mIsClear) // 못 깼으면? -> 재시작으로 교체
			{
				texture.m_file = "UI_btn_StageToStage.png";
				//texture.m_isVisible = false;
				//button.mIsEnable = false;
			}
			else // 깼으면? -> 다음으로 교체
			{
				mIsRestart = false; // 재시작이 아님을 알림

				texture.m_file = "UI_btn_StageToNext.png";
				if (mIsFinalStage == true)
				{
					texture.m_position.x = 1335.f;
					button->mUIPosition.x = texture.m_position.x;
					button->mUIHoveringPosition.x = texture.m_position.x;
					button->mUIPressedPosition.x = texture.m_position.x;
					tempTime = 0.0f;
					FadePreSetting(true);
				}
			}
		}

		if (name == "UI_Res11")
		{
			if (mIsFinalStage == true && mIsClear == true)// 만약 현재 스테이지가 마지막 스테이지고 클리어라면 메인으로는 없다
			{
				texture.m_isVisible = false;
				button->mIsEnable = false;
			}
			else
			{
				texture.m_isVisible = true;
				button->mIsEnable = true;
			}
		}
	}

	/// 용병을 결과 업데이트
	auto npcView = mRegistry.view<NPCComponent>();
	for (auto& entity : npcView)
	{
		mpUnitSystem->GetNPCSystem()->FinalUpdate(mpEntityManager->GetEntity(entity), mIsClear);
	}

	// 사용한 금액 계산
	_pSceneData->m_usedAmount = _pSceneData->m_totalAmount - _pSceneData->m_heldMoney;

	SetGameState(GameState::RESULT);
}

void LevelManager::ResultUpdate(SceneData* _pSceneData, float _dTime)
{
	JumpingAndRotUnit();

	if (mIsClear)
	{
		static float dTime;
		dTime += _dTime;
		if (dTime >= 0.7)
		{
			dTime -= 0.7;
			auto partView = mRegistry.view<ParticleComponent>();
			for (auto& entity : partView)
			{
				auto& name = mRegistry.get<Name>(entity).m_name;
				auto& particle = mRegistry.get<ParticleComponent>(entity);
				particle.m_pParticleData->position = Vector3(-40, RandomUtil::RandomInt(3, 10), RandomUtil::RandomInt(-20, 20));
				if (name == "FireCracker")
				{
					mpRenderManager->AddParticle(200, *particle.m_pParticleData);
				}
			}
		}
	}

	mResultEasingTimer += _dTime;

	auto textView = mRegistry.view<Text>();
	for (auto entity : textView)
	{
		auto& name = mRegistry.get<Name>(entity).m_name;

		if (name.find("UI_Res") != std::string::npos)
		{
			if (auto text = mRegistry.try_get<Text>(entity))
			{
				// 초기 금액
				if (name == "UI_Res10")
				{
					float price = 0;
					mpEasingFunc->EasingFuncMap["easeInQuint"](0, _pSceneData->m_totalAmount, mResultEasingTimer, &price);
					//std::string str = std::to_string(static_cast<int>(_pSceneData->m_totalAmount));
					std::string str = std::to_string(static_cast<int>(price));

					std::string formattedStr;
					int len = str.length();

					for (int i = 0; i < len; ++i)
					{
						if (i > 0 && (len - i) % 3 == 0)
						{
							formattedStr += ',';
						}
						formattedStr += str[i];

						// std::string을 std::u8string으로 변환
						text->m_text = std::u8string(formattedStr.begin(), formattedStr.end());
					}
				}

				// 사용 금액
				else if (name == "UI_Res7")
				{
					std::string str{};
					if (mResultEasingTimer >= 1.f)
					{
						float price = 0;
						mpEasingFunc->EasingFuncMap["easeInQuint"](0, _pSceneData->m_usedAmount, mResultEasingTimer - 1, &price);

						str = std::to_string(static_cast<int>(price));
					}
					else
					{
						str = std::to_string(static_cast<int>(0));

					}
					//std::string str = std::to_string(static_cast<int>(_pSceneData->m_usedAmount));

					std::string formattedStr;
					int len = str.length();

					for (int i = 0; i < len; ++i)
					{
						if (i > 0 && (len - i) % 3 == 0)
						{
							formattedStr += ',';
						}
						formattedStr += str[i];
					}

					// std::string을 std::u8string으로 변환
					text->m_text = std::u8string(formattedStr.begin(), formattedStr.end());
				}
				// 남은 금액
				else if (name == "UI_Res8")
				{
					std::string str{};
					if (mResultEasingTimer >= 2.f)
					{
						float price = 0;
						mpEasingFunc->EasingFuncMap["easeInQuint"](0, _pSceneData->m_remainAmount, mResultEasingTimer - 2, &price);
						str = std::to_string(static_cast<int>(price));

					}
					else
					{
						str = std::to_string(static_cast<int>(0));

					}
					//std::string str = std::to_string(static_cast<int>(_pSceneData->m_remainAmount));

					std::string formattedStr;
					int len = str.length();

					for (int i = 0; i < len; ++i)
					{
						if (i > 0 && (len - i) % 3 == 0)
						{
							formattedStr += ',';
						}
						formattedStr += str[i];
					}

					// std::string을 std::u8string으로 변환
					text->m_text = std::u8string(formattedStr.begin(), formattedStr.end());
				}
			}

		}
		auto resView = mRegistry.view<Button>();
		for (auto entity : resView)
		{
			auto& name = mRegistry.get<Name>(entity).m_name;
			auto& button = mRegistry.get<Button>(entity);

			if (button.GetOwnerName() == "UI_Res9" && mpUIManager->GetButtonState(button.m_pOwner) == ButtonState::PRESSED)
			{
				if (mIsClear == true) // 다음일 때
				{
					if (mIsFinalStage == true)
					{
						mIsEndingFade = true;
					}
					else
					{
						mIsNext = true;
						currentUIAniState = UIAnimationState::VISIBLE;
					}
				}
				else // 재시작일 때
				{
					currentLevelState = GameState::RESTART;
				}
				break;
			}

			if (button.GetOwnerName() == "UI_Res11")
			{
				if (mpUIManager->GetButtonState(button.m_pOwner) == ButtonState::PRESSED)
				{
					currentPopUpState = PopUpState::OPEN_MAINMENU;
					if (!mpSoundManager->IsPlaying("Snd_sfx_ClickBtn"))
					{
						mpSoundManager->PlaySFX("Snd_sfx_ClickBtn");
					}
					button.mButtonState = ButtonState::NORMAL;
					button.mIsEnable = false;
					//mIsGoMain = true;
				}
				else
				{
					button.mIsEnable = true;
				}
				continue;
			}
		}
	}

	auto textureView = mRegistry.view<Texture2D>();
	for (auto entity : textureView)
	{
		auto& name = mRegistry.get<Name>(entity).m_name;
		auto& img = mRegistry.get<Texture2D>(entity);

		// 랭크 패널
		if (name == "UI_Res6")
		{
			auto img = mRegistry.try_get<Texture2D>(entity);
			UpdateRandomColor(img->m_rgba, _dTime, 3.0f);
		}
	}


	if (mIsEndingFade == true)
	{
		tempTime += _dTime * 0.1f; /// 엔딩 크레딧 페이드 속도 조정하고 싶으면 이거 조정
		auto view = mRegistry.view<FadeInOut>();
		for (auto& entity : view)
		{
			auto& name = mRegistry.get<Name>(entity).m_name;
			auto& img = mRegistry.get<FadeInOut>(entity);
			if (name == "FaidInOut")
			{
				if (img.IsFadingOutFin(tempTime, 1.0f) == true)
				{
					mIsGameEnding = true;
				}
			}
		}
	}
}

void LevelManager::UpdateParticle(float _dTime)
{
	auto playerView = mRegistry.view<PlayerComponent>();
	auto enemyView = mRegistry.view<EnemyComponent>();

	for (auto& entity : playerView)
	{
		auto particle = mRegistry.try_get<ParticleComponent>(entity);
		auto& player = mRegistry.get<PlayerComponent>(entity);
		auto& trsComp = mRegistry.get<Transform>(entity);
		auto& name = mRegistry.get<Name>(entity).m_name;
		particle->m_pParticleData->position = trsComp.m_localPosition;
		particle->m_pParticleData->direction = player.tempDirection;
		particle->m_pParticleData->direction.Normalize();

		if (player.mOwnState == State::LOOT)
		{
			mpRenderManager->AddParticle(100, *particle->m_pParticleData);
		}
	}

	for (auto& entity : enemyView)
	{
		auto particle = mRegistry.try_get<ParticleComponent>(entity);
		auto& player = mRegistry.get<EnemyComponent>(entity);
		auto& trsComp = mRegistry.get<Transform>(entity);
		auto& name = mRegistry.get<Name>(entity).m_name;
		particle->m_pParticleData->direction = player.tempDirection;
		particle->m_pParticleData->direction.Normalize();

		particle->m_pParticleData->position = trsComp.m_localPosition;

		if (player.mOwnState == State::LOOT)
		{
			mpRenderManager->AddParticle(100, *particle->m_pParticleData);
		}
	}
}

void LevelManager::StartAnimationFixedUpdate(float _dTime)
{
	//static float time = 0;
	auto camera = GetWorldCamera();
	tempTime += _dTime * 0.5f;
	//[기획]2초 동안 좌표(0, 17, -17.5)로 이동하며 회전값(0, -0.8, 0.6)이 되도록 고개 숙이듯 회전
	if (tempTime <= 0.5)
	{
		//CameraWalk(camera->mViewPos, mCameraPosInfo.startPos, mCameraPosInfo.pos1, tempTime * 2);
		CameraEasing("easeInSine", camera->mViewPos, mCameraPosInfo.startPos, mCameraPosInfo.pos2, tempTime);
		//CameraRotate(camera->mViewDir, mCameraPosInfo.startRot, mCameraPosInfo.rot2, tempTime);
	}
	// 플밍이 추가한 구간
	else if (0.5 < tempTime && tempTime <= 1)
	{
		CameraEasing("easeInSine", camera->mViewPos, mCameraPosInfo.startPos, mCameraPosInfo.pos2, tempTime);
		CameraEasing("easeInSine", camera->mViewDir, mCameraPosInfo.startRot, mCameraPosInfo.rot2, (tempTime - 0.5f) * 2);
	}
	// [기획] 페이드인이었으나 정신 사나울 듯 하여 2초 동안 이동으로 임시로 함
	else if (1 < tempTime && tempTime <= 2)
	{
		CameraWalk(camera->mViewPos, mCameraPosInfo.pos2, mCameraPosInfo.pos3, (tempTime - 1.0f));
		CameraEasing("easeOutSine", camera->mViewDir, mCameraPosInfo.rot2, mCameraPosInfo.rot3, (tempTime - 1.0f));
	}
	// [기획] 좌표 (-3, 10, 0)으로 이동하며, 동시에 회전값이 (0.8, -0.6, 0)이 되도록 회전 // 알아서 1초로 정함
	else if (2.3f < tempTime && tempTime <= 2.8f)
	{
		CameraWalk(camera->mViewPos, mCameraPosInfo.pos3, mCameraPosInfo.pos4, (tempTime - 2.3f) * 2);
		CameraRotate(camera->mViewDir, mCameraPosInfo.rot3, mCameraPosInfo.rot4, (tempTime - 2.3f) * 2);
	}
	// 페이드 아웃
	else if (2.8f < tempTime && tempTime <= 2.9f)
	{
		FadePreSetting(true);
	}
	else if (2.9f < tempTime && tempTime <= 3.5f)
	{
		FadeOutScreen(_dTime);
	}
	else if (3.5f < tempTime && tempTime <= 3.6f)
	{
		FadePreSetting(false);
		mpRenderManager->CameraSetOrthographic(0.03);

		auto view = mRegistry.view<Texture2D>();
		for (auto& entity : view)
		{
			auto& name = mRegistry.get<Name>(entity).m_name;
			auto& texture = mRegistry.get<Texture2D>(entity);
			if (name == "UI_Effect_Start")
			{
				texture.m_isVisible = true;
			}
		}
	}
	else if (3.6f < tempTime && tempTime <= 4.2f)
	{
		FadeInScreen(_dTime);
		camera->SetEyePos(Vector3(-16.46, 18.06, -16.62));
		camera->SetDirection(Vector3(0.57735, -0.57735, 0.57735));
	}
	else if (4.2f < tempTime && tempTime <= 4.7f)
	{
		auto view = mRegistry.view<Texture2D>();
		for (auto& entity : view)
		{
			auto& name = mRegistry.get<Name>(entity).m_name;
			auto& texture = mRegistry.get<Texture2D>(entity);
			if (name == "UI_Effect_Start")
			{
				mpEasingFunc->EasingFuncMap["easeInSine"](0, -653.f, (tempTime - 4.2f) * 4, &texture.m_position.y);
			}
		}
	}
	else if (4.5f < tempTime)
	{
		auto view = mRegistry.view<FadeInOut>();
		for (auto& entity : view)
		{
			auto& name = mRegistry.get<Name>(entity).m_name;
			auto& img = mRegistry.get<FadeInOut>(entity);
			if (name == "UI_Effect_Start")
			{
				img.IsFadingInFin(_dTime) == true;
				if (img.state == FadeInOutState::NONE)
				{
					img.mAlpha = 0.0f;
					img.state = FadeInOutState::FADE_IN;
					currentLevelState = GameState::PRE_PLACEMENT;
					tempTime = 0.0f;
				}
			}
		}
	}
	SmogUpdate(_dTime);
}

void LevelManager::PlayFixedUpdate(float _dTime)
{
	auto playerView = mRegistry.view<PlayerComponent>();
	auto enemyView = mRegistry.view<EnemyComponent>();
	for (auto& entity : playerView)
	{
		mpUnitSystem->GetPlayerSystem()->FixedUpdate(mpEntityManager->GetEntity(entity), _dTime); // 아군 유닛을 업데이트 합니다.
	}

	for (auto& entity : enemyView)
	{
		mpUnitSystem->GetPlayerSystem()->FixedUpdate(mpEntityManager->GetEntity(entity), _dTime); // 적군 유닛을 업데이트 합니다.
	}

	mpUnitSystem->GetProjectileSystem()->FixedUpdate(_dTime);
}

void LevelManager::ResetPlayer(SceneData* _pSceneData)
{
	int numUnit = 0;
	// 무기를 가진 엔티티를 먼저 삭제한다.
	auto weaponView = mRegistry.view<BoxCollider, Transform>();
	for (auto& weaponEntity : weaponView)
	{
		auto& transform = mRegistry.get<Transform>(weaponEntity);
		if (transform.m_pParent)
		{
			auto& parentTrs = transform.m_pParent;
			if (parentTrs->m_pOwner->HasComponent<WeaponComponent>() && parentTrs->m_pOwner->HasComponent<PlayerComponent>())
			{
				mpEntityManager->RemoveEntity(static_cast<UID>(weaponEntity));
			}
		}
	}

	// 플레이어 컴포넌트를 가진 모든 엔티티를 삭제한다.
	auto playerView = mRegistry.view<PlayerComponent>();
	for (auto& playerEntity : playerView)
	{
		mIsOneDeploySet = true;  // 배치 완료 후 상태 변경
		// 사용한 돈 반환
		auto& playerComp = mRegistry.get<PlayerComponent>(playerEntity);
		if (playerComp.mIsPlaced)
		{
			_pSceneData->m_heldMoney += playerComp.mPlaceMoney;
		}
		// 캐릭터 삭제
		mpEntityManager->RemoveEntity(static_cast<UID>(playerEntity));
		numUnit++;
	}
	mIsClassButtonPressed == false; // 버튼이 눌린 상태 삭제
	auto userPlaceUnit = mpEntityManager->GetEntity(mTempUnitUID);
	mTempUnitUID = -1; // 선택된 엔티티 UID 초기화
	_pSceneData->m_totalAlly = 0;
	_pSceneData->m_aliveAlly = 0;

	// 배치된 정보 초기화
	for (auto& e : *(mpUnitSystem->GetPlayerSystem()->GetUnitDeployPosVec()))
	{
		e = Vector2();
	}
}

void LevelManager::UpdateRandomColor(Vector4& _rgba, float _dTime, float _speed)
{
	tempTime += _dTime;
	// 색상 변경 속도 조절

	// 시간에 따라 R, G, B 값을 변환
	_rgba.x = (sin(tempTime * _speed) + 1.0f) * 0.5f;   // 0~255 범위로 변환
	_rgba.y = (sin(tempTime * _speed + 2.0f) + 1.0f) * 0.5f;
	_rgba.z = (sin(tempTime * _speed + 4.0f) + 1.0f) * 0.5f;
}

void LevelManager::SetVolume(const std::string& _fileName, const shared_ptr<Entity>& _entity, int& _intNum, int& _whatVolume)
{
	if (_fileName == "UI_btn_VolumeUp.png")
	{
		_intNum = _whatVolume;
		if (mpUIManager->GetButtonState(_entity) == ButtonState::PRESSED
			&& mpInputManager->GetKeyUp(KEY::LBUTTON))
		{
			_whatVolume++;
			auto& buttonComp = _entity->GetComponent<Button>();
			buttonComp.mButtonState = ButtonState::NORMAL;
		}
	}

	if (_fileName == "UI_btn_VolumeDown.png"
		&& mpUIManager->GetButtonState(_entity) == ButtonState::PRESSED
		&& mpInputManager->GetKeyUp(KEY::LBUTTON))
	{
		_whatVolume--;
		auto& buttonComp = _entity->GetComponent<Button>();
		buttonComp.mButtonState = ButtonState::NORMAL;
	}

	if (_whatVolume < 0)
		_whatVolume = 0;
	else if (_whatVolume > 5)
		_whatVolume = 5;
}

void LevelManager::ManagePopupDisplay(PopUpState _state, PopUpState _state2/* = PopUpState::LAST*/)
{
	auto uiTextureView = mRegistry.view<Texture2D>();
	for (auto& uiEntity : uiTextureView)
	{
		auto& name = mRegistry.get<Name>(uiEntity).m_name;
		auto button = mRegistry.try_get<Button>(uiEntity);
		auto text = mRegistry.try_get<Text>(uiEntity);
		auto& texture = mRegistry.get<Texture2D>(uiEntity);

		if (currentPopUpState == _state || currentPopUpState == _state2)
		{
			// 팝업창이 뜰 경우 모두 보이게 한다.
			if (name.find("UI_Ver") != std::string::npos)
			{
				if (button)
				{
					// 버튼은 한 번만 클릭된다.
					button->mButtonState = ButtonState::NORMAL;
				}

				texture.m_isVisible = true;
				if (name == "UI_Verify_Main")
				{
					text->m_isVisible = true;
				}
			}
		}
		else if (currentPopUpState == PopUpState::CLOSED)
		{
			if (name.find("UI_Ver") != std::string::npos)
			{
				if (button)
				{
					// 버튼은 한 번만 클릭된다.
					button->mButtonState = ButtonState::NORMAL;
				}
				texture.m_isVisible = false;
				if (name == "UI_Verify_Main")
				{
					text->m_isVisible = false;
				}
			}
		}
		else
		{
			break;
		}
	}
}

bool LevelManager::AddBlockScreenPos(const Vector2& _mousePos, const Vector2& _PosXlYu, const Vector2& _WH)
{
	if (_PosXlYu.x < _mousePos.x && _mousePos.x < _PosXlYu.x + _WH.x &&
		_PosXlYu.y < _mousePos.y && _mousePos.y < _PosXlYu.y + _WH.y)
		return false;

	return true;
}

Vector3 LevelManager::GetTargetPosition(Vector2 _mousePos, int _screenWidth, int _screenHeight, Vector3 pointOffset, float cameraDistance)
{
	Camera* mpCamera = nullptr;
	for (auto& [uid, entity] : mpEntityManager->GetEntityMap())
	{		// 카메라의 마우스 위치 업데이트
		if (entity->GetName() == "Camera")
		{
			mpCamera = entity->GetComponent<CameraComponent>().m_pCamera;
		}
	}

	// 1. 화면 좌표를 NDC로 변환
	Vector3 ndcCoords = {};
	// 화면 좌표를 NDC로 변환
	float xNDC = (2.0f * _mousePos.x) / _screenWidth - 1.0f;
	float yNDC = 1.0f - (2.0f * _mousePos.y) / _screenHeight;

	// 2. NDC를 클립 좌표로 변환
	Vector4 clipCoords = Vector4(xNDC, yNDC, -1.0f, 1.0f);

	// 3. 클립 좌표를 뷰 좌표로 변환
	Matrix invProj = (mpCamera->GetProjRow()).Invert();
	Vector4 eyeCoords = Vector4::Transform(clipCoords, (mpCamera->GetProjRow()).Invert());
	eyeCoords.z = 1.0f;  // 기본값으로 설정
	eyeCoords.w = 0.0f;  // 기본값으로 설정

	// 4. 뷰 좌표를 월드 좌표로 변환
	Vector4 rayWorld = Vector4::Transform(eyeCoords, (mpCamera->GetViewRow()).Invert());

	/*if (rayWorld.y < 0)
		rayWorld.y = 5.0f;*/
		// 레이의 원점은 카메라의 위치, 방향은 rayWorld
	if (mpCamera != nullptr)
	{
		Vector3 offsetPosition = (mpCamera->mViewPos + pointOffset) + Vector3(rayWorld.x, rayWorld.y, rayWorld.z) * cameraDistance;
		return offsetPosition;
	}
	else
	{
		return Vector3();
	}
}

bool LevelManager::IsCanClickAreaXZ(const Vector3& _realPos, const Vector2& _PosXlZu, const Vector2& _PosXrZd, const int& _aliveNum)
{
	Vector2 posXZ(static_cast<int>(_realPos.x), static_cast<int>(_realPos.z));
	if (AddObjectArea(posXZ, _PosXlZu, _PosXrZd) == true// 기획에서 제공한 안쪽 범위
		&& (*mpAstar->GetAstarMap())[static_cast<int>(posXZ.y + abs(mStartPoint.y))][static_cast<int>(posXZ.x + abs(mStartPoint.x))] == 0) // 장애물이 없는 곳만 클릭 가능
	{
		for (int i = 0; i < _aliveNum; i++)
		{
			if (posXZ == (*mpUnitSystem->GetPlayerSystem()->GetUnitDeployPosVec())[i])
			{
				return false; // 중복된 위치면 클릭 불가하다.
			}
		}
		return true;
	}
	return false;
}

void LevelManager::JumpingAndRotUnit()
{
	auto HPView = mRegistry.view<HealthBarComponenet>();
	for (auto entity : HPView)
	{
		auto& hpEntity = mRegistry.get<HealthBarComponenet>(entity);
		hpEntity.mIsVisible = false;
		auto& animation = mRegistry.get<AnimationComponent>(entity);
		if (hpEntity.mHealthPercentage > 0)
		{
			auto& trs = mRegistry.get<Transform>(entity);
			trs.m_localRotation.y += DirectX::XMConvertToRadians(RandomUtil::RandomFloat(0.0f, 2.0f));
			animation.mpTargetAnimation = mpResourceManager->Get_Animation("Character@Cast Spell 02.FBX");
			animation.mPlaySpeed = RandomUtil::RandomFloat(0.5f, 2.0f);
		}
		else
		{
			animation.mPaused = true;
		}
	}
}

bool LevelManager::AddObjectArea(const Vector2& _worldMousePos, const Vector2& _PosXlZu, const Vector2& _PosXrZd)
{
	if (_PosXlZu.x < _worldMousePos.x && _worldMousePos.x < _PosXrZd.x &&
		_PosXrZd.y < _worldMousePos.y && _worldMousePos.y < _PosXlZu.y)
		return true;

	return false;
}

void LevelManager::UpdateRankBar(SceneData* _pSceneData)
{
	auto msgView = mRegistry.view<MessageBox2D>();
	for (auto& entity : msgView)
	{
		auto& name = mRegistry.get<Name>(entity).m_name;
		auto& msgComp = mRegistry.get<MessageBox2D>(entity);
		if (name == "UI_B_Rank_Gauge")
		{
			msgComp.mSize.x = 250.f * (static_cast<float>(_pSceneData->m_heldMoney) / _pSceneData->m_totalAmount);
			if (83.33f <= msgComp.mSize.x && msgComp.mSize.x < 166.67f) // B랭크
			{
				msgComp.mImgRgba = Vector4(0.72f, 0.72f, 0.70f, 1.0f);
			}
			else if (msgComp.mSize.x < 83.33f) // C 랭크
			{
				msgComp.mImgRgba = Vector4(0.85f, 0.54f, 0.4f, 1.0f);
			}
			else // A 랭크
			{
				msgComp.mImgRgba = Vector4(1.0f, 0.84f, 0.0f, 1.0f);
			}
		}

	}
}

void LevelManager::DrawCoordinatesXZ(bool _isVisible) // false 먼저 넣으면 터짐... 나중에 게임 엔진 고칠 것 ㅠㅠ 
{
	// 	for (int i = -15; i <= 16; i++)
	// 	{
	// 		//for (int j = 0; j <= 4; j++){
	// 		int j = 0;
	// 		auto coord = mpEntityManager->CreateEntity("coord");
	// 		coord->AddComponent<Transform>(Vector3(0, j, -0.5f + i), Vector3(0.f), Vector3(15.0f, 0.005f, 0.005f));
	// 		coord->AddComponent<MeshRenderer>("box.fbx", "Cube.001", false, false);
	// 		coord->AddComponent<Texture3D>("M-Cos-03-Purple.png");
	// 		coord->AddComponent<IndicatorComponent>();
	// 		auto coord1 = mpEntityManager->CreateEntity("coord");
	// 		coord1->AddComponent<Transform>(Vector3(-0.5f + i, j, 0.f), Vector3(0.f), Vector3(0.005f, 0.005f, 15.f));
	// 		coord1->AddComponent<MeshRenderer>("box.fbx", "Cube.001", false, false);
	// 		coord1->AddComponent<Texture3D>("M-Cos-03-Purple.png");
	// 		coord1->AddComponent<IndicatorComponent>();
	// 	}
}

void LevelManager::CreateCoordinatesXZ()
{

}

void LevelManager::KillAllEnemies()
{
	auto enemyView = mRegistry.view<EnemyComponent>();
	for (auto& entity : enemyView)
	{
		auto& enemyComp = mRegistry.get<EnemyComponent>(entity);
		enemyComp.mHP = 0;
	}
}

GameState LevelManager::GetGameState()
{
	return currentLevelState;
}

void LevelManager::SetGameState(GameState _state)
{
	currentLevelState = _state;
}

PopUpState LevelManager::GetPopUpState()
{
	return currentPopUpState;
}

void LevelManager::SetPopUpState(PopUpState _state)
{
	currentPopUpState = _state;
}

UIAnimationState LevelManager::GetUIAnimationState()
{
	return currentUIAniState;
}

void LevelManager::SetUIAnimationState(UIAnimationState _state)
{
	currentUIAniState = _state;
}

void LevelManager::CameraWalk(Vector3& _target, const Vector3& _start, const Vector3& _end, float _time)
{
	mpEasingFunc->EasingFuncMap["easeInSine"](_start.x, _end.x, _time, &_target.x);
	mpEasingFunc->EasingFuncMap["easeInSine"](_start.y, _end.y, _time, &_target.y);
	mpEasingFunc->EasingFuncMap["easeInSine"](_start.z, _end.z, _time, &_target.z);
}

void LevelManager::CameraRotate(Vector3& _target, const Vector3& _start, const Vector3& _end, float _time)
{
	mpEasingFunc->EasingFuncMap["easeOutSine"](_start.x, _end.x, _time, &_target.x);
	mpEasingFunc->EasingFuncMap["easeOutSine"](_start.y, _end.y, _time, &_target.y);
	mpEasingFunc->EasingFuncMap["easeOutSine"](_start.z, _end.z, _time, &_target.z);
}

void LevelManager::CameraEasing(const std::string& _fucName, Vector3& _target, const Vector3& _start, const Vector3& _end, float _time)
{
	mpEasingFunc->EasingFuncMap[_fucName](_start.x, _end.x, _time, &_target.x);
	mpEasingFunc->EasingFuncMap[_fucName](_start.y, _end.y, _time, &_target.y);
	mpEasingFunc->EasingFuncMap[_fucName](_start.z, _end.z, _time, &_target.z);
}

Camera* LevelManager::GetWorldCamera()
{
	auto view = mRegistry.view<CameraComponent>();
	for (auto& entity : view)
	{
		auto& cameraComp = mRegistry.get<CameraComponent>(entity);
		if (cameraComp.m_cameraEnum == 0)
		{
			return cameraComp.m_pCamera;
		}
	}

	return nullptr;
}

void LevelManager::FadePreSetting(bool _mIsFadeOut)
{
	auto view = mRegistry.view<FadeInOut>();
	for (auto& entity : view)
	{
		auto& name = mRegistry.get<Name>(entity).m_name;
		auto& img = mRegistry.get<FadeInOut>(entity);
		if (name == "FaidInOut")
		{
			if (img.state == FadeInOutState::NONE)
			{
				if (_mIsFadeOut == true)
				{
					img.mAlpha = 1.0f;
					img.mTime = 0.0f;
					img.state = FadeInOutState::FADE_OUT;
				}
				else
				{
					img.mAlpha = 0.0f;
					img.state = FadeInOutState::FADE_IN;
				}
			}
			else
			{
				break;
			}
		}
	}
}

void LevelManager::FadeInScreen(float _time)
{
	auto view = mRegistry.view<FadeInOut>();
	for (auto& entity : view)
	{
		auto& name = mRegistry.get<Name>(entity).m_name;
		auto& img = mRegistry.get<FadeInOut>(entity);
		if (name == "FaidInOut")
		{
			img.IsFadingInFin(_time);
		}
	}
}

void LevelManager::FadeOutScreen(float _time)
{
	auto view = mRegistry.view<FadeInOut>();
	for (auto& entity : view)
	{
		auto& name = mRegistry.get<Name>(entity).m_name;
		auto& img = mRegistry.get<FadeInOut>(entity);
		if (name == "FaidInOut")
		{
			img.IsFadingOutFin(_time, 1.f);
		}
	}
}

void LevelManager::Finalize()
{
	// 	delete mpEasingFunc;

	mCurrentSceneUID = -1;
	mMapDataVec.clear();
	mEnemyDataVec.clear();
}

void LevelManager::AddCamera(const Vector3& _pos, const Vector3& _viewDir)
{
	auto camera = mpEntityManager->CreateEntity("Camera");
	camera->AddComponent<CameraComponent>(mpRenderManager->GetScreenWidth(), mpRenderManager->GetScreenHeight()
		, 10.f, _pos, _viewDir, Vector3(0.f, 1.f, 0.f), Vector3(70.f, 0.1f, 10000.f), static_cast<unsigned int>(cameraEnum::WorldCamera));
	camera->GetComponent<CameraComponent>().m_pCamera->mIsFirstPersonMode = false;

	auto& cameraObj = camera->GetComponent<CameraComponent>().m_pCamera;

	auto& nearZ = camera->GetComponent<CameraComponent>().m_pCamera->mNearZ;
	auto& farZ = camera->GetComponent<CameraComponent>().m_pCamera->mFarZ;
}

std::vector<EnemyData> LevelManager::GetSceneEnemyDataVec()
{
	return mEnemyDataVec;
}

void LevelManager::DeleteMoney()
{
	// 동전 모두 삭제!
	auto moneyView = mRegistry.view<MoneyComponent>();
	for (auto& entity : moneyView)
	{
		auto& money = mRegistry.get<MoneyComponent>(entity);
		mpEntityManager->RemoveEntity(money.m_pOwner->GetUID());
	}

	mpUnitSystem->GetProjectileSystem()->GetMoneyPosVec()->clear();
}

void LevelManager::EnemyUpdate(float _dTime)
{
	auto enemyView = mRegistry.view<EnemyComponent>();
	for (auto& entity : enemyView)
	{
		auto& enemyComp = mRegistry.get<EnemyComponent>(entity);

		//mpUnitSystem->GetPlayerSystem()->EnemyUnitUpdate(m_pEntityManager->GetEntity(entity), _dTime); // 적군 유닛을 업데이트 합니다.
		mpUnitSystem->GetPlayerSystem()->EnemyUnitUpdate(enemyComp.m_pOwner, _dTime); // 적군 유닛을 업데이트 합니다.

		physx::PxFilterData filterData = mpPhysicsManager->GetFilterData(entity);
		filterData.word1 |= 8;
		mpPhysicsManager->SetFilterData(entity, filterData);
	}
}

void LevelManager::PlayerUpdate(float _dTime)
{
	auto playerView = mRegistry.view<PlayerComponent>();
	for (auto& entity : playerView)
	{
		auto& playerComp = mRegistry.get<PlayerComponent>(entity);

		// 플레이어 관련하여 업데이트한다.
		mpUnitSystem->GetPlayerSystem()->PlayerUnitUpdate(mpEntityManager->GetEntity(entity), _dTime); // 아군 유닛을 업데이트 합니다.

		physx::PxFilterData filterData = mpPhysicsManager->GetFilterData(entity);
		filterData.word1 |= 8;
		mpPhysicsManager->SetFilterData(entity, filterData);
	}
}

void LevelManager::DeleteEnemyAll()
{
	// 무기를 가진 엔티티를 먼저 삭제한다.
	auto weaponView = mRegistry.view<BoxCollider, Transform>();
	for (auto& weaponEntity : weaponView)
	{
		auto& transform = mRegistry.get<Transform>(weaponEntity);
		if (transform.m_pParent)
		{
			auto& name = mRegistry.get<Name>(weaponEntity).m_name;
			auto& parentTrs = transform.m_pParent;
			auto& name2 = mRegistry.get<Name>(static_cast<entt::entity>(parentTrs->m_pOwner->GetUID())).m_name;
			if (parentTrs->m_pOwner->HasComponent<WeaponComponent>() && parentTrs->m_pOwner->HasComponent<EnemyComponent>())
			{
				mpEntityManager->RemoveEntity(static_cast<UID>(weaponEntity));
			}
		}
	}

	// 적군 컴포넌트를 가진 모든 엔티티를 삭제한다.
	auto playerView = mRegistry.view<EnemyComponent>();
	for (auto& playerEntity : playerView)
	{
		mpEntityManager->RemoveEntity(static_cast<UID>(static_cast<UID>(playerEntity)));
	}
}

void LevelManager::TutorialClassDeploy()
{
	if (mpInputManager->GetKeyUp(KEY::LBUTTON))
	{
		// 근거리 직업 용병 버튼 활성화
		auto uiView = mRegistry.view<Button>();
		for (auto& uiEntity : uiView)
		{
			auto& name = mRegistry.get<Name>(uiEntity).m_name;
			auto& button = mRegistry.get<Button>(uiEntity);
			auto& img = mRegistry.get<Texture2D>(uiEntity);

			if (name == "UI_D_Class")
			{
				if (mpUIManager->GetButtonState(button.m_pOwner) == ButtonState::PRESSED)
				{
					// 					mpSoundManager->PlaySFX("Snd_sfx_ClickBtn");
					if (mpInputManager->GetKeyUp(KEY::LBUTTON))
					{
						if (img.m_file == "UI_btn_Merc01.png")
						{
							mClassName = "Mercenary";
							mIsClassButtonPressed = true;
						}
					}
				}

				if (mIsClassButtonPressed == true)
				{
					if (mTempUnitUID == -1)
					{
						// 유저가 배치하는 유닛을 생성한다.
						auto userPlaceUnit = mpEntityManager->CreateEntity("player");
						mTempUnitUID = userPlaceUnit->GetUID();
						mpUnitSystem->GetPlayerSystem()->AddDeplyUnit(userPlaceUnit, mClassName);
						userPlaceUnit->GetComponent<Transform>().m_localPosition = Vector3(-15.0f, 2, 0.0f);
						mpRenderManager->InitailizeEntity(userPlaceUnit);
						// 유저가 배치를 시작할 것임을 알린다.
						userPlaceUnit->GetComponent<PlayerComponent>().mIsDeploy = true;

						mTutorialFlag = true;
						break;
					}
				}
			}
		}
	}
}

void LevelManager::TutorialClassDeployed(Vector3& _cursorPos, bool _isPayment, SceneData* _pSceneData)
{
	auto playerView = mRegistry.view<PlayerComponent>();
	for (auto playerEntity : playerView)
	{
		auto& player = mRegistry.get<PlayerComponent>(playerEntity);
		auto& transform = mRegistry.get<Transform>(playerEntity);
		auto& owner = player.m_pOwner;

		// 1. 배치 중일 때만 마우스에 따라 위치 이동
		if (player.mIsDeploy == true)
		{
			if (_cursorPos.x >= mStartPoint.x && _cursorPos.x <= mEndPoint.x && _cursorPos.z >= mStartPoint.y && _cursorPos.z <= mEndPoint.y)
			{
				transform.m_localPosition = Vector3(static_cast<int>(_cursorPos.x), _cursorPos.y, static_cast<int>(_cursorPos.z));
				if (mIsOneDeploySet == true)
				{
					mIsOneDeploySet = false;
				}

				// 2. 먼저 마우스 버튼이 올라간 경우 배치함
				if (mpInputManager->GetKeyDown(KEY::LBUTTON))
				{
					// 눌렀을 때의 마우스 좌표를 기록한다.
					Vector2 tempMousePos = mpInputManager->GetMousePos(); /// 패널 입력을 막는 부분. 아마 이 전체 시스템의 가장 상단에 위치하게 될 것이나, 현재는 여기에 둔다.
					// 눌렀을 때의 마우스 좌표가 패널 바깥일 때만 배치된다.
					if (AddBlockScreenPos(tempMousePos, Vector2(0, 955), Vector2(1920, 80)) == true) // 패널 창 바깥쪽을 입력했을 경우만 처리한다.
					{
						if (IsCanClickAreaXZ(_cursorPos, Vector2(-15, 10), Vector2(-9, -10), _pSceneData->m_aliveAlly) == true)
						{
							// 무기를 쥐어준다.
							mpUnitSystem->GetEquipmentManager()->AddWeapon(mpEntityManager->GetEntity(playerEntity), mClassName);
							mpUnitSystem->GetPlayerSystem()->AddPhysics(owner);
							// 배치가 완료됐음을 표시한다.
							//mTempUnitUID = -1; // 또 배치할 게 아니기 때문에 여기서 리셋 안한다. (1회용 배치 튜토용)
							mIsOneDeploySet = true;  // 배치 완료 후 상태 변경
							player.mIsDeploy = false;
							_pSceneData->m_totalAlly++;
							(*mpUnitSystem->GetPlayerSystem()->GetUnitDeployPosVec())[_pSceneData->m_aliveAlly] = Vector2(static_cast<int>(_cursorPos.x), static_cast<int>(_cursorPos.z));
							_isPayment = true;
							_pSceneData->m_aliveAlly++;
							mpSoundManager->PlaySFX("Snd_sfx_DeployO");

							mTutorialFlag = true;

							// 근거리 직업 용병 버튼 비활성화
							auto uiView = mRegistry.view<Button>();
							for (auto& uiEntity : uiView)
							{
								auto& name = mRegistry.get<Name>(uiEntity).m_name;
								auto& button = mRegistry.get<Button>(uiEntity);
								auto& texture = mRegistry.get<Texture2D>(uiEntity);

								if (name == "UI_D_Class")
								{
									if (texture.m_file == "UI_btn_Merc01.png")
									{
										button.mIsEnable = false;
									}
								}
							}
							return;
						}
					}
					else
					{
						break;
					}
				}
			}
		}

	}
}

void LevelManager::AllDeploy(Vector3& _cursorPos, bool _isPayment, SceneData* _pSceneData)
{
	/// UI
		// UI 시작 버튼 - 누른 후 사라짐
	auto uiView = mRegistry.view<Button>();
	for (auto& uiEntity : uiView)
	{
		auto& name = mRegistry.get<Name>(uiEntity).m_name;
		auto& button = mRegistry.get<Button>(uiEntity);
		auto& texture = mRegistry.get<Texture2D>(uiEntity);

		// UI 클래스 버튼
		if (name == "UI_D_Class")
		{
			auto msg2D = mRegistry.try_get<MessageBox2D>(uiEntity);

			if (mpUnitSystem->GetPlayerSystem()->mMaxSetUnitNum > _pSceneData->m_aliveAlly)
			{
				//button.mIsEnable = true;

				if (mpUIManager->GetButtonState(button.m_pOwner) == ButtonState::PRESSED)
				{
					// 					mpSoundManager->PlaySFX("Snd_sfx_ClickBtn");
					if (mpInputManager->GetKeyUp(KEY::LBUTTON))
					{
						// 이미 생성한 UID가 같으면 스킵!
						if (texture.m_file == "UI_btn_Merc01.png")
						{
							mClassName = "Mercenary";
							mIsClassButtonPressed = true;
						}
						else if (texture.m_file == "UI_btn_Merc02.png")
						{
							mClassName = "Archer";
							mIsClassButtonPressed = true;
						}
					}
				}


				if (mIsClassButtonPressed == true && mIsGameStart == false)
				{
					if (mTempUnitUID == -1)
					{
						// 유저가 배치하는 유닛을 생성한다.
						auto userPlaceUnit = mpEntityManager->CreateEntity("player");
						mTempUnitUID = userPlaceUnit->GetUID();
						mpUnitSystem->GetPlayerSystem()->AddDeplyUnit(userPlaceUnit, mClassName);
						userPlaceUnit->GetComponent<Transform>().m_localPosition = Vector3(-15.0f, 2, 0.0f);
						mpRenderManager->InitailizeEntity(userPlaceUnit);
						// 유저가 배치를 시작할 것임을 알린다.
						userPlaceUnit->GetComponent<PlayerComponent>().mIsDeploy = true;
					}
					else
					{
						mpUnitSystem->GetPlayerSystem()->ChangeUnit(mpEntityManager->GetEntity(mTempUnitUID), mClassName);
						mpEntityManager->GetEntity(mTempUnitUID)->GetComponent<Transform>().m_localPosition = Vector3(-15.0f, 2, 0.0f);
						mpRenderManager->InitailizeEntity(mpEntityManager->GetEntity(mTempUnitUID));
					}
				}
			}
			else
			{
				mIsClassButtonPressed = false; // 버튼이 눌린 상태 삭제
				texture.m_isVisible = true;
				button.mIsEnable = false;
				mTutorialFlag = true; // 20마리를 배치했으면 플래그를 활성화하고, 이게 활성화되면 리셋 버튼은 더 못 누르게 막음
			}

			if (msg2D)
			{
				// 호버링 시 설명 문구가 뜨게 한다.
				if (mpUIManager->GetButtonState(button.m_pOwner) == ButtonState::HOVERED)
				{
					if (texture.m_file == "UI_btn_Merc01.png")
					{
						msg2D->mIsImgVisible = true;
						msg2D->mIsTextVisible = true;
					}
					else if (texture.m_file == "UI_btn_Merc02.png")
					{
						msg2D->mIsImgVisible = true;
						msg2D->mIsTextVisible = true;
					}
				}
				else
				{
					msg2D->mIsImgVisible = false;
					msg2D->mIsTextVisible = false;
				}
			}

		} //[End] : UI 클래스 버튼 관련 코드 종료
		// 배치 롤백 버튼
		if (name == "UI_D_Reset" && mTutorialFlag == false)
		{
			if (mpUIManager->GetButtonState(button.m_pOwner) == ButtonState::PRESSED)
			{
				if (mpInputManager->GetKeyUp(KEY::LBUTTON))
				{
					if (_pSceneData->m_aliveAlly != 0)
					{
						ResetPlayer(_pSceneData);
						mIsClassButtonPressed = false; // 버튼이 눌린 상태 삭제
						mTempUnitUID = -1;
						mpSoundManager->PlaySFX("Snd_sfx_RollbackAll");
					}
					else
					{
						mpSoundManager->PlaySFX("Snd_sfx_DeployX");
					}

					return;
				}
			}
		}
	}

	auto playerView = mRegistry.view<PlayerComponent>();
	for (auto playerEntity : playerView)
	{
		auto& player = mRegistry.get<PlayerComponent>(playerEntity);
		auto& transform = mRegistry.get<Transform>(playerEntity);
		auto& owner = player.m_pOwner;

		// 1. 배치 중일 때만 마우스에 따라 위치 이동
		if (player.mIsDeploy == true)
		{
			if (_cursorPos.x >= mStartPoint.x && _cursorPos.x <= mEndPoint.x && _cursorPos.z >= mStartPoint.y && _cursorPos.z <= mEndPoint.y)
			{
				transform.m_localPosition = Vector3(static_cast<int>(_cursorPos.x), _cursorPos.y, static_cast<int>(_cursorPos.z));
				if (GetmIsOneDeploySet() == true)
				{
					mIsOneDeploySet = false;
				}

				// 2. 먼저 마우스 버튼이 올라간 경우 배치함
				if (mpInputManager->GetKeyDown(KEY::LBUTTON))
				{
					// 눌렀을 때의 마우스 좌표를 기록한다.
					Vector2 tempMousePos = mpInputManager->GetMousePos(); /// 패널 입력을 막는 부분. 아마 이 전체 시스템의 가장 상단에 위치하게 될 것이나, 현재는 여기에 둔다.
					// 눌렀을 때의 마우스 좌표가 패널 바깥일 때만 배치된다.
					if (AddBlockScreenPos(tempMousePos, Vector2(0, 955), Vector2(1920, 80)) == true) // 패널 창 바깥쪽을 입력했을 경우만 처리한다.
					{
						if (IsCanClickAreaXZ(_cursorPos, Vector2(-15, 10), Vector2(-9, -10), _pSceneData->m_aliveAlly) == true)
						{
							// 무기를 쥐어준다.
							mpUnitSystem->GetEquipmentManager()->AddWeapon(owner, mClassName);
							mpUnitSystem->GetPlayerSystem()->AddPhysics(owner);
							// 배치가 완료됐음을 표시한다.
							mTempUnitUID = -1;
							mIsOneDeploySet = true;
							player.mIsDeploy = false;
							_pSceneData->m_totalAlly++;
							(*mpUnitSystem->GetPlayerSystem()->GetUnitDeployPosVec())[_pSceneData->m_aliveAlly] = Vector2(static_cast<int>(_cursorPos.x), static_cast<int>(_cursorPos.z));
							_isPayment = true;
							_pSceneData->m_aliveAlly++;
							mpSoundManager->PlaySFX("Snd_sfx_DeployO");
						}
					}
					else
					{
						break;
					}
				}
			}
		}
	}

	auto textView = mRegistry.view<Text>();
	for (auto text : textView)
	{
		auto& name = mRegistry.get<Name>(text).m_name;
		auto& textComp = mRegistry.get<Text>(text);

		if (name == "UI_B_Money")
		{
			if (_isPayment == true)
			{
				_pSceneData->m_heldMoney -= 100;
				_isPayment == false;
			}
			textComp.m_num1 = _pSceneData->m_heldMoney;
		}
	}
}

bool LevelManager::GetIsGameStart()
{
	return mIsGameStart;
}

void LevelManager::SetIsGameStart(bool _isGameStart)
{
	mIsGameStart = _isGameStart;
}

bool LevelManager::GetmIsOneDeploySet()
{
	return mIsGameStart;
}

void LevelManager::SetmIsOneDeploySet(bool _isOneDeploySet)
{
	mIsOneDeploySet = _isOneDeploySet;
}

bool LevelManager::GetIsClassButtonPressed()
{
	return mIsOneDeploySet;
}

void LevelManager::SetIsClassButtonPressed(bool _isClassButtonPressed)
{
	mIsClassButtonPressed = _isClassButtonPressed;
}

UID LevelManager::GetTempUnitUID()
{
	return mTempUnitUID;
}

void LevelManager::SetTempUnitUID(UID _tempUnitID)
{
	mTempUnitUID = _tempUnitID;
}

Vector2 LevelManager::GetStartPoint()
{
	return mStartPoint;
}

Vector2 LevelManager::GetEndPoint()
{
	return mEndPoint;
}

std::string LevelManager::GetUnitClassName()
{
	return mClassName;
}

void LevelManager::SetClassName(const std::string& _className)
{
	mClassName = _className;
}

void LevelManager::AddFireCracker()
{
	auto fireCracker = mpEntityManager->CreateEntity("FireCracker");
	fireCracker->AddComponent<ParticleComponent>(
		Vector3(-20, 0, 5)	// 위치
		, 0					// playtime
		, Vector3(1)		// 색상
		, 1.0					// 총 시간
		, Vector3(0, 0, 0)	// 방향
		, 10					// 속도
		, Vector3()			// 회전값
		, 1					// 투명도
		, Vector3(0.2)		// 스케일
		, 0					// 투명도 변화량
		, Vector3()			// 회전값 변화량
		, 0					// 스케일 랜덤값
		, Vector3(-0.2)		// 스케일 변화량
		, Vector3(1)		// 위치 랜덤값
		, Vector3()			// 회전 랜덤값
		, Vector3(1)		// 방향 랜덤값
		, Vector3()			// 색상 변화량
		, 0					// 중력 계수
		, Vector3(1));		// 색상 랜덤
}

CameraPosition LevelManager::GetCameraPosInfo()
{
	return mCameraPosInfo;
}

void LevelManager::AddSmog()
{
	auto fireCracker = mpEntityManager->CreateEntity("Smog");
	fireCracker->AddComponent<ParticleComponent>(
		Vector3(0, 2, 0)	// 위치
		, 0					// playtime
		, Vector3(0.3)		// 색상
		, 3.0				// 총 시간
		, Vector3(0, 1, 0)	// 방향
		, 1					// 속도
		, Vector3()			// 회전값
		, 0.5				// 투명도
		, Vector3(0.4)		// 스케일
		, 0					// 투명도 변화량
		, Vector3()			// 회전값 변화량
		, 0.5					// 스케일 랜덤값
		, Vector3(0.1)		// 스케일 변화량
		, Vector3(3, 0, 3)	// 위치 랜덤값
		, Vector3()			// 회전 랜덤값
		, Vector3()			// 방향 랜덤값
		, Vector3()			// 색상 변화량
		, 0					// 중력 계수
		, Vector3());		// 색상 랜덤
}

void LevelManager::SmogUpdate(float _dTime)
{
	static float dTime;
	dTime += _dTime;
	if (dTime >= 1.5)
	{
		dTime -= 1.5;
		auto partView = mRegistry.view<ParticleComponent>();
		for (auto& entity : partView)
		{
			auto& name = mRegistry.get<Name>(entity).m_name;
			auto& particle = mRegistry.get<ParticleComponent>(entity);
			//particle.m_pParticleData->position = Vector3(-40, RandomUtil::RandomInt(3, 10), RandomUtil::RandomInt(-20, 20));
			if (name == "Smog")
			{
				mpRenderManager->AddParticle(3, *particle.m_pParticleData);
			}
		}
	}

}
