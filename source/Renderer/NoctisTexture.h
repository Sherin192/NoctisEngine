#ifndef NOCTIS_TEXTURE_H
#define NOCTIS_TEXTURE_H
#include "Core_pch.h"
#if defined (_WIN32)
#include "Dx11Texture.h"
#endif
#include "Noctis3DTypes.h"
#include "NoctisSampler.h"

namespace noctis::rdr
{

	class Texture : public Dx11Texture
	{
		using Base = Dx11Texture;
	public:
		Texture();
		Texture(std::shared_ptr<RenderDevice>& renderDevice, void* data, std::string path, unsigned int width, unsigned int height, unsigned int nrChannels, TextureUsage type);
		TextureUsage GetType()	 const noexcept;
		const std::string& GetFilePath() const	noexcept;
		unsigned int GetWidth() const noexcept { return m_width; }
		unsigned int GetHeight() const noexcept { return m_height; }
	protected:
		TextureUsage											m_usage;
		std::string												m_filePath;
		unsigned int											m_width;
		unsigned int											m_height;
		unsigned int											m_nrChannels;
	};
}
#endif //NOCTIS_TEXTURE_H