#pragma once
#pragma once
#include <directxTk/SimpleMath.h>
#include <queue>
#include <iostream>

using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;

class AStar
{
public:
	AStar();
	~AStar();

	// Node 구조체 정의
	struct Node
	{
		Vector2 position;
		float gCost; // 시작 노드에서 해당 노드까지의 실제 소요 경비값
		float hCost; // 경험값
		float GetFCost() const { return gCost + hCost; }
		Node* parent = nullptr;
	};

	// Node 비교 연산자 정의
	// std::priority_queue는 기본적으로 최대 힙을 사용하여 가장 큰 요소를 루트로 유지하지만, 
	// 커스터마이징된 비교 기준을 제공하여 최소 힙을 만들거나 다른 기준에 따라 요소의 우선순위를 결정할 수 있다.
	struct NodeCompare
	{
		bool operator()(const Node* lhs, const Node* rhs)
		{
			if (lhs)
				return lhs->GetFCost() > rhs->GetFCost();
		}
	};

	// 기본적인 세팅을 위한 함수
	void Initialize(Vector2 _mapStart, Vector2 _mapEnd, std::vector<std::vector<int>>* _map);

	// A* 알고리즘 구현
	std::vector<Vector2>* Astar(Vector2& _startPoint, Vector2& _goalPoint);

	// Astar의 맵과 경로를 출력한다.
	void PrintAstarMap(const std::vector<std::vector<int>>& _map, const std::vector<Vector2> _path);

	// Astar의 경로를 좌표로 출력한다.
	void PrintSimpleAstar(const std::vector<Vector2> _path);

	// 타일의 센터로 조정하는 함수다.
	void AdjustToTileCenter(const Vector3& _realPos, Vector2& _xzPos);

	// 피킹 때 타일 맵의 정보를 받아오기 위해 만드는 get 함수
	std::vector<std::vector<int>>* GetAstarMap();
	Vector2 GetMapStartPoint();
	Vector2 GetMapEndPoint();
	std::vector<Vector2>* GetObstaclePos();

	/// 임시로 옮겨두는 함수들
	// 장애물이 있는지 없는지 확인하는 함수
	bool IsObstacle(const Vector2& _pos, const std::vector<Vector2>& _obstacles);
	// 장애물이 있을 때 새로운 위치로 계산해서 넣어줌[현재 posXY]
	Vector2 newPos(const Vector2& _point, const std::vector<Vector2>& _obstacles);
	Vector2 MovePos(const Vector2& _point);

private:
	// 타일 한 칸의 실제 크기 -> 지금은 1밖에 안 되지만.. 나중엔 꼭 확장해보자.
	float mTileScale = 1.0f;
	float mHalfTileScale = 0.5f;

	// 맵의 시작과 끝을 저장
	Vector2 mMapStart = Vector2();
	Vector2 mMapEnd = Vector2();
	std::vector<Vector2>* mObstacles = nullptr;
	std::vector<std::vector<int>>* mMap = nullptr;

	Vector2 directions[8];

	float Distance(const Vector2& _pos1, const Vector2& _pos2);

	bool IsMovable(const Vector2& _pos, const std::vector<Vector2>& _obstaclePos, const Vector2& _mapStartPos, const Vector2& _mapEndPos);

	Vector2 SelectBestDirection(const Vector2& _from, const Vector2& _to, const std::vector<Vector2>& directions);

	// 장애물을 Astar에 사용되는 데이터로 변환해주는 함수다.
	std::vector<Vector2>* ConvertObstacle(const Vector2& _startPoint, const std::vector<std::vector<int>> _map);

	// 가장 기본적인 형태의 Astar 경로 반환 함수다.(A* 알고리즘 구현)
	std::vector<Vector2>* BasicAstar(Vector2& _startPoint, Vector2& _goalPoint, const std::vector<Vector2>& obstacles, const Vector2& _mapStart, const Vector2& _mapEnd);

	// 클릭 위치가 사용가능한지 확인하기 위한 Astar맵
	//std::vector<std::vector<int>>* mAstarMap;

	// 장애물만 저장한다.
	std::vector<Vector2>* tempObstacles;
};

template <>
struct std::hash<Vector2>
{
	size_t operator()(const Vector2& v) const noexcept
	{
		return std::hash<float>()(v.x) ^ (std::hash<float>()(v.y) << 1);
	}
};

