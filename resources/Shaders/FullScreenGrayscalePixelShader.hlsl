#include "LightHelpers.hlsl"

struct PixelShaderInput
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
};

Texture2D SceneTexture : register(t9);
sampler Sampler : register(s0);

struct ps_Input
{
	float4 position : SV_Position;
	float2 texCoord : TEXCOORD;
};

float4 PS(ps_Input pin) : SV_TARGET
{
	float3 color = SceneTexture.Sample(Sampler, pin.texCoord).rgb;
	float grayscale = (color.r + color.g + color.b) / 3.0f;
	return float4(grayscale, grayscale, grayscale, 1.0f);
}