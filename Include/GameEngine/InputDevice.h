#pragma once
#include "pch.h"
#include "KeyInfo.h"

/// <summary>
/// 입력 장치 인터페이스 클래스입니다.
/// 2024.05.17 _Doyo
/// </summary>
class IInputDevice
{
public:
	virtual ~IInputDevice() = default;
	virtual void Update() = 0;
	virtual KeyInfo GetKeyInfo(KEY _key) const = 0;
};

/// <summary>
/// 키보드의 정보를 생성하고 업데이트하는 클래스입니다.
/// </summary>
class Keyboard : public IInputDevice
{
public:
	// 키보드에 해당하는 키 코드 초기화
	Keyboard()
	{
		std::vector<KEY> keys =
		{
			KEY::BACK, KEY::TAB, KEY::RETURN, KEY::SHIFT, KEY::CTRL, KEY::ALT,
			KEY::CAPSLOCK, KEY::ESCAPE, KEY::SPACE, KEY::PGUP, KEY::PGDN,
			KEY::END, KEY::HOME, KEY::LEFT, KEY::UP, KEY::RIGHT, KEY::DOWN,
			KEY::INS, KEY::DEL, KEY::D0, KEY::D1, KEY::D2, KEY::D3, KEY::D4,
			KEY::D5, KEY::D6, KEY::D7, KEY::D8, KEY::D9, KEY::A, KEY::B, KEY::C,
			KEY::D, KEY::E, KEY::F, KEY::G, KEY::H, KEY::I, KEY::J, KEY::K, KEY::L,
			KEY::M, KEY::N, KEY::O, KEY::P, KEY::Q, KEY::R, KEY::S, KEY::T, KEY::U,
			KEY::V, KEY::W, KEY::X, KEY::Y, KEY::Z, KEY::WIN, KEY::F1, KEY::F2,
			KEY::F3, KEY::F4, KEY::F5, KEY::F6, KEY::F7, KEY::F8, KEY::F9, KEY::F10,
			KEY::F11, KEY::F12, KEY::NUMLOCK, KEY::SCROLLLOCK, KEY::LSHIFT, KEY::RSHIFT,
			KEY::LCONTROL, KEY::RCONTROL, KEY::LMENU, KEY::RMENU, KEY::OEM_1, KEY::OEM_PLUS,
			KEY::OEM_COMMA, KEY::OEM_MINUS, KEY::OEM_PERIOD, KEY::OEM_2, KEY::OEM_3, KEY::OEM_4,
			KEY::OEM_5, KEY::OEM_6, KEY::OEM_7
		};

		for (auto& key : keys)
		{
			mKeyInfos[key] = KeyInfo();
		}
	}

	~Keyboard()
	{
		mKeyInfos.clear();
	}

	// 키보드 상태 업데이트
	virtual void Update() override
	{
		HWND hWnd = GetFocus();

		if (hWnd != nullptr)
		{
			for (auto& key : mKeyInfos)
			{
				if (GetAsyncKeyState(static_cast<int>(key.first)) & 0x8000)
				{
					key.second.keyState = key.second.isPushed
						? KEY_STATE::HOLD : KEY_STATE::TAP;
					key.second.isPushed = true;
				}
				else
				{
					key.second.keyState = key.second.isPushed
						? KEY_STATE::AWAY : KEY_STATE::NONE;
					key.second.isPushed = false;
				}
			}
		}
		else
		{
			for (auto& key : mKeyInfos)
			{
				key.second.isPushed = false;
				if (KEY_STATE::TAP == key.second.keyState
					|| KEY_STATE::HOLD == key.second.keyState)
				{
					key.second.keyState = KEY_STATE::AWAY;
				}
				else
				{
					key.second.keyState = KEY_STATE::NONE;
				}
			}
		}
	}

	// 키의 정보를 반환
	virtual KeyInfo GetKeyInfo(KEY _key) const override
	{
		if (mKeyInfos.find(_key) != mKeyInfos.end())
		{
			return mKeyInfos.find(_key)->second;
		}
	}

private:
	std::map<KEY, KeyInfo> mKeyInfos;

};

/// <summary>
/// 마우스의 정보를 생성하고 업데이트하는 클래스입니다.
/// </summary>
class Mouse : public IInputDevice
{
public:
	Mouse()
	{
		// 모든 버튼을 초기화합니다.
		std::vector<KEY> keys =
		{
			KEY::LBUTTON, KEY::RBUTTON, KEY::MBUTTON, KEY::XBUTTON_ONE, KEY::XBUTTON_TWO
		};

		for (auto& key : keys)
		{
			mKeyInfos[key] = KeyInfo();
		}
	}

	~Mouse()
	{
		mKeyInfos.clear();
	}

	virtual void Update() override
	{
		HWND hWnd = GetFocus();

		mPrevPosition = mCurrentPosition;
		POINT temp;
		GetCursorPos(&temp);
		ScreenToClient(hWnd, &temp);
		mCurrentPosition = Vector2(static_cast<float>(temp.x), static_cast<float>(temp.y));

		// 마우스 상태 업데이트
		if (hWnd != nullptr)
		{
			for (auto& key : mKeyInfos)
			{
				if (GetAsyncKeyState(static_cast<int>(key.first)) & 0x8000)
				{
					key.second.keyState = key.second.isPushed
						? KEY_STATE::HOLD : KEY_STATE::TAP;
					key.second.isPushed = true;
				}
				else
				{
					key.second.keyState = key.second.isPushed
						? KEY_STATE::AWAY : KEY_STATE::NONE;
					key.second.isPushed = false;
				}
			}
		}
		else
		{
			for (auto& key : mKeyInfos)
			{
				key.second.isPushed = false;
				if (KEY_STATE::TAP == key.second.keyState
					|| KEY_STATE::HOLD == key.second.keyState)
				{
					key.second.keyState = KEY_STATE::AWAY;
				}
				else
				{
					key.second.keyState = KEY_STATE::NONE;
				}
			}
		}
	}

	// 키의 정보를 반환
	virtual KeyInfo GetKeyInfo(KEY _key) const override
	{
		if (mKeyInfos.find(_key) != mKeyInfos.end())
		{
			return mKeyInfos.find(_key)->second;
		}
	}

	// 마우스의 위치 반환
	Vector2 GetMousePos() const
	{
		return mCurrentPosition;
	}

	// 이전 프레임과 현재 프레임의 마우스의 위치 차이 반환
	Vector2 GetDeltaMousePos() const
	{
		return mCurrentPosition - mPrevPosition;
	}

private:
	std::map<KEY, KeyInfo> mKeyInfos;
	Vector2 mCurrentPosition;
	Vector2 mPrevPosition;

};

/// <summary>
/// 입력 디바이스를 추가 할 경우 예시 코드입니다.
/// 2024.05.20 _Doyo;
/// </summary>
class CustomDevice : IInputDevice
{
public:
	CustomDevice()
	{
		// 모든 버튼을 초기화합니다.
		std::vector<KEY> keys =
		{
			/*디바이스의 키 코드들*/
		};

		for (auto& key : keys)
		{
			mKeyInfos[key] = KeyInfo();
		}
	}

	// 디바이스 상태 업데이트
	virtual void Update() override
	{
		HWND hWnd = GetFocus();

		if (hWnd != nullptr)
		{
			for (auto& key : mKeyInfos)
			{
				if (GetAsyncKeyState(static_cast<int>(key.first)) & 0x8000)
				{
					key.second.keyState = key.second.isPushed
						? KEY_STATE::HOLD : KEY_STATE::TAP;
					key.second.isPushed = true;
				}
				else
				{
					key.second.keyState = key.second.isPushed
						? KEY_STATE::AWAY : KEY_STATE::NONE;
					key.second.isPushed = false;
				}
			}
		}
		else
		{
			for (auto& key : mKeyInfos)
			{
				key.second.isPushed = false;
				if (KEY_STATE::TAP == key.second.keyState
					|| KEY_STATE::HOLD == key.second.keyState)
				{
					key.second.keyState = KEY_STATE::AWAY;
				}
				else
				{
					key.second.keyState = KEY_STATE::NONE;
				}
			}
		}
	}

private:
	std::map<KEY, KeyInfo> mKeyInfos;

};