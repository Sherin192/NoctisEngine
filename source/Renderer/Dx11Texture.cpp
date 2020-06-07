#include "Dx11Texture.h"

namespace noctis::rdr
{

//------------------------------------------------------------------------------------
//		NoctisToDx11Format: Converts Noctis formats into Dx11 formats.
//------------------------------------------------------------------------------------

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

//====================================================================================





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
				renderDevice->GetDeviceContext()->UpdateSubresource(m_pTexture.Get(), 0, NULL, data, width * nrChannels * (type == HDR? sizeof(float) : sizeof(unsigned char)), 0);

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
			
			D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
			uavDesc.Format = texDesc.Format;
			if (texDesc.ArraySize == 1)
			{
				uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
				uavDesc.Texture2D.MipSlice = 0;
			}
			//TODO:: Some formats are not supported for unorderred views, for now it is not needed but this will have to dealt with.
			//hResult = renderDevice->GetDevice()->CreateUnorderedAccessView(m_pTexture.Get(), &uavDesc, m_pTextureUAV.GetAddressOf());

			renderDevice->GetDeviceContext()->GenerateMips(m_pTextureSRV.Get());
			if (FAILED(hResult))
			{
				Log(LogLevel::Error, "Failed to generate mips.");
			}
			//TODO: Samplers should not be associated with each texture.
			m_pSampler = std::make_unique<Sampler>(renderDevice, SamplerType<FilterLinear, AddressWrap, CompareNever>{});
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
			texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | (data ? 0 : D3D11_BIND_UNORDERED_ACCESS); // if there was no data passes this means the cubemap needs to be filled.
			texDesc.CPUAccessFlags = 0;
			texDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

			D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc;
			SMViewDesc.Format = texDesc.Format;
			SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
			SMViewDesc.TextureCube.MipLevels = texDesc.MipLevels;
			SMViewDesc.TextureCube.MostDetailedMip = 0;
			if (data)
			{
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
			}
			else
			{
				HRESULT hr = renderDevice->GetDevice()->CreateTexture2D(&texDesc, NULL, m_pTexture.GetAddressOf());
				assert(hr == S_OK);
			}
			HRESULT hr = renderDevice->GetDevice()->CreateShaderResourceView(m_pTexture.Get(), &SMViewDesc, m_pTextureSRV.GetAddressOf());
			assert(hr == S_OK);
			
			if (!data)
			{
				D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
				uavDesc.Format = texDesc.Format;

				uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
				uavDesc.Texture2DArray.MipSlice = 0;
				uavDesc.Texture2DArray.FirstArraySlice = 0;
				uavDesc.Texture2DArray.ArraySize = texDesc.ArraySize;
			

				hr = renderDevice->GetDevice()->CreateUnorderedAccessView(m_pTexture.Get(), &uavDesc, m_pTextureUAV.GetAddressOf());
			}
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
//		GetSRV: Returns render target view.
//------------------------------------------------------------------------------------
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> Dx11Texture::GetRTV()	 const	noexcept
	{
		return m_pTextureRTV;
	}

//====================================================================================





//------------------------------------------------------------------------------------
//		GetUAV: Returns unordered access view.
//------------------------------------------------------------------------------------
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> Dx11Texture::GetUAV()	 const	noexcept
	{
		return m_pTextureUAV;
	}

//====================================================================================





//------------------------------------------------------------------------------------
//		Bind: Bind the texture to the traditional pipeline, currently only to the PS.
//------------------------------------------------------------------------------------

	void Dx11Texture::Bind(std::shared_ptr<RenderDevice>& renderDevice, AccessType type, int8_t slot) const noexcept
	{
		//if (type == AccessType::Read)
			renderDevice->GetDeviceContext()->PSSetShaderResources(slot, 1, GetSRV().GetAddressOf());
		//TODO:: Binding an uav to the pixel shader requires binding the rtv and dsv at the same time
		// with OMSetRenderTargetsAndUnorderedAccessViews call, as they share the same slots.
		//Currently there isn't a use for it, however it might need to be implemented in the future.
	}

//====================================================================================





//------------------------------------------------------------------------------------
//		Unbind: Unbind a resource from the traditional pipeline by binding a null view.
//------------------------------------------------------------------------------------

	void Dx11Texture::Unbind(std::shared_ptr<RenderDevice>& renderDevice, AccessType type, int8_t slot) const noexcept
	{
		static ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
		renderDevice->GetDeviceContext()->PSSetShaderResources(slot, 1, nullSRV);
	}

//====================================================================================





//------------------------------------------------------------------------------------
//		BindCompute: Bind the texture to the compute pipeline.
//------------------------------------------------------------------------------------

	void Dx11Texture::BindCompute(std::shared_ptr<RenderDevice>& renderDevice, AccessType type, int8_t slot) const noexcept
	{
		if (type == AccessType::Read)
			renderDevice->GetDeviceContext()->CSSetShaderResources(slot, 1, GetSRV().GetAddressOf());
		else
			renderDevice->GetDeviceContext()->CSSetUnorderedAccessViews(slot, 1, GetUAV().GetAddressOf(), nullptr);
	}

//====================================================================================





//------------------------------------------------------------------------------------
//		UnbindCompute: Unbind a resource from the compute pipeline by binding a null view.
//------------------------------------------------------------------------------------

	void Dx11Texture::UnbindCompute(std::shared_ptr<RenderDevice>& renderDevice, AccessType type, int8_t slot) const noexcept
	{
		ID3D11UnorderedAccessView* const nullUAV[] = { nullptr };
		renderDevice->GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, nullUAV, nullptr);
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
} //noctis::rdr