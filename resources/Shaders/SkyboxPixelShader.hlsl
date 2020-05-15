#include "LightHelpers.hlsl"

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