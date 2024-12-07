#pragma once
#include "Scene.h"
#include "LevelManager.h"
#include "EasingFunc.h"
/// <summary>
/// 게임의 메인 메뉴에 해당하는 씬입니다.
/// </summary>

class MainMenuScene :
	public Scene
{
public:
	/// <summary>
	/// 씬에서 기본적으로 사용되는 객체들을 생성자에서 주입해줍니다.
	/// </summary>
	MainMenuScene(entt::registry& _registry, const std::string& _name
		, EventManager* _pEventManager, RenderManager* _pRenderManager
		, PhysicsManager* _pPhysicsManager, InputManager* _pInpueManager
		, WorldManager* _worldManager, UIManager* _pUIManager, EntityManager* _pEntityManager
		, ResourceManager* _pResourceManager, SoundManager* _pSoundManager);

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
	virtual void AddResource() override;

private:
	/// <summary>
	/// 이징 함수를 쓰기위한 변수
	/// </summary>
	EasingFunc* tempEasing;

	/// <summary>
	/// 원래는 캡슐화를 시켜야하나 지금은 시간이 없으니 걍 쌩변수를 때려박는다
	/// </summary>
	float mtime = 0;
	float mtime2 = 0;
	float mTimeExit = 0.0f;
	//bool mCanMove = false;

	// 레벨 매니저는 모든 씬의 UI와 배치를 관리합니다. 추후 시간이 남으면 맵 매니저와 분리해야 합니다.
	LevelManager* mpLevelManager;

	// 게임 설명과 관련된 리소스 추가 함수
	void AddExplainUI();
	bool mIsPressedExplainButton = false;
};

