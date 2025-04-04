#pragma once
#include "IManager.h"

class RenderManager;

class WeaponManager
	: public IManager
{
public:
	/// <summary>
	/// entt::registry& 멤버로 가질경우 기본 생성자 삭제
	/// </summary>
	WeaponManager() = delete;

	/// <summary>
	/// 클래스에서 엔티티들의 컴포넌트를 사용할 경우 
	/// "entt::registry& mRegistry" 멤버로 추가하고, 생성자 추가
	/// </summary>
	/// <param name="_registry"></param>
	WeaponManager(entt::registry& _registry, RenderManager* _renderManager);

	~WeaponManager() = default;

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

	void Render();

private:
	entt::registry& mRegistry;
	RenderManager* mpRenderManager;
};

