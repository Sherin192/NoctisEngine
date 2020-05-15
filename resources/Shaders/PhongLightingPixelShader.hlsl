#include "LightHelpers.hlsl"

cbuffer ConstantBufferMaterial : register(b2)
{
	GPUMaterial material;						// 80 bytes
	//----------------------------------
}

Texture2D TexDiffuse : register(t0);
Texture2D TexSpecular : register(t1);
Texture2D TexNormal : register(t2);
Texture2D TexOpacity : register(t4);
Texture2D TexEmissive : register(t5);
SamplerState Sampler : register(s3);
//sampler SamplerLinearWrap : register(s3);

struct ps_Input
{
	float4 posH : SV_Position;
	float3 posW : POSITION;
	float2 texCoord : TEXCOORD;
	float3 T : TANGENT;
	float3 B : BITANGENT;
	float3 N : NORMAL;
};

float4 PS(ps_Input pin) : SV_TARGET
{
	//Interpolating normal can unnormalize it, so normalize it.
	float3 normal = normalize(pin.N);

	if (enabledNormalMapping && HasTextureMap(material.textureBitField, TEX_SLOT_NORMAL))
	{
		normal = TexNormal.Sample(Sampler, pin.texCoord).rgb;
		normal = normal * 2.0f - 1.0f;
		//mul(x, y) if x is a vector it is treated as row-major, if y is a vector is treated as column-major.
		//Since TBN was creaded with T, B and N as rows it is row-major hence the below multiplication ca be done this way as well:
		//mul(transpose(pin.TBN), normal) 
		float3x3 TBN = float3x3(normalize(pin.T), normalize(pin.B), normalize(pin.N));
		normal = normalize(mul(normal, TBN).xyz);
	}
	
	if (HasTextureMap(material.textureBitField, TEX_SLOT_HEIGHT))
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
		float3x3 TBN = float3x3(pin.T, pin.B, pin.N);
		normal = normalize(mul(normal, TBN).xyz);
	}
	
	float3 toEye = normalize(eyePos - pin.posW);
	
	//One globac ambient light
	GPUMaterial mat = material;

	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 emissive = float4(0.0f, 0.0f, 0.0f, 0.0f);
	//Sum the light contribution from each light source.
	float4 Dp, Sp;
	
	float4 texDiffuse = {0.0f, 0.0f, 0.0f, 0.0f};
	float4 texSpecular = {0.0f, 0.0f, 0.0f, 0.0f};

	if (HasTextureMap(material.textureBitField, TEX_SLOT_DIFFUSE))
	{
		texDiffuse = TexDiffuse.Sample(Sampler, pin.texCoord);
		if (texDiffuse.w < 0.5f)
			discard;
		mat.diffuse = texDiffuse;
	}
	float4 globalAmbient = pow(ambient, 2.2f) * mat.diffuse;
	
	if (HasTextureMap(material.textureBitField, TEX_SLOT_SPECULAR))
	{
		texSpecular = TexSpecular.Sample(Sampler, pin.texCoord);
		mat.specular.xyz = texSpecular.xyz;
	} 

	if (HasTextureMap(material.textureBitField, TEX_SLOT_EMISSIVE))
	{
		emissive = TexEmissive.Sample(Sampler, pin.texCoord);
	}
	if (dirLight.enabled)
		CalculateDirectionalLight(mat, dirLight, normal, toEye, diffuse, spec);

	[unroll]
	for (int i = 0; i < MAX_POINT_LIGHTS; ++i)
	{
		if (!pointLights[i].enabled) continue;

		CalculatePointLight(mat, pointLights[i], pin.posW, normal, toEye, Dp, Sp);

		diffuse += Dp;
		spec += Sp;
	}
	float4 litColor = emissive + globalAmbient + diffuse * mat.diffuse + spec * mat.specular;
	
	//Common to take alpha from diffuse materail.
	litColor.a = mat.diffuse.w;
	
	if (!HasTextureMap(material.textureBitField, TEX_SLOT_DIFFUSE))
		return pow(litColor, 2.2f);
	else 
		return litColor;
}