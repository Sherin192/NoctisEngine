#include "LightHelpers.hlsl"

#define MAX_POINT_LIGHTS 8

cbuffer ConstantBufferPerFrame : register(b1)
{
	DirectionalLight dirLight;					// 64 bytes
	//----------------------------------
	PointLight pointLights[MAX_POINT_LIGHTS];	// 512 bytes
	//----------------------------------
//	SpotLight spotLight;
	//----------------------------------
	float3 eyePos;								// 12 bytes
	float pad;									// 4  bytes
	//----------------------------------
};												//592 bytes total

cbuffer ConstantBufferPerObject : register(b0)
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

struct ps_Input
{
	float4 posH : SV_Position;
	float3 posW : POSITION;
	float3 normalW : NORMAL;
};

float4 PS(ps_Input pin) : SV_TARGET
{
		//Interpolating normal can unnormalize it, so normalize it.
		pin.normalW = normalize(pin.normalW);
	
		float3 toEye = normalize(eyePos - pin.posW);
	
		//Start with a sum of zero.
		float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
	
		//Sum the light contribution from each light source.
		float4 A, D, S;

		CalculateDirectionalLight(material, dirLight, pin.normalW, toEye, A, D, S);
		ambient += A;
		diffuse += D;
		spec += S;

		[unroll]
		for (int i = 0; i < MAX_POINT_LIGHTS; ++i)
		{
			if (!pointLights[i].enabled) continue;

			CalculatePointLight(material, pointLights[i], pin.posW, pin.normalW, toEye, A, D, S);
			ambient += A;
			diffuse += D;
			spec += S;
		}
		
		float4 litColor = diffuse + ambient + spec;
	
		//Common to take alpha from duffuse materail.
		litColor.a = litColor;
	
		return litColor;
}