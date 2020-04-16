#include "LightHelpers.hlsl"


// References upon which this is based:
// - Unreal Engine 4 PBR notes (https://blog.selfshadow.com/publications/s2013-shading-course/karis/s2013_pbs_epic_notes_v2.pdf)
// - Frostbite's SIGGRAPH 2014 paper (https://seblagarde.wordpress.com/2015/07/14/siggraph-2014-moving-frostbite-to-physically-based-rendering/)
// - Michal Siejak's PBR project (https://github.com/Nadrin)
// - Physically Based Rendering in Filament (https://google.github.io/filament/Filament.md.html#listing_glslbrdf)

#define MAX_POINT_LIGHTS 8

cbuffer ConstantBufferPerFrame : register(b1)
{
    DirectionalLight dirLight; // 32 bytes
	//----------------------------------
	PointLight pointLights[MAX_POINT_LIGHTS]; // 48 * 8 = 384 bytes
	//----------------------------------
//	SpotLight spotLight;
	//----------------------------------
    float3 eyePos; // 12 bytes
    float ambient; // 4  bytes
	//----------------------------------
};												//432 bytes total

cbuffer ConstantBufferPerObject : register(b0)
{
    float4x4 world; // 64 bytes
	//----------------------------------
    float4x4 worldInvTranspose; // 64 bytes
	//----------------------------------
    float4x4 worldViewProj; // 64 bytes
	//----------------------------------
    };												//272 bytes total

cbuffer ConstantBufferMaterial : register(b2)
{
	PBRMaterialData material; // 32 bytes
	//----------------------------------
};

Texture2D TexAlbedo		: register(t0);
Texture2D TexMetallic	: register(t1);
Texture2D TexNormal		: register(t2); //TODO add normal mapping.
Texture2D TexRoughness	: register(t3);

sampler Sampler			: register(s0);


struct ps_Input
{
    float4 posH : SV_Position;
    float3 posW : POSITION;
    float3 normalW : NORMAL;
    float2 texCoord : TEXCOORD;
    float3x3 TBN : TBN;
};

static const float Epsilon = 0.00001;

// GGX/Towbridge-Reitz normal distribution function.
float D_GGX(float cosLh, float roughness)
{
	float alpha = roughness * roughness;
	float alphaSq = alpha * alpha;

	float denom = (cosLh * cosLh) * (alphaSq - 1.0) + 1.0;
	return alphaSq / (PI * denom * denom);
}

// Single term for separable Schlick-GGX below.
float gaSchlickG1(float cosTheta, float k)
{
	return cosTheta / (cosTheta * (1.0 - k) + k);
}

// Schlick-GGX approximation of geometric attenuation function using Smith's method.
float gaSchlickGGX(float cosLi, float cosLo, float roughness)
{
	float r = roughness + 1.0;
	float k = (r * r) / 8.0; // Epic suggests using this roughness remapping for analytic lights.
	return gaSchlickG1(cosLi, k) * gaSchlickG1(cosLo, k);
}

// Shlick's approximation of the Fresnel factor.
float3 fresnelSchlick(float3 F0, float cosTheta)
{
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}


float4 PS(ps_Input pin) : SV_TARGET
{
	//Interpolating normal can unnormalize it, so normalize it.
	float3 normal = normalize(pin.normalW);

	if (HasTextureMap(material.textureBitField, TEX_SLOT_NORMAL))
	{
		normal = TexNormal.Sample(Sampler, pin.texCoord).rgb;
		normal = normalize(normal * 2.0f - 1.0f);
		//mul(x, y) if x is a vector it is treated as row-major, if y is a vector is treated as column-major.
		//Since TBN was creaded with T, B and N as rows it is row-major hence the below multiplication ca be done this way as well:
		//mul(transpose(pin.TBN), normal) 
		normal = normalize(mul(normal, pin.TBN).xyz);
	}

	float ao = 1.0f;
	float4 albedo = material.albedo;
	if (HasTextureMap(material.textureBitField, TEX_SLOT_ALBEDO))
	{
		albedo = TexAlbedo.Sample(Sampler, pin.texCoord);
		if (albedo.w < 0.5f)
			discard;
	}
	float metalness = material.metallic;
	bool hasMetallic = HasTextureMap(material.textureBitField, TEX_SLOT_METALLIC);
	if (hasMetallic)
	{
		metalness = TexMetallic.Sample(Sampler, pin.texCoord).x;
	}
	float roughness = material.roughness;
	if (HasTextureMap(material.textureBitField, TEX_SLOT_ROUGHNESS))
	{
		roughness = TexRoughness.Sample(Sampler, pin.texCoord).x;
	}
	else if (hasMetallic)
	{
		roughness = TexMetallic.Sample(Sampler, pin.texCoord).y;
	}
	
	float3 N = normal;
	float3 L = -normalize(dirLight.direction);
	float3 V = normalize(eyePos - pin.posW);
	float3 H = normalize(L + V);
	
	float NdV = max(0.0f, dot(N, V));
	float NdL = max(0.0f, dot(N, L));
	float NdH = max(0.0f, dot(N, H));
	float LdH = max(0.0f, dot(L, H));
	
	
	float3 F0 = { 0.04f, 0.04f, 0.04f };
	F0 = lerp(F0, albedo.rgb, metalness);
	
	float D = D_GGX(NdH, roughness);
	float G = gaSchlickGGX(NdL, NdV, roughness);
	float3 F = fresnelSchlick(F0, LdH);
	
	// Specular reflection vector.
	float3 Lr = 2.0 * NdV * N - V;
	float3 Lr1 = reflect(V, N);
	
	
	// Direct lighting calculation for analytical lights.
	float3 directLighting = 0.0f;

	float3 Lradiance = dirLight.color;
	
	// Diffuse scattering happens due to light being refracted multiple times by a dielectric medium.
	// Metals on the other hand either reflect or absorb energy, so diffuse contribution is always zero.
	// To be energy conserving we must scale diffuse BRDF contribution based on Fresnel factor & metalness.
	float3 kd = lerp(float3(1.0f, 1.0f, 1.0f) - F, float3(0.0f, 0.0f, 0.0f), metalness);

	// Lambert diffuse BRDF.
	// We don't scale by 1/PI for lighting & material units to be more convenient.
	// See: https://seblagarde.wordpress.com/2012/01/08/pi-or-not-to-pi-in-game-lighting-equation/
	float3 diffuseBRDF = kd * albedo.rgb;

	// Cook-Torrance specular microfacet BRDF.
	float3 specularBRDF = (F * D * G) / max(Epsilon, 4.0 * NdL * NdV);

	// Total contribution for this light.
	directLighting += (diffuseBRDF + specularBRDF)* Lradiance* NdL;


	[unroll]
	for (int i = 0; i < MAX_POINT_LIGHTS; ++i)
	{
		if (!pointLights[i].enabled)
			continue;

		float3 L = pointLights[i].position - pin.posW;
		float d = length(L);
		L = normalize(L);
		H = normalize(L + V);
	
		NdL = max(0.0f, dot(N, L));
		NdH = max(0.0f, dot(N, H));
		LdH = max(0.0f, dot(L, H));
	
		float3 Lradiance = pointLights[i].color;
		float att = 1.0f / dot(pointLights[i].attenuation, float3(1.0f, d, d * d));
		Lradiance *= att;

	
		float3 F0 = { 0.04f, 0.04f, 0.04f };
		F0 = lerp(F0, albedo.rgb, metalness);
	
		float D = D_GGX(NdH, roughness);
		float G = gaSchlickGGX(NdL, NdV, roughness);
		float3 F = fresnelSchlick(F0, LdH);
		
		// Diffuse scattering happens due to light being refracted multiple times by a dielectric medium.
		// Metals on the other hand either reflect or absorb energy, so diffuse contribution is always zero.
		// To be energy conserving we must scale diffuse BRDF contribution based on Fresnel factor & metalness.
		float3 kd = lerp(float3(1.0f, 1.0f, 1.0f) - F, float3(0.0f, 0.0f, 0.0f), metalness);

		// Lambert diffuse BRDF.
		// We don't scale by 1/PI for lighting & material units to be more convenient.
		// See: https://seblagarde.wordpress.com/2012/01/08/pi-or-not-to-pi-in-game-lighting-equation/
		float3 diffuseBRDF = kd * albedo.rgb;

		// Cook-Torrance specular microfacet BRDF.
		float3 specularBRDF = (F * D * G) / max(Epsilon, 4.0 * NdL * NdV);

		// Total contribution for this light.
		directLighting += (diffuseBRDF + specularBRDF) * Lradiance * NdL;
	}

	float3 globalAmbient = pow(ambient, 2.2f) * albedo;

	float3 color = globalAmbient + directLighting;

	return float4(color, 1.0f);
}