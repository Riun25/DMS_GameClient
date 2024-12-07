#pragma once
#include "EntityManager.h" // 이거 삭제하면 byte 오류 발생하니 중복되더라도 지우지 말 것
#include "Entity.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "SoundManager.h"
#include "UIManager.h"
#include "EquipmentManager.h"
#include "ProjectileSystem.h"
#include "NPCMeshInfo.h"

/// <summary>
/// NPC와 관련된 클래스입니다.
/// </summary>
class NPCSystem
{
public:
	NPCSystem(EntityManager* _pEntityManager, RenderManager* _pRenderManager, ResourceManager* _pResourceManager
		, SoundManager* _pSoundManager, UIManager* _pUIManager, ProjectileSystem* _pProjectileSystem, EquipmentManager* _pEquipmentManager);
	~NPCSystem();

public:
	
	void Initialize();

	// 유닛을 추가합니다. (이때 장비도 그냥 다 장착합니다.)
	void AddNPC(const std::string& _classType);

	// 애니메이션은 잘 작동해야 합니다. TODO : 부담이 되니 0.5초에 한 번 업데이트를 합시다.
	void Update(std::shared_ptr<Entity> _entity, uint8_t _numAlly, uint8_t _numEnemy);
	void FinalUpdate(std::shared_ptr<Entity> _entity, bool _mIsCleared);

	void TutorialUpdate(std::shared_ptr<Entity> _entity, int _textIdx);

	// 말을 할 수 있어야 합니다.
private:
	//UI 매니저를 가져야 합니다.
	// 엔티티 매니저, 렌더 매니저, 리소스 매니저, 사운드 매니저 가져야 합니다.
	EntityManager* mpEntityManager;
	RenderManager* mpRenderManager;
	ResourceManager* mpResourceManager;
	SoundManager* mpSoundManager;
	UIManager* mpUIManager;
	EquipmentManager* mpEquipmentManager;
	ProjectileSystem* mpProjectileSystem;

	std::vector<NPCMeshInfo> mpNPCMeshInfoVec;
};

