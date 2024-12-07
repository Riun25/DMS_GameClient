#pragma once
#include "ClassTypeEnum.h"
#include "FilterEnum.h"
#include "EquipmentMeshInfo.h"
#include "WeaponMeshInfo.h"
#include "EntityManager.h"
#include "PhysicsManager.h"
#include "RenderManager.h"
#include "ResourceManager.h"

/// <summary>
/// 랜더 매니저를 추가했습니다... 추후에 게임엔진에 구현하고 여기서는 뽑아냅니다.
/// </summary>
class EquipmentManager
{
public:
	EquipmentManager(entt::registry& _registry, EntityManager* _pEntityManager
		, PhysicsManager* _pPhysicsManager, RenderManager* _pRenderManager, ResourceManager* _pResourceManager);
	~EquipmentManager();

public:
	// 유저의 클래스 정보를 불러와 세팅한다.
	void Initialize();

	// 이미 위치가 정해진 유닛의 장비 추가 함수
	void AddInitUnitEquipment(std::shared_ptr<Entity> _entity, const std::string& _classType);

	// NPC용 유닛의 장비 추가 함수
	void AddNPCEquipment(std::shared_ptr<Entity> _entity, const std::string& _classType);

	// 배치할 때 쓰는 장비 추가 함수
	void AddUnitEquip(std::shared_ptr<Entity> _entity, const std::string& _classType, const int& _buffLv);

	// 무기는 배치를 완료해야만 생성되게끔한다.
	void AddWeapon(std::shared_ptr<Entity> _entity, const std::string& _classType);

	// 장비의 교체와 관련된 함수
	void UpdateEquipment(std::shared_ptr<Entity> _entity, const std::string& _classType, const int& _buffLv);

	// 임시로 만드는 파서 함수
		// Csv 파서 -------------------------------------------------------
	/// <summary> 첫 줄을 제외하고 읽는 string 값이 포함된 cvs 문서를 읽을 경우 쓰는 파서 </summary>
	/// <typeparam name="T">테이블 Info</typeparam> <param name="_fileName">파일 이름</param> <returns>std::vector<T></returns>
	template <typename T>
	std::vector<T> ParseEmptyCSV(const std::string& _fileName);

private:
	entt::registry& mRegistry;
	EntityManager* mpEntityManager;
	std::map<UID, std::vector<UID>> mUnitEquipmentMap; // 유닛에 착용하는 파츠에 대한 맵(같은 값끼리 정렬되는 게 좋아서 Map으로 구현)


	std::vector<EquipmentMeshInfo> mpEquipmentMeshVec;
	std::vector<WeaponMeshInfo> mpWeaponMeshVec;

	// 배열
	std::array<std::pair<int, std::string>, 14> mEquipPart;

	int asc[2];

	PhysicsManager* mpPhysicsManager;
	RenderManager* mpRenderManager;
	ResourceManager* mpResourceManager;

	// 무기를 제외한 장비를 입히는 함수
	void TestWearEquipment(std::shared_ptr<Entity> _entity, const EquipmentMeshInfo& _info, const std::string& _targetBoneName, bool _isInitalize);

	// 무기를 장착하는 함수
	void WearWeapon(std::shared_ptr<Entity> _entity, const WeaponMeshInfo& _info, bool _isInitalize);
};

template <typename T>
void DEBUG_Print(const T& _message)
{
#ifdef _DEBUG
	std::cout << _message << std::endl;
#endif
}

template <typename T>
std::vector<T> EquipmentManager::ParseEmptyCSV(const std::string& _fileName)
{
	std::vector<T> units = {};
	std::string filepath = mpResourceManager->GetFileManager()->GetFullFilePath(_fileName);
	std::ifstream file(filepath);
	std::string line;

	// 파일 오픈 실패 시 빈 벡터 반환
	if (!file.is_open())
	{
		std::cerr << "Failed to open file: " << filepath << std::endl;
		return units;
	}

	int columnCount = 0;
	// 첫 번째 줄(헤더)을 읽어들임 -> 칼럼 개수 카운트
	if (std::getline(file, line))
	{
		std::stringstream ss(line);
		std::string column;

		// 쉼표로 구분된 각 필드를 세어 칼럼 개수 카운트
		while (std::getline(ss, column, ','))
		{
			columnCount++;
		}
	}

	// 두 번째 줄부터 읽어준다.
	while (std::getline(file, line))
	{
		std::stringstream ss(line);
		std::string cell;
		std::vector<std::string> row;
		int currentColumnCount = 0; // 공백을 처리하기 위해 불완전한 것을 스킵하는 것을 방지하기 위한 변수 추가

		// 쉽표로 구분된 각 필드를 처리
		while (std::getline(ss, cell, ','))
		{
			if (cell.empty())
			{
				row.push_back(""); // 빈 셀을 ""로 저장
			}
			else
			{
				row.push_back(cell);
			}
		}
		// 만약 누락된 칼럼이 있다면 ""로 채운다.
		while (currentColumnCount < columnCount)
		{
			row.push_back("");  // 부족한 칼럼은 ""로 채움
			currentColumnCount++;
		}

		if (row.size() >= columnCount)
		{  // 모든 칼럼이 있는지 확인
			units.emplace_back(row);
		}
		else
		{
			std::cerr << "Incomplete row found and skipped." << std::endl;
		}
	}
	return units;
}