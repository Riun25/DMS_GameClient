#pragma once
#include "Entity.h"
#include "AStar.h"
#include "CustomFunctions.h"
#include "ResourceManager.h"

/// <summary>
/// 이 클래스는 모든 FSM에서 중복으로 사용되는 것들을 관리하기 위해서 사용됩니다.
/// 
/// </summary>
class FSMHelper
{
public:
	FSMHelper(ResourceManager* _pResourceManager);
	~FSMHelper();

	void Initialize();

	/// 투사체 관련 함수
	void SetMoneyVec(std::vector<std::pair<std::shared_ptr<Entity>, Vector3>>* _pMoneyPosVec);
	std::vector<std::pair<std::shared_ptr<Entity>, Vector3>>* GetMoneyVec();
	//int GetVecSize();

	/// Astar 관련 함수
	// Astar 객체의 포인터를 세팅한다.
	void SetAstar(AStar* _pAstar);
	// Astar의 경로를 재설정한다.
	void CalculatePath(std::vector<Vector2>*& _path, Vector2& _startPoint, Vector2& _goalPoint);
	void CalculatePathforEnemy(std::vector<Vector2>*& _path, Vector2& _startPoint, Vector2& _goalPoint);
	// Vector3인 좌표를 Astar에 사용되는 타일 중점에 맞춰지는 Vector2(_xzPos)로 변환시킵니다.
	void AdjustToTileCenter(const Vector3& _realPos, Vector2& _xzPos);
	std::vector<Vector2>* GetAstarObstacles();

	// customFuction의 funcMap을 가져옴
	std::unordered_map<std::string, std::function<bool(std::shared_ptr<Entity> _temp)>> GetFuctMap();

	// 방향을 보간하는 함수[내 위치, 상대 위치, 내 방향, 목표 회전값]
	void LookAtTarget(const Vector3& _unitPos, const Vector3& _targetPos, Vector3& _direction, float& _realRotationY);
	void LookAtPathDir(const Vector3& _unitPos, const Vector3& _targetPos, std::vector<Vector2>*& _path, const size_t& _currentPathIdx, Vector3& _direction, float& _realRotationY);
	void LookAtPathDir2(const Vector3& _unitPos, const Vector3& _targetPos, std::vector<Vector2>*& _path, const size_t& _currentPathIdx, Vector3& _direction, float& _realRotationY);

private:
	ResourceManager* mpResourceManager;

	AStar* mpAstar; // Astar 만들기
	AStar* mpAstarforEnemy; // Astar 만들기
	CustomFunctions* mpConditionFunctions; // FSM의 State를 변경하기 위한 함수 집합들

	std::vector<std::pair<std::shared_ptr<Entity>, Vector3>>* mpMoneyPosVec; // 투사체 -> 플레이어 시스템에서 받아온 포인터
	int mDesiredOffset; // 방향 보간을 자연스럽게 하기 위하여, Astar의 타깃 위치를 조정하는 함수 (추후 post-Smooth로 변경할 것)
	float mRotationSpeed; // 회전 보간 속도를 조정하기 위한 상수
};

