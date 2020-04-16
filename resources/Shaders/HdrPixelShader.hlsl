#include "LightHelpers.hlsl"


// Tone-mapping & gamma correction.

static const float gamma = 2.2;
static const float exposure = 1.0;
static const float pureWhite = 1.0;

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
	float3 color = SceneTexture.Sample(Sampler, pin.texCoord).rgb * exposure;

	// Reinhard tonemapping operator.
	// see: "Photographic Tone Reproduction for Digital Images", eq. 4
	//float luminance = dot(color, float3(0.2126, 0.7152, 0.0722));
	//float mappedLuminance = (luminance * (1.0 + luminance / (pureWhite * pureWhite))) / (1.0 + luminance);

	// reinhard tone mapping
	float3 mappedColor = color / (color + 1.0f);

	// Scale color by ratio of average luminances.
	//float3 mappedColor = (mappedLuminance / luminance) * color;

	// Gamma correction.
	return float4(pow(color, 1.0f / gamma), 1.0f);
}