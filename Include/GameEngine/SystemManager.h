#pragma once
#include "IManager.h"
#include "ISystem.h"

class SystemManager
	: public IManager
{
public:
	SystemManager() = delete;
	SystemManager(entt::registry& _registry);

	virtual ~SystemManager() = default;

	virtual bool Initialize() override;
	virtual void FixedUpdate(float _fixedDTime) override;
	virtual void Update(float _dTime) override;
	virtual void LateUpdate(float _dTime) override;
	virtual void Finalize() override;

	void AddSystem(std::shared_ptr<ISystem> _pSystem);

private:
	entt::registry& m_registry;
	std::vector<std::shared_ptr<ISystem>> m_pSystems;
};

