#ifndef CONSTANT_BUFFER_DATA_H
#define CONSTANT_BUFFER_DATA_H
//#include "Material.h"
#include "NoctisRenderDevice.h"
#include "Light.h"

namespace noctis::rdr
{
//------------------------------------------------------------------------------------
//		Forward Declarations:
//------------------------------------------------------------------------------------
	//class Material;
	struct Light;
	//struct GPUMaterial;
//====================================================================================




//------------------------------------------------------------------------------------
//		CBModelData: Holds the data passed to the GPU for each model
//------------------------------------------------------------------------------------
	struct CBModelData
	{
		math::Nmat4	world;
		math::Nmat4	worldView;
		math::Nmat4 worldInvTranspose;
		math::Nmat4	worldViewProj;

		//GPUMaterial material;
	
		CBModelData() = default;
		CBModelData(math::Nmat4 w, math::Nmat4 v, math::Nmat4 p);
		CBModelData(math::Nmat4 w, math::Nmat4 wvp);
	};

//====================================================================================





//------------------------------------------------------------------------------------
//		CBFrameData: Holds the data passed to the GPU once per frame
//------------------------------------------------------------------------------------
	struct CBFrameData
	{
		DirectionalLight dirLight;
		PointLight pointLights[8];
		math::Nvec3 eyePos;
		float ambient;
		float gammaCorrection;
		int enabledNormalMapping;
		int pad[2];
	};

//====================================================================================
}	//noctis::rdr
#endif //CONSTANT_BUFFER_DATA_H