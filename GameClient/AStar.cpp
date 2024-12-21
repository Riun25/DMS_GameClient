#include "AStar.h"

AStar::AStar()
{
	// sqrt(2) 중간 우선
	mDirections[0] = Vector2(1, 1);
	mDirections[1] = Vector2(1, -1);
	mDirections[2] = Vector2(-1, 1);
	mDirections[3] = Vector2(-1, -1);
	// sqrt(1) 후순위
	mDirections[4] = Vector2(1, 0);
	mDirections[5] = Vector2(-1, 0);
	mDirections[6] = Vector2(0, 1);
	mDirections[7] = Vector2(0, -1);

	tempObstacles = nullptr;
}

AStar::~AStar()
{

}

void AStar::Initialize(Vector2 _mapStart, Vector2 _mapEnd, std::vector<std::vector<int>>* _map)
{
	mMapStart = _mapStart;
	mMapEnd = _mapEnd;
	mMap = _map;
	mObstacles = ConvertObstacle(mMapStart, *mMap);
}

std::vector<Vector2>* AStar::BasicAstar(Vector2& _startPoint, Vector2& _goalPoint, const std::vector<Vector2>& obstacles, const Vector2& _mapStart, const Vector2& _mapEnd)
{
	/// 방어 코드 -------------------------------------------------------------
	if (_startPoint.x < _mapStart.x || _startPoint.y < _mapStart.y ||
		_startPoint.x > _mapEnd.x || _startPoint.y > _mapEnd.y)
	{
		//std::cout << "Astar() error - 시작 지점과 목표 지점이 맵의 크기를 벗어납니다." << std::endl;
		//return {};
		_startPoint = MovePos(_startPoint);
	}
	if (_goalPoint.x < _mapStart.x || _goalPoint.y < _mapStart.y ||
		_goalPoint.x > _mapEnd.x || _goalPoint.y > _mapEnd.y)
	{
		_goalPoint = MovePos(_goalPoint);
	}

	for (const auto& obs : obstacles)
	{
		if (obs == _startPoint)
		{
			_startPoint = newPos(_startPoint, *mObstacles);
			//std::cout << "Astar() error - 시작 지점과 목표 지점이 장애물과 같은 위치로 설정되었습니다." << std::endl;
			//return {};
		}
		if (obs == _goalPoint)
		{
			_goalPoint = newPos(_goalPoint, *mObstacles);
			//std::cout << "Astar() error - 시작 지점과 목표 지점이 장애물과 같은 위치로 설정되었습니다." << std::endl;
			//return {};
		}

		if (obs.x < _mapStart.x || obs.y < _mapStart.y || obs.x > _mapEnd.x || obs.y > _mapEnd.y)
		{
			std::cout << "Astar() error - 장애물 위치가 맵의 크기를 벗어납니다." << std::endl;
			return {};
		}
	}

	/// 방어 코드 끝 -------------------------------------------------------------

	std::priority_queue<Node*, std::vector<Node*>, NodeCompare> mOpenList;
	int width = static_cast<int>(_mapEnd.x - _mapStart.x) + 1;
	int height = static_cast<int>(_mapEnd.y - _mapStart.y) + 1;

	// 2차원 배열을 동적으로 할당
	Node*** nodeArray = new Node * *[width];
	bool** closeList = new bool* [width];
	for (int i = 0; i < width; i++)
	{
		nodeArray[i] = new Node * [height]();
		closeList[i] = new bool[height]();
	}


	// 가장 첫 번째 노드의 Distance(hCost)는 goal과 가장 먼 지점이므로 가장 큰 값을 갖게 된다.
	Node* startNode = new Node();
	startNode->position = _startPoint;
	startNode->hCost = Distance(_startPoint, _goalPoint);
	mOpenList.push(startNode);
	nodeArray[static_cast<int>(_startPoint.x - _mapStart.x)][static_cast<int>(_startPoint.y - _mapStart.y)] = startNode;

	// openList가 빌 떄까지 반복한다.
	while (!mOpenList.empty())
	{
		Node* currentNode = mOpenList.top();
		mOpenList.pop();

		// 만약 closedList에서 해당 노드의 위치가 있다면 그 위치로 가지 않는다.
		if (closeList[static_cast<int>(currentNode->position.x - _mapStart.x)][static_cast<int>(currentNode->position.y - _mapStart.y)])
		{
			continue;
		}
		closeList[static_cast<int>(currentNode->position.x - _mapStart.x)][static_cast<int>(currentNode->position.y - _mapStart.y)] = true;

		if (currentNode->position == _goalPoint)
		{
			std::vector<Vector2>* path = new std::vector<Vector2>();
			while (currentNode)
			{
				path->push_back(currentNode->position);
				currentNode = currentNode->parent;
			}
			std::reverse(path->begin(), path->end());

			for (int i = 0; i < width; ++i)
			{
				for (int j = 0; j < height; ++j)
				{
					delete nodeArray[i][j];
				}
				delete[] nodeArray[i];
				delete[] closeList[i];
			}
			delete[] nodeArray;
			delete[] closeList;

			return path;
		}

		//GetDirections();

		for (const auto& dir : mDirections)
		{

			Vector2 neighborPos = currentNode->position + dir;

			/// 움직이지 못하면 멈춰
			if (!IsMovable(neighborPos, obstacles, _mapStart, _mapEnd))
			{
				continue;
			}

			float newGCost = currentNode->gCost + Distance(currentNode->position, neighborPos);

			// 현재 위치를 맵에서 찾는다.
			int nx = static_cast<int>(neighborPos.x - _mapStart.x);
			int ny = static_cast<int>(neighborPos.y - _mapStart.y);
			Node* neighborNode = nodeArray[nx][ny];

			if (neighborNode == nullptr) // 노드가 없거나 새 노드의 gCost가 더 작으면 새로운 노드를 생성해 openList에 넣어준다.
			{
				Node* neighborNode = new Node;
				neighborNode->position = neighborPos;
				neighborNode->gCost = newGCost;
				// 디버깅을 위해 남겨둠
				//std::cout << neighborNode->position.x << " " << neighborNode->position.y << " " << neighborNode->position.z << ", " << neighborNode->gCost << std::endl;
				neighborNode->hCost = Distance(neighborPos, _goalPoint);
				neighborNode->parent = currentNode;
				mOpenList.push(neighborNode);
				nodeArray[nx][ny] = neighborNode;
			}
			else if (newGCost < neighborNode->gCost)
			{
				neighborNode->gCost = newGCost;
				neighborNode->parent = currentNode;
				mOpenList.push(neighborNode);
			}
		}
	}

	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
		{
			delete nodeArray[i][j];
		}
		delete[] nodeArray[i];
		delete[] closeList[i];
	}
	delete[] nodeArray;
	delete[] closeList;

	return nullptr;
}

std::vector<Vector2>* AStar::ModifyAstar(Vector2& _startPoint, Vector2& _goalPoint, const std::vector<Vector2>& obstacles, const Vector2& _mapStart, const Vector2& _mapEnd)
{
	/// 방어 코드 -------------------------------------------------------------
	if (_startPoint.x < _mapStart.x || _startPoint.y < _mapStart.y ||
		_startPoint.x > _mapEnd.x || _startPoint.y > _mapEnd.y)
	{
		_startPoint = MovePos(_startPoint);
	}
	if (_goalPoint.x < _mapStart.x || _goalPoint.y < _mapStart.y ||
		_goalPoint.x > _mapEnd.x || _goalPoint.y > _mapEnd.y)
	{
		_goalPoint = MovePos(_goalPoint);
	}

	for (const auto& obs : obstacles)
	{
		if (obs == _startPoint)
		{
			_startPoint = newPos(_startPoint, *mObstacles);
		}
		if (obs == _goalPoint)
		{
			_goalPoint = newPos(_goalPoint, *mObstacles);
		}

		if (obs.x < _mapStart.x || obs.y < _mapStart.y || obs.x > _mapEnd.x || obs.y > _mapEnd.y)
		{
			return {};
		}
	}

	/// 방어 코드 끝 -------------------------------------------------------------

	std::priority_queue<Node*, std::vector<Node*>, NodeCompare> mOpenList;
	int width = static_cast<int>(_mapEnd.x - _mapStart.x) + 1;
	int height = static_cast<int>(_mapEnd.y - _mapStart.y) + 1;

	// 2차원 배열을 동적으로 할당
	Node*** nodeArray = new Node** [width]; //new Node** [width]는 Node** 타입의 배열을 동적으로 할당. width 만큼의 포인터를 저장하고, 각 포인터는 다시 Node* 배열을 가리킨다.
	for (int i = 0; i < width; i++)
	{
		nodeArray[i] = new Node * [height](); // nodeArray의 각 i번째 행을 동적으로 할당. 각 행은 height개의 Node*를 가리키는 배열
	}
	std::unordered_set<Vector2> closedList; // 맵의 크기가 크기 때문에 std::vector를 사용하지 않고 탐색 시간이 빠른 unordered_set 사용

	// 가장 첫 번째 노드의 Distance(hCost)는 goal과 가장 먼 지점이므로 가장 큰 값을 갖게 된다.
	Node* startNode = new Node();
	startNode->position = _startPoint;
	startNode->hCost = Distance(_startPoint, _goalPoint);
	mOpenList.push(startNode);
	nodeArray[static_cast<int>(_startPoint.x - _mapStart.x)][static_cast<int>(_startPoint.y - _mapStart.y)] = startNode;

	// openList가 빌 떄까지 반복한다.
	while (!mOpenList.empty())
	{
		/// 1. openList에서 fCost가 가장 낮은 노드를 선택해서 제거
		Node* currentNode = mOpenList.top();
		mOpenList.pop();

		/// 2. 이미 더 좋은 경로로 갱신된 경우 무시
		// 우선순위 큐이기 때문에 무조건 가장 먼저 fCost가 작은 값이 정렬되어 있다는 것을 이용해서, 같은 pos는 버린다.
		int checkOverlapX = static_cast<int>(currentNode->position.x - _mapStart.x);
		int checkOverlapY = static_cast<int>(currentNode->position.y - _mapStart.y);
		if (nodeArray[checkOverlapX][checkOverlapY] != currentNode)
		{
			continue; // 현재 노드는 중복ID의 정보이므로 무시
		}

		/// 3. 현재 노드를 closedList에 추가
		closedList.insert(currentNode->position);

		/// 4. 목표 위치에 도달하면 경로를 반환
		if (currentNode->position == _goalPoint)
		{
			std::vector<Vector2>* path = new std::vector<Vector2>();
			while (currentNode)
			{
				path->push_back(currentNode->position);
				currentNode = currentNode->parent;
			}
			std::reverse(path->begin(), path->end());

			for (int i = 0; i < width; ++i)
			{
				for (int j = 0; j < height; ++j)
				{
					delete nodeArray[i][j];
				}
				delete[] nodeArray[i];
			}
			delete[] nodeArray;
			closedList.clear();	// closedList 비우기

			return path;
		}

		/// 5. 현재 노드의 이웃 노드 탐색
		for (const auto& dir : mDirections)
		{
			Vector2 neighborPos = currentNode->position + dir;

			/// 5.1 이미 closedList에 있는 노드인지 확인(있으면 종료)
			// 이미 closedList에 존재하는 노드일 경우 탐색이 종료된 노드이기 때문에 openList에 존재하지 않는다.
			if (closedList.find(neighborPos) != closedList.end())
			{
				continue;
			}

			/// 5.2 장애물이 있는지 확인해서, 장애물이 있으면 종료
			if (!IsMovable(neighborPos, obstacles, _mapStart, _mapEnd))
			{
				continue;
			}

			/// 5.3. gCost 계산
			float newGCost = currentNode->gCost + Distance(currentNode->position, neighborPos); // 새 노드는 현재 gCost에 두 노드의 거리를 더해야 한다.

			/// 5.4 현재 위치를 맵에서 찾는다.
			int nx = static_cast<int>(neighborPos.x - _mapStart.x);
			int ny = static_cast<int>(neighborPos.y - _mapStart.y);
			Node* neighborNode = nodeArray[nx][ny];

			if (neighborNode == nullptr) // 노드가 없을 경우
			{
				Node* neighborNode = new Node;
				neighborNode->position = neighborPos;
				neighborNode->gCost = newGCost;
				// 디버깅을 위해 남겨둠
				//std::cout << neighborNode->position.x << " " << neighborNode->position.y << " " << neighborNode->position.z << ", " << neighborNode->gCost << std::endl;
				neighborNode->hCost = Distance(neighborPos, _goalPoint);
				neighborNode->parent = currentNode;
				mOpenList.push(neighborNode);
				nodeArray[nx][ny] = neighborNode;
			}
			else if (newGCost < neighborNode->gCost) // Open List에 이미 있지만 더 나은 경로 발견
			{
				neighborNode->gCost = newGCost;
				neighborNode->parent = currentNode;
				mOpenList.push(neighborNode);
			}
		}
	}

	// 경로를 찾은 후, 또는 openList가 빈 상태로 종료된 후 nodeArray 해제
	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
		{
			delete nodeArray[i][j];
		}
		delete[] nodeArray[i];
	}
	delete[] nodeArray;

	closedList.clear();	// closedList 비우기

	return nullptr;
}

std::vector<Vector2>* AStar::Astar(Vector2& _startPoint, Vector2& _goalPoint)
{
	return ModifyAstar(_startPoint, _goalPoint, *mObstacles, mMapStart, mMapEnd);
	int a = 0;
}

std::vector<Vector2>* AStar::ConvertObstacle(const Vector2& _startPoint, const std::vector<std::vector<int>> _map)
{
	tempObstacles = new std::vector<Vector2>();
	for (size_t i = 0; i < _map.size(); i++)
	{
		for (size_t j = 0; j < _map[i].size(); j++)
		{
			if (_map[i][j] == 1)
			{
				tempObstacles->push_back({ static_cast<float>(j) + _startPoint.x, static_cast<float>(i) + _startPoint.y });
				std::cout << "[" << static_cast<float>(j) + _startPoint.x << ", " << static_cast<float>(i) + _startPoint.y << "] " << std::endl;
			}
		}
	}
	return tempObstacles;
}

void AStar::PrintAstarMap(const std::vector<std::vector<int>>& _map, const std::vector<Vector2> _path)
{
	int n = 0;
	for (const auto& pos : _path)
	{
		std::cout << "----------- " << n << "번째 이동 -----------" << std::endl;
		for (int i = 0; i < _map.size(); ++i)
		{
			for (int j = 0; j < _map[i].size(); ++j)
			{
				if (_map[i][j] == 1)
				{
					std::cout << "■";
				}
				else if (i == pos.y && j == pos.x)
				{
					std::cout << "♪";
				}
				else
				{
					std::cout << "□";
				}
			}
			std::cout << std::endl;
		}
		n++;
	}
}

void AStar::PrintSimpleAstar(const std::vector<Vector2> _path)
{
	for (const auto& pos : _path)
	{
		std::cout << "(" << pos.x << ", " << pos.y << ")\n";
	}
}

void AStar::AdjustToTileCenter(const Vector3& _realPos, Vector2& _xzPos)
{
	if (_realPos.x < -0.5f)
	{
		_xzPos.x = static_cast<int>((_realPos.x - mHalfTileScale) / mTileScale);
	}
	else if (_realPos.x > 0.5f)
	{
		_xzPos.x = static_cast<int>((_realPos.x + mHalfTileScale) / mTileScale);
	}
	else
	{
		_xzPos.x = static_cast<int>(_realPos.x);
	}

	if (_realPos.z < -0.5f)
	{
		_xzPos.y = static_cast<int>((_realPos.z - mHalfTileScale) / mTileScale);
	}
	else if (_realPos.z > 0.5f)
	{
		_xzPos.y = static_cast<int>((_realPos.z + mHalfTileScale) / mTileScale);
	}
	else
	{
		_xzPos.y = static_cast<int>(_realPos.z);
	}
}

std::vector<std::vector<int>>* AStar::GetAstarMap()
{
	return mMap;
}

Vector2 AStar::GetMapStartPoint()
{
	return mMapStart;
}

Vector2 AStar::GetMapEndPoint()
{
	return mMapEnd;
}

std::vector<Vector2>* AStar::GetObstaclePos()
{
	return tempObstacles;
}

bool AStar::IsObstacle(const Vector2& _pos, const std::vector<Vector2>& _obstacles)
{
	for (const auto& obstacle : _obstacles)
	{
		if (obstacle == _pos)
		{
			return true;
		}
	}
	return false;
}

// 근처의 안전한 위치로 이동 (예시: x나 y를 1씩 증가시키면서 확인)
Vector2 AStar::newPos(const Vector2& _point, const std::vector<Vector2>& _obstacles)
{
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			Vector2 newGoalPoint(_point.x + i, _point.y + j);
			//if (!IsObstacle(newGoalPoint, _obstacles))
			if (IsMovable(newGoalPoint, _obstacles, mMapStart, mMapEnd))
			{
				return newGoalPoint;
			}
		}
	}
	return _point;
}

Vector2 AStar::MovePos(const Vector2& _point)
{
	Vector2 newPoint = {};
	if (_point.x < mMapStart.x)
	{
		newPoint.x = mMapStart.x;
	}
	if (_point.x > mMapEnd.x)
	{
		newPoint.x = mMapEnd.x;
	}
	if (_point.y < mMapStart.y)
	{
		newPoint.y = mMapStart.y;
	}
	if (_point.y > mMapStart.y)
	{
		newPoint.y = mMapEnd.y;
	}
	return newPoint;
}

float AStar::Distance(const Vector2& _pos1, const Vector2& _pos2)
{
	return (_pos1 - _pos2).Length();
}

bool AStar::IsMovable(const Vector2& _pos, const std::vector<Vector2>& _obstaclePos, const Vector2& _mapStartPos, const Vector2& _mapEndPos)
{
	// 현재 위치가 맵의 좌표 내에 있어야 함
	int x = static_cast<int>(_pos.x);
	int y = static_cast<int>(_pos.y);

	if (x < static_cast<int>(_mapStartPos.x) || y < static_cast<int>(_mapStartPos.y) ||
		x > static_cast<int>(_mapEndPos.x) || y > static_cast<int>(_mapEndPos.y))
	{
		return false;
	}

	// 각 장애물에 대해 검사
	for (const auto& obs : _obstaclePos)
	{
		int obsX = static_cast<int>(obs.x);
		int obsY = static_cast<int>(obs.y);

		// 근처 장애물만 검사한다.
		if (_pos.x == obs.x && _pos.y == obs.y && Distance(_pos, obs) <= sqrt(2))
		{
			return false;
		}
	}

	return true;
}

Vector2 AStar::SelectBestDirection(const Vector2& _from, const Vector2& _to, const std::vector<Vector2>& directions)
{
	Vector2 toGoalDirection = _to - _from;
	float toGoalLength = toGoalDirection.Length();
	toGoalDirection /= toGoalLength; // 노멀 값을 구했다.


	Vector2 bestDirection = directions[0];

	float maxDot = 0.0f; // 절대값으로 구할 거라서 0을 최소값으로 한다.
	// 모든 방향 벡터에 대해서 내적을 구해서 가장 큰 값을 구한다.
	for (const auto& dir : directions)
	{
		float dotProduct = abs(toGoalDirection.Dot(dir));

		if (maxDot < dotProduct)
		{
			maxDot = dotProduct;
			bestDirection = dir;
		}
	}
	return bestDirection;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       	return bestDirection;
}