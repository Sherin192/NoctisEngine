#include "Core_pch.h"
#include "ConstantBufferData.h"
#include "DX11Utils.h"

namespace noctis::rdr
{
//------------------------------------------------------------------------------------
//		CBModelData Constructor: 
//------------------------------------------------------------------------------------
	CBModelData::CBModelData(DirectX::FXMMATRIX w, DirectX::CXMMATRIX v, DirectX::XMMATRIX p, Material& mat)
	{
		DirectX::XMStoreFloat4x4(&world, w);
		DirectX::XMStoreFloat4x4(&worldViewProj, DirectX::XMMatrixTranspose(XMMatrixMultiply(w, XMMatrixMultiply(v, p))));
		DirectX::XMStoreFloat4x4(&worldInvTranspose, InverseTranspose(w));
	
		memset(&material, 0, sizeof(GPUMaterial));
		material = mat.GetGPUMaterial();
	}

//====================================================================================





//------------------------------------------------------------------------------------
//		CBModelData Constructor: 
//------------------------------------------------------------------------------------
	CBModelData::CBModelData(DirectX::FXMMATRIX w, DirectX::CXMMATRIX wvp)
	{
		DirectX::XMStoreFloat4x4(&world, w);
		DirectX::XMStoreFloat4x4(&worldViewProj, DirectX::XMMatrixTranspose(wvp));
		DirectX::XMStoreFloat4x4(&worldInvTranspose, InverseTranspose(w));
		memset(&material, 0, sizeof(GPUMaterial));
	}

//====================================================================================
}	//noctis::rdr