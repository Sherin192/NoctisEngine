#include "Core_pch.h"
#include "ConstantBufferData.h"
#include "DX11Utils.h"
namespace noctis::rdr
{
	using namespace math;
//------------------------------------------------------------------------------------
//		CBModelData Constructor: 
//------------------------------------------------------------------------------------
	CBModelData::CBModelData(Nmat4 w, Nmat4 v, Nmat4 p)
	{
		world = w;
		worldView = v * w;
		worldViewProj = p * v * w;
		worldInvTranspose = math::inverseTranspose(w);
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
	}

	//====================================================================================
}	//noctis::rdr