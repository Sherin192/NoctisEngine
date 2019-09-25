#ifndef CONSTANT_BUFFER_DATA_H
#define CONSTANT_BUFFER_DATA_H

#include "Material.h"
#include "Light.h"

namespace noctis::rdr
{
//------------------------------------------------------------------------------------
//		Forward Declarations:
//------------------------------------------------------------------------------------
	class Material;
	struct Light;
	struct GPUMaterial;
//====================================================================================




//------------------------------------------------------------------------------------
//		CBModelData: Holds the data passed to the GPU for each model
//------------------------------------------------------------------------------------
	struct CBModelData
	{
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4X4 worldInvTranspose;
		DirectX::XMFLOAT4X4 worldViewProj;
		GPUMaterial material;
	
		CBModelData() = default;
		CBModelData(DirectX::FXMMATRIX w, DirectX::CXMMATRIX v, DirectX::XMMATRIX p, Material& mat);
		CBModelData(DirectX::FXMMATRIX w, DirectX::CXMMATRIX wvp);
	
	};

//====================================================================================





//------------------------------------------------------------------------------------
//		CBFrameData: Holds the data passed to the GPU once per frame
//------------------------------------------------------------------------------------
	struct CBFrameData
	{
		DirectionalLight dirLight;
		PointLight pointLights[8];
		DirectX::SimpleMath::Vector3 eyePos;
		float pad;
	};

//====================================================================================
}	//noctis::rdr
#endif //CONSTANT_BUFFER_DATA_H