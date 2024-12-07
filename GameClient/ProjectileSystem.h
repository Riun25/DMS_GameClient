#pragma once
#include "IManager.h"
#include "EventListener.h"
#include "Entity.h"
#include "AStar.h"
#include "UserData.h"

class EntityManager;
class PhysicsManager;
class RenderManager;
class SoundManager;
class InputManager;
class WorldManager;
class SceneData;

/// <summary>
/// 투사체 관련 시스템
/// </summary>
class ProjectileSystem
	: public IManager, public EventListener
{
public:
	/// <summary>
	/// entt::registry& 멤버로 가질경우 기본 생성자 삭제
	/// </summary>
	ProjectileSystem() = delete;

	/// <summary>
	/// 클래스에서 엔티티들의 컴포넌트를 사용할 경우 
	/// "entt::registry& m_registry" 멤버로 추가하고, 생성자 추가
	/// </summary>
	/// <param name="_registry"></param>
	ProjectileSystem(entt::registry& _registry, EventManager* _pEventManager
		, EntityManager* _pEntityManager, PhysicsManager* _pPhysicsManager
		, RenderManager* _pRenderManagerm, SoundManager* _pSoundManager
		, InputManager* _pInputManager, WorldManager* _pWorldManager);

	~ProjectileSystem() = default;

	/// <summary>
	/// 매니저 초기화 
	/// </summary>
	/// <returns></returns>
	virtual bool Initialize() override;

	/// <summary>
	/// 물리 연산이 있는 경우 여기서 업데이트 // 보통은 사용 안할듯
	/// </summary>
	/// <param name="_fixedDTime">1/60으로 고정된 프레임 간격</param>
	virtual void FixedUpdate(float _fixedDTime) override;

	/// <summary>
	/// 일반적인 로직들 업데이트 
	/// </summary>
	/// <param name="_dTime">프레임 간격</param>
	virtual void Update(float _dTime) override;

	/// <summary>
	/// 후처리가 필요한 로직들 업데이트
	/// </summary>
	/// <param name="_dTime">프레임 간격</param>
	virtual void LateUpdate(float _dTime) override;

	/// <summary>
	/// 메모리 정리	// 보통은 씬 단위로 정리함
	/// </summary>
	virtual void Finalize() override;

	void ThrowCoin(const Event& _event);

	void CreateMoney(const Event& _event);
	// 
	// 	void ShootProjectile(const Event& _event);

	void CreateProjectile(const Event& _event);

	void TutirialUpdateThrow(float _dTime);
	void TutirialUpdateMoney(float _dTime);


	void SetAstar(AStar* _pAstar);

	// 돈의 위치가 저장된 벡터를 가져오는 함수
	std::vector<std::pair<std::shared_ptr<Entity>, Vector3>>* GetMoneyPosVec();

	bool mIsThrow = false; // NPC인 용병왕에게 애니메이션을 시도하라는 플래그

private:
	entt::registry& m_registry;
	EntityManager* m_pEntityManager;
	PhysicsManager* m_pPhysicsManager;
	RenderManager* m_pRenderManager;
	SoundManager* m_pSoundManager;
	InputManager* m_pInputManager;
	WorldManager* m_pWorldManager;

	// 적군 아군 모두가 확인하는 돈의 위치 저장 벡터
	std::vector<std::pair<std::shared_ptr<Entity>, Vector3>> mMoneyPosVec;
	std::vector<std::vector<int>>* mAstarMap;
	Vector2 mStartPoint;
	int mMapwidth;
	AStar* mpAstar;
	SceneData* m_pSceneData;
	std::shared_ptr<UserData> m_pWorldData;
	float spawnTime = 0.f;
	float mTutoSpawnTime = 0.f;

	float m_coinSpeed = 1;
	float m_arrowSpeed = 1;
};
