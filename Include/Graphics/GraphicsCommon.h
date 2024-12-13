#pragma once
#include "RendererHelper.h"
#include "PipelineStateObject.h"
#include <vector>
using std::vector;

///이거 나중에 외부에서 막 가져다 쓸수도 있으니까 나중엔 막아둬야함 const를 붙이든, 함수로 접근하도록 구현하든.. 해서.. 

namespace Dears {

	// 참고: DirectX_Graphic-Samples 미니엔진
	// https://github.com/microsoft/DirectX-Graphics-Samples/blob/master/MiniEngine/Core/GraphicsCommon.h

	namespace Graphics {

		// Samplers
		extern ComPtr<ID3D11SamplerState> gpLinearWrapSS;
		extern ComPtr<ID3D11SamplerState> gpLinearClampSS;
		extern ComPtr<ID3D11SamplerState> gpShadowPointSS;
		extern ComPtr<ID3D11SamplerState> gpShadowCompareSS;
		extern vector<ID3D11SamplerState*> gSampleStates;

		// Rasterizer States
		extern ComPtr<ID3D11RasterizerState> gpSolidRS;
		extern ComPtr<ID3D11RasterizerState> gpWireRS;

		// Depth Stencil States
		extern ComPtr<ID3D11DepthStencilState> gpDrawDSS; // 일반적으로 그리기

		// Shaders
		extern ComPtr<ID3D11VertexShader> gpBasicVS;
		extern ComPtr<ID3D11VertexShader> gpAnimeVS;
		extern ComPtr<ID3D11VertexShader> gpEquimentVS;
		extern ComPtr<ID3D11VertexShader> gpSkyboxVS;
		extern ComPtr<ID3D11VertexShader> gpDepthOnlyVS;
		extern ComPtr<ID3D11VertexShader> gpDepthAniOnlyVS;
		extern ComPtr<ID3D11VertexShader> gpDepthEquipOnlyVS;
		extern ComPtr<ID3D11VertexShader> gpInstanceBasicVS;

		extern ComPtr<ID3D11PixelShader> gpBasicPS;
		extern ComPtr<ID3D11PixelShader> gpSkyboxPS;

		//ComputeShader
		extern ComPtr<ID3D11ComputeShader> gpParticleComputeShader;


		// Input Layouts
		extern ComPtr<ID3D11InputLayout> gpBasicIL;
		extern ComPtr<ID3D11InputLayout> gpAnimeIL;
		extern ComPtr<ID3D11InputLayout> gpSkyboxIL;
		extern ComPtr<ID3D11InputLayout> gpDepthOnlyIL;
		extern ComPtr<ID3D11InputLayout> gpDepthAniOnlyIL;
		extern ComPtr<ID3D11InputLayout> gpDepthEquipOnlyIL;

		extern ComPtr<ID3D11InputLayout> gpInstanceBasicIL;


		// Graphics Pipeline States
		extern PipelineStateObject gBasicGeometryPSO;
		extern PipelineStateObject gAnimeGeometryPSO;
		extern PipelineStateObject gEquipmentGeometryPSO;
		extern PipelineStateObject gDebugGeometryPSO;
		extern PipelineStateObject gCubeMapGeometryPSO;
		extern PipelineStateObject gDepthOnlyPSO;
		extern PipelineStateObject gDepthAniOnlyPSO;
		extern PipelineStateObject gDepthEquipOnlyPSO;
		extern PipelineStateObject gOpacityPSO;

		extern PipelineStateObject gBasicInstancingPSO;

		extern PipelineStateObject gPunchingPSO;
		extern PipelineStateObject gEdgePSO;
		extern PipelineStateObject gWaterPSO;
		
		extern PipelineStateObject gTestPSO;
		extern PipelineStateObject gParticlePSO;
		extern PipelineStateObject gSamplerPSO;
		extern PipelineStateObject gPostEffectPSO;

		// Blend States
		extern ComPtr<ID3D11BlendState> gpOpacityBS;


		void InitCommonStates(ComPtr<ID3D11Device>& device);

		// 내부적으로 InitCommonStates()에서 사용
		void InitSamplers(ComPtr<ID3D11Device>& device);
		void InitRasterizerStates(ComPtr<ID3D11Device>& device);
		void InitBlendStates(ComPtr<ID3D11Device>& device);
		void InitDepthStencilStates(ComPtr<ID3D11Device>& device);
		void InitPipelineStates(ComPtr<ID3D11Device>& device);
		void InitShaders(ComPtr<ID3D11Device>& device);

	} // namespace Graphics

} // namespace hlab
