#pragma once
#include "EntityManager.h" // 이거 삭제하면 byte 오류 발생하니 중복되더라도 지우지 말 것
#include "EnemyData.h"
#include "EquipmentManager.h"
#include "StatusManager.h"
#include "FSMHelper.h"
#include "AllyFSM.h"
#include "EnemyFSM.h"

class PlayerSystem
{
public:
	PlayerSystem(entt::registry& _registry, EntityManager* _pEntityManager, PhysicsManager* _pPhysicsManager
		, RenderManager* _pRenderManager, ResourceManager* _pResourceManager, EventManager* _pEventManager
	,SoundManager* _pSoundManager, EquipmentManager* _pEquipmentManager);
	~PlayerSystem();

	// 두 pair를 비교해서 a가 길면 a를 반환
	struct DistanceCompare
	{
		bool operator()(const std::pair<shared_ptr<Entity>, float>& a,
			const std::pair<shared_ptr<Entity>, float>& b)
		{
			return a.second > b.second;
		}
	};

	void Initialize(std::vector<std::pair<std::shared_ptr<Entity>, Vector3>>* _pMoneyPosVec, AStar* _pAstar);

	// 배치 후 시작되고 나서의 게임 플레이를 담당한다.
	void PlayerUnitUpdate(std::shared_ptr<Entity> _entity, float _dTime);
	void EnemyUnitUpdate(std::shared_ptr<Entity> _entity, float _dTime);
	
	void FixedUpdate(std::shared_ptr<Entity> _entity, float _fixedDTime);

	// 아직은 깡통이다. 추후에 게임 종료 후 결과창에 보낼 데이터들을 보내는 함수가 될 예정이다.
	void Finalize();

	// 초기 유닛AI을 추가(엔티티, 클래스 타입, 위치)
	void AddUnit(std::shared_ptr<Entity> _entity, const std::string& _classType, Vector3 _pos);

	// 배치할 유닛을 추가
	void AddDeplyUnit(std::shared_ptr<Entity> _entity, const std::string& _classType);

	// 배치 중 유닛 변경
	void ChangeUnit(std::shared_ptr<Entity> _entity, const std::string& _classType);

	// 배치 중 유닛 삭제
	void RemoveUnit(UID _entityUID);

	// 배치가 완료된 후 물리를 셋팅해야 오류가 안 난다.
	void AddPhysics(std::shared_ptr<Entity> _entity);

	// 적군 유닛을 스테이지에 맞게 추가. 레벨 매니저에 있는게 맞지만 임시로 여기에 만들어 둔다. 지금 구조에서는 가능하다.
	void AddSceneEnemyUnits(UID _sceneUID);

	// 게임 시작이 되기 전에 FSM의 가장 첫 상태를 세팅해준다.
	void SetInitializeState(std::shared_ptr<Entity> _entity, State _state);

	/// <summary>공격 범위를 설정한다.(공격범위를 int로 올림해서 적용함)</summary>
	/// <param name="_entity">내 유닛</param> <param name="_otherEntity">상대 유닛</param> <returns>true : 공격 가능 상태, false : 공격 불가 상태</returns>
	bool IsAttackable(const Vector2& _myPos, const Vector2& _otherPos, float _attackRange);

	// 임시로 추가하는 텍스트 코드
	void ShowState(std::shared_ptr<Entity> _entity);
	void UpdateStateText(std::shared_ptr<Entity> _entity);

	int mMaxSetUnitNum;  // 최대 배치 가능한 유닛 수

	void DetectGroundMoney(); //Enemy보다 우선 호출한다.

	// 아군용 : 적군 중에 가장 가까운 적을 타겟으로 삼는다. (맵 전체를 탐색한다.)
	std::shared_ptr<Entity> TargetNearestEnemy(std::shared_ptr<Entity> _entity);
	std::shared_ptr<Entity> EnemyTargetInRange(std::shared_ptr<Entity> _entity);

	// 적군용 : 아군 중에 가장 가까운 적을 타겟으로 삼는다. (맵 전체를 탐색한다.)
	std::shared_ptr<Entity> TargetNearestPlayer(std::shared_ptr<Entity> _entity);
	std::shared_ptr<Entity> PlayerTargetInRange(std::shared_ptr<Entity> _entity);

	std::vector<Vector2> *GetUnitDeployPosVec();

	StatusManager* mpStatusManager; // StatusManager 생성(classManager는 Status 내 생성)

private:
	entt::registry& mRegistry;
	EntityManager* mpEntityManager;
	PhysicsManager* mpPhysicsManager;
	ResourceManager* mpResourceManager;
	EquipmentManager* mpEquipmentManager; // EquipmentManager 생성
	SoundManager* mpSoundManager;

	// 삭제할 매니저임
	RenderManager* mpRenderManager;

	// FSM 매니저 생성
	FSMHelper* mpFSMHelper;

	AllyFSM* mpAllyFSM; // FSM 생성
	EnemyFSM* mpEnemyFSM; // FSM 생성
// 	StatusManager* mpStatusManager; // StatusManager 생성(classManager는 Status 내 생성)

	// 돈을 던질 때 변화를 감지해서 그때만 타겟팅하게끔 하기 위한 변수
	int tempMoneyVecSize;
	// 유닛이 배치된 자리를 기억하는 고정 크기 배열을 만든다.
	std::vector<Vector2> unitDeployPosVec;

	// 기획에서 제공한 값
	int mBuffLv1_Limit; 
	int mBuffLv2_Limit;

private:
	std::pair<std::shared_ptr<Entity>, float> FindClosestEntity(const Vector3& _pos);
	std::vector<EnemyData> mpEnemyPosVec;
};

