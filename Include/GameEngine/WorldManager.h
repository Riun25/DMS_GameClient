#pragma once
#include "IManager.h"
#include "World.h"
#include "Scene.h"
#include "Entity.h"

/// <summary>
/// 월드의 생성과 소멸, 씬 전환 등 게임 월드의 흐름을 관리하는 클래스입니다.
/// 2024.06.03 _Doyo;
/// </summary>
class WorldManager
	: public IManager
{
public:
	WorldManager() = delete;
	WorldManager(entt::registry& _registry);
	~WorldManager() = default;

	/// <summary>
	/// 월드 매니저를 초기화합니다.
	/// </summary>
	/// <returns>실패시 false 반환</returns>
	virtual bool Initialize() override;

	/// <summary>
	/// 고정된 프레임 간격으로 물리 업데이트를 수행합니다.
	/// </summary>
	/// <param name="_dTime">고정된 프레임시간</param>
	virtual void FixedUpdate(float _fixedDTime) override;

	/// <summary>
	/// 매 프레임 업데이트를 수행합니다.
	/// </summary>
	/// <param name="_dTime">프레임 시간</param>
	virtual void Update(float _dTime) override;

	/// <summary>
	/// 후처리 업데이트를 수행합니다.
	/// </summary>
	/// <param name="_dTime">프레임 시간</param>
	virtual void LateUpdate(float _dTime) override;

	/// <summary>
	/// 월드 매니저를 종료하고 정리합니다.
	/// </summary>
	virtual void Finalize() override;

	/// <summary>
	/// 새로운 월드를 생성합니다.
	/// 안써요 안써
	/// </summary>
	/// <returns>생성된 월드 객체</returns>
	// std::shared_ptr<World> CreateWorld();

	/// <summary>
	/// 월드 매니저에 월드 객체를 추가합니다.
	/// </summary>
	/// <returns>추가한 월드 객체</returns>
	std::shared_ptr<World> AddWorld(std::shared_ptr<World> _world);

	/// <summary>
	/// 현재 월드를 반환합니다.
	/// </summary>
	/// <returns>현재 월드 객체</returns>
	std::shared_ptr<World> GetCurrentWorld();

	bool ShouldQuit() const;
	void RequestQuit();

	std::shared_ptr<IData> GetWorldData(const std::string& _name);

	IData* GetSceneData();

private:
	entt::registry& mRegistry;
	std::unordered_map<UID, std::shared_ptr<World>> mpWorld;	// 월드 맵핑
	std::shared_ptr<World> mpCurrentWorld;						// 현재 월드

	bool mShouldQuit = false;	// 엔진을 종료할 것인지 확인하는 플래그
};
