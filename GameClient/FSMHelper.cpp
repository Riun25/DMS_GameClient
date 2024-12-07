#include "FSMHelper.h"
#include "Transform.h"

FSMHelper::FSMHelper(ResourceManager* _pResourceManager)
{
	mpResourceManager = _pResourceManager;
	mpConditionFunctions = nullptr;
	mpMoneyPosVec = nullptr;
	mDesiredOffset = 4;
	mRotationSpeed = 0.1f;
}

FSMHelper::~FSMHelper()
{
	delete mpConditionFunctions;
}

void FSMHelper::Initialize()
{
	mpConditionFunctions = new CustomFunctions();
	mpAstarforEnemy = new AStar();

}

void FSMHelper::SetMoneyVec(std::vector<std::pair<std::shared_ptr<Entity>, Vector3>>* _pMoneyPosVec)
{
	mpMoneyPosVec = _pMoneyPosVec;
}

std::vector<std::pair<std::shared_ptr<Entity>, Vector3>>* FSMHelper::GetMoneyVec()
{
	return mpMoneyPosVec;
}

void FSMHelper::SetAstar(AStar* _pAstar)
{
	mpAstar = _pAstar;
	*mpAstarforEnemy = *mpAstar;
}

void FSMHelper::CalculatePath(std::vector<Vector2>*& _path, Vector2& _startPoint, Vector2& _goalPoint)
{
	if (_path != nullptr)
	{
		delete _path;
		_path = nullptr;
	}

	_path = mpAstar->Astar(_startPoint, _goalPoint);
}

void FSMHelper::CalculatePathforEnemy(std::vector<Vector2>*& _path, Vector2& _startPoint, Vector2& _goalPoint)
{
	if (_path != nullptr)
	{
		delete _path;
		_path = nullptr;
	}

	_path = mpAstarforEnemy->Astar(_startPoint, _goalPoint);
}

void FSMHelper::AdjustToTileCenter(const Vector3& _realPos, Vector2& _xzPos)
{
	mpAstar->AdjustToTileCenter(_realPos, _xzPos);
}

std::vector<Vector2>* FSMHelper::GetAstarObstacles()
{
	return mpAstar->GetObstaclePos();
}

std::unordered_map<std::string, std::function<bool(std::shared_ptr<Entity> _temp)>> FSMHelper::GetFuctMap()
{
	return mpConditionFunctions->mFuncMap;
}
//언제쓰이는지?
//타겟이 변경될 경우 바라보는 방향을 정해주는 함수
void FSMHelper::LookAtTarget(const Vector3& _unitPos, const Vector3& _targetPos, Vector3& _direction, float& _realRotationY)
{
	/// 1. 방향 설정(타겟을 늘 바라보고 있도록 설정)
	_direction = Vector3(_targetPos.x - _unitPos.x, 0.0f, _targetPos.z - _unitPos.z);
	//_direction = _targetPos - _unitPos;
	float length = _direction.Length();

	if (length != 0)
	{
		_direction /= length;
		// 캐릭터가 보고 있는 방향
		Vector3 characterDir = -Vector3::UnitZ;

		float theta = atan2(_direction.x, _direction.z);

		// 목표 회전값: theta에 180도(또는 π 라디안)를 추가하여 이동 방향을 향하게 설정
		theta += DirectX::XM_PI;

		float deltaAngle = theta - _realRotationY;
		// deltaAngle을 -π에서 π 사이의 값으로 정규화
		deltaAngle = std::fmod(deltaAngle + DirectX::XM_PI, DirectX::XM_2PI) - DirectX::XM_PI;

		// 목표 회전값
		float _goalRotationY = _realRotationY + deltaAngle;

		// 부드럽게 회전 (Lerp 사용)
		//float newRotationY = _realRotationY + (_goalRotationY - _realRotationY) * mRotationSpeed /** _entity->GetComponent<PlayerComponent>().mMoveSpeed*/;

		// 새로운 회전값 적용 (y축 회전만 변경)
		_realRotationY = _realRotationY + deltaAngle;
		//_realRotationY = newRotationY;
	}
	else
	{
		_direction = { 0.0f,0.0f,0.0f }; // 분모가 0이면 걍 움직이지 못하게 함
	}
}

//언제쓰이는지?
//캐릭터가 이동중에 보는 방향을 결정해주기 위한 함수
void FSMHelper::LookAtPathDir(const Vector3& _unitPos, const Vector3& _targetPos, std::vector<Vector2>*& _path, const size_t& _currentPathIdx, Vector3& _direction, float& _realRotationY)
{
	// 경로가 없다면 종료함
	if (_path == nullptr)
	{
		return;
	}

	//// 남은 경로 길이 확인
	int remainingPathLength = (*_path).size() - _currentPathIdx;

	// 남은 경로 길이와 원하는 오프셋 중 작은 값을 선택
	int adjustedOffset = std::min(mDesiredOffset, remainingPathLength - 1);

	// adjustedOffset을 적용한 futureIndex 계산
	int futureIndex = _currentPathIdx + adjustedOffset;

	// futureIndex에 해당하는 위치 계산
	Vector3 nextPosition((*_path)[futureIndex].x, _unitPos.y, (*_path)[futureIndex].y);

	/// 1. 방향 설정(특정 경로 지점을 바라보고 있도록 설정)
	_direction = Vector3(nextPosition.x - _unitPos.x, 0.0f, nextPosition.z - _unitPos.z);
	float length = _direction.Length();

	// 방어 코드, 혹시라도 nextPosition의 값과 현재의 값이 같아 length가 지나치게 작을 경우 방향이 설정되지 않는 문제를 방지
	if (length < 1.5f) 
	{
		// 경로의 마지막 점일 경우 목표 위치를 바라보도록 설정
		if (_currentPathIdx == (*_path).size() - 1) 
		{
			_direction = Vector3(_targetPos.x - _unitPos.x, 0.0f, _targetPos.z - _unitPos.z);
		}
		else 
		{
			// 경로의 다음 두 번째 지점을 사용 (필요 시 futureIndex를 조정 가능)
			futureIndex = std::min(_currentPathIdx + 2, (*_path).size() - 1);
			nextPosition = Vector3((*_path)[futureIndex].x, 0.0f, (*_path)[futureIndex].y);
			_direction = Vector3(nextPosition.x - _unitPos.x, 0.0f, nextPosition.z - _unitPos.z);
		}
		length = _direction.Length();
	}

	// 너무 가까운 거리에서 방향 설정 방지
	if (length > 0.01f)
	{
		_direction /= length;
		// 캐릭터가 보고 있는 방향
		Vector3 characterDir = -Vector3::UnitZ;

		float theta = atan2(_direction.x, _direction.z);

		// 목표 회전값: theta에 180도(또는 π 라디안)를 추가하여 이동 방향을 향하게 설정
		theta += DirectX::XM_PI;

		float deltaAngle = theta - _realRotationY;
		// deltaAngle을 -π에서 π 사이의 값으로 정규화
		deltaAngle = std::fmod(deltaAngle + DirectX::XM_PI, DirectX::XM_2PI) - DirectX::XM_PI;

		// 목표 회전값
		float _goalRotationY = _realRotationY + deltaAngle;

		// 부드럽게 회전 (Lerp 사용)
		float newRotationY = _realRotationY + (_goalRotationY - _realRotationY) * mRotationSpeed /** _entity->GetComponent<PlayerComponent>().mMoveSpeed*/;

		// 새로운 회전값 적용 (y축 회전만 변경)
		//_realRotationY = _realRotationY + deltaAngle;
		_realRotationY = newRotationY;
	}
	else
	{
		_direction = { 0.0f,0.0f,0.0f }; // 분모가 0이면 걍 움직이지 못하게 함
	}
}

void FSMHelper::LookAtPathDir2(const Vector3& _unitPos, const Vector3& _targetPos, std::vector<Vector2>*& _path, const size_t& _currentPathIdx, Vector3& _direction, float& _realRotationY)
{
	// 경로가 없다면 종료함
	if (_path == nullptr)
	{
		return;
	}

	//// 남은 경로 길이 확인
	int remainingPathLength = (*_path).size() - _currentPathIdx;

	// 남은 경로 길이와 원하는 오프셋 중 작은 값을 선택
	int adjustedOffset = std::min(mDesiredOffset, remainingPathLength - 1);

	// adjustedOffset을 적용한 futureIndex 계산
	int futureIndex = _currentPathIdx + adjustedOffset;

	// futureIndex에 해당하는 위치 계산
	Vector3 nextPosition((*_path)[futureIndex].x, _unitPos.y, (*_path)[futureIndex].y);

	/// 1. 방향 설정(특정 경로 지점을 바라보고 있도록 설정)
	_direction = Vector3(nextPosition.x - _unitPos.x, 0.0f, nextPosition.z - _unitPos.z);
	float length = _direction.Length();

	// 방어 코드, 혹시라도 nextPosition의 값과 현재의 값이 같아 length가 지나치게 작을 경우 방향이 설정되지 않는 문제를 방지
	if (length < 1.5f)
	{
		// 경로의 마지막 점일 경우 목표 위치를 바라보도록 설정
		if (_currentPathIdx == (*_path).size() - 1)
		{
			_direction = Vector3(_targetPos.x - _unitPos.x, 0.0f, _targetPos.z - _unitPos.z);
		}
		else
		{
			// 경로의 다음 두 번째 지점을 사용 (필요 시 futureIndex를 조정 가능)
			futureIndex = std::min(_currentPathIdx + 2, (*_path).size() - 1);
			nextPosition = Vector3((*_path)[futureIndex].x, 0.0f, (*_path)[futureIndex].y);
			_direction = Vector3(nextPosition.x - _unitPos.x, 0.0f, nextPosition.z - _unitPos.z);
		}
		length = _direction.Length();
	}

	// 너무 가까운 거리에서 방향 설정 방지
	if (length > 0.01f)
	{
		_direction /= length;
		// 캐릭터가 보고 있는 방향
		Vector3 characterDir = -Vector3::UnitZ;

		float theta = atan2(_direction.x, _direction.z);

		// 목표 회전값: theta에 180도(또는 π 라디안)를 추가하여 이동 방향을 향하게 설정
		theta += DirectX::XM_PI;

		float deltaAngle = theta - _realRotationY;
		// deltaAngle을 -π에서 π 사이의 값으로 정규화
		deltaAngle = std::fmod(deltaAngle + DirectX::XM_PI, DirectX::XM_2PI) - DirectX::XM_PI;

		// 목표 회전값
		float _goalRotationY = _realRotationY + deltaAngle;

		// 부드럽게 회전 (Lerp 사용)
		float newRotationY = _realRotationY + (_goalRotationY - _realRotationY) * mRotationSpeed /** _entity->GetComponent<PlayerComponent>().mMoveSpeed*/;

		// 새로운 회전값 적용 (y축 회전만 변경)
		//_realRotationY = _realRotationY + deltaAngle;
		_realRotationY = newRotationY;
	}
	else
	{
		_direction = { 0.0f,0.0f,0.0f }; // 분모가 0이면 걍 움직이지 못하게 함
	}
}
