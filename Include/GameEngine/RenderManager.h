#pragma once
#include "IManager.h"
#include "AnimationManager.h"
#include "Graphics/DearsGraphicsEngine.h"

class WorldManager;
class TimeManager;
class ResourceManager;
class Camera;
class Entity;
class LightComponent;

/// <summary>
/// 게임 엔진에서 렌더링을 담당하는 클래스입니다.
/// 그래픽 엔진의 인터페이스를 가져와 랩핑해서 사용할 예정입니다.
/// 2024.04.22 _Doyo;
/// </summary>
class RenderManager
	: public IManager
{
public:
	RenderManager() = delete;
	RenderManager(entt::registry& _registry, std::shared_ptr<DearsGraphicsEngine> _pGraphicsEngine
		, WorldManager* _pWorldManager, TimeManager* _pTimeManager);
	~RenderManager();

	/// <summary>
	/// 렌더링 시스템을 초기화합니다.
	/// </summary>
	/// <returns>초기화 실패시 false 반환</returns>
	virtual bool Initialize() override;

	/// <summary>
	/// 씬 전환시 씬을 초기화합니다.
	/// </summary>
	/// <returns></returns>
	bool InitializeScene();

	/// <summary>
	/// 씬의 배경을 적용하는데 꼭 필요함.
	/// </summary>
	/// <param name="diffuseTextureName">diffuse_texture Key값</param>
	/// <param name="specularTextureName">specular_texture Key값</param>
	void SetCubeMap(std::string diffuseTextureName, std::string specularTextureName);

	/// <summary>
	/// 런타임에 엔티티 추가 시 초기화를 진행해줄 수 있다.
	/// </summary>
	/// <param name="entity">초기화를 진행할 엔티티</param>
	/// <returns></returns>
	bool InitailizeEntity(std::shared_ptr<Entity> entity);

	/// <summary>
	/// 고정된 시간 간격으로 업데이트를 수행합니다.
	/// </summary>
	/// <param name="_dTime"></param>
	virtual void FixedUpdate(float _dTime) override {}

	/// <summary>
	/// 매 프레임 업데이트를 수행합니다.
	/// </summary>
	/// <param name="_dTime"></param>
	virtual void Update(float _dTime) override;

	void UpdateEntityTexture(std::shared_ptr<Entity> _entity);

	/// <summary>
	/// 후처리 업데이트를 수행합니다.
	/// </summary>
	/// <param name="_dTime"></param>
	virtual void LateUpdate(float _dTime) override;

	/// <summary>
	/// 모델의 렌더링을 수행합니다.
	/// </summary>
	/// <param name="_dTime"></param>
	void Render(float _dTime);

	void BeginRender();

	/// <summary>
	/// 렌더링을 끝낸다.
	/// </summary>
	/// <param name="_dTime"></param>
	void EndRender();

	/// <summary>
	/// 렌더링 시스템을 종료하고 정리합니다.
	/// </summary>
	virtual void Finalize() override;

	/// <summary>
	/// 렌더링 모드를 변경합니다.
	/// </summary>
	/// <param name="_model">모델 출력</param>
	/// <param name="_geometry">콜라이더 출력</param>
	/// <param name="_shadow">그림자 출력</param>
	void SetRenderMode(bool _model, bool _geometry, bool _shadow);

	/// <summary>
	/// 메인 카메라를 변경합니다.
	/// </summary>
	/// <param name="_index">변경할 카메라의 인덱스</param>
	void ChangeCamera(const uint8_t _index);

	/// <summary> 스크린 가로를 가져온다.</summary> <returns>int</returns>
	int GetScreenWidth();

	/// <summary> 스크린 높이를 가져온다.</summary> <returns>int</returns>
	int GetScreenHeight();


	/// <summary> 모델을 추가한다.</summary>
	/// <param name="_basePath"> 기본경로 </param> <param name="_fileName">파일 이름</param>
	void AddModel(std::string _basePath, std::string _fileName);

	// 모델을 삭제하는 거에 대해서는 성동재에게 물어보고 채울 것

	/// <summary> 애니메이션을 추가한다. </summary>
	/// <param name="_basePath"> 기본경로 </param> <param name="_fileName">파일 이름</param>
	void AddAnimation(std::string _basePath, std::string _fileName);

	/// <summary> 애니메이션 정보를 가져온다. </summary>
	/// <param name="_animeName">애니메이션 이름</param> <returns>Animation*</returns>
	Animation* Get_Animation(std::string _animeName);

	/// <summary> 애니메이션을 삭제한다. </summary>
	/// <param name="_animName"> 파일 이름(애니메이션 이름)</param>
	void Erase_Animation(std::string _animName);

	/// <summary> 3D 텍스처를 추가한다.</summary>
	/// <param name="_basePath"> 기본경로 </param> <param name="_fileName">파일 이름</param>
	void Add3DTexture(std::string _basePath, std::string _fileName);

	void AddDDSTexture(std::string _basePath, std::string _fileName);

	/// <summary> 3D 텍스처를 삭제한다. </summary>
	/// <param name="_animName"> 파일 이름(텍스처 이름)</param>
	void Erase_Textures(std::string _textureName);


	/// <summary> 모델의 특정 본을 가져온다 -> 장비에 적용할 때 사용한다. </summary>
	/// <param name="_modelName">모델 이름</param> <param name="_boneName">본 이름</param> <returns>int</returns>
	int Get_TargetModelBoneIndex(std::string _modelName, std::string _boneName);

	// 실제 본의 위치를 받아오는 함수 -> 본의 offsetMatrix가 곱해지지 않은 본의 위치값만 받아온다, 기본 스케일 = 1

	/// <summary>실제 본의 위치를 받아오는 함수 -> 본의 offsetMatrix가 곱해지지 않은 본의 위치값만 받아온다 </summary>
	/// <param name="_targetModel">모델 이름</param> <param name="_targetBoneName">본 이름</param> <param name="_scale">스케일 = 1.0f</param>
	/// <returns>Matrix</returns>
	Matrix GetTargetBoneAboveMatrix(std::string _targetModel, std::string _targetBoneName, float _scale = 1.0f);

	/// <summary> 충돌 체크를 위해 AABB를 가져오는 함수 </summary>
	/// <param name="__targetModel">모델 이름</param> <returns>AABB</returns>
	AABB Get_AABB(std::string __targetModel);

	// 라이트 세팅 -------------------------------------------------------------------------------

	/// <summary> 원하는 Light의 개수를 만들고, PSConstantBufferData에 전해준다.</summary>
	/// <param name="_psBufferData">PSConstantBufferData</param> <param name="_num">개수</param>
	void LightInitialize(CommonConstantBufferData* _ccBufferData, UINT _num);

	/// <summary> Directional Light 설정 </summary>
	/// <param name="_psBufferData">PSConstantBufferData</param> <param name="_index">인덱스</param> <param name="_strength">강도</param> 
	/// <param name="_dir">방향</param> <param name="_color">색</param>
	void SetDirLight(CommonConstantBufferData* _ccBufferData, UINT _index, float _strength, Vector3 _dir, Vector3 _color = Vector3(1.0f, 1.0f, 1.0f));

	/// <summary> 현재 컨스턴트 버퍼의 빛의 개수와 정보 출력 </summary>
	/// <param name="_psBufferData">PSConstantBufferData</param>
	void PrintLightInfo(CommonConstantBufferData* _ccBufferData);

	void CameraSetPerspective();
	void CameraSetOrthographic(float _scale);

	void SetOpacityFactor(const Vector4& _value);

	void AddParticle(uint16_t _num, CSParticleData& _particleData) const;

	std::shared_ptr<DearsGraphicsEngine> m_pGraphicsEngine; // 그래픽스 엔진 객체

private:
	// 렌더링 관련 함수 분리
	void RenderShadow(LightComponent* _light);
	void RenderModel(LightComponent* _light);
	void RenderGeometry(LightComponent* _light);

	// 업데이트 함수들 분리
	void UpdateLight();
	void UpdateModel(float _dTime);
	void UpdateCubeMap();
	void UpdatePSCB();
	void UpdateTexture();

private:
	// 렌더링 모드 설정
	bool mRenderModel = true;
	bool mRenderGeometry = true;
	bool mRenderShadow = true;

	entt::registry& mRegistry;
	WorldManager* mpWorldManager;
	TimeManager* mpTimeManager;

	Camera* mpWorldCamera;									//#sung 아예 월드카메라와 라이트 카메라의 포인터를 변수로 가지게 하였다.
	Camera* mpLightCamera[MAX_LIGHTS];
	AnimationManager* mpAnimationManager;

	std::unique_ptr<entt::observer> mpLightObserver;
	std::unique_ptr<entt::observer> mpMeshObserver;
	std::unique_ptr<entt::observer> mpTexture3DObserver;
	std::unique_ptr<entt::observer> mpTexture2DObserver;

	float m_opacityFactor[4] = { 1.f,1.f,1.f,1.f };

};