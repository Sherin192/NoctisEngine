#include "Core_pch.h"
#include "ConstantBufferData.h"
#include "DX11Utils.h"
namespace noctis::rdr
{
	using namespace math;
//------------------------------------------------------------------------------------
//		CBModelData Constructor: 
//------------------------------------------------------------------------------------
	CBModelData::CBModelData(Nmat4 w, Nmat4 v, Nmat4 p, Material& mat)
	{
		world = w;
		worldViewProj = w * v * p;
		worldInvTranspose = math::inverseTranspose(w);
		memset(&material, 0, sizeof(GPUMaterial));
		material = mat.GetGPUMaterial();
	}

	//====================================================================================





	//------------------------------------------------------------------------------------
	//		CBModelData Constructor: 
	//------------------------------------------------------------------------------------
	CBModelData::CBModelData(Nmat4 w, Nmat4 wvp)
	{
		world = w;
		worldViewProj = wvp;
		worldInvTranspose = math::inverseTranspose(w);
		memset(&material, 0, sizeof(GPUMaterial));
	}

	//====================================================================================
}	//noctis::rdr