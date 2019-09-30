#include "LightHelpers.hlsl"

cbuffer ConstantBufferModelData : register(b0)
{
	float4x4 world;								// 64 bytes
	//----------------------------------
	float4x4 worldInvTranspose;					// 64 bytes
	//----------------------------------
	float4x4 worldViewProj;						// 64 bytes
	//----------------------------------
	Material material;							// 48 bytes
	//----------------------------------
};												//240 bytes total


struct vs_Input
{
	float4 position : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
};

struct vs_Output
{
	float4 posH : SV_Position;
	float3 posW : POSITION;
	float3 normalW : NORMAL;
};


//================================================================================
//Vertex Shader
//================================================================================

vs_Output VS(vs_Input vin)
{
		vs_Output vout;
		//Transform to world space.
		vout.posW = mul(vin.position, world).xyz;
		vout.normalW = mul(vin.normal, (float3x3)worldInvTranspose);
	
		//Transform to homogeneous clip space.
		vout.posH = mul(vin.position, worldViewProj);

		return vout;
}