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

	mTutorialTextVec = mpResourceManager->ParseCSV<TutotialTextTable>("TutorialText2.csv");
	mTextIdx = 0;
	mTutorialState = TutorialState::FINAL;
	mTime = 0.0f;
}

bool ExampleScene::Initialize()
{
	/// ���ҽ� �߰�
	AddResource();
	mpRenderManager->SetCubeMap("Atrium_diffuseIBL.dds", "Atrium_specularIBL.dds");

	// �������� ����
	mpSceneData = new SceneData();

	/// mpAstar �ʱ�ȭ : ������ �� �Ŵ��� �ȿ� �ִ� �� ������ ���� ���� �Ŵ����� ��� �κ� ó���ϰ� �־ ������ ���� ���� ���´�.
	// ���� �ð��� ���� �� ���� ������ �ٲ㺸��.
	mpAstar = new AStar();
	std::vector<std::vector<int>>* astarMap = new std::vector<std::vector<int>>();
	*astarMap = mpResourceManager->ParseMapCSV("Stage1.csv");
	mpAstar->Initialize(Vector2(-15.0f), Vector2(15.0f), astarMap);
	mStartPoint = mpAstar->GetMapStartPoint();
	mEndPoint = mpAstar->GetMapEndPoint();

	/// �Ŵ��� �� �ý��� �ʱ�ȭ
	// ���� �ý��� ����(����ü, �÷��̾�, NPC �ý��� �� ��� �Ŵ��� �ʱ�ȭ)
	mpUnitSystem = new UnitSystem(mRegistry, mpEntityManager, mpPhysicsManager, mpInputManager, mpWorldManager
		, mpRenderManager, mpResourceManager, mpEventManager, mpUIManager, mpSoundManager, mpAstar);
	mpUnitSystem->Initialize();

	mpLevelManager = new LevelManager(mRegistry, mpRenderManager, mpPhysicsManager, mpInputManager, mpUIManager, mpEntityManager
		, mpResourceManager, mpSoundManager, mpUnitSystem, mpAstar);
	mpLevelManager->Initialize(GetUID());
	mpLevelManager->SetGameState(GameState::PRE_PLACEMENT);

	/// �ؽ�Ʈ �ε��� �ʱ�ȭ
	mTextIdx = 0;

	/// �⺻���� ȯ�� ����
	// ī�޶� ��ƼƼ ����, ����
	mpLevelManager->AddCamera(Vector3(-12.f, 4.0f, 3.35f), Vector3(-1.f, -0.2f, 0.0f));
	// ����Ʈ ����
	mpLevelManager->AddLight();
	// ť��� ����
	mpLevelManager->AddCubeMap(1);

	auto camera = mpLevelManager->GetWorldCamera();
	camera->SetEyePos(Vector3(-15.42, 18.06, -17.72));
	camera->SetDirection(Vector3(0.57735, -0.57735, 0.57735));
	mpRenderManager->CameraSetOrthographic(0.03);

	/// ������Ʈ ����
	// ���� �� ��ġ(���Ÿ� ���� ����)
	mpLevelManager->AddPlaneNWall();
	// �ٴ� ��ǥ�� ��ġ
	mpLevelManager->DrawCoordinatesXZ(true);
	// �ε������� Ÿ�� ����
	mpLevelManager->AddSquareAtPointer("blue.png", "red.png");
	// �� ������ �߰�[��ȹ�� �� �ѹ�]
	mpLevelManager->AddMapData(1);
	// ���ּ��� --------------------------------------------------------
	// ���� ���� ����
	mpUnitSystem->GetPlayerSystem()->AddSceneEnemyUnits(0);

	// NPC ����
	mpUnitSystem->GetNPCSystem()->AddNPC("Chief"); // �Ʊ� �뺴��
	mpUnitSystem->GetNPCSystem()->AddNPC("RVC_C"); // ���� �뺴��
	//
	auto ui1 = mpEntityManager->CreateEntity("UI_T");
	mpUIManager->AddUI(ui1, "UI_pnl_MsgBoxClick1.png", Vector2(193, 489), Vector2(432, 210), static_cast<int>(Layer::COM_B), Vector4(1.0f));
	mpUIManager->AddText(ui1, u8"�ȳ�, ���� ��ġ ��Ŀ����� ��", "KIMM_Bold(60).ttf", Vector2(219, 554), static_cast<int>(Layer::COM_B), true, Vector4(1.0f));
	ui1->GetComponent<Text>().mScale = 0.33f;

	auto ui2 = mpEntityManager->CreateEntity("UI_T_Skip");
	mpUIManager->AddUI(ui2, "UI_btn_Skip.png", Vector2(1812, 997), Vector2(83, 58), static_cast<int>(Layer::COM_B), Vector4(1.0f));
	mpUIManager->AddButtonAllColor(ui2, Vector4(0.5f, 0.5f, 0.5f, 1.0f), Vector4(1.0f));

	auto ui3 = mpEntityManager->CreateEntity("UI_PointMoney");
	mpUIManager->AddUI(ui3, "UI_PointMoney.png", Vector2(915, 490), Vector2(90, 100), static_cast<int>(Layer::EFFECT), Vector4(1.0f), false);

	auto ui4 = mpEntityManager->CreateEntity("UI_H"); // ��ġ�� ���� ����
	ui4->AddComponent<Transform>(Vector3(5.f, 0.f, 0.f), Vector3(0.f), Vector3(0.7f, 0.005f, 0.7f));
	ui4->AddComponent<MeshRenderer>("box.fbx", "Cube.001");
	ui4->AddComponent<Texture3D>("White.png");
	ui4->AddComponent<AlphaBlendComponent>(0.2);
 	ui4->AddComponent<OutlineComponent>(1.05, Vector3(0, 0, 1));

	// ��ֹ� ��ƼƼ -> �ʹݿ� ���Դٰ� �ٷ� ���ŵ� �༮
	auto ui5 = mpEntityManager->CreateEntity("UI_H_Obstacle"); // ��ġ�� ���� ����
	ui5->AddComponent<Transform>(Vector3(5.f, 0.f, 0.f), Vector3(0.f), Vector3(0.5f, 1.0f, 0.5f));
	ui5->AddComponent<MeshRenderer>("box.fbx", "Cube.001");
	ui5->AddComponent<Texture3D>("Log.png");

	// �簢�� ����Ʈ ��ƼƼ
	auto effectRect = mpEntityManager->CreateEntity("UI_Effect_Rect");
	mpUIManager->AddRect(effectRect, Vector2(925, 410), Vector2(200.f), static_cast<int>(Layer::COM_M), Vector4(0.0f), 8.0f, Vector4(1.0f, 0.38f, 0.0f, 1.0f));
	effectRect->AddComponent<ShrinkRectComponent>(Vector2(200.f), Vector2(100.f), 20);
	effectRect->GetComponent<Box2D>().mIsVisible = false;
	mIsReadyResetShrinkRect = true;

	/*auto ui5 = mpEntityManager->CreateEntity("sssss");
	mpUIManager->AddText(ui5, u8"�ȳ�,\n ����\n��ġ \n��Ŀ�����\n ��", "KIMM_Bold(60).ttf", Vector2(0, 0), static_cast<int>(Layer::COM_B), true);
	ui1->GetComponent<Text>().mScale = 0.7f;*/

	/// UI
	mpLevelManager->AddBasicUI();
	mpLevelManager->AddBattleUI(dynamic_cast<SceneData*>(mpSceneData));
	mpLevelManager->AddStageNumAndObjects(u8"Ʃ�丮��");
	mpLevelManager->AddClassUI(0, true); // UI Ŭ���� ��ư
	mpLevelManager->AddVerifyPopUpUI();
	mpLevelManager->AddAnimationUI(true, static_cast<int>(mpWorldManager->GetCurrentWorld()->GetPreviousScene()));

	/// Ʃ�丮�� ���� ����
	mTutorialState = TutorialState::CLOUD;

	/// UI �ִϸ��̼� ���� ����
	mpLevelManager->SetUIAnimationState(UIAnimationState::PRETUTORIAL);

	/// Sound
	mpSoundManager->StopBGM();
	mpSoundManager->PlayBGM("Snd_bgm_BeforeBattle");
	return Scene::Initialize();
}

void ExampleScene::FixedUpdate(float _dTime)
{
	// play�� ����Ǵ� �κ��� �߰��ؼ� ������Ʈ �մϴ�.
	if (mTutorialState == TutorialState::THROW
		/*|| mTutorialState == TutorialState::ALLY_Play
		|| mTutorialState == TutorialState::BUFF*/
		|| mpLevelManager->GetGameState() == GameState::PLAY)
	{
		auto playerView = mRegistry.view<PlayerComponent>();
		auto enemyView = mRegistry.view<EnemyComponent>();
		for (auto& entity : playerView)
		{
			mpUnitSystem->GetPlayerSystem()->FixedUpdate(mpEntityManager->GetEntity(entity), _dTime); // �Ʊ� ������ ������Ʈ �մϴ�.
		}

		for (auto& entity : enemyView)
		{
			mpUnitSystem->GetPlayerSystem()->FixedUpdate(mpEntityManager->GetEntity(entity), _dTime); // ���� ������ ������Ʈ �մϴ�.
		}

		//mpUnitSystem->GetProjectileSystem()->FixedUpdate(_dTime);
	}

	if (mTutorialState == TutorialState::ALLY_Play
		|| mTutorialState == TutorialState::BUFF)
	{
		auto playerView = mRegistry.view<PlayerComponent>();
		for (auto& entity : playerView)
		{
			mpUnitSystem->GetPlayerSystem()->FixedUpdate(mpEntityManager->GetEntity(entity), _dTime); // �Ʊ� ������ ������Ʈ �մϴ�.
		}
		//mpUnitSystem->GetProjectileSystem()->FixedUpdate(_dTime);
	}

	mpUnitSystem->GetProjectileSystem()->FixedUpdate(_dTime);

}

void ExampleScene::Update(float _dTime)
{
	SceneData* pSceneData = dynamic_cast<SceneData*>(mpSceneData);

	//mpLevelManager->BasicUIUpdate(); ����â ��� �Ұ�

	auto skipView = mRegistry.view<Button>();
	for (auto& entity : skipView)
	{
		auto& name = mRegistry.get<Name>(entity).mName;
		auto& button = mRegistry.get<Button>(entity);

		if (name == "UI_T_Skip")
		{
			if (mpUIManager->GetButtonState(mpEntityManager->GetEntity(entity)) == ButtonState::PRESSED)
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
		mpWorldManager->GetCurrentWorld()->SetScene(static_cast<uint32_t>(SceneName::MAIN));
		return;
	}
	else if (mpLevelManager->mIsSkip == true)
	{
		mpLevelManager->mIsSkip = false;
		// Ȥ�ö� ���� ���� �簢�� �� ���̰�
		auto shrinkRectView = mRegistry.view<ShrinkRectComponent>();
		for (auto& entity : shrinkRectView)
		{
			auto box2D = mRegistry.try_get<Box2D>(entity);
			box2D->mIsVisible = false;
		}
		mTutorialState = TutorialState::END;
		mpLevelManager->SetGameState(GameState::TUTORIALEND);
		mpLevelManager->SetUIAnimationState(UIAnimationState::POSTBATTLE);
		//mpWorldManager->GetCurrentWorld()->SetScene(static_cast<uint32_t>(SceneName::SCENE1));
		return;
	}

	// ��ũ��
	mpLevelManager->UpdateRankBar(pSceneData);

	/// �÷��̾� �ý��� ������Ʈ
	switch (mpLevelManager->GetGameState())
	{
	case GameState::PRE_PLACEMENT:
	{
		// ���� �ִ� �� ����
		int numEnemy = 0;
		auto enemyView = mRegistry.view<EnemyComponent>();
		for (auto entity : enemyView)
		{
			numEnemy++;
		}

		/// ��ġ�� ���õ� ��� �ؽ�Ʈ�� �ؽ�ó �츮��
		// ��ġ�� ���õ� �ؽ�Ʈ ���̱�
		auto textView = mRegistry.view<Text>();
		for (auto uiEntity : textView)
		{
			auto& name = mRegistry.get<Name>(uiEntity).mName;
			auto& text = mRegistry.get<Text>(uiEntity);

			if (name.find("UI_D") != std::string::npos ||
				name.find("UI_B") != std::string::npos ||
				name == "UI_C_OutSetting" ||
				name.find("UI_T") != std::string::npos)
			{
				text.mIsVisible = true;

				if (name == "UI_B_Money")
				{
					text.mNum1 = dynamic_cast<SceneData*>(pSceneData)->m_heldMoney;
				}

				if (name == "UI_B_StageGuide")
				{
					text.mNum1 = numEnemy;
					text.mNum2 = numEnemy;
				}

				// Ʃ�丮�󿡼� ������� �ʴ� �͵��� ������ �ʰ� �Ѵ�.
				if (name == "UI_D_Guide")
				{
					text.mIsVisible = false;
				}

			}

			// Ʃ�丮�󿡼� ������� �ʴ� �͵��� ������ �ʰ� �Ѵ�.
			if (name == "UI_P_Timer")
			{
				text.mIsVisible = false;
			}
		}

		auto msgView = mRegistry.view<MessageBox2D>();
		for (auto uiEntity : msgView)
		{
			auto& name = mRegistry.get<Name>(uiEntity).mName;
			auto& msgComp = mRegistry.get<MessageBox2D>(uiEntity);

			if (name == "UI_B_Rank_Gauge")
			{
				msgComp.mIsImgVisible = true;
			}
		}

		// ��ġ�� ���õ� �ؽ�ó ���̱�
		auto textureView = mRegistry.view<Texture2D>();
		for (auto uiEntity : textureView)
		{
			auto& name = mRegistry.get<Name>(uiEntity).mName;
			auto& img = mRegistry.get<Texture2D>(uiEntity);
			auto button = mRegistry.try_get<Button>(uiEntity);

			if (name.find("UI_D") != std::string::npos ||
				name.find("UI_B") != std::string::npos ||
				name == "UI_C_OutSetting" ||
				name.find("UI_T") != std::string::npos)
			{
				img.mIsVisible = true;
				/// ��� ��ư ��Ȱ��ȭ : ����â ��ư�� ��� �����ؾ� �Ѵ�! + ��ŵ ��ư�� ��� �����ؾ� �Ѵ�!
				if (button)
				{
					if (name != "UI_C_OutSetting" && name != "UI_T_Skip")
					{
						button->mIsEnable = false;
					}
				}
			}

			// Ʃ�丮�󿡼� ������� �ʴ� �͵��� ������ �ʰ� �Ѵ�.
			if (name == "UI_P_Timer")
			{
				img.mIsVisible = false;
			}
		}
		mpLevelManager->FadePreSetting(false);
		mpLevelManager->SetGameState(GameState::PLACEMENT);
	}
	break;
	case GameState::PLACEMENT:
	{
		// ���� ��ǥ
		Vector3 cursorFollowPos = mpPhysicsManager->PickObejct("plane");
		if (cursorFollowPos.x > 0)cursorFollowPos.x += 0.5;
		else cursorFollowPos.x -= 0.5;
		if (cursorFollowPos.z < 0) cursorFollowPos.z -= 0.5;
		bool isPayment = false; /// �̰� ��ġ�� �����ؾ� �� �Ŵ�!

		/// Ʃ�丮�� �ؽ�Ʈ ���
		auto view2 = mRegistry.view<Text>();
		for (auto& entity : view2)
		{
			auto& name = mRegistry.get<Name>(entity).mName;
			auto& text = mRegistry.get<Text>(entity);
			auto img = mRegistry.try_get<Texture2D>(entity);

			if ((name == "UI_T"))
			{
				if (mTutorialTextVec[mTextIdx].idx <= 1) // ��ġ ��Ŀ����
				{
					text.mPosition = Vector2(219, 554);
					if (mTutorialTextVec[mTextIdx].idx == 0)
					{
						img->mFile = "UI_pnl_MsgBoxClick1.png";
					}
					else
					{
						img->mFile = "UI_pnl_MsgBox1.png";
					}
					img->mPosition = Vector2(193, 489);
					img->mSize = Vector2(432, 210);
				}
				else
				{
					text.mPosition = Vector2(392, 176);
					if (mTutorialTextVec[mTextIdx].idx == 2)
					{
						img->mFile = "UI_pnl_MsgBoxClick2.png";
					}
					else
					{
						img->mFile = "UI_pnl_MsgBox2.png";
					}
					img->mPosition = Vector2(367, 112);
					img->mSize = Vector2(1182, 170);
				}
				text.mText = mTutorialTextVec[mTextIdx].text;
			}

			// �÷��̾��� �����ݵ� ������ ������Ʈ �Ѵ�.
			if (name == "UI_B_Money")
			{
				text.mNum1 = pSceneData->m_heldMoney;
			}

			// ��ġ�� �Ʊ��� ���� ������Ʈ�Ѵ�.
			if (name == "UI_D_AliveNum")
			{
				if (9 < text.mNum1 && text.mNum1 < 20)
				{
					text.mText = u8"%d";
					text.mPosition.x = 867.f;
				}
				else if (text.mNum1 == 20)
				{
					text.mText = u8"%d";
					text.mPosition.x = 852.f;
				}
				else
				{
					text.mText = u8"0%d";
					text.mPosition.x = 852.f;
				}
				text.mNum1 = pSceneData->m_aliveAlly;
			}
		}

		/// ���⼭ �뺴���� ������Ʈ �Ѵ�.
		auto npcView = mRegistry.view<NPCComponent>();
		for (auto& entity : npcView)
		{
			mpUnitSystem->GetNPCSystem()->TutorialUpdate(mpEntityManager->GetEntity(entity), mTextIdx);
		}

		/// ���� ������ ���� ������Ʈ
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

			auto view = mRegistry.view<FadeInOut>();
			for (auto& entity : view)
			{
				auto& name = mRegistry.get<Name>(entity).mName;
				auto& img = mRegistry.get<FadeInOut>(entity);
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
			auto enemyView = mRegistry.view<EnemyComponent>();
			for (auto& entity : enemyView)
			{
				auto& name = mRegistry.get<Name>(entity).mName;
				auto& enemyComp = mRegistry.get<EnemyComponent>(entity);


				if (enemyComp.mIsDead == false)
				{
					numEnemy++;
				}
			}

			auto view2 = mRegistry.view<Text>();
			for (auto& entity : view2)
			{
				auto& name = mRegistry.get<Name>(entity).mName;
				auto& text = mRegistry.get<Text>(entity);
				if (name == "UI_B_StageGuide")
				{
					text.mNum1 = numEnemy;
				}
			}

			mTutorialState = TutorialState::INTRO;
		}
		break;
		case TutorialState::INTRO: // �۸� �ڵ����� �Ѿ�ϴ�.
		{
			SettingPlayingIndicator();

			PrintIntroText(TutorialState::INTRO);	// Ʃ�丮�� �ؽ�Ʈ ����

			if (mTextIdx == 3)
			{
				mpUnitSystem->GetProjectileSystem()->Update(_dTime);

				auto shrinkRectView = mRegistry.view<ShrinkRectComponent>();
				for (auto& entity : shrinkRectView)
				{
					auto& rect = mRegistry.get<ShrinkRectComponent>(entity);
					auto box2D = mRegistry.try_get<Box2D>(entity);
					box2D->mIsVisible = true;
					rect.Update(*box2D, _dTime);
				}

				/// �̺�Ʈ �߻� : ������ ��ֹ� ��ġ�� ������ �Ѵ�.
				if (mpInputManager->GetKeyUp(KEY::LBUTTON) && mpLevelManager->AddBlockScreenPos(mpInputManager->GetMousePos(), Vector2(1812, 997), Vector2(83, 58)))
				{
					if (cursorFollowPos.x >= 5 && cursorFollowPos.x <= 6 && cursorFollowPos.z >= -1 && cursorFollowPos.z <= 1)
					{
						mTime = 0.0f;							 // Ÿ�� ����
						InvisibleGuideTile();					 // �ٴ� ǥ�� �� ���̰� ����
						mpLevelManager->DeleteMoney();			 // ��� ���� ����!
						ResettingShrinkRect(Vector2(1215, 460), Vector2(100, 100), 4, 20);
						mTextIdx = 4;

						auto obsView = mRegistry.view<Texture3D>();
						for (auto& obsEntity : obsView)
						{
							auto& name = mRegistry.get<Name>(obsEntity).mName;
							auto& texture = mRegistry.get<Texture3D>(obsEntity);
							if (name == "UI_H_Obstacle")
							{
								mpEntityManager->RemoveEntity(static_cast<UID>(obsEntity));
							}
						}

						mTutorialState = TutorialState::THROW_O; // Ʃ�丮�� ���� ����

						return;
					}
				}
			}
		}
		break;
		case TutorialState::THROW_O:
		{
			SettingPlayingIndicator();

			PrintIntroText(TutorialState::THROW_O);	// Ʃ�丮�� �ؽ�Ʈ ����

			if (mTextIdx == 7)
			{
				mpUnitSystem->GetProjectileSystem()->TutirialUpdateThrow(_dTime);
				//mpUnitSystem->GetProjectileSystem()->TutirialUpdateMoney(_dTime);

				ChangeGuideTile("White.png", Vector2(10, -5), Vector2(0.7, 0.7));

				UpdateShrinkRect(_dTime);

				/// �̺�Ʈ �߻� : ������ ������ �پ�� ���� �ڸ��� ������. + �ٸ� ���� ���� ������ ���� ������Ų��.
				if (mpInputManager->GetKeyUp(KEY::LBUTTON) && mpLevelManager->AddBlockScreenPos(mpInputManager->GetMousePos(), Vector2(1812, 997), Vector2(83, 58)))
				{
					if (cursorFollowPos.x >= 10 && cursorFollowPos.x <= 11 && cursorFollowPos.z >= -6 && cursorFollowPos.z <= -5)
					{
						mTime = 0.0f;
						InvisibleGuideTile();					 // �ٴ� ǥ�� �� ���̰� ����

						ResettingShrinkRect(Vector2(631, 959), Vector2(90, 90), 4, 20);
						mTutorialState = TutorialState::THROW_O_WAIT;
					}
					else
					{
						// ���� ��� ����!
						mpLevelManager->DeleteMoney();
					}
				}
			}
		}
		break;
		case TutorialState::THROW_O_WAIT:// ������ ������ ������ ������ ���´�.
		{
			mTime += _dTime;
			//mpUnitSystem->GetProjectileSystem()->TutirialUpdateMoney(_dTime);
			if (mTime > 1.0f)
			{
				mTutorialState = TutorialState::THROW;
				mTextIdx = 8; // ����!\n���� ��Ȯ�� ���� �� ������?
				mTime = 0.0f;
				return;
			}
		}
		break;
		case TutorialState::THROW:
		{
			SettingPlayingIndicator();

			// Ư�� ���ǿ��� ���� �����ؼ� Ÿ���� �ϰ� �ϴ� �Լ���.
			mpUnitSystem->GetPlayerSystem()->DetectGroundMoney();

			PrintIntroText(TutorialState::THROW);	// Ʃ�丮�� �ؽ�Ʈ ����

			// ���� ������Ʈ
			mpLevelManager->EnemyUpdate(_dTime);

			mTime += _dTime;
			if (mTime < 1.0f)
			{
				mTextIdx = 8; // ����!\n���� ��Ȯ�� ���� �� ������?
			}
			else if (1.0f <= mTime && mTime < 3.0f)
			{
				mTextIdx = 9; //���� �������Գ� �����մϴ�.\n������ ���� ���� �̷��� �޷���ŵ��. �Ʊ����.
			}
			else if (3.0f <= mTime && mTime < 3.5f)
			{
				mTextIdx = 10; //��!�ӾҾ�!
			}
			else if (3.5f <= mTime) // ������ ���� �԰� ������ ����
			{
				// �ٰŸ� ���� �뺴 ��ư Ȱ��ȭ
				auto uiView = mRegistry.view<Button>();
				for (auto& uiEntity : uiView)
				{
					auto& name = mRegistry.get<Name>(uiEntity).mName;
					auto& button = mRegistry.get<Button>(uiEntity);
					auto& img = mRegistry.get<Texture2D>(uiEntity);

					if (name == "UI_D_Class")
					{
						if (img.mFile == "UI_btn_Merc01.png")
						{
							img.mRgba = Vector4(0.8f, 0.8f, 0.8f, 1.0f);
							button.mNormalRGBA = img.mRgba;
							button.mPressedRGBA = Vector4(1.0f);
							button.mHoveredRGBA = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
							button.mIsEnable = true;
							mpRenderManager->UpdateEntityTexture(mpEntityManager->GetEntity(uiEntity));
						}
					}
				}

				// ���� ��� ����!
				auto moneyView = mRegistry.view<MoneyComponent>();
				for (auto& entity : moneyView)
				{
					auto& money = mRegistry.get<MoneyComponent>(entity);
					mpEntityManager->RemoveEntity(money.mpOwner->GetUID());
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

			PrintIntroText(TutorialState::Enemy_Play);	// Ʃ�丮�� �ؽ�Ʈ ����

			mpLevelManager->DeleteEnemyAll();// �� �� ���� ������Ʈ ��ü ����

			if (mTextIdx == 13)
			{
				/// �̺�Ʈ �߻� : �ٰŸ� Ŭ���� ��ư�� Ŭ���Ѵ�.
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

			PrintIntroText(TutorialState::BUTTON_M);	// Ʃ�丮�� �ؽ�Ʈ ����

			/// �̺�Ʈ �߻� : ��ġ ��ġ�� Ŭ��
			mpLevelManager->TutorialClassDeployed(cursorFollowPos, isPayment, pSceneData);
			if (mpLevelManager->mTutorialFlag == true)
			{
				InvisibleGuideTile();					 // �ٴ� ǥ�� �� ���̰� ����
				mTutorialState = TutorialState::DEPLOY_M;
				mpLevelManager->mTutorialFlag = false;

			}
		}
		break;
		case TutorialState::DEPLOY_M:
		{
			SettingPlayingIndicator();
			PrintIntroText(TutorialState::DEPLOY_M);	// Ʃ�丮�� �ؽ�Ʈ ����

			if (mTextIdx == 15)
			{
				UpdateShrinkRect(_dTime);
			}
			if (mTextIdx == 16)
			{
				ResettingShrinkRect(Vector2(735, 600), Vector2(100, 100), 4, 20);
			}

			if (mTextIdx == 18)// �뺴�ܿ����� ���� �ν��ϴ� ������ ���� ������\n�ο�� �͵� ���ΰ� �켱 �ֿ췯 ���ϴ�. ����� ����������!
			{
				ChangeGuideTile("White.png", Vector2(-5.f, 0.f), Vector2(0.7f, 0.7f));
				mpUnitSystem->GetProjectileSystem()->TutirialUpdateThrow(_dTime);
				//mpUnitSystem->GetProjectileSystem()->TutirialUpdateMoney(_dTime);

				UpdateShrinkRect(_dTime);

				/// �̺�Ʈ �߻� : ������ �Ʊ��� �پ�� ���� �ڸ��� ������.
				if (mpInputManager->GetKeyUp(KEY::LBUTTON) && mpLevelManager->AddBlockScreenPos(mpInputManager->GetMousePos(), Vector2(1812, 997), Vector2(83, 58)))
				{
					if (cursorFollowPos.x >= -6 && cursorFollowPos.x <= -5 && cursorFollowPos.z >= -1 && cursorFollowPos.z <= 1)
					{
						mTime = 0.0f;
						InvisibleGuideTile();					 // �ٴ� ǥ�� �� ���̰� ����

						ResettingShrinkRect(Vector2(800, 150), Vector2(210, 210), 4, 20);
						mTutorialState = TutorialState::THROW_WAIT;
					}
					else
					{
						// ���� ��� ����!
						mpLevelManager->DeleteMoney();
					}
				}
			}

			auto textView = mRegistry.view<Text>();
			for (auto text : textView)
			{
				auto& name = mRegistry.get<Name>(text).mName;
				auto& textComp = mRegistry.get<Text>(text);
				if (name == "UI_B_Money")
				{
					textComp.mNum1 = pSceneData->m_heldMoney;
				}

				// ��ġ�� �Ʊ��� ���� ������Ʈ�Ѵ�.
				/*if (name == "UI_D_AliveNum")
				{
					textComp.mNum1 = pSceneData->m_aliveAlly;
				}*/

				if (name == "UI_B_Money")
				{
					if (isPayment == true)
					{
						pSceneData->m_heldMoney -= 100;
						isPayment == false;
					}
					textComp.mNum1 = pSceneData->m_heldMoney;
				}
			}
		}
		break;
		case TutorialState::THROW_WAIT:// ������ ������ ������ ������ ���´�.
		{
			mTime += _dTime;
			//mpUnitSystem->GetProjectileSystem()->TutirialUpdateMoney(_dTime);
			if (mTime > 1.0f)
			{
				// ���⼭ �÷��̾� ���� ���� ������ �����ϱ� ���� ü�¹ٸ� �߰��Ѵ�(�������� �ʰ�)
				auto playerView = mRegistry.view<PlayerComponent>();
				for (auto& entity : playerView)
				{
					mpUIManager->Add3DHPBar(mpEntityManager->GetEntity(entity), Vector3(0, 3.f, 0), Vector2(50, 5), 5, Vector4(0.2f, 0.2f, 0.2f, 1), Vector4(0.f, 0.f, 1.f, 1));
					mRegistry.get<HealthBarComponenet>(entity).mIsVisible = false;
					mpUnitSystem->GetPlayerSystem()->SetInitializeState(mpEntityManager->GetEntity(entity), State::IDLE);
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

			PrintIntroText(TutorialState::ALLY_Play);	// Ʃ�丮�� �ؽ�Ʈ ����

			// Ư�� ���ǿ��� ���� �����ؼ� Ÿ���� �ϰ� �ϴ� �Լ���.
			mpUnitSystem->GetPlayerSystem()->DetectGroundMoney();
			if (mTextIdx > 19) // �뺴�� ��ź�縦 ���� ���� �� �����Ⱑ �Ұ��մϴ�.
			{
				mpUnitSystem->GetProjectileSystem()->Update(_dTime);
			}

			// �÷��̾� ������Ʈ
			auto playerView = mRegistry.view<PlayerComponent>();
			for (auto& entity : playerView)
			{
				auto& playerComp = mRegistry.get<PlayerComponent>(entity);

				// �÷��̾� �����Ͽ� ������Ʈ�Ѵ�.
				mpUnitSystem->GetPlayerSystem()->PlayerUnitUpdate(mpEntityManager->GetEntity(entity), _dTime); // �Ʊ� ������ ������Ʈ �մϴ�.

				physx::PxFilterData filterData = mpPhysicsManager->GetFilterData(entity);
				filterData.word1 |= 8;
				mpPhysicsManager->SetFilterData(entity, filterData);
			}

			if (mTextIdx == 21)// ���� ��ư�� �� ������ ������\n�����ؼ� ���� �� �־�!
			{
				if (mpInputManager->GetKey(KEY::LBUTTON))
				{
					mTime = 0.0f;
					// ��� ���� ����!
					mpLevelManager->DeleteMoney();
					mTutorialState = TutorialState::BUFF;
					return;
					/*mPressClickTime += _dTime;
					if (mpInputManager->GetKeyUp(KEY::LBUTTON) && mpLevelManager->AddBlockScreenPos(mpInputManager->GetMousePos(), Vector2(1812, 997), Vector2(83, 58)))
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
				//	// ��� ���� ����!
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
			PrintIntroText(TutorialState::BUFF);	// Ʃ�丮�� �ؽ�Ʈ ����

			// ���� ������
			mpUnitSystem->GetPlayerSystem()->DetectGroundMoney();
			// �÷��̾� ������Ʈ
			mpLevelManager->PlayerUpdate(_dTime);

			if (mTextIdx == 25) // ������ �� ������ �ƴϾ��� ?
			{
				// ���� �뺴 �ٽ� ��ġ
				mpUnitSystem->GetPlayerSystem()->AddSceneEnemyUnits(0);

				// ���� ������Ʈ
				auto enemyView = mRegistry.view<EnemyComponent>();
				for (auto& entity : enemyView)
				{
					mpRenderManager->InitailizeEntity(mpEntityManager->GetEntity(entity));
				}
				mTutorialState = TutorialState::ENEMY;
				return;
			}
		}
		break;
		case TutorialState::ENEMY:
		{
			void SettingDeployIndicator(Vector3 _cursorPos, SceneData * _pSceneData);
			PrintIntroText(TutorialState::ENEMY);	// Ʃ�丮�� �ؽ�Ʈ ����

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
			else if (mTextIdx == 33)// �� ó���̴� �̹����� ���� ���� ��� �־�帮���� ����.
			{
				mTime += _dTime;

				// ��ġ�� ���� ����
				mpLevelManager->ResetPlayer(pSceneData);

				// ��ġ�� ���õ� ��ư ��� Ȱ��ȭ + ���� ������ ����
				auto textureView = mRegistry.view<Texture2D>();
				for (auto uiEntity : textureView)
				{
					auto& name = mRegistry.get<Name>(uiEntity).mName;
					auto& img = mRegistry.get<Texture2D>(uiEntity);
					auto button = mRegistry.try_get<Button>(uiEntity);

					if (name.find("UI_D") != std::string::npos ||
						name.find("UI_B") != std::string::npos)
					{
						/// ��� ��ư Ȱ��ȭ : ���� ���� ��ư�� ��� �Ұ�!
						if (button && name != "UI_D_Start")
						{
							button->mIsEnable = true;
						}

						if (button && img.mFile == "UI_btn_Merc02.png")
						{
							img.mRgba = Vector4(0.8f, 0.8f, 0.8f, 1.0f);
							button->mNormalRGBA = img.mRgba;
							button->mPressedRGBA = Vector4(1.0f);
							button->mHoveredRGBA = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
						}
					}

					if (name == "UI_PointMoney")
					{
						img.mIsVisible = true;
						if (img.mPosition.x > 215)
						{
							img.mPosition.x -= 17.5f; // (915 - 215)/40
						}
						else
						{
							img.mIsVisible = false;
							pSceneData->m_totalAmount = 5000;
							pSceneData->m_heldMoney = 5000;
						}
						if (img.mPosition.y > 50)
						{
							img.mPosition.y -= 11.f; //( 490 - 50)/40
						}
						if (img.mSize.x > 45)
						{
							img.mSize.x -= 1.f;
						}
						if (img.mSize.y > 50)
						{
							img.mSize.y -= 1.f;
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
						auto& name = mRegistry.get<Name>(uiEntity).mName;
						auto& img = mRegistry.get<Texture2D>(uiEntity);
						if (name == "UI_PointMoney")
						{
							img.mIsVisible = false;
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

			PrintIntroText(TutorialState::MONEY);	// Ʃ�丮�� �ؽ�Ʈ ����
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
				auto view = mRegistry.view<Text>();
				for (auto& entity : view)
				{
					auto& name = mRegistry.get<Name>(entity).mName;
					auto& text = mRegistry.get<Text>(entity);
					auto img = mRegistry.try_get<Texture2D>(entity);

					if (name == "UI_T")
					{
						text.mIsVisible = false;
						img->mIsVisible = false;
					}
				}
				ResettingShrinkRect(Vector2(1605, 950), Vector2(90, 90), 4, 20);
			}
		}
		break;

		case TutorialState::FINAL:
		{
			PrintIntroText(TutorialState::FINAL);	// Ʃ�丮�� �ؽ�Ʈ ����

			InvisibleGuideTile(); // Ÿ�� �� ���̰�

			UpdateShrinkRect(_dTime);

			auto view = mRegistry.view<Text>();
			for (auto& entity : view)
			{
				auto& name = mRegistry.get<Name>(entity).mName;
				auto& text = mRegistry.get<Text>(entity);
				auto img = mRegistry.try_get<Texture2D>(entity);

				if (name == "UI_T")
				{
					text.mIsVisible = true;
					img->mIsVisible = true;
				}
			}

			// ���� ���� ��ư Ȱ��ȭ
			auto textureView = mRegistry.view<Texture2D>();
			for (auto uiEntity : textureView)
			{
				auto& name = mRegistry.get<Name>(uiEntity).mName;
				auto button = mRegistry.try_get<Button>(uiEntity);

				if (name.find("UI_D") != std::string::npos ||
					name.find("UI_B") != std::string::npos)
				{
					/// ��� ��ư Ȱ��ȭ : ���� ���� ��ư Ȱ��ȭ!
					if (name == "UI_D_Start")
					{
						button->mIsEnable = true;
						if (mpUIManager->GetButtonState(button->mpOwner) == ButtonState::PRESSED)
						{
							if (mpInputManager->GetKeyUp(KEY::LBUTTON) && mpLevelManager->AddBlockScreenPos(mpInputManager->GetMousePos(), Vector2(1812, 997), Vector2(83, 58)))
							{
								mpLevelManager->SetGameState(GameState::PRE_PLAY);
								mpLevelManager->SetIsGameStart(true);
								if (mpLevelManager->GetIsClassButtonPressed() == true)
								{
									mpLevelManager->SetmIsOneDeploySet(true);  // ��ġ �Ϸ� �� ���� ����
									/*auto entity = mpEntityManager->GetEntity(mpLevelManager->GetTempUnitUID());
									entity->GetComponent<PlayerComponent>().mIsDeploy = false;

									mpEntityManager->RemoveEntity(mpLevelManager->GetTempUnitUID());*/
									mpLevelManager->SetTempUnitUID(-1);
									mpLevelManager->SetIsClassButtonPressed(false);
								}
								mpSoundManager->PlaySFX("Snd_sfx_BattleStart");

								ResettingShrinkRect(Vector2(925, 410), Vector2(100, 100), 4, 20);
								// 								mpSoundManager->PlaySFX("Snd_sfx_ClickBtn");
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
		auto view2 = mRegistry.view<Text>();
		for (auto& entity : view2)
		{
			auto& name = mRegistry.get<Name>(entity).mName;
			auto& text = mRegistry.get<Text>(entity);
			auto img = mRegistry.try_get<Texture2D>(entity);

			if (name == "UI_T")
			{
				text.mIsVisible = false;
				img->mIsVisible = false;
			}

			// Ÿ�̸Ӹ� �մϴ�.
			if (name == "UI_P_Timer")
			{
				text.mIsVisible = true;
				if (img) // �� �̰� �� �̹��� ����
				{
					img->mIsVisible = true;
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

		// Ư�� ���ǿ��� ���� �����ؼ� Ÿ���� �ϰ� �ϴ� �Լ���.
		mpUnitSystem->GetPlayerSystem()->DetectGroundMoney();

		// ��ġ Ÿ�� �� �ٲٱ�
		SettingRealPlayingIndicator(pSceneData);

		// ü�¹�
		auto playerView = mRegistry.view<PlayerComponent>();
		auto enemyView = mRegistry.view<EnemyComponent>();
		for (auto& entity : playerView)
		{
			auto& playerComp = mRegistry.get<PlayerComponent>(entity);
			if (auto helthBar = mRegistry.try_get<HealthBarComponenet>(entity)) // ü�¹ٰ� �ִ� �� �� �÷��̾� ������Ʈ�� ���� ���̴�.
			{
				float healthPercent = playerComp.mHP / playerComp.mMaxHP;
				helthBar->mHealthPercentage = healthPercent;
			}

			// �÷��̾� �����Ͽ� ������Ʈ�Ѵ�.
			mpUnitSystem->GetPlayerSystem()->PlayerUnitUpdate(mpEntityManager->GetEntity(entity), _dTime); // �Ʊ� ������ ������Ʈ �մϴ�.

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
			if (auto helthBar = mRegistry.try_get<HealthBarComponenet>(entity)) // ü�¹ٰ� �ִ� �� �� �÷��̾� ������Ʈ�� ���� ���̴�.
			{
				float healthPercent = enemyComp.mHP / enemyComp.mMaxHP;
				helthBar->mHealthPercentage = healthPercent;
			}

			mpUnitSystem->GetPlayerSystem()->EnemyUnitUpdate(mpEntityManager->GetEntity(entity), _dTime); // ���� ������ ������Ʈ �մϴ�.

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

		/// ���⼭ �뺴���� ������Ʈ �Ѵ�.
		auto npcView = mRegistry.view<NPCComponent>();
		for (auto& entity : npcView)
		{
			mpUnitSystem->GetNPCSystem()->Update(mpEntityManager->GetEntity(entity), numAlly, numEnemy);
		}

		auto textView = mRegistry.view<Text>();
		for (auto& entity : textView)
		{
			auto& name = mRegistry.get<Name>(entity).mName;
			auto& text = mRegistry.get<Text>(entity);
			// Ÿ�̸�
			if (name == "UI_P_Timer")
			{
				pSceneData->m_time -= _dTime;

				int minutes = static_cast<int>(pSceneData->m_time) / 60;
				int seconds = static_cast<int>(pSceneData->m_time) % 60;

				std::ostringstream oss;
				oss << std::setfill('0') << std::setw(2) << minutes << ":"
					<< std::setfill('0') << std::setw(2) << seconds;

				// std::u8string���� ��ȯ
				std::string str = oss.str();
				text.mText = std::u8string(str.begin(), str.end());

				// 10�� ��������, ���ȿ���� ���
				if (std::fabs(pSceneData->m_time - 10.0f) < std::numeric_limits<float>::epsilon())
				{
					mpSoundManager->PlaySFX("Snd_sfx_TimeLow");
				}

				// �ð��� �� �Ǹ� �ڵ����� �Ѿ
				if (pSceneData->m_time < std::numeric_limits<float>::epsilon())
				{
					pSceneData->m_time = pSceneData->m_totalTime;
					DLOG(LOG_INFO, "Game Over");
					mpLevelManager->SetGameState(GameState::TUTORIALEND);
					return;
				}
			}

			// �÷��̾��� �������� ������Ʈ �Ѵ�.
			if (name == "UI_B_Money")
			{
				text.mNum1 = pSceneData->m_heldMoney;
			}

			if (name == "UI_B_StageGuide")
			{
				text.mNum1 = numEnemy;
			}

		} // End for�� : mpEntityManager->GetEntityMap()
		if (numAlly == 0 || numEnemy == 0)
		{
			pSceneData->m_aliveAlly = numAlly;
			mpLevelManager->SetGameState(GameState::TUTORIALEND);
			mpSoundManager->StopBGM();
			if (numAlly == 0)
			{
				mpSoundManager->PlayBGM("Snd_bgm_AfterBattleLose");
			}
			else
			{
				mpSoundManager->PlayBGM("Snd_bgm_AfterBattleWin");
			}
			mpLevelManager->FadePreSetting(true);
			auto HPView = mRegistry.view<HealthBarComponenet>();
			for (auto entity : HPView)
			{
				auto& hpEntity = mRegistry.get<HealthBarComponenet>(entity);
				hpEntity.mIsVisible = false;
				auto& animation = mRegistry.get<AnimationComponent>(entity);
				animation.mPaused = true;
			}

			auto rigidView = mRegistry.view<Rigidbody>();
			for (auto& entity : rigidView)
			{
				auto& rigid = mRegistry.get<Rigidbody>(entity);
				rigid.mIsKinematic = true;
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
			auto view2 = mRegistry.view<Text>();
			for (auto& entity : view2)
			{
				auto& name = mRegistry.get<Name>(entity).mName;
				auto& text = mRegistry.get<Text>(entity);
				auto img = mRegistry.try_get<Texture2D>(entity);
				if (name == "UI_T")
				{
					text.mIsVisible = true;
					img->mIsVisible = true;
					text.mPosition = Vector2(392, 176);
					img->mFile = "UI_pnl_MsgBox2.png";
					img->mPosition = Vector2(367, 112);
					img->mSize = Vector2(1182, 170);
					text.mText = mTutorialTextVec[mTextIdx].text;
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
				mpWorldManager->GetCurrentWorld()->SetScene(static_cast<uint32_t>(SceneName::SCENE1));
				return;
			}
			/*auto view = mRegistry.view<FadeInOut>();
			for (auto& entity : view)
			{
				auto& name = mRegistry.get<Name>(entity).mName;
				auto& img = mRegistry.get<FadeInOut>(entity);
				if (name == "FaidInOut")
				{
					if (img.IsFadingOutFin(_dTime / 2, 1.0f) == true)
					{
						mpWorldManager->GetCurrentWorld()->SetScene(static_cast<uint32_t>(SceneName::SCENE1));
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
	if (mpSceneData)
	{
		delete mpSceneData;
	}
}

void ExampleScene::AddResource()
{
	//mpResourceManager->GetFileManager()->PrintAll(false);
	/// ���ҽ� �߰�
	// �� �߰�
	//mpRenderManager->AddModel("../TestAsset/", "box.fbx");

	/////UI
	//mpUIManager->AddTexture2D("../Resources/Texture/", "blue.png");

	//// �ִϸ��̼� �߰�
	//mpResourceManager->AddFilesInDirAni("Animation");

	//// 3D �ؽ�ó �߰�
	//mpResourceManager->AddFilesInDir3D("Texture3D");

	//// ť��� �ؽ�ó �߰�
	//mpResourceManager->AddFilesInDirDDS("CubeMap");

	//// Fbx �߰� -> ���� Ŭ���� ���� �ɰ� ��
	//mpResourceManager->AddFilesInDirModel("Mercenary");
	//mpResourceManager->AddFilesInDirModel("Environment");
	//mpResourceManager->AddFilesInDirModel("Archer");
	//mpResourceManager->AddFilesInDirModel("Enemy");
	//mpResourceManager->AddFilesInDirModel("money");
	//mpResourceManager->AddFilesInDirModel("Chief");
}

void ExampleScene::SettingDeployIndicator(Vector3 _cursorPos, SceneData* _pSceneData)
{
	// Ÿ�� �ε������ʹ� ������ �����Ų��.
	auto indicatorView = mRegistry.view<IndicatorComponent>();
	for (auto indiEntity : indicatorView)
	{
		if (mRegistry.get<Name>(indiEntity).mName == "selectedSquare")
		{
			auto& texture3dComp = mRegistry.get<Texture3D>(indiEntity);
			auto& indicatorComp = mRegistry.get<IndicatorComponent>(indiEntity);

			auto outline = mRegistry.try_get<OutlineComponent>(indiEntity);
			auto alpha = mRegistry.try_get<AlphaBlendComponent>(indiEntity);

			if (alpha->mAlpha < 0.5)
			{
				alpha->mAlpha += 0.01;
			}
			else
			{
				alpha->mAlpha = 0;
			}

			if (_cursorPos.x >= -15 && _cursorPos.x <= 15 && _cursorPos.z >= -15 && _cursorPos.z <= 15)
			{
				if (mpLevelManager->IsCanClickAreaXZ(_cursorPos, Vector2(-15, 10), Vector2(-9, -10), _pSceneData->m_aliveAlly) == true)
				{
					if (texture3dComp.mDiffuse != indicatorComp.mOriginalColor)
					{
						texture3dComp.mDiffuse = indicatorComp.mOriginalColor;
						outline->mColor = Vector3(0, 0, 1);
						mpRenderManager->UpdateEntityTexture(mpEntityManager->GetEntity(indiEntity));
					}
				}
				else
				{
					if (texture3dComp.mDiffuse != indicatorComp.mChangedColor)
					{
						texture3dComp.mDiffuse = indicatorComp.mChangedColor;
						outline->mColor = Vector3(1, 0, 0);
						mpRenderManager->UpdateEntityTexture(mpEntityManager->GetEntity(indiEntity));
					}
				}
				texture3dComp.mpOwner->GetComponent<Transform>().mLocalPosition = Vector3(static_cast<int>(_cursorPos.x), 0.1f, static_cast<int>(_cursorPos.z));
			}
		}
	}
}

void ExampleScene::SettingPlayingIndicator()
{
	// ��ġ Ÿ�� �� �ٲٱ�
	auto indicatorView = mRegistry.view<IndicatorComponent>();
	for (auto& entity : indicatorView)
	{
		if (mRegistry.get<Name>(entity).mName == "selectedSquare")
		{
			auto& tile = mRegistry.get<Texture3D>(entity);
			Vector3 cursorPos = mpPhysicsManager->PickObejct("plane");
			if (cursorPos.x > 0)cursorPos.x += 0.5;
			else cursorPos.x -= 0.5;
			if (cursorPos.z < 0) cursorPos.z -= 0.5;

			if (cursorPos.x >= -15 && cursorPos.x <= 15 && cursorPos.z >= -15 && cursorPos.z <= 15)
			{
				cursorPos = Vector3(static_cast<int>(cursorPos.x), 0.1f, static_cast<int>(cursorPos.z));
				Vector2 astarPos = {};
				mpAstar->AdjustToTileCenter(cursorPos, astarPos);
				if ((*mpAstar->GetAstarMap())[static_cast<int>(astarPos.y + abs(mStartPoint.y))][static_cast<int>(astarPos.x + abs(mStartPoint.x))] == 0) // ��ֹ��� ���� ���� Ŭ�� ����
				{
					tile.mDiffuse = "blue.png";
					mpRenderManager->UpdateEntityTexture(mpEntityManager->GetEntity(entity));
				}
				else
				{
					tile.mDiffuse = "red.png";
					mpRenderManager->UpdateEntityTexture(mpEntityManager->GetEntity(entity));
				}
				tile.mpOwner->GetComponent<Transform>().mLocalPosition = cursorPos;
			}
		}
	}
}

void ExampleScene::SettingRealPlayingIndicator(SceneData* _pSceneData)
{
	auto indicatorView = mRegistry.view<IndicatorComponent>();
	for (auto indiEntity : indicatorView)
	{
		if (mRegistry.get<Name>(indiEntity).mName == "selectedSquare")
		{
			auto& texture3dComp = mRegistry.get<Texture3D>(indiEntity);
			auto& indicatorComp = mRegistry.get<IndicatorComponent>(indiEntity);
			auto outline = mRegistry.try_get<OutlineComponent>(indiEntity);
			auto alpha = mRegistry.try_get<AlphaBlendComponent>(indiEntity);
			Vector3 cursorFollowPos = mpPhysicsManager->PickObejct("plane");
			if (cursorFollowPos.x > 0)cursorFollowPos.x += 0.5;
			else cursorFollowPos.x -= 0.5;
			if (cursorFollowPos.z < 0) cursorFollowPos.z -= 0.5;

			if (alpha->mAlpha < 0.5)
			{
				alpha->mAlpha += 0.01;
			}
			else
			{
				alpha->mAlpha = 0;
			}

			if (cursorFollowPos.x >= -15 && cursorFollowPos.x <= 15 && cursorFollowPos.z >= -15 && cursorFollowPos.z <= 15)
			{
				cursorFollowPos = Vector3(static_cast<int>(cursorFollowPos.x), 0.1f, static_cast<int>(cursorFollowPos.z));
				Vector2 astarPos = {};
				mpAstar->AdjustToTileCenter(cursorFollowPos, astarPos);
				if ((*mpAstar->GetAstarMap())[static_cast<int>(astarPos.y + abs(mStartPoint.y))][static_cast<int>(astarPos.x + abs(mStartPoint.x))] == 0) // ��ֹ��� ���� ���� Ŭ�� ����
				{
					texture3dComp.mDiffuse = "blue.png";
					outline->mColor = Vector3(0, 0, 1);
					mpRenderManager->UpdateEntityTexture(mpEntityManager->GetEntity(indiEntity));
				}
				else
				{
					texture3dComp.mDiffuse = "red.png";
					outline->mColor = Vector3(1, 0, 0);
					mpRenderManager->UpdateEntityTexture(mpEntityManager->GetEntity(indiEntity));
				}
				texture3dComp.mpOwner->GetComponent<Transform>().mLocalPosition = cursorFollowPos;
			}
		}
	}
}

void ExampleScene::ChangeGuideTile(const std::string& _texFileName, Vector2 _posXZ, Vector2 _sizeXZ)
{
	auto view = mRegistry.view<Texture3D>();
	for (auto& entity : view)
	{
		auto& name = mRegistry.get<Name>(entity).mName;
		auto& img = mRegistry.get<Texture3D>(entity);
		if (name == "UI_H")
		{
			auto trs = mRegistry.try_get<Transform>(entity);
			img.mDiffuse = _texFileName;
			trs->mLocalPosition.x = _posXZ.x;
			trs->mLocalPosition.y = 0.005f;
			trs->mLocalPosition.z = _posXZ.y;
			trs->mLocalScale.x = _sizeXZ.x;
			trs->mLocalScale.z = _sizeXZ.y;
			mpRenderManager->UpdateEntityTexture(mpEntityManager->GetEntity(entity));
			return;
		}
	}
}

void ExampleScene::ChangeGuideTileTrs(Vector2 _posXZ, Vector2 _sizeXZ)
{
	auto view = mRegistry.view<Texture3D>();
	for (auto& entity : view)
	{
		auto& name = mRegistry.get<Name>(entity).mName;
		auto& img = mRegistry.get<Texture3D>(entity);
		if (name == "UI_H")
		{
			auto trs = mRegistry.try_get<Transform>(entity);
			trs->mLocalPosition.x = _posXZ.x;
			trs->mLocalPosition.y = 0.005f;
			trs->mLocalPosition.z = _posXZ.y;
			trs->mLocalScale.x = _sizeXZ.x;
			trs->mLocalScale.z = _sizeXZ.y;
			return;
		}
	}
}

void ExampleScene::InvisibleGuideTile()
{
	auto view = mRegistry.view<Texture3D>();
	for (auto& entity : view)
	{
		auto& name = mRegistry.get<Name>(entity).mName;
		auto& img = mRegistry.get<Texture3D>(entity);
		if (name == "UI_H")
		{
			auto trs = mRegistry.try_get<Transform>(entity);
			trs->mLocalPosition.y = -0.1f;
			return;
		}
	}
}

void ExampleScene::ChangeGuideTileTex(const std::string& _texFileName)
{
	auto view = mRegistry.view<Texture3D>();
	for (auto& entity : view)
	{
		auto& name = mRegistry.get<Name>(entity).mName;
		auto& img = mRegistry.get<Texture3D>(entity);
		if (name == "UI_H")
		{
			img.mDiffuse = _texFileName;
			mpRenderManager->UpdateEntityTexture(mpEntityManager->GetEntity(entity));
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
		// 1 : �ȳ�, ���� ��ġ ��Ŀ����� �� // ���� ���� ����, ��¥ ������ ����!
		// 2 : �׸��� ���� ��Ȯ�� ���ϴ� ���� ���� ���� �� �־�. ����?
		// 3 : ���콺 ���� ��ư���� ������ ���� �� �ֽ��ϴ�. ������ ��ġ�� ������ ����������.
		break;
	case TutorialState::THROW_O:
		SkipText(7);
		// 4 : ��ֹ����� ���� �� ����. �� �뺴���� ��ģ�ٰ�.
		// 5 : �׷� �� ������ ��������..
		// 6 : ��!\n�� �Ʊ�� ���� �� �ۿ��� ����!\n�뺴�鿡�� �� ������ ���̾�!
		// 7 : �׷� �̰��� �������ô�.
		break;
	case TutorialState::THROW:
		SkipText(9);
		// 8 : ����!\n���� ��Ȯ�� ���� �� ������?
		// 9 : ���� �������Գ� �����մϴ�.\n������ ���� ���� �̷��� �޷���ŵ��. �Ʊ����.
		break;
	case TutorialState::Enemy_Play:
		SkipText(13);
		// 10 : ��! �ӾҾ�!
		// 11 : ��ġ ��Ŀ�� ���� ���ϴ� ��� �Ʊ� �뺴���� ���� �ຸ���� ����.\n�Ʊ� �뺴�� ��ȯ�غ��ô�.
		// 12 : ���Ͷ�! ���Ÿ� �뺴!
		// 13 : �ٰŸ� �뺴���� ��ġ�սô�.\n�ϴܿ� ù��° ��ư�� ���������?
		break;
	case TutorialState::BUTTON_M:
		SkipText(14);
		// 14 : ���߾��.\n���������� ǥ�õ� ������ ��ġ�սô�.
		break;
	case TutorialState::DEPLOY_M:
		SkipText(18);
		// 15 : ���� ���� �� �� ��ġ�ߴ��� ������ �� �� �־�.
		// 16 : Ŭ���� ��ư�� �̹� ���� ä��� ��ġ ������ �����Ӱ� �� ��ġ�� �� �־��.
		// 17 : ���Ÿ� �뺴 ��ư�� ������ ���Ÿ� �뺴�� ��ġ�� �� ����!
		// 18 : �뺴�ܿ����� ���� �ν��ϴ� ������ ���� ������\n�ο�� �͵� ���ΰ� �켱 �ֿ췯 ���ϴ�. ����� ����������!
		break;
	case TutorialState::ALLY_Play:
		SkipText(21);
		// 19 : ũ! �� �뺴!\n�� �����̴� ��!\n4����̶��! \n���� 100G�� ��ġ�� �־�!
		// 20 : ���� ��� ���̸� ���� ������ �޽��ϴ�.\n��� ���� ����������.
		// 21 : ���� ��ư�� �� ������ ������\n�����ؼ� ���� �� �־�!
		break;
	case TutorialState::BUFF:
		SkipText(25);
		// 22 : ������ ������ ���ݷµ� �������� ü�µ� �������ϴ�.\n������ �� 3�ܰ�� �� �̻��� ���� �Ծ �ҿ��� �����.
		// 23 : �̷�����!\n�뺴���� ���� ��� �������ٰ� �ߴµ�!
		// 24 : ��, �׷� ���� �ٰŸ��� ���Ÿ� �뺴�� ���������� ��ġ�غ���.\n������ �뺴 Ÿ���� Ȯ���ϰ� ��ġ�Ѵٸ� �·��� �� �ö󰡰���?
		// 25 : ������ �� ������ �ƴϾ��� ?
		break;
	case TutorialState::ENEMY:
		mIsReadyResetShrinkRect = false;
		SkipText(33);
		// 26 : ���� ����⸦ �� �뺴�� �ٰŸ� �뺴�̰�,
		// 27 : ���� Ȱ�� �� �뺴�� ���Ÿ� �뺴�̿���. 
		// 28 : �뺴 �ϳ��� 100G�ۿ� ���ϴϱ�\n20�� �� ��ġ����!
		// 29 : ��ġ ��Ŀ������ �̹��� ���� �� ���� 3000G�ۿ� �� �ȴٴ� �� �ؾ��׿�. 
		// 30 : �´�!\n���� ������ �� �������� Ȯ���� �� �־�.
		// 31 : ������ �������� ���� ��ũ ������ Ȯ���� �� �ֽ��ϴ�.\n���� ���� �������� ���� ��ũ�� ���� �� �־��.
		// 32 : ���� ���ݸ� ��߰���?
		// 33 : �� ó���̴� �̹����� ���� ���� ��� �־�帮���� ����.
		break;
	case TutorialState::MONEY:
		mIsReadyResetShrinkRect = true;
		SkipText(34);
		// 34 : ��ȣ!\n20�� �� ��ġ����!\n������ �Ʒ� ������ ��ư��\n������ ��ġ�� �ٽ� �� �� �־�!
		break;
	case TutorialState::FINAL:
		SkipText(35);
		// 35 : ������ �����ϼ���!
		break;
	case TutorialState::PLAYING:
		break;
	case TutorialState::END:
		SkipText(36);
		// 36 : ��ġ ��Ŀ�� ���� �� ��Ź�մϴ�.
		break;
	case TutorialState::LAST:
		break;
	default:
		break;
	}


}

void ExampleScene::SkipText(int _endIdx)
{
	while (mTextIdx < _endIdx && mpInputManager->GetKeyUp(KEY::LBUTTON) && mpLevelManager->AddBlockScreenPos(mpInputManager->GetMousePos(), Vector2(1812, 997), Vector2(83, 58)))
	{
		mTextIdx++;
		mIsReadyResetShrinkRect = true;
		return;
	}
}

void ExampleScene::UpdateShrinkRect(float _dTime)
{
	auto shrinkRectView = mRegistry.view<ShrinkRectComponent>();
	for (auto& entity : shrinkRectView)
	{
		auto& rect = mRegistry.get<ShrinkRectComponent>(entity);
		auto box2D = mRegistry.try_get<Box2D>(entity);
		box2D->mIsVisible = true;
		rect.Update(*box2D, _dTime);
	}
}

void ExampleScene::ResettingShrinkRect(Vector2 _targetRectPos, Vector2 _targetSize, int _sizeMultiplier, int _shrinkAmount)
{
	if (mIsReadyResetShrinkRect == true)
	{
		Vector2 adjustPos = _targetRectPos - _targetSize * (_sizeMultiplier - 1) * 0.5f;
		auto shrinkRectView = mRegistry.view<ShrinkRectComponent>();
		for (auto& entity : shrinkRectView)
		{
			auto& rect = mRegistry.get<ShrinkRectComponent>(entity);
			auto box2D = mRegistry.try_get<Box2D>(entity);
			box2D->mIsVisible = false;
			rect.Resetting(*box2D, adjustPos, _targetSize * _sizeMultiplier, _targetSize, _shrinkAmount);
		}
	}
}

