#include "Dx11Texture.h"

namespace noctis::rdr
{

	auto NoctisToDx11Format(Format format)
	{
		switch (format)
		{
		case kRGBA32F:		return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case kRGBA16F:		return DXGI_FORMAT_R16G16B16A16_FLOAT;
		case kRGBA8UN:		return DXGI_FORMAT_R8G8B8A8_UNORM;
		case kRGBA8UN_SRGB:	return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		default:			return DXGI_FORMAT_R8G8B8A8_UNORM;
		};
	}
//------------------------------------------------------------------------------------
//		Dx11Texture Constructor:
//------------------------------------------------------------------------------------
	Dx11Texture::Dx11Texture(std::shared_ptr<RenderDevice>& renderDevice, void* data, unsigned int width, unsigned int height, unsigned int nrChannels, TextureUsage type, Format format /*Format::kRGBA8UN*/)
	{
		if (type != TextureUsage::CUBE_MAP)
		{
			//TODO: This has to be passed to the constructor.
			D3D11_TEXTURE2D_DESC texDesc;
			texDesc.Width = width;
			texDesc.Height = height;
			texDesc.MipLevels = 0;					//Use 1 for a multisampled texture; or 0 to generate a full set of subtextures.
			texDesc.ArraySize = 1;
			texDesc.Format = NoctisToDx11Format(format);
			texDesc.SampleDesc.Count = 1;
			texDesc.SampleDesc.Quality = 0u; ;// D3D11_STANDARD_MULTISAMPLE_PATTERN;
			texDesc.Usage = D3D11_USAGE_DEFAULT;
			texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
			texDesc.CPUAccessFlags = 0;
			texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

			HRESULT hResult = renderDevice->GetDevice()->CreateTexture2D(&texDesc, NULL, m_pTexture.GetAddressOf());
			if (FAILED(hResult))
			{
				Log(LogLevel::Warning, "Failed to create a 2D texture.");
			}
			if (data)
				renderDevice->GetDeviceContext()->UpdateSubresource(m_pTexture.Get(), 0, NULL, data, width * nrChannels, 0);

			D3D11_SHADER_RESOURCE_VIEW_DESC desc;
			desc.Format = texDesc.Format;
			desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipLevels = -1;
			desc.Texture2D.MostDetailedMip = 0;

			D3D11_RENDER_TARGET_VIEW_DESC RTViewDesc;
			RTViewDesc.Format = texDesc.Format;
			RTViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			RTViewDesc.Texture2D.MipSlice = 0;

			hResult = renderDevice->GetDevice()->CreateRenderTargetView(m_pTexture.Get(), &RTViewDesc, m_pTextureRTV.GetAddressOf());

			hResult = renderDevice->GetDevice()->CreateShaderResourceView(m_pTexture.Get(), &desc, m_pTextureSRV.GetAddressOf());
			renderDevice->GetDeviceContext()->GenerateMips(m_pTextureSRV.Get());
			if (FAILED(hResult))
			{
				Log(LogLevel::Error, "Failed to generate mips.");
			}

			m_pSampler = std::make_unique<Sampler>(renderDevice, SamplerType<FilterLinear, AddressUWrap, AddressVWrap, AddressWClamp, CompareNever>{});
		}
		else
		{
			D3D11_TEXTURE2D_DESC texDesc;
			texDesc.Width = width;
			texDesc.Height = height;
			texDesc.MipLevels = 1;					//Use 1 for a multisampled texture; or 0 to generate a full set of subtextures.
			texDesc.ArraySize = 6;
			texDesc.Format = NoctisToDx11Format(format);
			texDesc.SampleDesc.Count = 1;
			texDesc.SampleDesc.Quality = 0;
			texDesc.Usage = D3D11_USAGE_DEFAULT;
			texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			texDesc.CPUAccessFlags = 0;
			texDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

			D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc;
			SMViewDesc.Format = texDesc.Format;
			SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
			SMViewDesc.TextureCube.MipLevels = texDesc.MipLevels;
			SMViewDesc.TextureCube.MostDetailedMip = 0;

			D3D11_SUBRESOURCE_DATA pData[6];
			auto d = reinterpret_cast<unsigned char**>(data);
			for (int cubeMapFaceIndex = 0; cubeMapFaceIndex < 6; cubeMapFaceIndex++)
			{
				pData[cubeMapFaceIndex].pSysMem = d[cubeMapFaceIndex];
				pData[cubeMapFaceIndex].SysMemPitch = width * 4;
				pData[cubeMapFaceIndex].SysMemSlicePitch = 0;
			}

			HRESULT hr = renderDevice->GetDevice()->CreateTexture2D(&texDesc, &pData[0], m_pTexture.GetAddressOf());
			assert(hr == S_OK);

			hr = renderDevice->GetDevice()->CreateShaderResourceView(m_pTexture.Get(), &SMViewDesc, m_pTextureSRV.GetAddressOf());
			assert(hr == S_OK);
			
			m_pSampler = std::make_unique<Sampler>(renderDevice, SamplerType<FilterLinear, AddressUWrap, AddressVWrap, AddressWClamp, CompareNever>{});
		}


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
//		GetSRV: Returns shader resource view.
//------------------------------------------------------------------------------------
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> Dx11Texture::GetRTV()	 const	noexcept
	{
		return m_pTextureRTV;
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