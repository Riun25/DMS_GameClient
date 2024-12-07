#pragma once
#include "Scene.h"
#include "UnitSystem.h"
#include "PlayerSystem.h"
#include "ProjectileSystem.h"
#include "NPCSystem.h"
#include "SceneData.h"
#include "LevelManager.h"
#include "TutotialTextTable.h"
#include "TutorialEnum.h"

class ExampleScene
	: public Scene
{
public:
	/// <summary>
	/// 씬에서 기본적으로 사용되는 객체들을 생성자에서 주입해줍니다.
	/// </summary>
	ExampleScene(entt::registry& _registry, const std::string& _name
		, EventManager* _pEventManager, RenderManager* _pRenderManager
		, PhysicsManager* _pPhysicsManager, InputManager* _pInpueManager
		, WorldManager* _worldManager, UIManager* _UIManager, EntityManager* _pEntityManager
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

	// 인디케이터에 배치구역을 쓸지 안 쓸지 구분해서 작동
	void SettingDeployIndicator(Vector3 _cursorPos, SceneData* _pSceneData);
	void SettingPlayingIndicator();
	void SettingRealPlayingIndicator(SceneData* _pSceneData);
	 
private:
	UnitSystem* mpUnitSystem;
	LevelManager* mpLevelManager;
	AStar* mpAstar;
	std::vector<TutotialTextTable> mTutorialTextVec;
	size_t mTextIdx; // 텍스트 벡터의 인덱스
	Vector2 mStartPoint;
	Vector2 mEndPoint;

	TutorialState mTutorialState;
	float mTime; // 텍스트 자동 재생을 위한 타임 + ui 애니메이션에도 사용
	//float mPressClickTime = 0.0f;

	//타일 색 및 위치 변환
	void ChangeGuideTile(const std::string& _texFileName, Vector2 _posXZ, Vector2 _sizeXZ);
	void ChangeGuideTileTrs(Vector2 _posXZ, Vector2 _sizeXZ);
	void InvisibleGuideTile();
	void ChangeGuideTileTex(const std::string& _texFileName);

	// 텍스트
	void PrintIntroText(TutorialState _tutorialState);
	void SkipText(int _endIdx);

	// 지시 사각형
	void UpdateShrinkRect(float _dTime);
	void ResettingShrinkRect(Vector2 _targetRectPos, Vector2 _targetSize, int _sizeMultiplier, int _shrinkAmount);
	bool mIsReadyResetShrinkRect;
};