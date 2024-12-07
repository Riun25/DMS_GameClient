#pragma once
#include <WinUser.h>
#include "GameEngine/BaseWindowsProcessor.h"

class GameEngine;
class IManager;

class GameProcessor :
    public BaseWindowsProcessor
{
public:
    GameProcessor(HINSTANCE _hInstance, const WCHAR* _szTitle, int _iconResourceId);
    ~GameProcessor() = default;

    /// <summary>
    /// 게임 초기설정
    /// </summary>
    /// <returns></returns>
    bool Initialize() override;

    /// <summary>
    /// 게임 루프
    /// </summary>
    void Run();

    /// <summary>
    /// 메모리 정리
    /// </summary>
    void Finalize() override;

private:
	LRESULT CALLBACK WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam);

	/// <summary>
	/// 게임 업데이트
	/// </summary>
	void Update() override;

private:
    std::unique_ptr<GameEngine> m_pGameEngine;          // 게임 엔진
    std::vector<std::shared_ptr<IManager>> m_pManagers; // 추가로 작성한 매니저들

    float m_accumulator = 0;
    float m_accumulator2 = 0;

    float m_fixedDeltaTime;
    float m_deltaTime;
};

