#include "ExampleScene.h"
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
#include "MessageBox2D.h"
#include "SceneNumEnum.h"
#include "LayerEnum.h"
#include "FadeInOut.h"
#include "ShrinkRectComponent.h"

ExampleScene::ExampleScene(entt::registry& _registry, const std::string& _name
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

	mTutorialTextVec = m_pResourceManager->ParseCSV<TutotialTextTable>("TutorialText2.csv");
	mTextIdx = 0;
	mTutorialState = TutorialState::FINAL;
	mTime = 0.0f;
}

bool ExampleScene::Initialize()
{
	/// 리소스 추가
	AddResource();
	m_pRenderManager->SetCubeMap("Atrium_diffuseIBL.dds", "Atrium_specularIBL.dds");

	// 씬데이터 생성
	m_pSceneData = new SceneData();

	/// mpAstar 초기화 : 원래는 맵 매니저 안에 있는 게 좋지만 현재 레벨 매니저가 상당 부분 처리하고 있어서 밖으로 따로 빼둔 상태다.
	// 추후 시간이 남을 때 예쁜 구조로 바꿔보자.
	mpAstar = new AStar();
	std::vector<std::vector<int>>* astarMap = new std::vector<std::vector<int>>();
	*astarMap = m_pResourceManager->ParseMapCSV("Stage1.csv");
	mpAstar->Initialize(Vector2(-15.0f), Vector2(15.0f), astarMap);
	mStartPoint = mpAstar->GetMapStartPoint();
	mEndPoint = mpAstar->GetMapEndPoint();

	/// 매니저 및 시스템 초기화
	// 유닛 시스템 설정(투사체, 플레이어, NPC 시스템 및 장비 매니저 초기화)
	mpUnitSystem = new UnitSystem(m_registry, m_pEntityManager, m_pPhysicsManager, m_pInputManager, m_pWorldManager
		, m_pRenderManager, m_pResourceManager, m_pEventManager, m_pUIManager, m_pSoundManager, mpAstar);
	mpUnitSystem->Initialize();

	mpLevelManager = new LevelManager(m_registry, m_pRenderManager, m_pPhysicsManager, m_pInputManager, m_pUIManager, m_pEntityManager
		, m_pResourceManager, m_pSoundManager, mpUnitSystem, mpAstar);
	mpLevelManager->Initialize(GetUID());
	mpLevelManager->SetGameState(GameState::PRE_PLACEMENT);

	/// 텍스트 인덱스 초기화
	mTextIdx = 0;

	/// 기본적인 환경 설정
	// 카메라 엔티티 생성, 세팅
	mpLevelManager->AddCamera(Vector3(-12.f, 4.0f, 3.35f), Vector3(-1.f, -0.2f, 0.0f));
	// 라이트 세팅
	mpLevelManager->AddLight();
	// 큐브맵 세팅
	mpLevelManager->AddCubeMap(1);

	auto camera = mpLevelManager->GetWorldCamera();
	camera->SetEyePos(Vector3(-15.42, 18.06, -17.72));
	camera->SetDirection(Vector3(0.57735, -0.57735, 0.57735));
	m_pRenderManager->CameraSetOrthographic(0.03);

	/// 오브젝트 생성
	// 투명 벽 배치(원거리 무기 삭제)
	mpLevelManager->AddPlaneNWall();
	// 바닥 좌표계 배치
	mpLevelManager->DrawCoordinatesXZ(true);
	// 인디케이터 타일 생성
	mpLevelManager->AddSquareAtPointer("blue.png", "red.png");
	// 맵 데이터 추가[기획의 씬 넘버]
	mpLevelManager->AddMapData(1);
	// 유닛세팅 --------------------------------------------------------
	// 적군 유닛 세팅
	mpUnitSystem->GetPlayerSystem()->AddSceneEnemyUnits(0);

	// NPC 세팅
	mpUnitSystem->GetNPCSystem()->AddNPC("Chief"); // 아군 용병왕
	mpUnitSystem->GetNPCSystem()->AddNPC("RVC_C"); // 적군 용병왕
	//
	auto ui1 = m_pEntityManager->CreateEntity("UI_T");
	m_pUIManager->AddUI(ui1, "UI_pnl_MsgBoxClick1.png", Vector2(193, 489), Vector2(432, 210), static_cast<int>(Layer::COM_B), Vector4(1.0f));
	m_pUIManager->AddText(ui1, u8"안녕, 나는 리치 러커스라고 해", "KIMM_Bold(60).ttf", Vector2(219, 554), static_cast<int>(Layer::COM_B), true, Vector4(1.0f));
	ui1->GetComponent<Text>().m_scale = 0.33f;

	auto ui2 = m_pEntityManager->CreateEntity("UI_T_Skip");
	m_pUIManager->AddUI(ui2, "UI_btn_Skip.png", Vector2(1812, 997), Vector2(83, 58), static_cast<int>(Layer::COM_B), Vector4(1.0f));
	m_pUIManager->AddButtonAllColor(ui2, Vector4(0.5f, 0.5f, 0.5f, 1.0f), Vector4(1.0f));

	auto ui3 = m_pEntityManager->CreateEntity("UI_PointMoney");
	m_pUIManager->AddUI(ui3, "UI_PointMoney.png", Vector2(915, 490), Vector2(90, 100), static_cast<int>(Layer::EFFECT), Vector4(1.0f), false);

	auto ui4 = m_pEntityManager->CreateEntity("UI_H"); // 배치를 돕는 영역
	ui4->AddComponent<Transform>(Vector3(5.f, 0.f, 0.f), Vector3(0.f), Vector3(0.7f, 0.005f, 0.7f));
	ui4->AddComponent<MeshRenderer>("box.fbx", "Cube.001");
	ui4->AddComponent<Texture3D>("White.png");
	ui4->AddComponent<AlphaBlendComponent>(0.2);
 	ui4->AddComponent<OutlineComponent>(1.05, Vector3(0, 0, 1));

	// 장애물 엔티티 -> 초반에 나왔다가 바로 제거될 녀석
	auto ui5 = m_pEntityManager->CreateEntity("UI_H_Obstacle"); // 배치를 돕는 영역
	ui5->AddComponent<Transform>(Vector3(5.f, 0.f, 0.f), Vector3(0.f), Vector3(0.5f, 1.0f, 0.5f));
	ui5->AddComponent<MeshRenderer>("box.fbx", "Cube.001");
	ui5->AddComponent<Texture3D>("Log.png");

	// 사각형 이펙트 엔티티
	auto effectRect = m_pEntityManager->CreateEntity("UI_Effect_Rect");
	m_pUIManager->AddRect(effectRect, Vector2(925, 410), Vector2(200.f), static_cast<int>(Layer::COM_M), Vector4(0.0f), 8.0f, Vector4(1.0f, 0.38f, 0.0f, 1.0f));
	effectRect->AddComponent<ShrinkRectComponent>(Vector2(200.f), Vector2(100.f), 20);
	effectRect->GetComponent<Box2D>().mIsVisible = false;
	mIsReadyResetShrinkRect = true;

	/*auto ui5 = m_pEntityManager->CreateEntity("sssss");
	m_pUIManager->AddText(ui5, u8"안녕,\n 나는\n리치 \n러커스라고\n 해", "KIMM_Bold(60).ttf", Vector2(0, 0), static_cast<int>(Layer::COM_B), true);
	ui1->GetComponent<Text>().m_scale = 0.7f;*/

	/// UI
	mpLevelManager->AddBasicUI();
	mpLevelManager->AddBattleUI(dynamic_cast<SceneData*>(m_pSceneData));
	mpLevelManager->AddStageNumAndObjects(u8"튜토리얼");
	mpLevelManager->AddClassUI(0, true); // UI 클래스 버튼
	mpLevelManager->AddVerifyPopUpUI();
	mpLevelManager->AddAnimationUI(true, static_cast<int>(m_pWorldManager->GetCurrentWorld()->GetPreviousScene()));

	/// 튜토리얼 상태 정의
	mTutorialState = TutorialState::CLOUD;

	/// UI 애니메이션 상태 정의
	mpLevelManager->SetUIAnimationState(UIAnimationState::PRETUTORIAL);

	/// Sound
	m_pSoundManager->StopBGM();
	m_pSoundManager->PlayBGM("Snd_bgm_BeforeBattle");
	return Scene::Initialize();
}

void ExampleScene::FixedUpdate(float _dTime)
{
	// play가 진행되는 부분을 추가해서 업데이트 합니다.
	if (mTutorialState == TutorialState::THROW
		/*|| mTutorialState == TutorialState::ALLY_Play
		|| mTutorialState == TutorialState::BUFF*/
		|| mpLevelManager->GetGameState() == GameState::PLAY)
	{
		auto playerView = m_registry.view<PlayerComponent>();
		auto enemyView = m_registry.view<EnemyComponent>();
		for (auto& entity : playerView)
		{
			mpUnitSystem->GetPlayerSystem()->FixedUpdate(m_pEntityManager->GetEntity(entity), _dTime); // 아군 유닛을 업데이트 합니다.
		}

		for (auto& entity : enemyView)
		{
			mpUnitSystem->GetPlayerSystem()->FixedUpdate(m_pEntityManager->GetEntity(entity), _dTime); // 적군 유닛을 업데이트 합니다.
		}

		//mpUnitSystem->GetProjectileSystem()->FixedUpdate(_dTime);
	}

	if (mTutorialState == TutorialState::ALLY_Play
		|| mTutorialState == TutorialState::BUFF)
	{
		auto playerView = m_registry.view<PlayerComponent>();
		for (auto& entity : playerView)
		{
			mpUnitSystem->GetPlayerSystem()->FixedUpdate(m_pEntityManager->GetEntity(entity), _dTime); // 아군 유닛을 업데이트 합니다.
		}
		//mpUnitSystem->GetProjectileSystem()->FixedUpdate(_dTime);
	}

	mpUnitSystem->GetProjectileSystem()->FixedUpdate(_dTime);

}

void ExampleScene::Update(float _dTime)
{
	SceneData* pSceneData = dynamic_cast<SceneData*>(m_pSceneData);

	//mpLevelManager->BasicUIUpdate(); 세팅창 사용 불가

	auto skipView = m_registry.view<Button>();
	for (auto& entity : skipView)
	{
		auto& name = m_registry.get<Name>(entity).m_name;
		auto& button = m_registry.get<Button>(entity);

		if (name == "UI_T_Skip")
		{
			if (m_pUIManager->GetButtonState(m_pEntityManager->GetEntity(entity)) == ButtonState::PRESSED)
			{
				mpLevelManager->SetPopUpState(PopUpState::OPEN_SKIP);
				break;
			}
		}
	}

	mpLevelManager->PopUPUITutorial();

	if (mpLevelManager->mIsGoMain == true)
	{
		mpLevelManager->mIsGoMain == false;
		m_pWorldManager->GetCurrentWorld()->SetScene(static_cast<uint32_t>(SceneName::MAIN));
		return;
	}
	else if (mpLevelManager->mIsSkip == true)
	{
		mpLevelManager->mIsSkip = false;
		// 혹시라도 있을 지시 사각형 안 보이게
		auto shrinkRectView = m_registry.view<ShrinkRectComponent>();
		for (auto& entity : shrinkRectView)
		{
			auto box2D = m_registry.try_get<Box2D>(entity);
			box2D->mIsVisible = false;
		}
		mTutorialState = TutorialState::END;
		mpLevelManager->SetGameState(GameState::TUTORIALEND);
		mpLevelManager->SetUIAnimationState(UIAnimationState::POSTBATTLE);
		//m_pWorldManager->GetCurrentWorld()->SetScene(static_cast<uint32_t>(SceneName::SCENE1));
		return;
	}

	// 랭크바
	mpLevelManager->UpdateRankBar(pSceneData);

	/// 플레이어 시스템 업데이트
	switch (mpLevelManager->GetGameState())
	{
	case GameState::PRE_PLACEMENT:
	{
		// 적군 최대 수 세기
		int numEnemy = 0;
		auto enemyView = m_registry.view<EnemyComponent>();
		for (auto entity : enemyView)
		{
			numEnemy++;
		}

		/// 배치와 관련된 모든 텍스트와 텍스처 살리기
		// 배치와 관련된 텍스트 보이기
		auto textView = m_registry.view<Text>();
		for (auto uiEntity : textView)
		{
			auto& name = m_registry.get<Name>(uiEntity).m_name;
			auto& text = m_registry.get<Text>(uiEntity);

			if (name.find("UI_D") != std::string::npos ||
				name.find("UI_B") != std::string::npos ||
				name == "UI_C_OutSetting" ||
				name.find("UI_T") != std::string::npos)
			{
				text.m_isVisible = true;

				if (name == "UI_B_Money")
				{
					text.m_num1 = dynamic_cast<SceneData*>(pSceneData)->m_heldMoney;
				}

				if (name == "UI_B_StageGuide")
				{
					text.m_num1 = numEnemy;
					text.m_num2 = numEnemy;
				}

				// 튜토리얼에서 사용하지 않는 것들은 보이지 않게 한다.
				if (name == "UI_D_Guide")
				{
					text.m_isVisible = false;
				}

			}

			// 튜토리얼에서 사용하지 않는 것들은 보이지 않게 한다.
			if (name == "UI_P_Timer")
			{
				text.m_isVisible = false;
			}
		}

		auto msgView = m_registry.view<MessageBox2D>();
		for (auto uiEntity : msgView)
		{
			auto& name = m_registry.get<Name>(uiEntity).m_name;
			auto& msgComp = m_registry.get<MessageBox2D>(uiEntity);

			if (name == "UI_B_Rank_Gauge")
			{
				msgComp.mIsImgVisible = true;
			}
		}

		// 배치와 관련된 텍스처 보이기
		auto textureView = m_registry.view<Texture2D>();
		for (auto uiEntity : textureView)
		{
			auto& name = m_registry.get<Name>(uiEntity).m_name;
			auto& img = m_registry.get<Texture2D>(uiEntity);
			auto button = m_registry.try_get<Button>(uiEntity);

			if (name.find("UI_D") != std::string::npos ||
				name.find("UI_B") != std::string::npos ||
				name == "UI_C_OutSetting" ||
				name.find("UI_T") != std::string::npos)
			{
				img.m_isVisible = true;
				/// 모든 버튼 비활성화 : 설정창 버튼은 사용 가능해야 한다! + 스킵 버튼도 사용 가능해야 한다!
				if (button)
				{
					if (name != "UI_C_OutSetting" && name != "UI_T_Skip")
					{
						button->mIsEnable = false;
					}
				}
			}

			// 튜토리얼에서 사용하지 않는 것들은 보이지 않게 한다.
			if (name == "UI_P_Timer")
			{
				img.m_isVisible = false;
			}
		}
		mpLevelManager->FadePreSetting(false);
		mpLevelManager->SetGameState(GameState::PLACEMENT);
	}
	break;
	case GameState::PLACEMENT:
	{
		// 현재 좌표
		Vector3 cursorFollowPos = m_pPhysicsManager->PickObejct("plane");
		if (cursorFollowPos.x > 0)cursorFollowPos.x += 0.5;
		else cursorFollowPos.x -= 0.5;
		if (cursorFollowPos.z < 0) cursorFollowPos.z -= 0.5;
		bool isPayment = false; /// 이거 위치는 조정해야 될 거다!

		/// 튜토리얼 텍스트 출력
		auto view2 = m_registry.view<Text>();
		for (auto& entity : view2)
		{
			auto& name = m_registry.get<Name>(entity).m_name;
			auto& text = m_registry.get<Text>(entity);
			auto img = m_registry.try_get<Texture2D>(entity);

			if ((name == "UI_T"))
			{
				if (mTutorialTextVec[mTextIdx].idx <= 1) // 리치 러커스용
				{
					text.m_position = Vector2(219, 554);
					if (mTutorialTextVec[mTextIdx].idx == 0)
					{
						img->m_file = "UI_pnl_MsgBoxClick1.png";
					}
					else
					{
						img->m_file = "UI_pnl_MsgBox1.png";
					}
					img->m_position = Vector2(193, 489);
					img->m_size = Vector2(432, 210);
				}
				else
				{
					text.m_position = Vector2(392, 176);
					if (mTutorialTextVec[mTextIdx].idx == 2)
					{
						img->m_file = "UI_pnl_MsgBoxClick2.png";
					}
					else
					{
						img->m_file = "UI_pnl_MsgBox2.png";
					}
					img->m_position = Vector2(367, 112);
					img->m_size = Vector2(1182, 170);
				}
				text.m_text = mTutorialTextVec[mTextIdx].text;
			}

			// 플레이어의 소지금도 언제나 업데이트 한다.
			if (name == "UI_B_Money")
			{
				text.m_num1 = pSceneData->m_heldMoney;
			}

			// 배치한 아군의 수를 업데이트한다.
			if (name == "UI_D_AliveNum")
			{
				if (9 < text.m_num1 && text.m_num1 < 20)
				{
					text.m_text = u8"%d";
					text.m_position.x = 867.f;
				}
				else if (text.m_num1 == 20)
				{
					text.m_text = u8"%d";
					text.m_position.x = 852.f;
				}
				else
				{
					text.m_text = u8"0%d";
					text.m_position.x = 852.f;
				}
				text.m_num1 = pSceneData->m_aliveAlly;
			}
		}

		/// 여기서 용병왕을 업데이트 한다.
		auto npcView = m_registry.view<NPCComponent>();
		for (auto& entity : npcView)
		{
			mpUnitSystem->GetNPCSystem()->TutorialUpdate(m_pEntityManager->GetEntity(entity), mTextIdx);
		}

		/// 땅에 떨어진 동전 업데이트
		mpUnitSystem->GetProjectileSystem()->TutirialUpdateMoney(_dTime);

		switch (mTutorialState)
		{
		case TutorialState::CLOUD:
		{
			mpLevelManager->UIAniUpdate(_dTime);
			if (mpLevelManager->GetUIAnimationState() == UIAnimationState::NORMAL)
			{
				mTutorialState = TutorialState::FADE;
			}
		}
		break;
		case TutorialState::FADE:
		{
			/*mpLevelManager->FadeInScreen(_dTime);

			auto view = m_registry.view<FadeInOut>();
			for (auto& entity : view)
			{
				auto& name = m_registry.get<Name>(entity).m_name;
				auto& img = m_registry.get<FadeInOut>(entity);
				if (name == "FaidInOut")
				{
					if (img.IsFadingInFin(_dTime) == true)
					{
						mTutorialState = TutorialState::INTRO;
						return;
					}
				}
			}*/

			uint8_t numEnemy = 0;
			auto enemyView = m_registry.view<EnemyComponent>();
			for (auto& entity : enemyView)
			{
				auto& name = m_registry.get<Name>(entity).m_name;
				auto& enemyComp = m_registry.get<EnemyComponent>(entity);


				if (enemyComp.mIsDead == false)
				{
					numEnemy++;
				}
			}

			auto view2 = m_registry.view<Text>();
			for (auto& entity : view2)
			{
				auto& name = m_registry.get<Name>(entity).m_name;
				auto& text = m_registry.get<Text>(entity);
				if (name == "UI_B_StageGuide")
				{
					text.m_num1 = numEnemy;
				}
			}

			mTutorialState = TutorialState::INTRO;
		}
		break;
		case TutorialState::INTRO: // 글만 자동으로 넘어갑니다.
		{
			SettingPlayingIndicator();

			PrintIntroText(TutorialState::INTRO);	// 튜토리얼 텍스트 삽입

			if (mTextIdx == 3)
			{
				mpUnitSystem->GetProjectileSystem()->Update(_dTime);

				auto shrinkRectView = m_registry.view<ShrinkRectComponent>();
				for (auto& entity : shrinkRectView)
				{
					auto& rect = m_registry.get<ShrinkRectComponent>(entity);
					auto box2D = m_registry.try_get<Box2D>(entity);
					box2D->mIsVisible = true;
					rect.Update(*box2D, _dTime);
				}

				/// 이벤트 발생 : 임의의 장애물 위치에 던지게 한다.
				if (m_pInputManager->GetKeyUp(KEY::LBUTTON) && mpLevelManager->AddBlockScreenPos(m_pInputManager->GetMousePos(), Vector2(1812, 997), Vector2(83, 58)))
				{
					if (cursorFollowPos.x >= 5 && cursorFollowPos.x <= 6 && cursorFollowPos.z >= -1 && cursorFollowPos.z <= 1)
					{
						mTime = 0.0f;							 // 타임 리셋
						InvisibleGuideTile();					 // 바닥 표시 안 보이게 변경
						mpLevelManager->DeleteMoney();			 // 모든 동전 삭제!
						ResettingShrinkRect(Vector2(1215, 460), Vector2(100, 100), 4, 20);
						mTextIdx = 4;

						auto obsView = m_registry.view<Texture3D>();
						for (auto& obsEntity : obsView)
						{
							auto& name = m_registry.get<Name>(obsEntity).m_name;
							auto& texture = m_registry.get<Texture3D>(obsEntity);
							if (name == "UI_H_Obstacle")
							{
								m_pEntityManager->RemoveEntity(static_cast<UID>(obsEntity));
							}
						}

						mTutorialState = TutorialState::THROW_O; // 튜토리얼 상태 변경

						return;
					}
				}
			}
		}
		break;
		case TutorialState::THROW_O:
		{
			SettingPlayingIndicator();

			PrintIntroText(TutorialState::THROW_O);	// 튜토리얼 텍스트 삽입

			if (mTextIdx == 7)
			{
				mpUnitSystem->GetProjectileSystem()->TutirialUpdateThrow(_dTime);
				//mpUnitSystem->GetProjectileSystem()->TutirialUpdateMoney(_dTime);

				ChangeGuideTile("White.png", Vector2(10, -5), Vector2(0.7, 0.7));

				UpdateShrinkRect(_dTime);

				/// 이벤트 발생 : 임의의 적군이 뛰어올 만한 자리에 던진다. + 다른 곳에 던진 동전은 전부 삭제시킨다.
				if (m_pInputManager->GetKeyUp(KEY::LBUTTON) && mpLevelManager->AddBlockScreenPos(m_pInputManager->GetMousePos(), Vector2(1812, 997), Vector2(83, 58)))
				{
					if (cursorFollowPos.x >= 10 && cursorFollowPos.x <= 11 && cursorFollowPos.z >= -6 && cursorFollowPos.z <= -5)
					{
						mTime = 0.0f;
						InvisibleGuideTile();					 // 바닥 표시 안 보이게 변경

						ResettingShrinkRect(Vector2(631, 959), Vector2(90, 90), 4, 20);
						mTutorialState = TutorialState::THROW_O_WAIT;
					}
					else
					{
						// 동전 모두 삭제!
						mpLevelManager->DeleteMoney();
					}
				}
			}
		}
		break;
		case TutorialState::THROW_O_WAIT:// 동전이 던져질 때까지 진행을 막는다.
		{
			mTime += _dTime;
			//mpUnitSystem->GetProjectileSystem()->TutirialUpdateMoney(_dTime);
			if (mTime > 1.0f)
			{
				mTutorialState = TutorialState::THROW;
				mTextIdx = 8; // 하하!\n정말 정확한 곳에 잘 던지지?
				mTime = 0.0f;
				return;
			}
		}
		break;
		case TutorialState::THROW:
		{
			SettingPlayingIndicator();

			// 특정 조건에만 돈을 감지해서 타겟팅 하게 하는 함수다.
			mpUnitSystem->GetPlayerSystem()->DetectGroundMoney();

			PrintIntroText(TutorialState::THROW);	// 튜토리얼 텍스트 삽입

			// 적군 업데이트
			mpLevelManager->EnemyUpdate(_dTime);

			mTime += _dTime;
			if (mTime < 1.0f)
			{
				mTextIdx = 8; // 하하!\n정말 정확한 곳에 잘 던지지?
			}
			else if (1.0f <= mTime && mTime < 3.0f)
			{
				mTextIdx = 9; //돈은 누구에게나 소중합니다.\n적군도 돈을 보면 이렇게 달려들거든요. 아까워라.
			}
			else if (3.0f <= mTime && mTime < 3.5f)
			{
				mTextIdx = 10; //악!속았어!
			}
			else if (3.5f <= mTime) // 적군이 돈을 먹고 가만히 있음
			{
				// 근거리 직업 용병 버튼 활성화
				auto uiView = m_registry.view<Button>();
				for (auto& uiEntity : uiView)
				{
					auto& name = m_registry.get<Name>(uiEntity).m_name;
					auto& button = m_registry.get<Button>(uiEntity);
					auto& img = m_registry.get<Texture2D>(uiEntity);

					if (name == "UI_D_Class")
					{
						if (img.m_file == "UI_btn_Merc01.png")
						{
							img.m_rgba = Vector4(0.8f, 0.8f, 0.8f, 1.0f);
							button.mNormalRGBA = img.m_rgba;
							button.mPressedRGBA = Vector4(1.0f);
							button.mHoveredRGBA = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
							button.mIsEnable = true;
							m_pRenderManager->UpdateEntityTexture(m_pEntityManager->GetEntity(uiEntity));
						}
					}
				}

				// 동전 모두 삭제!
				auto moneyView = m_registry.view<MoneyComponent>();
				for (auto& entity : moneyView)
				{
					auto& money = m_registry.get<MoneyComponent>(entity);
					m_pEntityManager->RemoveEntity(money.m_pOwner->GetUID());
				}
				mTime = 0.0f;
				mTutorialState = TutorialState::Enemy_Play;
				mTextIdx = 11;
				return;
			}
		}
		break;
		case TutorialState::Enemy_Play:
		{
			SettingDeployIndicator(cursorFollowPos, pSceneData);

			PrintIntroText(TutorialState::Enemy_Play);	// 튜토리얼 텍스트 삽입

			mpLevelManager->DeleteEnemyAll();// 이 때 적군 컴포넌트 전체 삭제

			if (mTextIdx == 13)
			{
				/// 이벤트 발생 : 근거리 클래스 버튼을 클릭한다.
				mpLevelManager->TutorialClassDeploy();

				UpdateShrinkRect(_dTime);

				if (mpLevelManager->mTutorialFlag == true)
				{
					mTutorialState = TutorialState::BUTTON_M;
					mTextIdx = 14;
					ChangeGuideTile("blue.png", Vector2(-12.f, 0.0f), Vector2(2.7f, 9.7f));

					ResettingShrinkRect(Vector2(815, 5), Vector2(290, 105), 4, 20);
					mpLevelManager->mTutorialFlag = false;
				}
			}
		}
		break;
		case TutorialState::BUTTON_M:
		{
			SettingDeployIndicator(cursorFollowPos, pSceneData);

			PrintIntroText(TutorialState::BUTTON_M);	// 튜토리얼 텍스트 삽입

			/// 이벤트 발생 : 배치 위치에 클릭
			mpLevelManager->TutorialClassDeployed(cursorFollowPos, isPayment, pSceneData);
			if (mpLevelManager->mTutorialFlag == true)
			{
				InvisibleGuideTile();					 // 바닥 표시 안 보이게 변경
				mTutorialState = TutorialState::DEPLOY_M;
				mpLevelManager->mTutorialFlag = false;

			}
		}
		break;
		case TutorialState::DEPLOY_M:
		{
			SettingPlayingIndicator();
			PrintIntroText(TutorialState::DEPLOY_M);	// 튜토리얼 텍스트 삽입

			if (mTextIdx == 15)
			{
				UpdateShrinkRect(_dTime);
			}
			if (mTextIdx == 16)
			{
				ResettingShrinkRect(Vector2(735, 600), Vector2(100, 100), 4, 20);
			}

			if (mTextIdx == 18)// 용병단원들은 돈을 인식하는 범위에 돈이 있으면\n싸우는 것도 관두고 우선 주우러 갑니다. 여기로 던져보세요!
			{
				ChangeGuideTile("White.png", Vector2(-5.f, 0.f), Vector2(0.7f, 0.7f));
				mpUnitSystem->GetProjectileSystem()->TutirialUpdateThrow(_dTime);
				//mpUnitSystem->GetProjectileSystem()->TutirialUpdateMoney(_dTime);

				UpdateShrinkRect(_dTime);

				/// 이벤트 발생 : 임의의 아군이 뛰어올 만한 자리에 던진다.
				if (m_pInputManager->GetKeyUp(KEY::LBUTTON) && mpLevelManager->AddBlockScreenPos(m_pInputManager->GetMousePos(), Vector2(1812, 997), Vector2(83, 58)))
				{
					if (cursorFollowPos.x >= -6 && cursorFollowPos.x <= -5 && cursorFollowPos.z >= -1 && cursorFollowPos.z <= 1)
					{
						mTime = 0.0f;
						InvisibleGuideTile();					 // 바닥 표시 안 보이게 변경

						ResettingShrinkRect(Vector2(800, 150), Vector2(210, 210), 4, 20);
						mTutorialState = TutorialState::THROW_WAIT;
					}
					else
					{
						// 동전 모두 삭제!
						mpLevelManager->DeleteMoney();
					}
				}
			}

			auto textView = m_registry.view<Text>();
			for (auto text : textView)
			{
				auto& name = m_registry.get<Name>(text).m_name;
				auto& textComp = m_registry.get<Text>(text);
				if (name == "UI_B_Money")
				{
					textComp.m_num1 = pSceneData->m_heldMoney;
				}

				// 배치한 아군의 수를 업데이트한다.
				/*if (name == "UI_D_AliveNum")
				{
					textComp.m_num1 = pSceneData->m_aliveAlly;
				}*/

				if (name == "UI_B_Money")
				{
					if (isPayment == true)
					{
						pSceneData->m_heldMoney -= 100;
						isPayment == false;
					}
					textComp.m_num1 = pSceneData->m_heldMoney;
				}
			}
		}
		break;
		case TutorialState::THROW_WAIT:// 동전이 던져질 때까지 진행을 막는다.
		{
			mTime += _dTime;
			//mpUnitSystem->GetProjectileSystem()->TutirialUpdateMoney(_dTime);
			if (mTime > 1.0f)
			{
				// 여기서 플레이어 버프 업뎃 오류를 방지하기 위해 체력바를 추가한다(보이지는 않게)
				auto playerView = m_registry.view<PlayerComponent>();
				for (auto& entity : playerView)
				{
					m_pUIManager->Add3DHPBar(m_pEntityManager->GetEntity(entity), Vector3(0, 3.f, 0), Vector2(50, 5), 5, Vector4(0.2f, 0.2f, 0.2f, 1), Vector4(0.f, 0.f, 1.f, 1));
					m_registry.get<HealthBarComponenet>(entity).mIsVisible = false;
					mpUnitSystem->GetPlayerSystem()->SetInitializeState(m_pEntityManager->GetEntity(entity), State::IDLE);
				}

				mTutorialState = TutorialState::ALLY_Play;
				mTextIdx = 19;
				return;
			}
		}
		break;
		case TutorialState::ALLY_Play:
		{
			SettingPlayingIndicator();

			PrintIntroText(TutorialState::ALLY_Play);	// 튜토리얼 텍스트 삽입

			// 특정 조건에만 돈을 감지해서 타겟팅 하게 하는 함수다.
			mpUnitSystem->GetPlayerSystem()->DetectGroundMoney();
			if (mTextIdx > 19) // 용병이 감탄사를 뱉을 때는 돈 던지기가 불가합니다.
			{
				mpUnitSystem->GetProjectileSystem()->Update(_dTime);
			}

			// 플레이어 업데이트
			auto playerView = m_registry.view<PlayerComponent>();
			for (auto& entity : playerView)
			{
				auto& playerComp = m_registry.get<PlayerComponent>(entity);

				// 플레이어 관련하여 업데이트한다.
				mpUnitSystem->GetPlayerSystem()->PlayerUnitUpdate(m_pEntityManager->GetEntity(entity), _dTime); // 아군 유닛을 업데이트 합니다.

				physx::PxFilterData filterData = m_pPhysicsManager->GetFilterData(entity);
				filterData.word1 |= 8;
				m_pPhysicsManager->SetFilterData(entity, filterData);
			}

			if (mTextIdx == 21)// 왼쪽 버튼을 꾹 누르고 있으면\n연속해서 던질 수 있어!
			{
				if (m_pInputManager->GetKey(KEY::LBUTTON))
				{
					mTime = 0.0f;
					// 모든 동전 삭제!
					mpLevelManager->DeleteMoney();
					mTutorialState = TutorialState::BUFF;
					return;
					/*mPressClickTime += _dTime;
					if (m_pInputManager->GetKeyUp(KEY::LBUTTON) && mpLevelManager->AddBlockScreenPos(m_pInputManager->GetMousePos(), Vector2(1812, 997), Vector2(83, 58)))
					{
						if (mPressClickTime < 1.5f)
						{
							mPressClickTime = 0.0f;
						}
					}*/
				}

				//if (mPressClickTime > 1.5f)
				//{
				//	mPressClickTime = 0.0f;
				//	mTime = 0.0f;
				//	// 모든 동전 삭제!
				//	mpLevelManager->DeleteMoney();
				//	mTutorialState = TutorialState::BUFF;
				//	return;

				//}
			}
		}
		break;
		case TutorialState::BUFF:
		{
			SettingPlayingIndicator();
			PrintIntroText(TutorialState::BUFF);	// 튜토리얼 텍스트 삽입

			// 오류 방지용
			mpUnitSystem->GetPlayerSystem()->DetectGroundMoney();
			// 플레이어 업데이트
			mpLevelManager->PlayerUpdate(_dTime);

			if (mTextIdx == 25) // 적군이 한 종류가 아니었어 ?
			{
				// 적군 용병 다시 배치
				mpUnitSystem->GetPlayerSystem()->AddSceneEnemyUnits(0);

				// 적군 업데이트
				auto enemyView = m_registry.view<EnemyComponent>();
				for (auto& entity : enemyView)
				{
					m_pRenderManager->InitailizeEntity(m_pEntityManager->GetEntity(entity));
				}
				mTutorialState = TutorialState::ENEMY;
				return;
			}
		}
		break;
		case TutorialState::ENEMY:
		{
			void SettingDeployIndicator(Vector3 _cursorPos, SceneData * _pSceneData);
			PrintIntroText(TutorialState::ENEMY);	// 튜토리얼 텍스트 삽입

			if (mTextIdx == 26)
			{
				UpdateShrinkRect(_dTime);

			}
			else if (mTextIdx == 27)
			{
				if (mIsReadyResetShrinkRect == true)
				{
					ResettingShrinkRect(Vector2(1280, 450), Vector2(210, 210), 4, 20);
					return;
				}
				UpdateShrinkRect(_dTime);
			}
			else if (mTextIdx == 28)
			{
				ResettingShrinkRect(Vector2(185, 20), Vector2(300, 68), 4, 20);
			}
			else if (mTextIdx == 30)
			{
				UpdateShrinkRect(_dTime);
			}
			else if (mTextIdx == 31)
			{
				if (mIsReadyResetShrinkRect == true)
				{
					ResettingShrinkRect(Vector2(185, 80), Vector2(285, 75), 4, 20);
					return;
				}
				UpdateShrinkRect(_dTime);
			}
			else if (mTextIdx == 32)
			{
				ResettingShrinkRect(Vector2(1605, 950), Vector2(90, 90), 4, 20);
				return;
			}
			else if (mTextIdx == 33)// 뭐 처음이니 이번에는 제가 돈을 듬뿍 넣어드리도록 하죠.
			{
				mTime += _dTime;

				// 배치된 유닛 삭제
				mpLevelManager->ResetPlayer(pSceneData);

				// 배치와 관련된 버튼 모두 활성화 + 동전 아이콘 띄우기
				auto textureView = m_registry.view<Texture2D>();
				for (auto uiEntity : textureView)
				{
					auto& name = m_registry.get<Name>(uiEntity).m_name;
					auto& img = m_registry.get<Texture2D>(uiEntity);
					auto button = m_registry.try_get<Button>(uiEntity);

					if (name.find("UI_D") != std::string::npos ||
						name.find("UI_B") != std::string::npos)
					{
						/// 모든 버튼 활성화 : 게임 시작 버튼은 사용 불가!
						if (button && name != "UI_D_Start")
						{
							button->mIsEnable = true;
						}

						if (button && img.m_file == "UI_btn_Merc02.png")
						{
							img.m_rgba = Vector4(0.8f, 0.8f, 0.8f, 1.0f);
							button->mNormalRGBA = img.m_rgba;
							button->mPressedRGBA = Vector4(1.0f);
							button->mHoveredRGBA = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
						}
					}

					if (name == "UI_PointMoney")
					{
						img.m_isVisible = true;
						if (img.m_position.x > 215)
						{
							img.m_position.x -= 17.5f; // (915 - 215)/40
						}
						else
						{
							img.m_isVisible = false;
							pSceneData->m_totalAmount = 5000;
							pSceneData->m_heldMoney = 5000;
						}
						if (img.m_position.y > 50)
						{
							img.m_position.y -= 11.f; //( 490 - 50)/40
						}
						if (img.m_size.x > 45)
						{
							img.m_size.x -= 1.f;
						}
						if (img.m_size.y > 50)
						{
							img.m_size.y -= 1.f;
						}
					}
				}
				if (mTime > 2.0f)
				{
					mTutorialState = TutorialState::MONEY;
					mTextIdx = 34;
					mTime = 0.0f;
					/*for (auto uiEntity : textureView)
					{
						auto& name = m_registry.get<Name>(uiEntity).m_name;
						auto& img = m_registry.get<Texture2D>(uiEntity);
						if (name == "UI_PointMoney")
						{
							img.m_isVisible = false;
						}
					}*/
					return;
				}
			}

		}
		break;
		case TutorialState::MONEY:
		{
			SettingDeployIndicator(cursorFollowPos, pSceneData);

		ChangeGuideTile("red.png", Vector2(-12.f, 0.0f), Vector2(2.7f, 9.7f));

			PrintIntroText(TutorialState::MONEY);	// 튜토리얼 텍스트 삽입
			mpLevelManager->AllDeploy(cursorFollowPos, isPayment, pSceneData);

			ChangeGuideTile("blue.png", Vector2(-12.f, 0.0f), Vector2(2.7f, 9.7f));

			UpdateShrinkRect(_dTime);

			if (mpLevelManager->mTutorialFlag == true)
			{
				mTutorialState = TutorialState::FINAL;

				ResettingShrinkRect(Vector2(300, 955), Vector2(200, 80), 4, 20);
				mpLevelManager->mTutorialFlag = false;
				mTime = 0.0;
			}
			mTime += _dTime;
			if (mTime > 3.0f)
			{
				auto view = m_registry.view<Text>();
				for (auto& entity : view)
				{
					auto& name = m_registry.get<Name>(entity).m_name;
					auto& text = m_registry.get<Text>(entity);
					auto img = m_registry.try_get<Texture2D>(entity);

					if (name == "UI_T")
					{
						text.m_isVisible = false;
						img->m_isVisible = false;
					}
				}
				ResettingShrinkRect(Vector2(1605, 950), Vector2(90, 90), 4, 20);
			}
		}
		break;

		case TutorialState::FINAL:
		{
			PrintIntroText(TutorialState::FINAL);	// 튜토리얼 텍스트 삽입

			InvisibleGuideTile(); // 타일 안 보이게

			UpdateShrinkRect(_dTime);

			auto view = m_registry.view<Text>();
			for (auto& entity : view)
			{
				auto& name = m_registry.get<Name>(entity).m_name;
				auto& text = m_registry.get<Text>(entity);
				auto img = m_registry.try_get<Texture2D>(entity);

				if (name == "UI_T")
				{
					text.m_isVisible = true;
					img->m_isVisible = true;
				}
			}

			// 게임 시작 버튼 활성화
			auto textureView = m_registry.view<Texture2D>();
			for (auto uiEntity : textureView)
			{
				auto& name = m_registry.get<Name>(uiEntity).m_name;
				auto button = m_registry.try_get<Button>(uiEntity);

				if (name.find("UI_D") != std::string::npos ||
					name.find("UI_B") != std::string::npos)
				{
					/// 모든 버튼 활성화 : 게임 시작 버튼 활성화!
					if (name == "UI_D_Start")
					{
						button->mIsEnable = true;
						if (m_pUIManager->GetButtonState(button->m_pOwner) == ButtonState::PRESSED)
						{
							if (m_pInputManager->GetKeyUp(KEY::LBUTTON) && mpLevelManager->AddBlockScreenPos(m_pInputManager->GetMousePos(), Vector2(1812, 997), Vector2(83, 58)))
							{
								mpLevelManager->SetGameState(GameState::PRE_PLAY);
								mpLevelManager->SetIsGameStart(true);
								if (mpLevelManager->GetIsClassButtonPressed() == true)
								{
									mpLevelManager->SetmIsOneDeploySet(true);  // 배치 완료 후 상태 변경
									/*auto entity = m_pEntityManager->GetEntity(mpLevelManager->GetTempUnitUID());
									entity->GetComponent<PlayerComponent>().mIsDeploy = false;

									m_pEntityManager->RemoveEntity(mpLevelManager->GetTempUnitUID());*/
									mpLevelManager->SetTempUnitUID(-1);
									mpLevelManager->SetIsClassButtonPressed(false);
								}
								m_pSoundManager->PlaySFX("Snd_sfx_BattleStart");

								ResettingShrinkRect(Vector2(925, 410), Vector2(100, 100), 4, 20);
								// 								m_pSoundManager->PlaySFX("Snd_sfx_ClickBtn");
																//mpLevelManager->SetPopUpState(PopUpState::OPEN_UNITDEPLOY);

								mTutorialState = TutorialState::PLAYING;
								return;
							}
						}
					}
				}
			}
		}
		break;
		}
	}
	break;
	case GameState::PRE_PLAY:
	{
		mpLevelManager->PreplayUpdate(pSceneData);
		auto view2 = m_registry.view<Text>();
		for (auto& entity : view2)
		{
			auto& name = m_registry.get<Name>(entity).m_name;
			auto& text = m_registry.get<Text>(entity);
			auto img = m_registry.try_get<Texture2D>(entity);

			if (name == "UI_T")
			{
				text.m_isVisible = false;
				img->m_isVisible = false;
			}

			// 타이머를 켭니다.
			if (name == "UI_P_Timer")
			{
				text.m_isVisible = true;
				if (img) // 아 이거 왜 이미지 없어
				{
					img->m_isVisible = true;
				}
			}
		}
	}
	break;
	case GameState::PLAY:
	{
		mpUnitSystem->GetProjectileSystem()->Update(_dTime);

		uint8_t numAlly = 0;
		uint8_t numEnemy = 0;

		// 특정 조건에만 돈을 감지해서 타겟팅 하게 하는 함수다.
		mpUnitSystem->GetPlayerSystem()->DetectGroundMoney();

		// 배치 타일 색 바꾸기
		SettingRealPlayingIndicator(pSceneData);

		// 체력바
		auto playerView = m_registry.view<PlayerComponent>();
		auto enemyView = m_registry.view<EnemyComponent>();
		for (auto& entity : playerView)
		{
			auto& playerComp = m_registry.get<PlayerComponent>(entity);
			if (auto helthBar = m_registry.try_get<HealthBarComponenet>(entity)) // 체력바가 있는 건 다 플레이어 컴포넌트가 있을 것이다.
			{
				float healthPercent = playerComp.mHP / playerComp.mMaxHP;
				helthBar->mHealthPercentage = healthPercent;
			}

			// 플레이어 관련하여 업데이트한다.
			mpUnitSystem->GetPlayerSystem()->PlayerUnitUpdate(m_pEntityManager->GetEntity(entity), _dTime); // 아군 유닛을 업데이트 합니다.

			if (playerComp.mIsDead == false)
			{
				numAlly++;
			}
			else
			{
				physx::PxFilterData filterData = m_pPhysicsManager->GetFilterData(entity);
				filterData.word1 |= 8;
				m_pPhysicsManager->SetFilterData(entity, filterData);
			}

		}

		for (auto& entity : enemyView)
		{
			auto& enemyComp = m_registry.get<EnemyComponent>(entity);
			if (auto helthBar = m_registry.try_get<HealthBarComponenet>(entity)) // 체력바가 있는 건 다 플레이어 컴포넌트가 있을 것이다.
			{
				float healthPercent = enemyComp.mHP / enemyComp.mMaxHP;
				helthBar->mHealthPercentage = healthPercent;
			}

			mpUnitSystem->GetPlayerSystem()->EnemyUnitUpdate(m_pEntityManager->GetEntity(entity), _dTime); // 적군 유닛을 업데이트 합니다.

			if (enemyComp.mIsDead == false)
			{
				numEnemy++;
			}
			else
			{
				physx::PxFilterData filterData = m_pPhysicsManager->GetFilterData(entity);
				filterData.word1 |= 8;
				m_pPhysicsManager->SetFilterData(entity, filterData);
			}
		}

		/// 여기서 용병왕을 업데이트 한다.
		auto npcView = m_registry.view<NPCComponent>();
		for (auto& entity : npcView)
		{
			mpUnitSystem->GetNPCSystem()->Update(m_pEntityManager->GetEntity(entity), numAlly, numEnemy);
		}

		auto textView = m_registry.view<Text>();
		for (auto& entity : textView)
		{
			auto& name = m_registry.get<Name>(entity).m_name;
			auto& text = m_registry.get<Text>(entity);
			// 타이머
			if (name == "UI_P_Timer")
			{
				pSceneData->m_time -= _dTime;

				int minutes = static_cast<int>(pSceneData->m_time) / 60;
				int seconds = static_cast<int>(pSceneData->m_time) % 60;

				std::ostringstream oss;
				oss << std::setfill('0') << std::setw(2) << minutes << ":"
					<< std::setfill('0') << std::setw(2) << seconds;

				// std::u8string으로 변환
				std::string str = oss.str();
				text.m_text = std::u8string(str.begin(), str.end());

				// 10초 남았을때, 경고효과음 재생
				if (std::fabs(pSceneData->m_time - 10.0f) < std::numeric_limits<float>::epsilon())
				{
					m_pSoundManager->PlaySFX("Snd_sfx_TimeLow");
				}

				// 시간이 다 되면 자동으로 넘어감
				if (pSceneData->m_time < std::numeric_limits<float>::epsilon())
				{
					pSceneData->m_time = pSceneData->m_totalTime;
					DLOG(LOG_INFO, "Game Over");
					mpLevelManager->SetGameState(GameState::TUTORIALEND);
					return;
				}
			}

			// 플레이어의 소지금을 업데이트 한다.
			if (name == "UI_B_Money")
			{
				text.m_num1 = pSceneData->m_heldMoney;
			}

			if (name == "UI_B_StageGuide")
			{
				text.m_num1 = numEnemy;
			}

		} // End for문 : m_pEntityManager->GetEntityMap()
		if (numAlly == 0 || numEnemy == 0)
		{
			pSceneData->m_aliveAlly = numAlly;
			mpLevelManager->SetGameState(GameState::TUTORIALEND);
			m_pSoundManager->StopBGM();
			if (numAlly == 0)
			{
				m_pSoundManager->PlayBGM("Snd_bgm_AfterBattleLose");
			}
			else
			{
				m_pSoundManager->PlayBGM("Snd_bgm_AfterBattleWin");
			}
			mpLevelManager->FadePreSetting(true);
			auto HPView = m_registry.view<HealthBarComponenet>();
			for (auto entity : HPView)
			{
				auto& hpEntity = m_registry.get<HealthBarComponenet>(entity);
				hpEntity.mIsVisible = false;
				auto& animation = m_registry.get<AnimationComponent>(entity);
				animation.mPaused = true;
			}

			auto rigidView = m_registry.view<Rigidbody>();
			for (auto& entity : rigidView)
			{
				auto& rigid = m_registry.get<Rigidbody>(entity);
				rigid.m_isKinematic = true;
			}

			mTutorialState = TutorialState::POSTPLAYEING;
			return;
		}
	}
	break;
	case GameState::TUTORIALEND:
	{
		mTime += _dTime;
		if (mTutorialState == TutorialState::POSTPLAYEING)
		{
			mTextIdx = 36;
			auto view2 = m_registry.view<Text>();
			for (auto& entity : view2)
			{
				auto& name = m_registry.get<Name>(entity).m_name;
				auto& text = m_registry.get<Text>(entity);
				auto img = m_registry.try_get<Texture2D>(entity);
				if (name == "UI_T")
				{
					text.m_isVisible = true;
					img->m_isVisible = true;
					text.m_position = Vector2(392, 176);
					img->m_file = "UI_pnl_MsgBox2.png";
					img->m_position = Vector2(367, 112);
					img->m_size = Vector2(1182, 170);
					text.m_text = mTutorialTextVec[mTextIdx].text;
				}
			}

			if (mTime > 2.0f)
			{
				mTutorialState = TutorialState::END;
				mpLevelManager->SetUIAnimationState(UIAnimationState::POSTBATTLE);
			}
		}
		else if (mTutorialState == TutorialState::END)
		{
			mpLevelManager->UIAniUpdate(_dTime);

			if (mpLevelManager->GetUIAnimationState() == UIAnimationState::NEXTSCENE)
			{
				m_pWorldManager->GetCurrentWorld()->SetScene(static_cast<uint32_t>(SceneName::SCENE1));
				return;
			}
			/*auto view = m_registry.view<FadeInOut>();
			for (auto& entity : view)
			{
				auto& name = m_registry.get<Name>(entity).m_name;
				auto& img = m_registry.get<FadeInOut>(entity);
				if (name == "FaidInOut")
				{
					if (img.IsFadingOutFin(_dTime / 2, 1.0f) == true)
					{
						m_pWorldManager->GetCurrentWorld()->SetScene(static_cast<uint32_t>(SceneName::SCENE1));
						return;
					}
				}
			}*/

		}
	}
	break;
	default:
		break;
	}

	mpLevelManager->UpdateRankBar(pSceneData);
}

void ExampleScene::LateUpdate(float _dTime)
{
	if (m_pInputManager->GetKeyUp(KEY::M))
	{
		auto camera = mpLevelManager->GetWorldCamera();
		camera->SetEyePos(Vector3(0, 17, -17.5));
		camera->SetDirection(Vector3(0, -0.8, 0.6));

		m_pRenderManager->CameraSetPerspective();
	}
	if (m_pInputManager->GetKeyUp(KEY::N))
	{
		auto camera = mpLevelManager->GetWorldCamera();
		camera->SetEyePos(Vector3(-15.42, 18.06, -17.72));
		camera->SetDirection(Vector3(0.57735, -0.57735, 0.57735));

		m_pRenderManager->CameraSetOrthographic(0.03);
	}
}

void ExampleScene::Finalize()
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
	if (m_pSceneData)
	{
		delete m_pSceneData;
	}
}

void ExampleScene::AddResource()
{
	//m_pResourceManager->GetFileManager()->PrintAll(false);
	/// 리소스 추가
	// 모델 추가
	//m_pRenderManager->AddModel("../TestAsset/", "box.fbx");

	/////UI
	//m_pUIManager->AddTexture2D("../Resources/Texture/", "blue.png");

	//// 애니메이션 추가
	//m_pResourceManager->AddFilesInDirAni("Animation");

	//// 3D 텍스처 추가
	//m_pResourceManager->AddFilesInDir3D("Texture3D");

	//// 큐브맵 텍스처 추가
	//m_pResourceManager->AddFilesInDirDDS("CubeMap");

	//// Fbx 추가 -> 추후 클래스 별로 쪼갤 것
	//m_pResourceManager->AddFilesInDirModel("Mercenary");
	//m_pResourceManager->AddFilesInDirModel("Environment");
	//m_pResourceManager->AddFilesInDirModel("Archer");
	//m_pResourceManager->AddFilesInDirModel("Enemy");
	//m_pResourceManager->AddFilesInDirModel("money");
	//m_pResourceManager->AddFilesInDirModel("Chief");
}

void ExampleScene::SettingDeployIndicator(Vector3 _cursorPos, SceneData* _pSceneData)
{
	// 타일 인디케이터는 언제나 실행시킨다.
	auto indicatorView = m_registry.view<IndicatorComponent>();
	for (auto indiEntity : indicatorView)
	{
		if (m_registry.get<Name>(indiEntity).m_name == "selectedSquare")
		{
			auto& texture3dComp = m_registry.get<Texture3D>(indiEntity);
			auto& indicatorComp = m_registry.get<IndicatorComponent>(indiEntity);

			auto outline = m_registry.try_get<OutlineComponent>(indiEntity);
			auto alpha = m_registry.try_get<AlphaBlendComponent>(indiEntity);

			if (alpha->m_alpha < 0.5)
			{
				alpha->m_alpha += 0.01;
			}
			else
			{
				alpha->m_alpha = 0;
			}

			if (_cursorPos.x >= -15 && _cursorPos.x <= 15 && _cursorPos.z >= -15 && _cursorPos.z <= 15)
			{
				if (mpLevelManager->IsCanClickAreaXZ(_cursorPos, Vector2(-15, 10), Vector2(-9, -10), _pSceneData->m_aliveAlly) == true)
				{
					if (texture3dComp.m_diffuse != indicatorComp.mOriginalColor)
					{
						texture3dComp.m_diffuse = indicatorComp.mOriginalColor;
						outline->m_color = Vector3(0, 0, 1);
						m_pRenderManager->UpdateEntityTexture(m_pEntityManager->GetEntity(indiEntity));
					}
				}
				else
				{
					if (texture3dComp.m_diffuse != indicatorComp.mChangedColor)
					{
						texture3dComp.m_diffuse = indicatorComp.mChangedColor;
						outline->m_color = Vector3(1, 0, 0);
						m_pRenderManager->UpdateEntityTexture(m_pEntityManager->GetEntity(indiEntity));
					}
				}
				texture3dComp.m_pOwner->GetComponent<Transform>().m_localPosition = Vector3(static_cast<int>(_cursorPos.x), 0.1f, static_cast<int>(_cursorPos.z));
			}
		}
	}
}

void ExampleScene::SettingPlayingIndicator()
{
	// 배치 타일 색 바꾸기
	auto indicatorView = m_registry.view<IndicatorComponent>();
	for (auto& entity : indicatorView)
	{
		if (m_registry.get<Name>(entity).m_name == "selectedSquare")
		{
			auto& tile = m_registry.get<Texture3D>(entity);
			Vector3 cursorPos = m_pPhysicsManager->PickObejct("plane");
			if (cursorPos.x > 0)cursorPos.x += 0.5;
			else cursorPos.x -= 0.5;
			if (cursorPos.z < 0) cursorPos.z -= 0.5;

			if (cursorPos.x >= -15 && cursorPos.x <= 15 && cursorPos.z >= -15 && cursorPos.z <= 15)
			{
				cursorPos = Vector3(static_cast<int>(cursorPos.x), 0.1f, static_cast<int>(cursorPos.z));
				Vector2 astarPos = {};
				mpAstar->AdjustToTileCenter(cursorPos, astarPos);
				if ((*mpAstar->GetAstarMap())[static_cast<int>(astarPos.y + abs(mStartPoint.y))][static_cast<int>(astarPos.x + abs(mStartPoint.x))] == 0) // 장애물이 없는 곳만 클릭 가능
				{
					tile.m_diffuse = "blue.png";
					m_pRenderManager->UpdateEntityTexture(m_pEntityManager->GetEntity(entity));
				}
				else
				{
					tile.m_diffuse = "red.png";
					m_pRenderManager->UpdateEntityTexture(m_pEntityManager->GetEntity(entity));
				}
				tile.m_pOwner->GetComponent<Transform>().m_localPosition = cursorPos;
			}
		}
	}
}

void ExampleScene::SettingRealPlayingIndicator(SceneData* _pSceneData)
{
	auto indicatorView = m_registry.view<IndicatorComponent>();
	for (auto indiEntity : indicatorView)
	{
		if (m_registry.get<Name>(indiEntity).m_name == "selectedSquare")
		{
			auto& texture3dComp = m_registry.get<Texture3D>(indiEntity);
			auto& indicatorComp = m_registry.get<IndicatorComponent>(indiEntity);
			auto outline = m_registry.try_get<OutlineComponent>(indiEntity);
			auto alpha = m_registry.try_get<AlphaBlendComponent>(indiEntity);
			Vector3 cursorFollowPos = m_pPhysicsManager->PickObejct("plane");
			if (cursorFollowPos.x > 0)cursorFollowPos.x += 0.5;
			else cursorFollowPos.x -= 0.5;
			if (cursorFollowPos.z < 0) cursorFollowPos.z -= 0.5;

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
				cursorFollowPos = Vector3(static_cast<int>(cursorFollowPos.x), 0.1f, static_cast<int>(cursorFollowPos.z));
				Vector2 astarPos = {};
				mpAstar->AdjustToTileCenter(cursorFollowPos, astarPos);
				if ((*mpAstar->GetAstarMap())[static_cast<int>(astarPos.y + abs(mStartPoint.y))][static_cast<int>(astarPos.x + abs(mStartPoint.x))] == 0) // 장애물이 없는 곳만 클릭 가능
				{
					texture3dComp.m_diffuse = "blue.png";
					outline->m_color = Vector3(0, 0, 1);
					m_pRenderManager->UpdateEntityTexture(m_pEntityManager->GetEntity(indiEntity));
				}
				else
				{
					texture3dComp.m_diffuse = "red.png";
					outline->m_color = Vector3(1, 0, 0);
					m_pRenderManager->UpdateEntityTexture(m_pEntityManager->GetEntity(indiEntity));
				}
				texture3dComp.m_pOwner->GetComponent<Transform>().m_localPosition = cursorFollowPos;
			}
		}
	}
}

void ExampleScene::ChangeGuideTile(const std::string& _texFileName, Vector2 _posXZ, Vector2 _sizeXZ)
{
	auto view = m_registry.view<Texture3D>();
	for (auto& entity : view)
	{
		auto& name = m_registry.get<Name>(entity).m_name;
		auto& img = m_registry.get<Texture3D>(entity);
		if (name == "UI_H")
		{
			auto trs = m_registry.try_get<Transform>(entity);
			img.m_diffuse = _texFileName;
			trs->m_localPosition.x = _posXZ.x;
			trs->m_localPosition.y = 0.005f;
			trs->m_localPosition.z = _posXZ.y;
			trs->m_localScale.x = _sizeXZ.x;
			trs->m_localScale.z = _sizeXZ.y;
			m_pRenderManager->UpdateEntityTexture(m_pEntityManager->GetEntity(entity));
			return;
		}
	}
}

void ExampleScene::ChangeGuideTileTrs(Vector2 _posXZ, Vector2 _sizeXZ)
{
	auto view = m_registry.view<Texture3D>();
	for (auto& entity : view)
	{
		auto& name = m_registry.get<Name>(entity).m_name;
		auto& img = m_registry.get<Texture3D>(entity);
		if (name == "UI_H")
		{
			auto trs = m_registry.try_get<Transform>(entity);
			trs->m_localPosition.x = _posXZ.x;
			trs->m_localPosition.y = 0.005f;
			trs->m_localPosition.z = _posXZ.y;
			trs->m_localScale.x = _sizeXZ.x;
			trs->m_localScale.z = _sizeXZ.y;
			return;
		}
	}
}

void ExampleScene::InvisibleGuideTile()
{
	auto view = m_registry.view<Texture3D>();
	for (auto& entity : view)
	{
		auto& name = m_registry.get<Name>(entity).m_name;
		auto& img = m_registry.get<Texture3D>(entity);
		if (name == "UI_H")
		{
			auto trs = m_registry.try_get<Transform>(entity);
			trs->m_localPosition.y = -0.1f;
			return;
		}
	}
}

void ExampleScene::ChangeGuideTileTex(const std::string& _texFileName)
{
	auto view = m_registry.view<Texture3D>();
	for (auto& entity : view)
	{
		auto& name = m_registry.get<Name>(entity).m_name;
		auto& img = m_registry.get<Texture3D>(entity);
		if (name == "UI_H")
		{
			img.m_diffuse = _texFileName;
			m_pRenderManager->UpdateEntityTexture(m_pEntityManager->GetEntity(entity));
			return;
		}
	}
}

void ExampleScene::PrintIntroText(TutorialState _tutorialState)
{

	switch (_tutorialState)
	{
	case TutorialState::INTRO:
		SkipText(3);
		// 1 : 안녕, 나는 리치 러커스라고 해 // 나는 돈이 정말, 진짜 정말로 많아!
		// 2 : 그리고 나는 정확히 원하는 곳에 돈을 던질 수 있어. 볼래?
		// 3 : 마우스 왼쪽 버튼으로 동전을 던질 수 있습니다. 지정된 위치에 동전을 던져보세요.
		break;
	case TutorialState::THROW_O:
		SkipText(7);
		// 4 : 장애물에는 던질 수 없어. 내 용병들이 다친다고.
		// 5 : 그럼 맵 밖으로 던져보세..
		// 6 : 야!\n내 아까운 돈을 왜 밖에다 던져!\n용병들에게 줄 소중한 돈이야!
		// 7 : 그럼 이곳에 던져봅시다.
		break;
	case TutorialState::THROW:
		SkipText(9);
		// 8 : 하하!\n정말 정확한 곳에 잘 던지지?
		// 9 : 돈은 누구에게나 소중합니다.\n적군도 돈을 보면 이렇게 달려들거든요. 아까워라.
		break;
	case TutorialState::Enemy_Play:
		SkipText(13);
		// 10 : 악! 속았어!
		// 11 : 리치 로커스 씨가 원하는 대로 아군 용병에게 돈을 줘보도록 하죠.\n아군 용병을 소환해봅시다.
		// 12 : 나와라! 원거리 용병!
		// 13 : 근거리 용병부터 배치합시다.\n하단에 첫번째 버튼을 눌러볼까요?
		break;
	case TutorialState::BUTTON_M:
		SkipText(14);
		// 14 : 잘했어요.\n빨간색으로 표시된 영역에 배치합시다.
		break;
	case TutorialState::DEPLOY_M:
		SkipText(18);
		// 15 : 지금 내가 몇 명 배치했는지 위에서 볼 수 있어.
		// 16 : 클래스 버튼을 이미 누른 채라면 배치 영역에 자유롭게 더 배치할 수 있어요.
		// 17 : 원거리 용병 버튼을 누르면 원거리 용병을 배치할 수 있지!
		// 18 : 용병단원들은 돈을 인식하는 범위에 돈이 있으면\n싸우는 것도 관두고 우선 주우러 갑니다. 여기로 던져보세요!
		break;
	case TutorialState::ALLY_Play:
		SkipText(21);
		// 19 : 크! 내 용병!\n저 번뜩이는 눈!\n4배속이라니! \n역시 100G의 가치가 있어!
		// 20 : 돈을 계속 먹이면 점점 버프를 받습니다.\n계속 돈을 던져보세요.
		// 21 : 왼쪽 버튼을 꾹 누르고 있으면\n연속해서 던질 수 있어!
		break;
	case TutorialState::BUFF:
		SkipText(25);
		// 22 : 버프를 받으면 공격력도 강해지고 체력도 좋아집니다.\n버프는 총 3단계로 그 이상은 돈을 먹어도 소용이 없어요.
		// 23 : 이럴수가!\n용병들은 내게 계속 강해진다고 했는데!
		// 24 : 자, 그럼 이제 근거리와 원거리 용병을 전략적으로 배치해보죠.\n적군의 용병 타입을 확인하고 배치한다면 승률이 더 올라가겠죠?
		// 25 : 적군이 한 종류가 아니었어 ?
		break;
	case TutorialState::ENEMY:
		mIsReadyResetShrinkRect = false;
		SkipText(33);
		// 26 : 여기 막대기를 든 용병이 근거리 용병이고,
		// 27 : 여기 활을 든 용병이 원거리 용병이에요. 
		// 28 : 용병 하나당 100G밖에 안하니까\n20명 다 배치하자!
		// 29 : 리치 로커스씨가 이번에 갖고 온 돈이 3000G밖에 안 된다는 걸 잊었네요. 
		// 30 : 맞다!\n왼쪽 위에서 내 소지금을 확인할 수 있어.
		// 31 : 오른쪽 위에서는 현재 랭크 정도를 확인할 수 있습니다.\n남은 돈이 많을수록 높은 랭크를 받을 수 있어요.
		// 32 : 돈… 조금만 써야겠지?
		// 33 : 뭐 처음이니 이번에는 제가 돈을 듬뿍 넣어드리도록 하죠.
		break;
	case TutorialState::MONEY:
		mIsReadyResetShrinkRect = true;
		SkipText(34);
		// 34 : 야호!\n20명 다 배치하자!\n오른쪽 아래 휴지통 버튼을\n누르면 배치를 다시 할 수 있어!
		break;
	case TutorialState::FINAL:
		SkipText(35);
		// 35 : 전투를 시작하세요!
		break;
	case TutorialState::PLAYING:
		break;
	case TutorialState::END:
		SkipText(36);
		// 36 : 리치 로커스 씨를 잘 부탁합니다.
		break;
	case TutorialState::LAST:
		break;
	default:
		break;
	}


}

void ExampleScene::SkipText(int _endIdx)
{
	while (mTextIdx < _endIdx && m_pInputManager->GetKeyUp(KEY::LBUTTON) && mpLevelManager->AddBlockScreenPos(m_pInputManager->GetMousePos(), Vector2(1812, 997), Vector2(83, 58)))
	{
		mTextIdx++;
		mIsReadyResetShrinkRect = true;
		return;
	}
}

void ExampleScene::UpdateShrinkRect(float _dTime)
{
	auto shrinkRectView = m_registry.view<ShrinkRectComponent>();
	for (auto& entity : shrinkRectView)
	{
		auto& rect = m_registry.get<ShrinkRectComponent>(entity);
		auto box2D = m_registry.try_get<Box2D>(entity);
		box2D->mIsVisible = true;
		rect.Update(*box2D, _dTime);
	}
}

void ExampleScene::ResettingShrinkRect(Vector2 _targetRectPos, Vector2 _targetSize, int _sizeMultiplier, int _shrinkAmount)
{
	if (mIsReadyResetShrinkRect == true)
	{
		Vector2 adjustPos = _targetRectPos - _targetSize * (_sizeMultiplier - 1) * 0.5f;
		auto shrinkRectView = m_registry.view<ShrinkRectComponent>();
		for (auto& entity : shrinkRectView)
		{
			auto& rect = m_registry.get<ShrinkRectComponent>(entity);
			auto box2D = m_registry.try_get<Box2D>(entity);
			box2D->mIsVisible = false;
			rect.Resetting(*box2D, adjustPos, _targetSize * _sizeMultiplier, _targetSize, _shrinkAmount);
		}
	}
}

