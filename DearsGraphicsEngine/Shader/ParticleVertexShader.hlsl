#include "Common.hlsli"
#define MAX_INSTANCE 100

struct PSInput // GS가 있다면 GSInput으로 사용됨
{
    float4 position : SV_POSITION;
    float3 color : COLOR;
    float opacity :OPACITY;
    unsigned int available : AVAILABLE;        //사용자 정의 시맨틱
};

struct ParticleVertexInput
{
    matrix scalexTrans;             // scale * transpose Matrix
    matrix rot;                     // rotationMatrix
    float3 color;
    float opacity;
};

struct ParticleIndexInputs
{
    unsigned int available;
};

StructuredBuffer<ParticleVertexInput> ParticleVertexInputs : register(t0);
StructuredBuffer<ParticleIndexInputs> AvilableIndexInputs : register(t1);


// VSInput이 없이 vertexID만 사용
PSInput main(VertexShaderInput input, uint instanceID : SV_InstanceID)
{

    // 인스턴스 위치를 적용하여 정점의 월드 위치를 계산
    float4 worldPos = float4(input.pos, 1.0);

    Matrix viewNoTrans = invView;
    viewNoTrans[3] = float4(0, 0, 0, 1);
    worldPos = mul(worldPos, ParticleVertexInputs[instanceID].rot);
    worldPos = mul(worldPos, viewNoTrans);

    worldPos = mul(worldPos, ParticleVertexInputs[instanceID].scalexTrans);


    // 월드 -> 뷰 -> 프로젝션 변환
    worldPos = mul(worldPos, view);
    worldPos = mul(worldPos, proj);

    PSInput output;
    output.position = worldPos;
    output.available = AvilableIndexInputs[instanceID].available;
    output.opacity = ParticleVertexInputs[instanceID].opacity;
    output.color = ParticleVertexInputs[instanceID].color;

    return output;

}
