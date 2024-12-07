#pragma once
#include "IManager.h"
#include "KeyInfo.h"

class IInputDevice;

// 디바이스의 컨셉트 정의
// 템플릿 클래스가 Update 와 GetKeyInfo 메서드를 제공하는지 확인합니다.
template <typename T>
concept InputDevice = requires(T _device)
{
	{ _device.Update() }->std::same_as<void>;
	{ _device.GetKeyInfo(KEY{}) }->std::same_as<KeyInfo>;
};

/// <summary>
/// 키보드, 마우스 등의 입력 디바이스들을 관리하는 클래스입니다.
/// 다른 디바이스들도 추가할 수 있습니다.
/// 2024.05.17 _Doyo;
/// </summary>
class InputManager
	: public IManager
{
public:
	InputManager() = default;
	~InputManager() = default;

	virtual bool Initialize() override;
	virtual void FixedUpdate(float _fixedDTime) override {}
	virtual void Update(float _dTime) override;
	virtual void LateUpdate(float _dTime) override {}
	virtual void Finalize() override;

	KEY_STATE GetKeyState(KEY _key);

	bool GetKey(KEY _key);
	bool GetKeyDown(KEY _key);
	bool GetKeyUp(KEY _key);

	Vector2 GetMousePos() const;
	Vector2 GetDeltaMousePos() const;

	void AddMouseCursor(const std::string& _file);
	void SetMouseCursor(int _cursorName);
	void SetMouseCursor();

	// imgui의 beginrender에서 마우스 커서 초기화가 있기 때문에 beginrender이후에 다시 설정해줘야함
	HCURSOR GetCurrentCursor() const;

private:
	// 매니저에 디바이스를 추가하는 함수입니다. 초기화 함수 내에서 호출하여 디바이스를 추가합니다.
	template <InputDevice T>
	void AddDevice(std::shared_ptr<T> _pDevice)
	{
		mpDevices.push_back(_pDevice);
	}

private:
	std::vector<std::shared_ptr<IInputDevice>> mpDevices;	// 관리하고있는 디바이스들의 배열
	std::vector<HCURSOR> mCursors;
	HCURSOR mCurrentCursor;
};

