#include "DearsGameEngineAPI.h"
#include "LoadingScene.h"
#include "ResourceManager.h"
#include "LayerEnum.h"
#include "SceneNumEnum.h"
#include "MessageBox2D.h"

/// [Riun] ���� �޴����� �ε��� ������ �� Ȯ���ϴ� �÷��װ� �־�� ��. �� ������ �Ʒ������� �̵��ϵ��� �����ؾ���

LoadingScene::LoadingScene(entt::registry& _registry, const std::string& _name,
	EventManager* _pEventManager, RenderManager* _pRenderManager,
	PhysicsManager* _pPhysicsManager, InputManager* _pInpueManager,
	WorldManager* _worldManager, UIManager* _pUIManager, EntityManager* _pEntityManager,
	ResourceManager* _pResourceManager, SoundManager* _pSoundManager)
	: Scene(_registry, _name, _pEventManager, _pRenderManager, _pPhysicsManager
		, _pInpueManager, _worldManager, _pUIManager, _pEntityManager, _pResourceManager
		, _pSoundManager)
	, tempEasing(nullptr)
{
	mpLevelManager = nullptr;
	accumulateTime = 0;
	maxTimeforPercetage = 6.0f;
	mLoadingMaxSize = 1500.f;
}

bool LoadingScene::Initialize()
{
	AddResource();
	tempEasing = new EasingFunc();
	accumulateTime = 0;

	mpLevelManager = new LevelManager(mRegistry, mpRenderManager, mpInputManager, mpUIManager, mpEntityManager
		, mpResourceManager, mpSoundManager);
	mpLevelManager->InitializeforNoneFbx(GetUID());

	// ī�޶� ��ƼƼ ����, ����
	mpLevelManager->AddCamera(Vector3(0.f, 5.f, -50.f), Vector3(0.f, 0.f, 1.f));

	// �ε��� ���� �̹���
	auto ui2 = mpEntityManager->CreateEntity("UI_L_Img"); // ������ 1080+96 = 1176���� ����ش�.
	std::string randimLodingImg = "UI_img_LoadingImg" + to_string(RandomUtil::RandomInt(1, 10)) + ".png";
	mpUIManager->AddUI(ui2, randimLodingImg, Vector2(495, -1080), Vector2(930, 797), static_cast<int>(Layer::BG), Vector4(1.f), false);

	// �ε� ������ �޹�� + ������ ���� + �ε� �ؽ�Ʈ
	auto ui3 = mpEntityManager->CreateEntity("UI_L_Loading");
	mpUIManager->AddUI(ui3, "UI_img_LoadingBG.png", Vector2(190, -185), Vector2(1540, 100), static_cast<int>(Layer::BG_P), Vector4(1.f), false);
	mpUIManager->AddMessageBox2D(ui3, "UI_img_LoadingBar.png", Vector2(210, -185), Vector2(1500, 60), Vector4(1.0f), u8"Now Loading...", Vector2(759, -271), 0.8f, "KIMM_Bold(60).ttf", Vector4(1.0f), static_cast<int>(Layer::BG_B), false);

	/// UI
	mpLevelManager->AddAnimationUI(false, static_cast<int>(mpWorldManager->GetCurrentWorld()->GetPreviousScene()));
	mpLevelManager->SetUIAnimationState(UIAnimationState::PRELOADING); // UI �ִϸ��̼� ���� ����
	tempTime = 0.0f;

	// ���� ���� ��Ʋ���̾����� Ȯ��
	if (mpWorldManager->GetCurrentWorld()->GetPreviousScene() >= static_cast<uint32_t>(SceneName::TUTORIAL))
	{
		auto view = mRegistry.view<Texture2D>();
		for (auto& entity : view)
		{
			auto& name = mRegistry.get<Name>(entity).mName;
			auto& texture = mRegistry.get<Texture2D>(entity);
			if (name.find("UI_A_Cloud") != std::string::npos)
			{
				texture.mPosition.y = -726.f;
			}
		}
	}

	return Scene::Initialize();
}

void LoadingScene::FixedUpdate(float _dTime)
{
	switch (mpLevelManager->GetUIAnimationState())
	{
	case UIAnimationState::PRELOADING:
	{
		tempTime += _dTime;

		auto view = mRegistry.view<Texture2D>();
		for (auto& entity : view)
		{
			auto& name = mRegistry.get<Name>(entity).mName;
			auto& texture = mRegistry.get<Texture2D>(entity);
			if (name.find("UI_A_Cloud") != std::string::npos)
			{
				if (mpWorldManager->GetCurrentWorld()->GetPreviousScene() == static_cast<uint32_t>(SceneName::MAIN))
				{
					tempEasing->EasingFuncMap["easeInSine"](-2983, 1080, tempTime, &texture.mPosition.y);
				}
				else // ��ε� �ÿ��� ���� ��ġ�� -726
				{
					tempEasing->EasingFuncMap["easeInSine"](-726, 1080, tempTime, &texture.mPosition.y);
				}

				if (texture.mPosition.y == 1080)
				{
					mpLevelManager->SetUIAnimationState(UIAnimationState::VISIBLE); // UI �ִϸ��̼� ���� ����
					tempTime = 0.0f;
					return;
				}

			}
		}
	}
	break;
	case UIAnimationState::VISIBLE:	  // ������ �ڵ� :���⼭ �ε��� ���õ� ui�� ���̰� �����մϴ�.
	{
		auto view = mRegistry.view<Texture2D>();
		for (auto& entity : view)
		{
			auto& name = mRegistry.get<Name>(entity).mName;
			auto& texture = mRegistry.get<Texture2D>(entity);
			if (name.find("UI_L") != std::string::npos)
			{
				texture.mIsVisible = true;
			}
		}

		auto uiView = mRegistry.view<MessageBox2D>();
		for (auto& loadingEntity : uiView)
		{
			auto& loadingBar = mRegistry.get<MessageBox2D>(loadingEntity);
			auto& name = mRegistry.get<Name>(loadingEntity).mName;
			if (name == "UI_L_Loading")
			{
				loadingBar.mIsImgVisible = true;
				loadingBar.mIsTextVisible = true;
			}

		}
		mpLevelManager->SetUIAnimationState(UIAnimationState::NORMAL); // UI �ִϸ��̼� ���� ����
	}
	break;
	case UIAnimationState::NORMAL:	  // ������ �ڵ�
	{
		tempTime += _dTime;

		auto view = mRegistry.view<Texture2D>();
		for (auto& entity : view)
		{
			auto& name = mRegistry.get<Name>(entity).mName;
			auto& texture = mRegistry.get<Texture2D>(entity);
			if (name == "UI_L_Img")
			{
				tempEasing->EasingFuncMap["easeOutSine"](-1080, 96, tempTime * 0.5f, &texture.mPosition.y);
			}

			if (name == "UI_L_Loading")
			{
				tempEasing->EasingFuncMap["easeOutSine"](-185, 971, tempTime * 0.5f, &texture.mPosition.y);
			}
		}

		auto uiView = mRegistry.view<MessageBox2D>();
		for (auto& loadingEntity : uiView)
		{
			auto& loadingBar = mRegistry.get<MessageBox2D>(loadingEntity);
			auto& name = mRegistry.get<Name>(loadingEntity).mName;
			if (name == "UI_L_Loading")
			{
				tempEasing->EasingFuncMap["easeOutSine"](-185, 991, tempTime * 0.5f, &loadingBar.mTexturePosition.y);
				tempEasing->EasingFuncMap["easeOutSine"](-271, 905, tempTime * 0.5f, &loadingBar.mTextPosition.y);
			}
		}
	}
	break;
	case UIAnimationState::POSTLOATING: // �� �κ��� �ε��� 100% �Ϸ�Ǹ� LoadingResourceUpdate���� ���¸� �Ѱ��ش�.
	{
		tempTime += _dTime;

		auto view = mRegistry.view<Texture2D>();
		for (auto& entity : view)
		{
			auto& name = mRegistry.get<Name>(entity).mName;
			auto& texture = mRegistry.get<Texture2D>(entity);
			if (name == "UI_L_Img")
			{
				tempEasing->EasingFuncMap["easeOutSine"](96, -1080, tempTime, &texture.mPosition.y);
			}

			if (name == "UI_L_Loading")
			{
				tempEasing->EasingFuncMap["easeOutSine"](991, -185, tempTime, &texture.mPosition.y);
			}

			if (name.find("UI_A_Cloud") != std::string::npos)
			{
				if (tempTime > 1.0f)
				{
					tempEasing->EasingFuncMap["easeInSine"](1080, -726, (tempTime - 1.0f), &texture.mPosition.y);
					if (name == "UI_A_CloudR" && texture.mPosition.y == -726)
					{
						mpLevelManager->SetUIAnimationState(UIAnimationState::NEXTSCENE); // UI �ִϸ��̼� ���� ����
						tempTime = 0.0f;
						return;
					}
				}
			}
		}

		auto uiView = mRegistry.view<MessageBox2D>();
		for (auto& loadingEntity : uiView)
		{
			auto& loadingBar = mRegistry.get<MessageBox2D>(loadingEntity);
			auto& name = mRegistry.get<Name>(loadingEntity).mName;
			if (name == "UI_L_Loading")
			{
				tempEasing->EasingFuncMap["easeOutSine"](991, -185, tempTime, &loadingBar.mTexturePosition.y);
				tempEasing->EasingFuncMap["easeOutSine"](905, -271, tempTime, &loadingBar.mTextPosition.y);
			}
		}
	}
	break;
	case UIAnimationState::NEXTSCENE: // ������ �ڵ�
	{
		tempTime = 0.0f;
		accumulateTime = 0.0f;

		if (mpWorldManager->GetCurrentWorld()->GetPreviousScene() >= static_cast<uint32_t>(SceneName::SCENE1))
		{
			mpWorldManager->GetCurrentWorld()->SetScene(mpWorldManager->GetCurrentWorld()->GetPreviousScene() + 1);
		}
		else if (mpWorldManager->GetCurrentWorld()->GetPreviousScene() == static_cast<uint32_t>(SceneName::MAIN))
		{
			mpWorldManager->GetCurrentWorld()->SetScene(static_cast<uint32_t>(SceneName::TUTORIAL)); // [TODO] �� �κ� �ڱⰡ �� �ɷ� �ؼ� �����;� ���ٵ�
		}
	}
	break;
	default:
		break;
	}
}

void LoadingScene::Update(float _dTime)
{
	// ���� �ð��� ���� �ؽ�Ʈ�� �����ϰ�, ���ҽ��� ���� ���̿� �߰��մϴ�.
	accumulateTime += _dTime;

	LoadingTextUpdate();
	LoadingResourceUpdate();
}

void LoadingScene::LateUpdate(float _dTime)
{
}

void LoadingScene::Finalize()
{
	Scene::Finalize();
}

void LoadingScene::AddResource()
{
	//���ҽ� �߰�
	mpRenderManager->AddModel("../TestAsset/", "box.fbx");
}

void LoadingScene::LoadingTextUpdate()
{
	// �ؽ�Ʈ ����
	auto uiView = mRegistry.view<MessageBox2D>();
	for (auto& loadingEntity : uiView)
	{
		auto& loadingBar = mRegistry.get<MessageBox2D>(loadingEntity);
		auto img = mRegistry.try_get<Texture2D>(loadingEntity);
		auto& name = mRegistry.get<Name>(loadingEntity).mName;
		float nowSize = 0;

		if (accumulateTime > maxTimeforPercetage)
		{
			nowSize = maxTimeforPercetage;
		}
		else
		{
			nowSize = accumulateTime;
		}
		loadingBar.mSize.x = (nowSize / maxTimeforPercetage) * mLoadingMaxSize; 

		if (accumulateTime - static_cast<int>(accumulateTime) > 0)
		{
			loadingBar.mText = u8"Now Loading.";
		}
		if (accumulateTime - static_cast<int>(accumulateTime) > loadingTextInterval)
		{
			loadingBar.mText = u8"Now Loading..";
		}
		if (accumulateTime - static_cast<int>(accumulateTime) > loadingTextInterval * 2)
		{
			loadingBar.mText = u8"Now Loading...";
		}
	}
}

void LoadingScene::LoadingResourceUpdate()
{
	if (mpLevelManager->GetUIAnimationState() == UIAnimationState::POSTLOATING
		|| mpLevelManager->GetUIAnimationState() == UIAnimationState::NEXTSCENE) // POSTLOATIN, NEXTSCENE�� ���� ������ �ʿ� ����
	{
		return;
	}

	if (mIsResourcesAlreadyLoaded && accumulateTime > 5.0f)
	{
		// �ε� �Ϸ� �� �� ��ȯ
		mpLevelManager->SetUIAnimationState(UIAnimationState::POSTLOATING); // UI �ִϸ��̼� ���� ����
		tempTime = 0.0f;
		return;
	}


	// 0~1��: �� �߰�, UI �ؽ�ó �߰�, �ִϸ��̼� �߰�
	if (accumulateTime > 0.0f && mIsResourceLoaded1 == false)
	{
		mpResourceManager->AddFilesInDirModel("Geometry");
		mpResourceManager->AddFilesInDirAni("Animation");
		mIsResourceLoaded1 = true;
	}

	// 1~6��: 3D �ؽ�ó �߰�
	if (accumulateTime > 1.0f && mIsResourceLoaded2 == false)
	{
		mpResourceManager->AddFilesInDir3D("Texture3D");
		mpResourceManager->AddFilesInDirDDS("CubeMap");
		mIsResourceLoaded2 = true;
	}

	// 5��: ť��� �ؽ�ó �߰�
	if (accumulateTime > 2.0f && mIsResourceLoaded3 == false)
	{
		mpResourceManager->AddFilesInDirModel("Mercenary");
		mpResourceManager->AddFilesInDirModel("Archer");
		mIsResourceLoaded3 = true;
	}

	// 6�� ~ 10��: �� �߰� (����), ���� �߰�
	if (accumulateTime > 3.0f && mIsResourceLoaded4 == false)
	{
		mpResourceManager->AddFilesInDirModel("Enemy");
		mpResourceManager->AddFilesInDirModel("money");
		mpResourceManager->AddFilesInDirModel("Chief");
		mIsResourceLoaded4 = true;

	}
	else if (accumulateTime > 5.0f && mIsResourceLoaded5 == false)
	{
		mpResourceManager->AddFilesInDirModel("Environment");
		mIsResourceLoaded5 = true;
	}
	else if (accumulateTime > 6.0f)
	{
		mpResourceManager->AddFilesInDirBGMSound("BGM");
		mpResourceManager->AddFilesInDirSFXSound("SFX"); // ���� �ٸ� ���� �ű�
		// �ε� �Ϸ� �� �� ��ȯ
		mIsResourcesAlreadyLoaded = true;
		mpLevelManager->SetUIAnimationState(UIAnimationState::POSTLOATING); // UI �ִϸ��̼� ���� ����
		tempTime = 0.0f;
		maxTimeforPercetage = 5.0f;
		return;
	}
}
