#pragma once
#include "Component.h"
#include "GraphicsDefine.h"

/// <summary>
/// 카메라 생성에 사용되는 카메라 컴포넌트입니다.
/// <para>
/// CameraComponent(Screen Width, Screen Height, Speed, Position, ViewDir, UpDir, Axes, Proj);
/// </para>
/// </summary>

enum class cameraEnum
{
	WorldCamera,
	LightCamera,
};

struct CameraComponent
	: public Component
{
public:
	CameraComponent(std::shared_ptr<Entity> _owner, int _width, int _height, float _speed, const Vector3& _eyePos
		, const Vector3& _viewDir, const Vector3& _viewUp, const Vector3& _proj, unsigned int _cameraEnum);
	~CameraComponent();

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<CameraComponent>(*this);
	}
	void setViewDir(Vector3 _viewDir);						   //#Sung 컴포넌트안에 세팅하는 함수를 만든다.
	void setEyePos(Vector3 _eyePos);
	void setLightCameraIndex(unsigned int _lightIndex);
	unsigned int mCameraEnum;
public:
	Camera* mpCamera;
	unsigned int mLightIndex;
	bool mIsPerspective = true;
	static uint8_t mNumCamera;

};