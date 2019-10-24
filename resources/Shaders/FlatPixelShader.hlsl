#include "LightHelpers.hlsl"

#define MAX_POINT_LIGHTS 8

cbuffer ConstantBufferPerFrame : register(b1)
{
    DirectionalLight dirLight; // 64 bytes
	//----------------------------------
    PointLight pointLights[MAX_POINT_LIGHTS]; // 512 bytes
	//----------------------------------
//	SpotLight spotLight;
	//----------------------------------
    float3 eyePos; // 12 bytes
    float pad; // 4  bytes
	//----------------------------------
};												//720 bytes total

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


struct ps_Input
{
    float4 posH : SV_Position;
};

float4 PS(ps_Input pin) : SV_TARGET
{
    return material.diffuse;
}