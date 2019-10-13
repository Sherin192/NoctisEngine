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
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
	float2 texCoord : TEXCOORD;
};

struct vs_Output
{
	float4 posH : SV_Position;
	float3 posW : POSITION;
	float3 normalW : NORMAL;
	float2 texCoord : TEXCOORD;
	float3x3 TBN : TBN;
};


//================================================================================
//Vertex Shader
//================================================================================

vs_Output VS(vs_Input vin)
{
		vs_Output vout;
		//Transform to world space.
    vout.posW = mul(world, float4(vin.position, 1.0f)).xyz;
    vout.normalW = mul((float3x3) worldInvTranspose, vin.normal);
	
		//Transform to homogeneous clip space.
    vout.posH = mul(worldViewProj, float4(vin.position, 1.0f));
		
		vout.texCoord = vin.texCoord;

    float3 T = normalize(mul(world, float4(vin.tangent, 1.0f)).xyz);
    float3 B = normalize(mul(world, float4(vin.bitangent, 1.0f)).xyz);
    float3 N = normalize(mul(world, float4(vin.normal, 1.0f)).xyz);
		vout.TBN = float3x3(T, B, N);
		
		return vout;
}