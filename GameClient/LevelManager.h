#pragma once
#include <vector>
#include "Utility.h"
#include "CustomFilter.h"
#include "RenderManager.h"
#include "PhysicsManager.h"
#include "UIManager.h"
#include "EntityManager.h"
#include "SoundManager.h"
#include "UnitSystem.h"
#include "SceneData.h"
#include "MapData.h"
#include "EnemyData.h"
#include "GameStateEnum.h"
#include "AStar.h"
#include "EasingFunc.h"
#include "CameraPosition.h"

/// <summary>
/// 전반적인 맵 배치와, 각 씬마다 중복되는 함수를 저장하는 죄를 짓는 매니저입니다.
/// 클릭과 관련된 bool 값 : 클릭 가능한 곳이  true, 불가능한 곳이 false;
/// </summary>
class LevelManager
{
public:
	// 배틀 씬에서 사용하는 LevelManager
	LevelManager(entt::registry& _registry, RenderManager* _pRenderManager
		, PhysicsManager* _pPhysicsManager, InputManager* _pInputManager
		, UIManager* _UIManager, EntityManager* _pEntityManager
		, ResourceManager* _pResourceManager, SoundManager* _pSoundManager, UnitSystem* _pUnitSystem, AStar* _pAstar);
	// 일반 씬에서 사용하는 LevelManager -> 레지스트리는 예민해서 멤버 변수로 갖고 있으면 무조건 초기화 해야 함
	LevelManager(entt::registry& _registry, RenderManager* _pRenderManager, InputManager* _pInputManager, UIManager* _UIManager
		, EntityManager* _pEntityManager, ResourceManager* _pResourceManager, SoundManager* _pSoundManager);
	~LevelManager();

	// FBX가 없는 메인 씬 같은 곳에서 사용하는 초기화입니다.
	void InitializeforNoneFbx(const UID& _sceneUID);
	// 초기화 (씬의 넘버를 하고, 모든 레벨 셋팅 자료를 csv로 읽어서 저장해둔다.
	void Initialize(const UID& _sceneUID);
	// 예비용으로 추가한 함수 (씬 넘버 셋팅)
	void SetSceneUID(const UID& _sceneUID);

	// 저장된 씬 넘버 초기화
	void Finalize();

	/// 초기화에 사용되는 함수
	// 카메라를 세팅합니다.
	void AddCamera(const Vector3& _pos, const Vector3& _viewDir);
	// 라이트를 세팅합니다.
	void AddLight();
	// 큐브맵을 세팅합니다.
	void AddCubeMap(uint8_t _mapNum);
	// 맵에 오브젝트를 배치한다.
	void AddMapData(const int& _sceneNum);
	// 기본적인 맵의 바닥과 가상의 외곽 투명 벽을 제작한다.
	void AddPlaneNWall();
	// 인디케이터용 타일을 생성한다.
	void AddSquareAtPointer(const std::string& _originalTexture, const std::string& _changedTexture);

	// 배치 영역 생성
	void AddPlaceIndicator();

	/// <summary>
	///배치 영역 표시 활성화
	/// </summary>
	/// <param name="_on">true: on / false: off</param>
	void DisablePlaceIndicator();

	/// 씬의 기본적인 세팅에 사용되는 함수
	// 모든씬에 사용되는 UI를 추가합니다.(환경설정 버튼 관련)
	void AddBasicUI();
	// 배틀씬에 사용되는 UI를 추가합니다.
	void AddBattleUI(SceneData* _pSceneData);
	// 스테이지마다 바뀌는 요소는 따로 UI를 추가합니다.
	void AddStageNumAndObjects(const std::u8string& _stageNum);
	// UI 클래스 버튼을 생성합니다.[활성화할 클래스의 개수]
	void AddClassUI(const int& _activeClassNum, bool _isToturial);
	// 확인 팝업 창을 띄우는 UI입니다.
	void AddVerifyPopUpUI();
	// 기본적인 UIAnimation을 위해 사용되는 UI입니다.
	void AddAnimationUI(bool _isBettelScene, int _priviusSceneNum);

	/// 업데이트에 사용되는 함수
	void BasicUIUpdate();
	void PopUpUIUpdate(SceneData* _pSceneData);
	void PopUpUIMainUpdate();
	void PopUPUITutorial();
	void UIAniUpdate(float _dTime);

	std::vector<pair<float, float*>> mEasingVec;
	// 모든 ui를 끌어내립니다.
	void PushAllUI();

	void StartNoneUI();
	void PrePlacementUpdate(SceneData* _pSceneData);
	void PlacementUpdate(SceneData* _pSceneData, float _dTime);
	void PreplayUpdate(SceneData* _pSceneData);
	void PlayUpdate(SceneData* _pSceneData, float _dTime);
	void NoneUI();
	void PostplayUpdate(SceneData* _pSceneData);
	void ResultUpdate(SceneData* _pSceneData, float _dTime);

	void UpdateParticle(float _dTime);

	/// 고정된 프레임 업데이트에 사용되는 함수들
	void StartAnimationFixedUpdate(float _dTime);
	void PlayFixedUpdate(float _dTime);
	void PostAnimationFixedUpdate(float _dTime);

	/// 게임 배치/ 진행 중 사용하는 함수
	// 유저가 특정 위치의 스크린을 클릭했을 때, 월드 좌표로 변환되지 않게 막는다. (구역의 안쪽이면 false)
	bool AddBlockScreenPos(const Vector2& _mousePos, const Vector2& _PosXlYu, const Vector2& _WH);
	Vector3 GetTargetPosition(Vector2 _mousePos, int _screenWidth, int _screenHeight, Vector3 pointOffset, float cameraDistance);
	bool IsCanClickAreaXZ(const Vector3& _realPos, const Vector2& _PosXlZu, const Vector2& _PosXrZd, const int& _aliveNum);

	/// 게임 세레모니 관련 함수
	void JumpingAndRotUnit();
	// 배치한 플레이어를 삭제하는 함수
	void ResetPlayer(SceneData* _pSceneData);
	// 컬러 업데이트 함수
	void UpdateRandomColor(Vector4& _rgba, float _dTime, float _speed);

	/// 게임 진행 중에 사용하는 함수
	// 맵의 장애물 구역에 마우스 위치가 있으면, false를 반환한다. (구역의 안쪽이면 false)
	bool AddObjectArea(const Vector2& _worldMousePos, const Vector2& _PosXlZu, const Vector2& _PosXrZd);
	// 랭크 바를 업데이트 시킨다.
	void UpdateRankBar(SceneData* _pSceneData);

	/// 디버깅에 사용되는 함수
	void DrawCoordinatesXZ(bool _isVisible);
	void CreateCoordinatesXZ();
	void KillAllEnemies();

	/// 씬 한 개의 상태에 대해 사용되는 함수
	GameState GetGameState();
	void SetGameState(GameState _state);
	PopUpState GetPopUpState();
	void SetPopUpState(PopUpState _state);
	UIAnimationState GetUIAnimationState();
	void SetUIAnimationState(UIAnimationState _state);

	// 카메라 연출 관련 함수
	void CameraWalk(Vector3& _target, const Vector3& _start, const Vector3& _end, float _time);
	void CameraRotate(Vector3& _target, const Vector3& _start, const Vector3& _end, float _time);
	void CameraEasing(const std::string& _fucName, Vector3& _target, const Vector3& _start, const Vector3& _end, float _time);

	// 현재 사용중인 월드 카메라를 가져옴
	Camera* GetWorldCamera();

	// 화면 페이드 인/아웃
	void FadePreSetting(bool _mIsFadeOut);
	void FadeInScreen(float _time);
	void FadeOutScreen(float _time);

	std::vector<EnemyData> GetSceneEnemyDataVec();
	bool mIsNext = false; // 다음으로 버튼을 눌렀나요?
	bool mIsGoMain = false;
	bool mIsSkip = false;
	bool mIsExit = false;
	bool mIsClear = false;	// 전투 승리시
	bool mIsFinalStage = false;
	bool mIsEndingFade = false;
	bool mIsGameEnding = false;
	bool mIsRestart = false; // Restart 상태인가?

	/// 튜토리얼에서 사용할 함수들
	void DeleteMoney();// 동전 삭제
	void EnemyUpdate(float _dTime);// 적군 업데이트
	void PlayerUpdate(float _dTime);// 아군 업데이트
	void DeleteEnemyAll(); // 적군 컴포넌트 삭제
	void TutorialClassDeploy();	// 근거리 클래스 하나 배치하는데 쓰는 함수
	void TutorialClassDeployed(Vector3& _cursorPos, bool _isPayment, SceneData* _pSceneData);
	void AllDeploy(Vector3& _cursorPos, bool _isPayment, SceneData* _pSceneData);
	
	// 변수
	bool mTutorialFlag = false; // 튜토리얼에 특정 메세지 및 스테이트 변경을 위해 전달하는 플래그

	/// GET/SET -> 튜토리얼에서 사용
	bool GetIsGameStart();
	void SetIsGameStart(bool _isGameStart);
	bool GetmIsOneDeploySet();
	void SetmIsOneDeploySet(bool _isOneDeploySet);
	bool GetIsClassButtonPressed();
	void SetIsClassButtonPressed(bool _isClassButtonPressed);
	UID GetTempUnitUID();
	void SetTempUnitUID(UID _tempUnitID);
	Vector2 GetStartPoint();
	Vector2 GetEndPoint();
	std::string GetUnitClassName();
	void SetClassName(const std::string& _className);
	EasingFunc* mpEasingFunc;

	void AddFireCracker();
	CameraPosition GetCameraPosInfo();

	void AddSmog();
	void SmogUpdate(float _dTime);

private:
	entt::registry& mRegistry;

	std::vector<MapData> mMapDataVec; // 모든 씬의 환경을 저장하는 벡터입니다.
	std::vector<EnemyData> mEnemyDataVec; // 모든 씬의 적군 배치를 저장하는 벡터입니다.
	UID mCurrentSceneUID;	// 어차피 한 씬마다 실행될 거라면 초기화 때 씬 넘버를 초기화 해준다.

	RenderManager* mpRenderManager;
	PhysicsManager* mpPhysicsManager;
	InputManager* mpInputManager;
	UIManager* mpUIManager;
	EntityManager* mpEntityManager;
	ResourceManager* mpResourceManager;
	SoundManager* mpSoundManager;
	AStar* mpAstar;
	UnitSystem* mpUnitSystem;
	

	// 현재 게임 상태를 저장 -> 추후 더 세분화 될 수 있음 -> 배치 완료와 게임 시작 전 상태 같은 것들.
	GameState currentLevelState;
	SettingState currentSettingState = SettingState::NORMAL;
	PopUpState currentPopUpState = PopUpState::NORMAL;
	UIAnimationState currentUIAniState = UIAnimationState::NORMAL;

	std::list<std::shared_ptr<Entity>> coordinateList;

	bool mIsGameStart = false; // 게임 시작을 알리는 플래그 : 배치를 위해 대기된 유닛을 지우기 위한 플래그
	bool mIsOneDeploySet = false; // 하나의 유닛이 배치가 완료되었는지를 확인하는 플래그
	UID mTempUnitUID = -1;// 클래스 변경을 위해 현재 유닛 엔티티 정보를 담는다. // 불가... 우리의 엔티티 매니저는 UID를 찾을 수 없...네..
	bool mIsClassButtonPressed = false;
	std::string mClassName = "";
	Vector2 mStartPoint;
	Vector2 mEndPoint;
	CameraPosition mCameraPosInfo;


	// 음량과 관련하여 셋팅하는 함수
	void SetVolume(const std::string& _fileName, const shared_ptr<Entity>& _entity, int& _intNum, int& _whatVolume);
	int BGVolume = 3;
	int ESVolume = 3;
	float tempTime = 0.f;
	bool mPlayWarningSound = false;
	float mUIAnimationTime = 0.0f;

	// 팝업에서 보일지 안 보일지를 처리하는 함수
	void ManagePopupDisplay(PopUpState _state, PopUpState _state2 = PopUpState::LAST);

	float mResultEasingTimer = 0;
};

