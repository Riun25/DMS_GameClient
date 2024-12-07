#pragma once
#include "Scene.h"
#include "AllyFSM.h"

class AnimationTestScene : public Scene
{
public:
	/// <summary>
	/// 씬에서 기본적으로 사용되는 객체들을 생성자에서 주입해줍니다.
	/// </summary>
	AnimationTestScene(entt::registry& _registry, const std::string& _name
		, EventManager* _pEventManager, RenderManager* _pRenderManager
		, PhysicsManager* _pPhysicsManager, InputManager* _pInpueManager
		, WorldManager* _worldManager, UIManager* _UIManager, EntityManager* _pEntityManager
		, ResourceManager* _pResourceManager);

	/// <summary>
	/// 씬을 초기화합니다.
	/// </summary>
	/// <returns>__super::Initialize() 반환</returns>
	virtual bool Initialize() override;

	/// <summary>
	/// 고정된 시간 간격으로 물리 업데이트를 수행합니다.
	/// </summary>
	/// <param name="_dTime">고정된 델타타임</param>
	virtual void FixedUpdate(float _dTime) override;

	/// <summary>
	/// 매 프레임 업데이트를 수행합니다.
	/// </summary>
	/// <param name="_dTime">프레임 시간</param>
	virtual void Update(float _dTime) override;

	/// <summary>
	/// 후처리 업데이트를 수행합니다.
	/// </summary>
	/// <param name="_dTime">프레임 시간</param>
	virtual void LateUpdate(float _dTime) override;

	/// <summary>
	/// 씬을 종료하고 정리합니다.
	/// </summary>
	virtual void Finalize() override;

	/// <summary>
	/// 씬에 리소스를 추가합니다.
	/// </summary>
	virtual void AddResource();

	Vector3 ScreenToNDC(int mouseX, int mouseY, float screenWidth, float screenHeight);
	Vector4 NDCToClip(Vector3 _NDCCoords);
	Vector4 ClipToEye(Vector4 clipCoords, Matrix invProj);
	Vector3 EyeToWorld(Vector4 eyeCoords, Matrix invView);
	Vector3 ScreenToWorldRay();

	// 객체를 마우스 위치에서 일정 거리만큼 떨어진 위치로 이동
	Vector3 GetTargetPosition(Vector3 rayWorldDirection, Vector3 cameraPosition, Vector3 pointOffset, float cameraDistance);

private:
	std::vector<std::shared_ptr<Entity>> m_pCoins;
	float mCoolTime = 0.0f;
	float interpolationTime = 0.0f;
	float animationPlaytime = 0.0f;
};