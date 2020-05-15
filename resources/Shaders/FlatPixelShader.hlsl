#include "LightHelpers.hlsl"

#define MAX_POINT_LIGHTS 8


cbuffer ConstantBufferMaterial :register(b2)
{
    GPUMaterial material; // 80 bytes
    //----------------------------------
}
struct ps_Input
{
    float4 posH : SV_Position;
};

float4 PS(ps_Input pin) : SV_TARGET
{
    return material.diffuse;
}