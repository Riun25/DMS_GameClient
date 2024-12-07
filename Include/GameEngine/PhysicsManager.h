#pragma once
#include "IManager.h"
#include "PhysicsDefine.h"

class CollisionCallback;
class Entity;
class GameEngine;

/// <summary>
/// 물리 시뮬레이션을 관리하는 클래스입니다.
/// NVIDIA에서 제공하는 PhysX api를 사용해서 제작되었습니다.
/// 2024.06.03 _Doyo;
/// 
/// 속도 설정 추가
/// 박스 이외의 콜라이더도 제대로 작동하도록 수정
/// 캐릭터 컨트롤러 기능 수정
/// 
/// 충돌 그룹 설정 추가
/// 물리 씬의 설정을 바꿔서 재생성하는 메서드 추가 // 추후에 클라이언트에서 물리 씬 설정 후 생성하도록 수정할 예정
/// </summary>
class PhysicsManager
	: public IManager
{
public:
	PhysicsManager() = delete;
	PhysicsManager(entt::registry& _registry);
	PhysicsManager(entt::registry& _registry, GameEngine* _pEngine);
	~PhysicsManager() = default;

	/// <summary>
	/// 물리 시스템의 초기화합니다.
	/// </summary>
	/// <returns>초기화 실패시 false를 반환합니다.</returns>
	virtual bool Initialize();

	/// <summary>
	/// 물리 씬을 재생성합니다.
	/// 일단은 씬 데스크 구조체를 이용해서 생성
	/// <param name="_sceneDesc">씬 데스크 구조체</param>
	/// </summary>
	void CreatePhysicsScene(physx::PxSceneDesc _sceneDesc);

	/// <summary>
	/// 씬을 재생성하기 위한 씬 데스크를 생성합니다.
	/// </summary>
	/// <param name="_callback">시뮬레이션 이벤트 콜백 객체</param>
	/// <param name="PxSimulationFilterShader">시뮬레이션 필터 셰이더 함수 객체</param>
	/// <returns>씬 데스크</returns>
	physx::PxSceneDesc CreateSceneDesc(physx::PxSimulationEventCallback* _callback, physx::PxSimulationFilterShader _filterShader);

	/// <summary>
	/// 고정된 시간 간격으로 물리 업데이트를 수행합니다.
	/// </summary>
	/// <param name="_dTime">고정된 델타타임</param>
	virtual void FixedUpdate(float _fixedDTime) override;

	/// <summary>
	/// PhysX 시뮬레이션을 업데이트하고 객체의 변환 행렬을 업데이트합니다.
	/// </summary>
	/// <param name="_dTime">프레임 시간</param>
	virtual void Update(float _dTime) override;

	/// <summary>
	/// 후처리 업데이트를 수행합니다.
	/// </summary>
	/// <param name="_dTime">프레임 시간</param>
	virtual void LateUpdate(float _dTime) override;

	/// <summary>
	/// physx 종료 및 자원을 해제합니다.
	/// </summary>
	virtual void Finalize() override;

	/// <summary>
	/// 엔티티와 연결되어있는 물리 객체를 추가합니다.
	/// </summary>
	/// <param name="_pEntity">추가할 엔티티</param>
	/// <param name="_groupID">물리 객체의 충돌 그룹 설정</param>
	/// <param name="_filterData">물리 객체에 필터 데이터 추가</param>
 	//void AddPhysicsObject(std::shared_ptr<Entity>& _pEntity, physx::PxU32 _groupID = 0, physx::PxU32 _data = 0);
	void AddPhysicsObject(const UID& _uid, physx::PxU32 _groupID = 0, physx::PxU32 _data = 0);

	/// <summary>
	/// 연결된 물리 객체를 삭제합니다.
	/// </summary>
	void RemovePhysicsObject(std::shared_ptr<Entity>& _pEntity);

	void ClearScene();

	/// <summary>	
	/// 엔티티에 힘을 적용합니다.
	/// </summary>
	/// <param name="_pEntity">힘을 적용할 엔티티</param>
	/// <param name="_force">적용할 힘 벡터</param>
	void ApplyForce(std::shared_ptr<Entity>& _pEntity, const Vector3& _force, int _mode = 0);

	/// <summary>	
	/// 엔티티의 속도를 직접 설정합니다.
	/// </summary>
	/// <param name="_pEntity">속도를 설정할 엔티티</param>
	/// <param name="_force">적용할 속도 벡터</param>
	void SetVelocity(std::shared_ptr<Entity>& _pEntity, const Vector3& _velocity);

	/// <summary>
	/// 엔티티의 키네마틱 On/Off를 설정합니다.
	/// </summary>
	/// <param name="_pEntity">키네마틱 On/Off를 설정할 엔팉티</param>
	/// <param name="_isTrue">true면 위치값 고정</param>
	void SetKinematic(std::shared_ptr<Entity>& _pEntity, bool _isTrue);

	/// <summary>
	/// 캐릭터 컨트롤러를 추가합니다.
	/// </summary>
	/// <param name="_pEntity">추가할 엔티티</param>
	void AddCharacterController(std::shared_ptr<Entity>& _pEntity);

	/// <summary>
	/// 캐릭터 컨트롤러를 이동시킵니다.
	/// </summary>
	/// <param name="_pEntity">이동시킬 엔티티</param>
	/// <param name="_displacement">이동 벡터</param>
	/// <param name="_dTime">프레임 시간</param>
	void MoveCharacterController(std::shared_ptr<Entity>& _pEntity, const Vector3& _displacement, float _dTime);

	/// <summary>
	/// 연결된 캐릭터 컨트롤러를 삭제합니다.
	/// </summary>
	void RemoveCharacterController(std::shared_ptr<Entity>& _pEntity);

	void SetGroupCollisionFlag(uint16_t _gourp1ID, uint16_t _gourp2ID, bool _enableCollision = true);

	void SetKinematicTarget(std::shared_ptr<Entity> _entity, Vector3 _position, Quaternion _rotation);

	void SetFixPosition(std::shared_ptr<Entity> _entity, bool _isTrue);

	void UpdatePosition(std::shared_ptr<Entity> _entity, Vector3 _newPos);

	Vector3 PickObejct();

	Vector3 PickObejct(const std::string& _name);

// 	void OnOffSimulation(bool _on);

	physx::PxFilterData GetFilterData(std::shared_ptr<Entity> _pEntity);
	physx::PxFilterData GetFilterData(const entt::entity& _entity);
	void SetFilterData(std::shared_ptr<Entity>& _pEntity, const physx::PxFilterData& newFilterData);
	void SetFilterData(physx::PxShape* _pShape, const physx::PxFilterData& newFilterData);
	void SetFilterData(const entt::entity& _entity, const physx::PxFilterData& newFilterData);

	std::shared_ptr<Entity> GetEntityFromActor(physx::PxRigidActor* actor);

	void SetObjectSleep(std::shared_ptr<Entity> _pEntity);
	void SetObjectWake(std::shared_ptr<Entity> _pEntity);

	void SetALLObjectSleep();
	void SetALLObjectWake();

	void ClearFilterUpdate();

private:
	// 	struct Impl;
	// 	std::unique_ptr<Impl> m_pImpl;
	GameEngine* m_pEngine;
	entt::registry& m_registry;

	// physx 관련 멤버들
	physx::PxDefaultAllocator m_allocator;				// 기본 메모리 할당자
	physx::PxDefaultErrorCallback m_errorCallback;		// 기본 에러 콜백
	physx::PxFoundation* m_pFoundation;					// Foundation 객체, physx의 전반적인 설정 및 리소스를 관리
	physx::PxPhysics* m_pPhysics;						// Physics 객체
	physx::PxDefaultCpuDispatcher* m_pDispatcher;		// CPU 디스패쳐, 병렬 시뮬레이션을 위해 필요
	physx::PxScene* m_pScene;							// physx Scene 객체
	physx::PxMaterial* m_pMaterial;						// 디폴트로 사용 될 physx Material 객체
	physx::PxControllerManager* m_pControllerManager;	// physx Controller Manager 객체

	// 
#ifdef _DEBUG
	physx::PxPvd* m_pPvd;
	physx::PxPvdTransport* m_pTransport;
#endif

	//std::unordered_map<std::shared_ptr<Entity>, physx::PxRigidActor*> m_entities;	// 물리 객체의 맵
 	//std::unordered_map<std::shared_ptr<Entity>, physx::PxCapsuleController*> m_controllers; // 캐릭터 컨트롤러의 맵
 
 	//std::vector<physx::PxMaterial*> m_pMaterials;

	bool m_simulationOn = true;

	std::queue<std::pair<physx::PxShape*, physx::PxFilterData>> m_filterDataUpdates;

	std::unique_ptr<entt::observer> m_pRigidbodyObserber;
};

