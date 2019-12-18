#ifndef _DX11_TEXTURE_H
#define _DX11_TEXTURE_H
#include "Core_pch.h"
#include "NoctisRenderDevice.h"
#include <wrl.h>

namespace noctis::rdr
{
//------------------------------------------------------------------------------------
//		Dx11Texture: Dx11 base class for textures.
//------------------------------------------------------------------------------------
	class Dx11Texture
	{
	protected:
		//C.35: A base class destructor should be either public and virtual, or protected and nonvirtual
		~Dx11Texture() = default;
	public:
		Dx11Texture() = default;

		Dx11Texture(std::shared_ptr<RenderDevice>& renderDevice, void* data, unsigned int width, unsigned int height, unsigned int nrChannels, TextureUsage type = TextureUsage::UNSPECIFIED);
		
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>		GetSRV()	 const	noexcept;
		
		std::shared_ptr<Sampler>& GetSampler();

	private:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>		m_pTextureSRV;
		Microsoft::WRL::ComPtr<ID3D11Texture2D>					m_pTexture;
		//TODO: Make a cache of common samplers which will be bound to the pipeline at each draw
		std::shared_ptr<Sampler>								m_pSampler;
	};

//====================================================================================
}
#endif //_DX11_TEXTURE_H