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
// Uses Disney's reparametrization of alpha = roughness^2.
float ndfGGX(float cosLh, float roughness)
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
	float4 albedo = material.albedo;
	if (HasTextureMap(material.textureBitField, TEX_SLOT_ALBEDO))
	{
		albedo = TexAlbedo.Sample(Sampler, pin.texCoord);
		if (albedo.w < 0.5f)
			discard;
	}
	float metalness = material.metallic;
	if (HasTextureMap(material.textureBitField, TEX_SLOT_METALLIC))
	{
		metalness = TexMetallic.Sample(Sampler, pin.texCoord).x;
		//if (metalness.w < 0.5f)
		//	discard;
	}
	float roughness = material.roughness;
	if (HasTextureMap(material.textureBitField, TEX_SLOT_ROUGHNESS))
	{
		roughness = TexRoughness.Sample(Sampler, pin.texCoord).x;
		//if (metalness.w < 0.5f)
		//	discard;
	}
	// Outgoing light direction (vector from world-space fragment position to the "eye").
	float3 Lo = normalize(eyePos - pin.posW);
	
	float3 N = normalize(pin.normalW);
	// Angle between surface normal and outgoing light direction.
	float cosLo = max(0.0, dot(N, Lo));
		
	// Specular reflection vector.
	float3 Lr = 2.0 * cosLo * N - Lo;
	
	float3 F0 = { 0.04f, 0.04f, 0.04f };
	F0 = lerp(F0, albedo, metalness);
	
	// Direct lighting calculation for analytical lights.
	float3 directLighting = 0.0;

		float3 Li = -normalize(dirLight.direction);

	// Half-vector between Li and Lo.
		float3 Lh = normalize(Li + Lo);

	// Calculate angles between surface normal and various light vectors.
		float cosLi = max(0.0, dot(N, Li));
		float cosLh = max(0.0, dot(N, Lh));
	
		float3 Lradiance = dirLight.diffuse;
	
		float3 F = fresnelSchlick(F0, max(0.0, dot(Lh, Lo)));
		// Calculate normal distribution for specular BRDF.
		float D = ndfGGX(cosLh, roughness);
		// Calculate geometric attenuation for specular BRDF.
		float G = gaSchlickGGX(cosLi, cosLo, roughness);

		// Diffuse scattering happens due to light being refracted multiple times by a dielectric medium.
		// Metals on the other hand either reflect or absorb energy, so diffuse contribution is always zero.
		// To be energy conserving we must scale diffuse BRDF contribution based on Fresnel factor & metalness.
		float3 kd = lerp(float3(1, 1, 1) - F, float3(0, 0, 0), metalness);

		// Lambert diffuse BRDF.
		// We don't scale by 1/PI for lighting & material units to be more convenient.
		// See: https://seblagarde.wordpress.com/2012/01/08/pi-or-not-to-pi-in-game-lighting-equation/
		float3 diffuseBRDF = kd * albedo;

		// Cook-Torrance specular microfacet BRDF.
		float3 specularBRDF = (F* D * G) / max(Epsilon, 4.0 * cosLi * cosLo);

		// Total contribution for this light.
		directLighting += (diffuseBRDF + specularBRDF)* Lradiance* cosLi;


	[unroll]
	for (int i = 0; i < MAX_POINT_LIGHTS; ++i)
	{
		if (!pointLights[i].enabled)
			continue;

		float3 lightVec = pointLights[i].position - pin.posW;
		float d = length(lightVec);
		float3 Li = normalize(lightVec);

		// Half-vector between Li and Lo.
		float3 Lh = normalize(Li + Lo);

		// Calculate angles between surface normal and various light vectors.
		float cosLi = max(0.0, dot(N, Li));
		float cosLh = max(0.0, dot(N, Lh));

		float3 Lradiance = pointLights[i].specular;
		float att = 1.0f / dot(pointLights[i].attenuation, float3(1.0f, d, d * d));
		Lradiance *= att;

		float3 F = fresnelSchlick(F0, max(0.0, dot(Lh, Lo)));
		// Calculate normal distribution for specular BRDF.
		float D = ndfGGX(cosLh, roughness);
		// Calculate geometric attenuation for specular BRDF.
		float G = gaSchlickGGX(cosLi, cosLo, roughness);

		// Diffuse scattering happens due to light being refracted multiple times by a dielectric medium.
		// Metals on the other hand either reflect or absorb energy, so diffuse contribution is always zero.
		// To be energy conserving we must scale diffuse BRDF contribution based on Fresnel factor & metalness.
		float3 kd = lerp(float3(1, 1, 1) - F, float3(0, 0, 0), metalness);

		// Lambert diffuse BRDF.
		// We don't scale by 1/PI for lighting & material units to be more convenient.
		// See: https://seblagarde.wordpress.com/2012/01/08/pi-or-not-to-pi-in-game-lighting-equation/
		float3 diffuseBRDF = kd * albedo;

		// Cook-Torrance specular microfacet BRDF.
		float3 specularBRDF = (F * D * G) / max(Epsilon, 4.0 * cosLi * cosLo);

		// Total contribution for this light.
		directLighting += (diffuseBRDF + specularBRDF) * Lradiance * cosLi;
	}

	float3 ambient = float3(0.03, 0.03, 0.03) * albedo;

	float3 color = ambient + directLighting;

	color = pow(color, float3(1.0 / 2.2, 1.0 / 2.2, 1.0 / 2.2));

	return float4(color, 1.0f);
}