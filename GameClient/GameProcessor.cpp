#include "DearsGameEngineAPI.h"
#include "GameProcessor.h"
#include "GameWorld.h"
#include "WeaponManager.h"

GameProcessor::GameProcessor(HINSTANCE _hInstance, const WCHAR* _szTitle, int _iconResourceId)
	: BaseWindowsProcessor(_hInstance, _szTitle, _iconResourceId)
	, mpGameEngine(nullptr)
	, mFixedDeltaTime()
	, mDeltaTime()
{
	mScreenPosX = 0;
	mScreenPosY = 0;
}

bool GameProcessor::Initialize()
{

	if (!__super::Initialize())
	{
		return false;
	}

	/// ���� ���� �ʱ�ȭ
	mpGameEngine = std::make_unique<GameEngine>(mHWnd, mScreenWidth, mScreenHeight);
	mpGameEngine->Initialize();
	mpGameEngine->GetTimeManager()->SetTargetFPS(60);

	/// �߰��� �ۼ��� �Ŵ��� Ŭ���� ����, �ʱ�ȭ
// 	std::shared_ptr<IManager> weaponManager = std::make_shared<WeaponManager>(m_pGameEngine->mRegistry, m_pGameEngine->GetRenderManager());
// 	weaponManager->Initialize();
// 	m_pManagers.push_back(weaponManager);

	/// ���� ����
	auto gameWorld = std::make_shared<GameWorld>(mpGameEngine->mRegistry, mpGameEngine.get()
		, mpGameEngine->GetEventManager());
	mpGameEngine->GetWorldManager()->AddWorld(gameWorld);
	gameWorld->Initialize();
	mpGameEngine->GetWorldManager()->GetCurrentWorld()->SetScene(0);

	return true;
}

void GameProcessor::Run()
{
	auto timer = mpGameEngine->GetTimeManager();

	while (true)
	{
		if (PeekMessage(&mMsg, NULL, 0, 0, PM_REMOVE))
		{
			if (mMsg.message == WM_QUIT)
			{
				break;
			}

			if (!TranslateAccelerator(mMsg.hwnd, mHAccelTable, &mMsg))
			{
				TranslateMessage(&mMsg);
				DispatchMessage(&mMsg);
			}
		}
		else
		{
			// ���� �������� ���� ��û Ȯ��
			if (mpGameEngine->GetWorldManager()->ShouldQuit())
			{
				// Finalize ȣ�� �� �����ϰ� PostQuitMessage ȣ��
				Finalize();
				PostQuitMessage(0);
				break;
			}

			// Ÿ�̸� ������Ʈ
			timer->Update();
			double frameTime = 1.0 / timer->GetTargetFPS();
			double deltaTime = timer->DeltaTime();

			// ������ ������ ���� ���� �ð��� ������ ��ٸ�
			if (deltaTime < frameTime)
			{
				double waitTime = frameTime - deltaTime;
				uint64_t startWait = timer->GetSystemTime().time_since_epoch().count();
				uint64_t endWait = startWait + static_cast<uint64_t>(waitTime * 1e9); // ������ ����

				while (timer->GetSystemTime().time_since_epoch().count() < endWait)
				{
					// ���� �ð� ���� �ƹ��͵� ���� �ʰ� ���
					//Sleep(1);
				}

				// Ÿ�̸� �ٽ� ������Ʈ (��� �� ��Ȯ�� �ð� ���)
				timer->Update();
			}

			// ���� ������Ʈ �� ������
			Update();
		}
	}
}

void GameProcessor::Finalize()
{
	__super::Finalize();
	mpGameEngine->Finalize();
}

LRESULT CALLBACK GameProcessor::WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	switch (_message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(_wParam);
		// �޴� ������ ���� �м��մϴ�:
		switch (wmId)
		{
		default:
			return DefWindowProc(_hWnd, _message, _wParam, _lParam);
		}
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		// ���� Ȯ�� �޽����� ���ų�, �۾��� ������ �� �ִ� ��ȸ�� ����
		if (MessageBox(_hWnd, L"������ �����Ͻðڽ��ϱ�?", L"���� Ȯ��", MB_OKCANCEL) == IDOK)
		{
			DestroyWindow(_hWnd); // â�� �ı��Ͽ� WM_DESTROY�� �߻���Ŵ
		}
		break;
	default:
		return DefWindowProc(_hWnd, _message, _wParam, _lParam);
	}
	return 0;
}

void GameProcessor::Update()
{
	// Ÿ�̸� ������Ʈ
	auto timer = mpGameEngine->GetTimeManager();
	timer->Update();

	mFixedDeltaTime = timer->FixedDeltaTime();

	mDeltaTime = timer->DeltaTime();
	mAccumulator += mDeltaTime;

	// ������ ������ ������Ʈ
	if(mAccumulator >= mFixedDeltaTime)
	{
		mpGameEngine->FixedUpdate(mFixedDeltaTime);

		// �߰� �Ŵ��� ���� ������ ������Ʈ
		for (auto& manager : mpManagers)
		{
			manager->FixedUpdate(mFixedDeltaTime);
		}

		mAccumulator -= mFixedDeltaTime; // ���������� ������ ������ �ð���ŭ ��
	}

	// ������Ʈ
	mpGameEngine->Update(mDeltaTime);

	// �߰� �Ŵ��� ������Ʈ
	for (auto& manager : mpManagers)
	{
		manager->Update(mDeltaTime);
	}

	// ��ó�� ������Ʈ
	mpGameEngine->LateUpdate(mDeltaTime);

	// �߰� �Ŵ��� ��ó�� ������Ʈ
	for (auto& manager : mpManagers)
	{
		manager->LateUpdate(mDeltaTime);
	}

	// ������
	mpGameEngine->BeginRender();
	
	// ingui���� �� ������ BeginRender�ÿ� ���콺 Ŀ���� �ʱ�ȭ��Ű�⶧���� ���⿡ �߰���
	mpGameEngine->GetInputManager()->SetMouseCursor();	
	
	mpGameEngine->Render(mDeltaTime);
	mpGameEngine->EndRender();

}
