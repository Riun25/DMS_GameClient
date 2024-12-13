#pragma once
#include<wrl.h>
#include<d3d11.h>
#include "ModelInfo.h"
#include "BufferData.h"
using Microsoft::WRL::ComPtr;

// enum class AnimationPlayType
// {
// 	Loop,
// 	Once,
// };

struct ModelBuffer // 1개의 모델에 대해 여러 개의 객체를 가질 경우를 대비해 만드는 버퍼
{
	ComPtr<ID3D11Buffer> pVertexBuffer;					// 버텍스 버퍼
	ComPtr<ID3D11Buffer> pIndexBuffer;					// 인덱스 버퍼

	ComPtr<ID3D11Buffer> pVSConstantBuffer;				// 버텍스 컨스턴트 버퍼
	ComPtr<ID3D11Buffer> pPSConstantBuffer;				// 픽셸 컨스턴트 버퍼
	ComPtr<ID3D11Buffer> pBoneConstantBuffer;				// 본 컨스턴트 버퍼	   
	ComPtr<ID3D11Buffer> pTargetBoneConstantBuffer;		// 타겟 본 컨스턴트 버퍼	   

	ComPtr<ID3D11Buffer> pInstanceConstantBuffer;			//인스턴싱을 할때 필요한 인스턴스 컨스턴트 버퍼	   
	unsigned int numInstances = 0;								//인스턴스 개수

	ComPtr<ID3D11Buffer> pVSEdgeConstantBuffer;			// 외곽선 컨스턴트 버퍼
	ComPtr<ID3D11Buffer> pPSEdgeConstantBuffer;			// 외곽선 컨스턴트 버퍼

	ComPtr<ID3D11Buffer> pVSWaterConstantBuffer;			//물 컨스턴트 버퍼 -> 후에 물 쉐이딩으로 발전시킨다.

	ComPtr<ID3D11ShaderResourceView> pDiffuseTexture;		// 텍스처와 쉐이더 리소스 뷰 구조체
	ComPtr<ID3D11ShaderResourceView> pNormalTexture;
	ComPtr<ID3D11ShaderResourceView> pCubeMapDiffuseResourceView;
	ComPtr<ID3D11ShaderResourceView> pCubeMapSpecularResourceView;
	
	UINT offset = 0;										// 이거는 Mesh에 있는 게 더 어울리는 거 같은데..

	unsigned int numIndices = 0;
												
	Model* pTargetModel;

	Animation* pTargetAnimation = nullptr;						   //어떤 애니메이션을 재생시킬 것인지
	Animation* pNextTargetAnimation = nullptr;					   //어떤 애니메이션을 다음으로 재생시킬 것인지

	double animationPlaytime = 0;
	double nextAnimationPlaytime = 0;

// 	AnimationPlayType mAnimationPlayType = AnimationPlayType::Loop;
// 	AnimationPlayType mNextAnimationPlayType = AnimationPlayType::Loop;

	void operator=(ModelBuffer& _modelBuffer)
	{
		pVertexBuffer = _modelBuffer.pVertexBuffer;
		pIndexBuffer = _modelBuffer.pIndexBuffer;
		pVSConstantBuffer = _modelBuffer.pVSConstantBuffer;
		pPSConstantBuffer = _modelBuffer.pPSConstantBuffer;
		pBoneConstantBuffer = _modelBuffer.pBoneConstantBuffer;
		pDiffuseTexture = _modelBuffer.pDiffuseTexture;
		pNormalTexture = _modelBuffer.pNormalTexture;
		offset = _modelBuffer.offset;
	}
};


