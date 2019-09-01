#include "Core_pch.h"
#include "Texture.h"

namespace noctis::rdr
{
	Texture::Texture(std::shared_ptr<RenderDevice>& renderDevice, void* data, std::string path, unsigned int width, unsigned int height, unsigned int nrChannels, TextureType type)
		:m_type(type), m_filePath(path), m_width(width), m_height(height), m_nrChannels(nrChannels)
	{
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
			printf("bad stuff");
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
			printf("bad stuff");
		}

		//Create the sampler state

		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samplerDesc.BorderColor[0] = 1.0f;
		samplerDesc.BorderColor[1] = 1.0f;
		samplerDesc.BorderColor[2] = 1.0f;
		samplerDesc.BorderColor[3] = 1.0f;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		hResult = renderDevice->GetDevice()->CreateSamplerState(&samplerDesc, m_pSampler.GetAddressOf());
		if (FAILED(hResult))
		{
			printf("bad stuff");
		}
	}
}
