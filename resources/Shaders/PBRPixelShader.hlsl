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

float3 fresnelSchlick(float cosTheta, float3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0f);
}


float DistributionGGX(float3 N, float3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
	
    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom; // prevent divide by zero for roughness=0.0 and NdotH=1.0
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0f);
    float k = (r * r) / 8.0f;

    float num = NdotV;
    float denom = NdotV * (1.0f - k) + k;
	
    return num / denom;
}
float GeometrySmith(float3 N, float3 V, float3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0f);
    float NdotL = max(dot(N, L), 0.0f);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

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
	float3 albedo = material.albedo.xyz;
	/*if (HasTextureMap(material.textureBitField, TEX_SLOT_DIFFUSE))
	{
		albedo = TexDiffuse.Sample(Sampler, pin.texCoord);
		if (albedo.w < 0.5f)
			discard;
	}*/
	float metalness = material.metallic;
	//if (HasTextureMap(material.textureBitField, TEX_SLOT_NORMAL))
	//{
	//	metalness = TexNormal.Sample(Sampler, pin.texCoord).x;
	//	//if (metalness.w < 0.5f)
	//	//	discard;
	//}
	float roughness = material.roughness;
	//if (HasTextureMap(material.textureBitField, TEX_SLOT_SPECULAR))
	//{
	//	roughness = TexSpecular.Sample(Sampler, pin.texCoord).x;
	//	//if (metalness.w < 0.5f)
	//	//	discard;
	//}
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
	//[unroll]
	//for (int i = 0; i < MAX_POINT_LIGHTS; ++i)
	//{
	/*	if (!pointLights[i].enabled)
			continue;*/

		float3 Li = -normalize(dirLight.direction);

	// Half-vector between Li and Lo.
		float3 Lh = normalize(Li + Lo);

	// Calculate angles between surface normal and various light vectors.
		float cosLi = max(0.0, dot(N, Li));
		float cosLh = max(0.0, dot(N, Lh));
	
		float3 Lradiance = dirLight.specular;
	
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
//}

	float3 ambient = float3(0.03, 0.03, 0.03) * albedo;

	float3 color = ambient + directLighting;

	// HDR tonemapping
	color = color / (color + float3(1.0, 1.0, 1.0));
	color = pow(color, float3(1.0 / 2.2, 1.0 / 2.2, 1.0 / 2.2));

	return float4(color, 1.0f);
	////Interpolating normal can unnormalize it, so normalize it.
 //   float3 N = normalize(pin.normalW);
 //   float3 V = normalize(eyePos - pin.posW);
	
 //   float3 F0 = { 0.04, 0.04, 0.04 };
 //   F0 = lerp(F0, material.albedo.xyz, material.metallic);

 //   float3 Lo = { 0.0f, 0.0f ,0.0f };
	//[unroll]
 //   for (int i = 0; i < MAX_POINT_LIGHTS; ++i)
 //   {
 //       if (!pointLights[i].enabled)
 //           continue;


	//	// calculate per-light radiance
 //       float3 L = pointLights[i].position - pin.posW;
 //       float3 H = normalize(V + L);
 //       float distance = length(pointLights[i].position - pin.posW);
 //       L = L / distance;
 //       float attenuation = 1.0 / (distance * distance);
 //       float3 radiance = pointLights[i].diffuse.xyz * attenuation;
 //       // cook-torrance brdf
 //       float NDF = DistributionGGX(N, H, material.roughness);
 //       float G = GeometrySmith(N, V, L, material.roughness);
 //       float3 F = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);
        
 //       float3 kS = F;
 //       float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
 //       kD *= 1.0f - material.metallic;
        
 //       float3 numerator = NDF * G * F;
 //       float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
 //       float3 specular = numerator / max(denominator, 0.0001);
            
 //       // add to outgoing radiance Lo
 //       float NdotL = max(dot(N, L), 0.0);
 //       Lo += (kD * material.albedo.xyz / PI + specular) * radiance * NdotL;
 //   }
 //   float3 ambient = float3(0.03f, 0.03f, 0.03f) * material.albedo.xyz * material.ao;
 //   float3 color = ambient + Lo;
	
 //   //color = color / (color + float3(1.0f, 1.0f, 1.0f));
 //   //color = pow(color, float3(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f));
   
 //   return float4(color, 1.0);
}