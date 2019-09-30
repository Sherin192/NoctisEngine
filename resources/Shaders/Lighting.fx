struct GPUMaterial
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float4 reflect;
};

struct DirectionalLight
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float3 direction;
	float pad;
};

struct PointLight
{
	float4 ambient;
	float4 diffuse;
	float4 specular;

	float3 position;
	float range;

	float3 attenuation;
	float pad;
};

struct SpotLight
{

	float4 ambient;
	float4 diffuse;
	float4 specular;

	float3 position;
	float range;

	float3 direction;
	float spot;

	float3 attenuation;
	float pad;
};

Texture2D Texture : register(t0);
sampler Sampler : register(s0);

void CalculateDirectionalLight(Material mat, DirectionalLight light, float3 normal, float3 eye, out float4 ambient, out float4 diffuse, out float4 specular)
{
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float3 lightVec = -light.direction;

	ambient = mat.ambient * light.ambient;

	float diffuseFactor = dot(lightVec, normal);

	if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, eye), 0.0f), mat.specular.w);

		diffuse = diffuseFactor * mat.diffuse * light.diffuse;
		specular = specFactor * mat.specular * light.specular;
	}
}
cbuffer ConstantBufferPerFrame : register(b1)
{
	DirectionalLight dirLight;
	PointLight pointLight;
	SpotLight spotLight;
	float3 eyePos;
};

cbuffer ConstantBufferPerObject : register(b0)
{
	float4x4 world;
	float4x4 worldInvTranspose;
	float4x4 worldViewProj;
	Material material;
};

struct vs_Input
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float2 texCoord : TEXCOORD0;
};

struct vs_Output
{
	float4 posH : SV_Position;
	float3 posW : POSITION;
	float3 normalW : NORMAL;
	float2 texCoord : TEXCOORD0;
};




vs_Output VS(vs_Input IN)
{
	vs_Output OUT = (vs_Output)0;
	OUT.posH = mul(IN.position, worldViewProj);
	OUT.texCoord = IN.texCoord;
	return OUT;
}

float4 PS(vs_Output IN) : SV_TARGET
{
	return float4(1.0f, 0.0f, 0.0f, 1.0f);
}

//vs_Output VS(vs_Input vin)
//{
//	vs_Output vout;
//	//Transform to world space.
//	vout.posW = mul(float4(vin.position, 1.0f), world).xyz;
//	vout.normalW = mul(vin.normal, (float3x3)worldInvTranspose);
//
//	//Transform to homogeneous clip space.
//	vout.posH = mul(float4(vin.position, 1.0f), worldViewProj);
//
//	return vout;
//}
//
//float4 PS(vs_Output pin) : SV_TARGET
//{
//	//Interpolating normal can unnormalize it, so normalize it.
//	pin.normalW = normalize(pin.normalW);
//
//	float3 toEye = normalize(eyePos - pin.posW);
//
//	//Start with a sum of zero.
//	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
//	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
//	float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
//
//	//Sum the light contribution from each light source.
//	float4 A, D, S;
//
//	CalculateDirectionalLight(material, dirLight, pin.normalW, toEye, A, D, S);
//
//	ambient += A;
//	diffuse += D;
//	spec += S;
//
//	float4 litColor = ambient + diffuse + spec;
//
//	//Common to take alpha from duffuse materail.
//	litColor.a = material.diffuse.a;
//
//	litColor = float4(1.0f, 0.0f, 0.0f, 1.0f);
//	return litColor;
//}
