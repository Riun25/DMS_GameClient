#pragma once
#include "pch.h"

/// <summary>
/// 게임 엔진의 기본 클래스입니다.
/// 윈도우 메시지 루프가 돌아가고있습니다.
/// 2024.04.18 _Doyo
/// 
/// 윈도우 루프만 돌아가는 클래스로 변경
/// 2024.06.18
/// </summary>
class BaseWindowsProcessor
{
public:
	// 기본 생성자 삭제
	BaseWindowsProcessor() = delete;

	/// <summary>
	/// 생성자
	/// </summary>
	/// <param name="_hInstance">핸들 인스턴스</param>
	/// <param name="_szTitle">타이틀 문자열</param>
	/// <param name="_iconResourceId">아이콘 id</param>
	BaseWindowsProcessor(HINSTANCE _hInstance, const WCHAR* _szTitle, int _iconResourceId);
	BaseWindowsProcessor(int _hInstance, const WCHAR* _szTitle, int _iconResourceId);
	virtual ~BaseWindowsProcessor() = default;

	/// <summary>
	/// 윈도우 프로세스를 초기화합니다.
	/// </summary>
	/// <returns>실패시 false 반환</returns>
	virtual bool Initialize();

	void OnResize(uint16_t _width, uint16_t _height);
	void Run();
	virtual void Finalize();	

	HWND GetHWND() const;

protected:
	static LRESULT CALLBACK WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam);
	virtual void Update();

	void CreateConsole();

protected:
	HWND mHWnd;
	HINSTANCE mHInstance;
	HACCEL mHAccelTable;
	MSG mMsg;
	WCHAR mSzTitle[100];
	int mIconResourceId;

	uint16_t mScreenWidth = 1920;
	uint16_t mScreenHeight = 1080;
	uint16_t mScreenPosX = 0;
	uint16_t mScreenPosY = 0;
};

