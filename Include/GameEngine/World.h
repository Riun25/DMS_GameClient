#pragma once
#include "pch.h"
#include "Scene.h"
#include "EventListener.h"

class IData;
class RenderManager;
class PhysicsManager;

/// <summary>
/// 씬의 컨셉트 정의, 베이스 클래스가 Scene인지 확인합니다.
/// </summary>
template <typename T>
concept IsScene = std::is_base_of<Scene, T>::value
|| std::is_same<Scene, T>::value;

/// <summary>
/// 게임 전체에 해당하는 월드 오브젝트 클래스입니다.
/// 게임의 씬들과 월드의 환경 등의 정보를 가지고 있습니다.
/// </summary>
class World
	: public EventListener
{
public:
	World() = delete;
	// World(entt::registry& _registry);
	World(entt::registry& _registry, EventManager* _pEventManager);
	virtual ~World() = default;

	/// <summary>
	/// 월드를 초기화합니다.
	/// </summary>
	/// <returns></returns>
	virtual bool Initialize();

	/// <summary>
	/// 고정된 시간 간격으로 물리 업데이트를 수행합니다.
	/// </summary>
	/// <param name="_dTime">고정된 델타타임</param>
	virtual void FixedUpdate(float _dTime);

	/// <summary>
	/// 매 프레임 업데이트를 수행합니다.
	/// </summary>
	/// <param name="_dTime">프레임 시간</param>
	virtual void Update(float _dTime);

	/// <summary>
	/// 후처리 업데이트를 수행합니다.
	/// </summary>
	/// <param name="_dTime">프레임 시간</param>
	virtual void LateUpdate(float _dTime);

	/// <summary>
	/// 월드를 종료하고 정리합니다.
	/// </summary>
	virtual void Finalize();

	/// <summary>
	/// 월드에 씬을 추가합니다.
	/// </summary>
	/// <param name="_pScene">추가할 씬 객체</param>
	std::shared_ptr<Scene> AddScene(std::shared_ptr<Scene> _pScene);

	/// <summary>
	/// 월드에 새로운 씬을 추가합니다.
	/// </summary>
	/// <param name="_name">씬 이름</param>
	/// <returns>생성된 씬 객체</returns>
	//std::shared_ptr<Scene> CreateScene(const std::string& _name);

	/// <summary>
	/// 월드에서 씬을 제거합니다.
	/// </summary>
	/// <param name="_pScene">제거할 씬 객체</param>
	void RemoveScene(std::shared_ptr<Scene> _pScene);

	/// <summary>
	/// 현재 씬을 _uid에 해당하는 씬으로 변경합니다.
	/// </summary>
	/// <param name="_uid">변경할 씬의 UID</param>
	void SetScene(const UID& _uid);

	/// <summary>
	/// 현재 씬을 _uid에 해당하는 씬으로 변경합니다.
	/// </summary>
	/// <param name="_uid">변경할 씬의 이름</param>
	void SetScene(const std::string& _name);

	/// <summary>
	/// 씬 맵을 반환합니다.
	/// </summary>
	/// <returns></returns>
	std::unordered_map<UID, std::shared_ptr<Scene>> GetSceneMap() const;

	/// <summary>
	/// 현재 씬을 반환합니다.
	/// </summary>
	/// <returns>현재 씬 객체</returns>
	std::shared_ptr<Scene> GetCurrentScene();

	/// <summary>
	/// 이전 씬을 반환합니다.
	/// </summary>
	/// <returns>이전 씬 UID</returns>
	UID GetPreviousScene();

	/// <summary>
	/// 월드의 UID를 반환합니다.
	/// </summary>
	/// <returns>월드 UID</returns>
	UID GetUID() const;

	std::shared_ptr<IData> GetWorldData(const std::string& _name);

protected:
	UID m_uid;													// 객체의 고유식별자
	std::unordered_map<UID, std::shared_ptr<Scene>> m_pScenes;	// 월드에 존재하는 씬 맵핑
	std::shared_ptr<Scene> m_pCurrentScene;						// 현재 씬
	std::unordered_map<std::string, std::shared_ptr<IData>> m_pWorldData;						// 월드에서 사용되는 데이터들
	entt::registry& m_registry;									// 엔티티 레지스트리
	UID m_previousScene;										// 이전 씬
};
