#pragma once
#include "ClassTypeEnum.h"
#include "Entity.h"
#include "CharacterMeshInfo.h"
#include "PlayerComponent.h"
#include "EnemyComponent.h"
#include "ResourceManager.h"

/// <summary>
/// 유닛의 클래스를 관리하고, 애니메이션, 본, 기본 메쉬를 넣어줍니다.
/// </summary>
class ClassManager
{
public:
	ClassManager(EntityManager* _pEntityManager, ResourceManager* _pResourceManager);
	~ClassManager();

	// 유저의 클래스 정보를 불러와 세팅한다.
	void Initialize();

	// 

	/// <summary> 유닛의 기본 메쉬 정보를 찾아서 추가한다. </summary>
	/// <param name="_entity">유닛 엔티티</param> <param name="_classType">클래스</param>
	void AddUnitMesh(std::shared_ptr<Entity> _entity, const std::string& _classType);

	/// <summary> 배치 상태에 클래스 버튼을 다른 것을 누를 때 유닛의 기본 메쉬 정보를 변경한다. </summary>
	/// <param name="_entity">유닛 엔티티</param> <param name="_classType">클래스</param>
	void UpdateUnitMesh(std::shared_ptr<Entity> _entity, const std::string& _classType);

	// ClassTypeVec을 반환한다.
	std::vector<std::pair<std::string, ClassType>>* GetClassTypeVec();

public:
	/// <summary> 이넘 타입의 클래스를 string 값으로 변환합니다. </summary>
	/// <param name="_classTypeEnum">ClassType 이넘</param> <returns>std::string</returns>
	std::string ConvertClasstypeToString(ClassType _classTypeEnum);

private:
	std::vector<CharacterMeshInfo> mpClassMeshInfoVec;
	// 데이터가 적어서 맵에서 Vector로 바꿨고, 삽입 삭제가 벌어지지 않아 벡터로 제작
	std::vector<std::pair<std::string, ClassType>> mStringToClassTypeVec; // 받아온 스트링 값을 클래스타입으로 변환하기 위한 맵
	ClassType ConvertStringToClasstype(const std::string& _classType);
	ResourceManager* mpResourceManager;
};