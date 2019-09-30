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
};												//720 bytes total

cbuffer ConstantBufferPerObject : register(b0)
{
	float4x4 world;								// 64 bytes
	//----------------------------------
	float4x4 worldInvTranspose;					// 64 bytes
	//----------------------------------
	float4x4 worldViewProj;						// 64 bytes
	//----------------------------------
	GPUMaterial material;						// 80 bytes
	//----------------------------------
};												//272 bytes total


Texture2D TexDiffuse : register(t0);
Texture2D TexSpecular : register(t1);
Texture2D TexNormal : register(t2);
sampler Sampler : register(s0);


struct ps_Input
{
	float4 posH : SV_Position;
	float3 posW : POSITION;
	float3 normalW : NORMAL;
	float2 texCoord : TEXCOORD;
	float3x3 TBN : TBN;
};

float4 PS(ps_Input pin) : SV_TARGET
{
		//Interpolating normal can unnormalize it, so normalize it.
		float3 normal = normalize(pin.normalW);

		if (HasTextureMap(material.textureBitField, TEX_SLOT_NORMAL))
		{
			normal = TexNormal.Sample(Sampler, pin.texCoord).rgb;
			normal = normalize(normal * 2.0f - 1.0f);
			normal = normalize(mul(pin.TBN, normal).xyz);
		}
		else if (HasTextureMap(material.textureBitField, TEX_SLOT_HEIGHT))
		{
			//This bump mapping implementation was taken from https://www.3dgep.com/forward-plus/
			float height = TexNormal.Sample(Sampler, pin.texCoord).r * -2.0f;				//Multiplied by a scaling factor which for now is constant
			float heightU = TexNormal.Sample(Sampler, pin.texCoord, int2(1, 0)).r * -2.0f;
			float heightV = TexNormal.Sample(Sampler, pin.texCoord, int2(0, 1)).r * -2.0f;
			
			float3 p = { 0, 0, height };
			float3 pU = { 1, 0, heightU };
			float3 pV = { 0, 1, heightV };

			// normal = tangent x bitangent
			normal = cross(normalize(pU - p), normalize(pV - p));
			normal = normalize(mul(pin.TBN, normal).xyz);
		}
		float3 toEye = normalize(eyePos - pin.posW);
	
		//Start with a sum of zero.
		float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
	
		//Sum the light contribution from each light source.
		float4 Ad, Dd, Sd;
		float4 Ap, Dp, Sp;
		
		float4 texDiffuse = {0.0f, 0.0f, 0.0f, 0.0f};
		float4 texSpecular = { 0.0f, 0.0f, 0.0f, 0.0f };


		GPUMaterial mat = material;

		if (HasTextureMap(material.textureBitField, TEX_SLOT_DIFFUSE))
		{
			texDiffuse = TexDiffuse.Sample(Sampler, pin.texCoord);
			if (texDiffuse.w < 0.5f)
				discard;
			//mat.ambient *= texDiffuse;
		 	mat.diffuse *= texDiffuse;
		}
		if (HasTextureMap(material.textureBitField, TEX_SLOT_SPECULAR))
		{
			texSpecular = TexSpecular.Sample(Sampler, pin.texCoord);
			mat.specular.xyz *= texSpecular.xyz;
		} 	
	
		CalculateDirectionalLight(mat, dirLight, normal, toEye, Ad, Dd, Sd);

		[unroll]
		for (int i = 0; i < MAX_POINT_LIGHTS; ++i)
		{
			if (!pointLights[i].enabled) continue;

			CalculatePointLight(mat, pointLights[i], pin.posW, normal, toEye, Ap, Dp, Sp);

			Ad += Ap;
			Dd += Dp;
			Sd += Sp;
		}

		float4 litColor = Ad + Dd + Sd;
	
		//Common to take alpha from diffuse materail.
		litColor.a = mat.diffuse.w;
	
		return litColor;
}