#pragma once
#include<wrl.h>
#include<d3d11.h>

using Microsoft::WRL::ComPtr;
struct CubeMapInfo
{
	ComPtr<ID3D11ShaderResourceView> pCubeMapResourceView;
	ComPtr<ID3D11PixelShader> pPixelShader;
	ComPtr<ID3D11VertexShader> pVertexShader;
	ComPtr<ID3D11InputLayout> pInputLayout;
};