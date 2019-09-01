#ifndef _TEXTURE_H
#define _TEXTURE_H
#include "Core_pch.h"
#include "NoctisRenderDevice.h"
#include "Noctis3DTypes.h"
#include <wrl.h>

namespace noctis::rdr
{
	class RenderDevice;

class Texture
{
public:
	enum class FORMAT { RGB8, RGB16, RGB32 };

	Texture() = default;
	Texture(std::shared_ptr<RenderDevice>&, void*, std::string, unsigned int, unsigned int, unsigned int, TextureType);

	TextureType												GetType()	 const noexcept { return m_type; }
	const std::string& GetFilePath() const	noexcept { return m_filePath; }
	Microsoft::WRL::ComPtr<ID3D11SamplerState>				GetSampler() const	noexcept { return m_pSampler; }
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>		GetSRV()	 const	noexcept { return m_pTextureSRV; }

private:
	TextureType												m_type;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>		m_pTextureSRV;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>					m_pTexture;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>				m_pSampler;

	std::string												m_filePath;
	unsigned int											m_width;
	unsigned int											m_height;
	unsigned int											m_nrChannels;
};
}
#endif //_TEXTURE_H