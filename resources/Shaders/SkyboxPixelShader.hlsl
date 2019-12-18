#include "LightHelpers.hlsl"

cbuffer ConstantBufferPerObject : register(b0)
{
    float4x4 world; // 64 bytes
    //----------------------------------
    float4x4 worldInvTranspose; // 64 bytes
    //----------------------------------
    float4x4 worldViewProj; // 64 bytes
    //----------------------------------
    GPUMaterial material; // 80 bytes
    //----------------------------------
};												//272 bytes total

TextureCube CubeMap : register(t0);
sampler Sampler : register(s0);

struct ps_Input
{
    float4 posH : SV_Position;
    float3 texCoord : TEXCOORD;
};

float4 PS(ps_Input pin) : SV_TARGET
{
    return CubeMap.Sample(Sampler, pin.texCoord);
}