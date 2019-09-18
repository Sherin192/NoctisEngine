#ifndef _TEXTURE_H
#define _TEXTURE_H
#include "Core_pch.h"
#if defined (_WIN32)
#include "Dx11Texture.h"
#endif
#include "Noctis3DTypes.h"
#include "NoctisSampler.h"

namespace noctis::rdr
{
	class Texture : public Dx11Texture<Texture>
	{
		using Base = Dx11Texture<Texture>;

		Base& GetImpl() { return static_cast<Base&>(*this); }
	public:
		Texture();
		Texture(std::shared_ptr<RenderDevice>& renderDevice, void* data, std::string path, unsigned int width, unsigned int height, unsigned int nrChannels, TextureUsage type);
		TextureUsage GetType()	 const noexcept;
		const std::string& GetFilePath() const	noexcept;

	protected:
		TextureUsage											m_usage;
		std::string												m_filePath;
		unsigned int											m_width;
		unsigned int											m_height;
		unsigned int											m_nrChannels;
	};
}
#endif //_TEXTURE_H