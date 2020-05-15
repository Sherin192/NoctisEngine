#include "LightHelpers.hlsl"

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