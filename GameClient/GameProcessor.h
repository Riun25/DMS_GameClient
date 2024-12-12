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
    /// ���� �ʱ⼳��
    /// </summary>
    /// <returns></returns>
    bool Initialize() override;

    /// <summary>
    /// ���� ����
    /// </summary>
    void Run();

    /// <summary>
    /// �޸� ����
    /// </summary>
    void Finalize() override;

private:
	LRESULT CALLBACK WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam);

	/// <summary>
	/// ���� ������Ʈ
	/// </summary>
	void Update() override;

private:
    std::unique_ptr<GameEngine> mpGameEngine;          // ���� ����
    std::vector<std::shared_ptr<IManager>> mpManagers; // �߰��� �ۼ��� �Ŵ�����

    float mAccumulator = 0;
    float mAccumulator2 = 0;

    float mFixedDeltaTime;
    float mDeltaTime;
};

