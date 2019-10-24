#include "LightHelpers.hlsl"

cbuffer ConstantBufferPerObject : register(b0)
{
	float4x4 world;								// 64 bytes
	//----------------------------------
	float4x4 worldInvTranspose;					// 64 bytes
	//----------------------------------
	float4x4 worldViewProj;						// 64 bytes
	//----------------------------------
	GPUMaterial material;						// 48 bytes
	//----------------------------------
};												//240 bytes total
												//304 testing

struct vs_Input
{
	float3 position : POSITION;
};

struct vs_Output
{
	float4 posH : SV_Position;
};


//================================================================================
//Vertex Shader
//================================================================================

vs_Output VS(vs_Input vin)
{
	vs_Output vout;
		//Transform to homogeneous clip space.
	vout.posH = mul(worldViewProj, float4(vin.position, 1.0f));	
	return vout;
}