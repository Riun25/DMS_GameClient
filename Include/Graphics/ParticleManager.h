#pragma once
#include<wrl.h>
#include<d3d11.h>
#include <unordered_map>
#include<string>

#include "BufferData.h"
#include "ObjectPool.h"

using Microsoft::WRL::ComPtr;

class ParticleManager
{
public:
	ParticleManager() = delete;
	ParticleManager(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pDeviceContext, unsigned int _scale);
private:
	ComPtr<ID3D11Device> mpDevice;											//디바이스
	ComPtr<ID3D11DeviceContext> mpDeviceContext;							//디바이스 컨텍스트
	//ComPtr<ID3D11DeviceContext1> mpDeviceContext;						//디바이스 컨텍스트
public:
	ObjectPool<CSParticleData>* mpParticleObjectpool;

	ComPtr<ID3D11Buffer> mpParticleStructedBuffer;				//파티클에 사용할 structedBuffer
	ComPtr<ID3D11UnorderedAccessView> mpParticleUAV;			//파티클에 사용할 UnorderedAccessView
	ComPtr<ID3D11ShaderResourceView> mpParticleSRV;			//파티클에 사용할 UnorderedAccessView

	ComPtr<ID3D11Buffer> mpParticleIndexBuffer;				//파티클에 사용할 IndexStructedBuffer
	ComPtr<ID3D11UnorderedAccessView> mpParticleIndexUAV;		//파티클에 사용할 UnorderedAccessView
	ComPtr<ID3D11ShaderResourceView> mpParticleIndexSRV;		//파티클에 사용할 UnorderedAccessView

	
	ComPtr<ID3D11Buffer> mpParticleVertexinfoBuffer;
	ComPtr<ID3D11UnorderedAccessView> mpParticleVertexInfoUAV;
	ComPtr<ID3D11ShaderResourceView> mpParticleVertexInfoSRV;

	//스테이징 버퍼들
	ComPtr<ID3D11Buffer> mpParticleInfoStagingBuffer;
	ComPtr<ID3D11Buffer> mpIndexStagingBuffer;

	ComPtr<ID3D11Buffer> mpVertexBuffer;
	ComPtr<ID3D11Buffer> mpIndexBuffer;
	unsigned int mNumIndices = 0;

public:
	void Initialize();
	void CreateParticleStructedBufferandUAVandSRV();
	void CreateStagingBuffer();
	void Update();
	void Render();
	void ComputeShaderBarrier();
	void SetVertexBufferAndIndexBuffer(ComPtr<ID3D11Buffer> _vertexBuffer, ComPtr<ID3D11Buffer> _indexBuffer, unsigned int _numIndices);

public:
	void AddParticle(unsigned int _particleNum, CSParticleData& _particleData);
};

