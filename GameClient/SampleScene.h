#pragma once
#include "Scene.h"

/// <summary>
/// 새로운 아키타입의 씬을 만들려면 Scene 클래스를 상속
/// </summary>
class SampleScene :
	public Scene
{
public:
	/// <summary>
	/// 씬에서 기본적으로 사용되는 객체들을 생성자에서 주입해줍니다.
	/// </summary>
	SampleScene(entt::registry& _registry, const std::string& _name
		, EventManager* _pEventManager, RenderManager* _pRenderManager
		, PhysicsManager* _pPhysicsManager, InputManager* _pInpueManager
		, WorldManager* _pWorldManager, UIManager* _UIManager
		, EntityManager* _pEntityManager, ResourceManager* _pResourceManage);

	/// <summary>
	/// 추가로 멤버로 사용할 객체가 있다면 생성자를 새로 정의합니다.
	/// </summary>
	// SampleScene(entt::registry& _registry, const std::string& _name
	// 	, EventManager* _pEventManager, RenderManager* _pRenderManager
	// 	, PhysicsManager* _pPhysicsManager, InputManager* _pInpueManager
	// 	, const int& _i, const float& _f);

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

private:
	void DoSomething(const Event& _event);
};

