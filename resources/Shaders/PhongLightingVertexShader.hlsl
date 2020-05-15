#include "LightHelpers.hlsl"


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
	float2 texCoord : TEXCOORD;
	float3 T : TANGENT;
	float3 B : BITANGENT;
	float3 N : NORMAL;
};


//================================================================================
//Vertex Shader
//================================================================================

vs_Output VS(vs_Input vin)
{
		vs_Output vout;
		//Transform to world space.
    vout.posW = mul(world, float4(vin.position, 1.0f)).xyz;
	
		//Transform to homogeneous clip space.
    vout.posH = mul(worldViewProj, float4(vin.position, 1.0f));
		
	vout.texCoord = vin.texCoord;

	vout.T = normalize(mul((float3x3) world, vin.tangent));
	vout.B = normalize(mul((float3x3) world, vin.bitangent));
    vout.N = normalize(mul((float3x3)world, vin.normal));

	return vout;
}