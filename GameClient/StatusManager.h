#pragma once
#include "Entity.h"
#include "StatusTable.h"
#include "Transform.h"
#include "PlayerComponent.h"
#include "EnemyComponent.h"
#include "HealthBarComponenet.h"
#include "ClassManager.h"

/// <summary>
/// 유닛의 스테이터스를 관리하는 클래스입니다.
/// </summary>
class StatusManager
{
public:
	StatusManager(EntityManager* _pEntityManager, ResourceManager* _pResourceManager);
	~StatusManager();

	// 유저의 클래스 정보를 불러와 세팅한다.
	void Initialize();

	/// <summary> 아군(배치되는 것) 스테이터스를 추가한다. </summary>
	/// <param name="_entity">유닛 엔티티</param> <param name="_classType">클래스</param> <param name="_level">버프 레벨</param>
	void AddStatus(std::shared_ptr<Entity> _entity, const std::string& _classType, const int& _buffLv);

	/// <summary> 적군(미리 설정된 유닛) 스테이터스를 추가한다. </summary>
	/// <param name="_entity">유닛 엔티티</param> <param name="_classType">클래스</param>
	void AddInitStatus(std::shared_ptr<Entity> _entity, const std::string& _classType);

	/// <summary> 스테이터스가 바뀌면 업데이트한다. </summary>
	/// <param name="_entity">유닛 엔티티</param> <param name="_classType">클래스</param> <param name="_buffLv">버프 레벨</param>
	void UpdateAllyStatus(std::shared_ptr<Entity> _entity, const std::string& _classType, const int& _buffLv);

	/// <summary> 클래스가 변경되는 특수한 경우에 업데이트한다.</summary>
	/// <param name="_entity">유닛 엔티티</param> <param name="_classType">클래스</param>
	void UpdateStautsNClass(std::shared_ptr<Entity> _entity, const std::string& _classType);

	/// <summary> 이넘 타입의 클래스를 string 값으로 변환합니다. </summary>
	/// <param name="_classTypeEnum">ClassType 이넘</param> <returns>std::string</returns>
	std::string ConvertClasstypeToString(ClassType _classTypeEnum);

	/// <summary> ClassTypeMap을 클래스 매니저에서 가져오는 함수 </summary>
	/// <returns>std::vector&lt;std::pair&lt;std::string, ClassType&gt;&gt;*</returns>
	std::vector<std::pair<std::string, ClassType>>* GetClassTypeVec();

	/// <summary> 여기서부터는 게임 상수를 선언한다.(기획 값) </summary>
	float mMoneyBuffCoef = 1.5f;	// 돈버프 때 향상되는 수치
	float mBuffUnitSizeCoef = 1.4f; // 유닛 사이즈가 돈 버프 때 향상되는 수치

private:
	ClassManager* mpClassManager;
	ResourceManager* mpResourceManager;

	// CSV 파서로 불러올 테이블 벡터
	std::vector<StatusTable> mpStatusTableVec;
};

