#include "Core_pch.h"
#include "NoctisTexture.h"

namespace noctis::rdr
{
	Texture::Texture() 
		: m_usage(UNSPECIFIED), m_filePath(), m_width(), m_height(), m_nrChannels(), Base() {}





	Texture::Texture(std::shared_ptr<RenderDevice>& renderDevice, void* data, std::string path, unsigned int width, unsigned int height, unsigned int nrChannels, TextureUsage type)
		: m_usage(type), m_filePath(path), m_width(width), m_height(height), m_nrChannels(nrChannels), Base(renderDevice, data, path, width, height, nrChannels, type) {}






	TextureUsage Texture::GetType()	 const noexcept
	{ 
		return m_usage; 
	}





	const std::string& Texture::GetFilePath() const	noexcept
	{ 
		return m_filePath; 
	}

}
