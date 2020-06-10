//Predefined texture slots for a particular use.
#define TEX_SLOT_DIFFUSE 	0
#define TEX_SLOT_ALBEDO		0
#define TEX_SLOT_SPECULAR 	1
#define TEX_SLOT_METALLIC	1
#define TEX_SLOT_NORMAL 	2
#define TEX_SLOT_HEIGHT 	3
#define TEX_SLOT_ROUGHNESS	3
#define TEX_SLOT_OPACITY 	4
#define TEX_SLOT_EMISSIVE 	5

#define MAX_POINT_LIGHTS 50


cbuffer ConstantBufferPerObject : register(b0)
{
	float4x4 world; // 64 bytes
	//----------------------------------
	float4x4 worldView; // 64 bytes
	//----------------------------------
	float4x4 worldInvTranspose; // 64 bytes
	//----------------------------------
	float4x4 worldViewProj; // 64 bytes
	//----------------------------------
};												//272 bytes total


static const float PI = 3.141592;

struct GPUMaterial
{
	float4 diffuse;								// 16 bytes
	//-----------------------------------
	float4 specular;							// 16 bytes
	//-----------------------------------
	float4 reflect;								// 16 bytes
	//-----------------------------------
	int textureBitField;						// 4  bytes
	bool blinn;
	float2 pad;									// 12 bytes
};												// 64 bytes total

struct PBRMaterialData
{
    float4 albedo;								// 16 bytes
	//-----------------------------------
    float metallic;								// 4  bytes
    float roughness;							// 4  bytes
    float ao;									// 4  bytes
    int textureBitField;						// 4  bytes
};												// 32 bytes total



struct DirectionalLight
{
	float4 color;								// 16 bytes
	//-----------------------------------
    float3 direction;							// 12 bytes
	bool enabled;								// 4  bytes
	//----------------------------------
};												//32 bytes total

struct PointLight
{
	float4 color;								// 16 bytes
	//----------------------------------
	float3 position;							// 12 bytes
	float pad;									// 4  bytes
	//----------------------------------
	float3 attenuation;							// 12 bytes
	bool enabled;								// 4  bytes
	//----------------------------------
};												//48 bytes total

struct SpotLight
{
	float4 diffuse;								// 16 bytes
	//----------------------------------
	float4 specular;							// 16 bytes
	//----------------------------------
	float3 position;							// 12 bytes
	float range;								// 4  bytes
	//----------------------------------
	float3 direction;							// 12 bytes
	float spot;									// 4  bytes
	//----------------------------------
	float3 attenuation;							// 12 bytes
	float pad;									// 4  bytes
	//----------------------------------
};												//96 bytes total


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
	float gammaCorrection; // 4 bytes
	bool enabledNormalMapping; // 4  bytes
	int2 pad; // 12 bytes
};		

//--------------------------------------------------------------------------------------
//Helper function for texture bitfield tests
//--------------------------------------------------------------------------------------
inline bool HasTextureMap(int mask, int texture_type) { return (mask & (1 << texture_type)) ? true : false;}
//======================================================================================




float Phong(float3 light, float3 view, float3 normal)
{
    float3 r = reflect(light, normal);
    return max(dot(r, view), 0.0f);
}

float BlinnPhong(float3 light, float3 view, float3 normal)
{
    float3 half_vec = normalize(light + view);
    return max(dot(normal, half_vec), 0.0f);
}




void CalculateDirectionalLight(GPUMaterial mat, DirectionalLight light, float3 normal, float3 eye, out float4 diffuse, out float4 specular, bool blinn = false)
{
	float3 L = normalize(-light.direction);
	float NdL = max(0.0f, dot(L, normal));
	diffuse = NdL * light.color;
	if (blinn)
		specular = light.color * pow(BlinnPhong(L, eye, normal), mat.specular.w);
	else
		specular = light.color * pow(Phong(-L, eye, normal), mat.specular.w);
}



void CalculatePointLight(GPUMaterial mat, PointLight light, float3 pos, float3 normal, float3 toEye,
	out float4 diffuse, out float4 spec, bool blinn = false)
{
	// Initialize outputs.
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The vector from the surface to the light.
	float3 lightVec = light.position - pos;

	// The distance from surface to light.
	float d = length(lightVec);
	
	// Normalize the light vector.
	lightVec /= d;
 
	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.

	float diffuseFactor = max(0.0f, dot(normal, lightVec));

	float specFactor = blinn ? pow(BlinnPhong(lightVec, toEye, normal), mat.specular.w) : pow(Phong(-lightVec, toEye, normal), mat.specular.w);
	diffuse = diffuseFactor * light.color;
	spec = specFactor * light.color;

	float att = 1.0f / dot(light.attenuation, float3(1.0f, d, d*d));

	diffuse *= att;
	spec.xyz *= att;
}