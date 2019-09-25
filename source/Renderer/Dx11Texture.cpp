#include "Dx11Texture.h"

namespace noctis::rdr
{
//------------------------------------------------------------------------------------
//		Dx11Texture Constructor:
//------------------------------------------------------------------------------------
	Dx11Texture::Dx11Texture(std::shared_ptr<RenderDevice>& renderDevice, void* data, std::string path, unsigned int width, unsigned int height, unsigned int nrChannels, TextureUsage type)
	{
		//TODO: This has to be passed to the constructor.
		D3D11_TEXTURE2D_DESC texDesc;
		texDesc.Width = width;
		texDesc.Height = height;
		texDesc.MipLevels = 0;					//Use 1 for a multisampled texture; or 0 to generate a full set of subtextures.
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = D3D11_STANDARD_MULTISAMPLE_PATTERN;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		HRESULT hResult = renderDevice->GetDevice()->CreateTexture2D(&texDesc, NULL, m_pTexture.GetAddressOf());
		if (FAILED(hResult))
		{
			printf("failed to create a 2D texture.");
		}

		renderDevice->GetDeviceContext()->UpdateSubresource(m_pTexture.Get(), 0, NULL, data, width * nrChannels, 0);

		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = -1;
		desc.Texture2D.MostDetailedMip = 0;

		hResult = renderDevice->GetDevice()->CreateShaderResourceView(m_pTexture.Get(), &desc, m_pTextureSRV.GetAddressOf());
		renderDevice->GetDeviceContext()->GenerateMips(m_pTextureSRV.Get());
		if (FAILED(hResult))
		{
			printf("Failed to generate mips.");
		}

		m_pSampler = std::make_unique<Sampler>(renderDevice, SamplerType<FilterLinear, AddressUWrap, AddressVWrap, AddressWClamp, CompareNever>{});

	}

//====================================================================================





//------------------------------------------------------------------------------------
//		GetSRV: Returns shader resource view.
//------------------------------------------------------------------------------------
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Dx11Texture::GetSRV()	 const	noexcept 
	{ 
		return m_pTextureSRV; 
	}

//====================================================================================





//------------------------------------------------------------------------------------
//		GetSampler: Returns the sampler.
//------------------------------------------------------------------------------------
	std::shared_ptr<Sampler>& Dx11Texture::GetSampler() 
	{ 
		return m_pSampler; 
	}

//====================================================================================
}