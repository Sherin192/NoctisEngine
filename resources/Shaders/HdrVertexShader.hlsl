#include "LightHelpers.hlsl"


struct VSIn
{
	uint vertexID : SV_VertexID;
};

struct vs_Output
{
	float4 position : SV_Position;
	float2 texCoord : TEXCOORD;
};


//================================================================================
//Vertex Shader
//================================================================================

vs_Output VS(VSIn input)
{
	vs_Output vout;

	if (input.vertexID == 2) {
		vout.texCoord = float2(1.0, -1.0);
		vout.position = float4(1.0, 3.0, 0.0, 1.0);
	}
	else if (input.vertexID == 1) {
		vout.texCoord = float2(-1.0, 1.0);
		vout.position = float4(-3.0, -1.0, 0.0, 1.0);
	}
	else /* if(vertexID == 0) */ {
		vout.texCoord = float2(1.0, 1.0);
		vout.position = float4(1.0, -1.0, 0.0, 1.0);
	}
	return vout;
}