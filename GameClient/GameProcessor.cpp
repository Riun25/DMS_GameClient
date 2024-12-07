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

	/// 게임 엔진 초기화
	mpGameEngine = std::make_unique<GameEngine>(mHWnd, mScreenWidth, mScreenHeight);
	mpGameEngine->Initialize();
	mpGameEngine->GetTimeManager()->SetTargetFPS(60);

	/// 추가로 작성한 매니저 클래스 생성, 초기화
// 	std::shared_ptr<IManager> weaponManager = std::make_shared<WeaponManager>(m_pGameEngine->mRegistry, m_pGameEngine->GetRenderManager());
// 	weaponManager->Initialize();
// 	m_pManagers.push_back(weaponManager);

	/// 월드 생성
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
			// 게임 엔진에서 종료 요청 확인
			if (mpGameEngine->GetWorldManager()->ShouldQuit())
			{
				// Finalize 호출 후 안전하게 PostQuitMessage 호출
				Finalize();
				PostQuitMessage(0);
				break;
			}

			// 타이머 업데이트
			timer->Update();
			double frameTime = 1.0 / timer->GetTargetFPS();
			double deltaTime = timer->DeltaTime();

			// 프레임 제한을 위해 남은 시간이 있으면 기다림
			if (deltaTime < frameTime)
			{
				double waitTime = frameTime - deltaTime;
				uint64_t startWait = timer->GetSystemTime().time_since_epoch().count();
				uint64_t endWait = startWait + static_cast<uint64_t>(waitTime * 1e9); // 나노초 단위

				while (timer->GetSystemTime().time_since_epoch().count() < endWait)
				{
					// 남은 시간 동안 아무것도 하지 않고 대기
					//Sleep(1);
				}

				// 타이머 다시 업데이트 (대기 후 정확한 시간 계산)
				timer->Update();
			}

			// 게임 업데이트 및 렌더링
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
		// 메뉴 선택을 구문 분석합니다:
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
		// 종료 확인 메시지를 띄우거나, 작업을 저장할 수 있는 기회를 제공
		if (MessageBox(_hWnd, L"정말로 종료하시겠습니까?", L"종료 확인", MB_OKCANCEL) == IDOK)
		{
			DestroyWindow(_hWnd); // 창을 파괴하여 WM_DESTROY를 발생시킴
		}
		break;
	default:
		return DefWindowProc(_hWnd, _message, _wParam, _lParam);
	}
	return 0;
}

void GameProcessor::Update()
{
	// 타이머 업데이트
	auto timer = mpGameEngine->GetTimeManager();
	timer->Update();

	mFixedDeltaTime = timer->FixedDeltaTime();

	mDeltaTime = timer->DeltaTime();
	mAccumulator += mDeltaTime;

	// 고정된 프레임 업데이트
	if(mAccumulator >= mFixedDeltaTime)
	{
		mpGameEngine->FixedUpdate(mFixedDeltaTime);

		// 추가 매니저 고정 프레임 업데이트
		for (auto& manager : mpManagers)
		{
			manager->FixedUpdate(mFixedDeltaTime);
		}

		mAccumulator -= mFixedDeltaTime; // 누적값에서 고정된 프레임 시간만큼 뺌
	}

	// 업데이트
	mpGameEngine->Update(mDeltaTime);

	// 추가 매니저 업데이트
	for (auto& manager : mpManagers)
	{
		manager->Update(mDeltaTime);
	}

	// 후처리 업데이트
	mpGameEngine->LateUpdate(mDeltaTime);

	// 추가 매니저 후처리 업데이트
	for (auto& manager : mpManagers)
	{
		manager->LateUpdate(mDeltaTime);
	}

	// 렌더링
	mpGameEngine->BeginRender();
	
	// ingui에서 매 프레임 BeginRender시에 마우스 커서를 초기화시키기때문에 여기에 추가함
	mpGameEngine->GetInputManager()->SetMouseCursor();	
	
	mpGameEngine->Render(mDeltaTime);
	mpGameEngine->EndRender();

}
