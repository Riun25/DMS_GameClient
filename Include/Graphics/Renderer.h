#pragma once
#include<windows.h>
#include<wrl.h>
#include<d3d11.h>
#include <directxtk/SimpleMath.h>
#include "ModelBuffer.h"
#include "GraphicsResourceManager.h"
#include "RendererHelper.h"
#include "Camera.h"
#include "PipelineStateObject.h "
#include "GraphicsCommon.h"

using Microsoft::WRL::ComPtr;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector4;
using DirectX::SimpleMath::Quaternion;

///랜더러는 그래픽스 엔진의 내에서 랜더링만을 담당하는 클래스.
//Graphics 엔진내에 존재한다.
class Renderer
{
public:
	//생성자 호출 시 윈도우핸들과 시작픽셀, 끝픽셀을 받는다.
	Renderer(HWND _hWnd, int _startScreenWidth, int _startScreenHeight, int _endScreenWidth, int _endScreenHeight,
			ComPtr<ID3D11Device>& _pDevice, ComPtr<ID3D11DeviceContext> _pDeviceContext);
	~Renderer();
private:
	HWND mHWnd;				//핸들 윈도우
	int mStartScreenWidth;		//화면의 시작(가로 픽셀)
	int mStartScreenHeight;		//화면의 시작(세로 픽셀)
	int mEndScreenWidth;		//화면의 끝(가로 픽셀)
	int mEndScreenHeight;		//화면의 끝(세로 픽셀)

	int mScreenWidth;
	int mScreenHeight;

	///D3D에서 쓸 기본적인 변수들
public:
	ComPtr<ID3D11Device> mpDevice;										//디바이스
	ComPtr<ID3D11DeviceContext> mpDeviceContext;						//디바이스 컨텍스트


	ComPtr<IDXGISwapChain> mpSwapChain;									//스왑체인

	ComPtr<ID3D11Texture2D> mpBackBuffer;								//백버퍼, 메인렌더타겟
	ComPtr<ID3D11ShaderResourceView> mpMainRenderTargetSRV;				//메인렌더타겟(버퍼)의 SRV
	ComPtr<ID3D11RenderTargetView> mpRenderTargetView;					//렌더링 타겟뷰  //디퍼드 렌더링 시 2개 이상 필요

	ComPtr<ID3D11Texture2D> mpTempBuffer;								//실험용 버퍼
	ComPtr<ID3D11ShaderResourceView> mpTempTargetSRV;					//실험용 버퍼의 SRV
	ComPtr<ID3D11RenderTargetView> mpTempTargetView;					//실험용 버퍼의 렌더타겟

	D3D11_VIEWPORT mD3dScreenViewport;								// 뷰 포트 공간을 정의하는 구조체
	D3D11_VIEWPORT mD3dtempViewport;									// 실험용 뷰 포트 공간
	
	ComPtr<ID3D11DepthStencilView> mpDepthStencilView;					// 깊이 버퍼 스텐실 뷰
	
	UINT mNumQualityLevels;

	ComPtr<ID3D11ShaderResourceView> mpCubeMapDiffuseResourceView;
	ComPtr<ID3D11ShaderResourceView> mpCubeMapSpecularResourceView;

	
public:
	GraphicsResourceManager* mpResourceManager;
	RendererHelper* mpRenderHelper;
	Camera* mpTargetCamera;

	// Depth buffer 관련
	//ComPtr<ID3D11RenderTargetView> mpResolvedRTV;					//뎁스맵에서 사용하는 척하는 렌더링 타겟뷰
	//ComPtr<ID3D11Texture2D> mpDepthOnlyBuffer;  // No MSAA
	ComPtr<ID3D11DepthStencilView> mpDepthOnlyDSV; // No MSAA
	//ComPtr<ID3D11DepthStencilView> mpDepthStencilView; // use MSAA
	ComPtr<ID3D11ShaderResourceView> mpDepthOnlySRV;

	ComPtr<ID3D11Buffer> mpCommonConstantBuffer;


public:
	void SetCamera(Camera* _pTargetCamera);
public:
	bool Initialize(GraphicsResourceManager* _pResourceManager);
	void BeginRender();

	void SetCommonShaderResource(ComPtr<ID3D11ShaderResourceView> _diffuseTexture,
									ComPtr<ID3D11ShaderResourceView> _specularTexture);

	//공통 셰이더 리소스를 GPU에 Set해준다
	bool SetCommonShaderResourceToGPU();


	//공통 컨스턴트를 Set해준다.(Render 호출전에 필수)common
	void SetCommonConstant(ComPtr<ID3D11Buffer>& commonConstsBuffer);
	void Render(ModelBuffer* _modelbuffer);
	void Render_CubeMap(ModelBuffer* _modelbuffer);
	void EndRender();
	void Finalize();
	void SetPipelineState(const PipelineStateObject& _pso);

	void LightUpdate(CommonConstantBufferData* _psBufferData);
	void UpdateCommonConstantBuffer(CommonConstantBufferData& _CommonBufferData);

	void RenderDepthMap(ModelBuffer* _modelbuffer);
	void RenderAniDepthMap(ModelBuffer* _modelbuffer);
	void RenderEquipDepthMap(ModelBuffer* _modelbuffer);

	void RenderEdge(ModelBuffer* _modelbuffer);
	void RenderSampler(ModelBuffer* _modelbuffer);
	void RenderPostProcessing(ModelBuffer* _modelbuffer);

	void Rend_InstancedModels(ModelBuffer* _modelBuffers);

private:
	bool InitalizeD3D();

public:
	//mpDevice를 보내는 임시함수
	ComPtr<ID3D11Device> GetDevice() const { return mpDevice; }
};


