#include "Core_pch.h"
#include "NoctisTexture.h"

namespace noctis::rdr
{
//------------------------------------------------------------------------------------
//		Texture Default Constructor:
//------------------------------------------------------------------------------------
	Texture::Texture() 
		: m_usage(UNSPECIFIED), m_filePath(), m_width(), m_height(), m_nrChannels(), Base() {}

//====================================================================================




//------------------------------------------------------------------------------------
//		Texture Constructor:
//------------------------------------------------------------------------------------
	Texture::Texture(std::shared_ptr<RenderDevice>& renderDevice, void* data, std::string path,
					 unsigned int width, unsigned int height, unsigned int nrChannels, TextureUsage type /*= TextureUsage::UNSPECIFIED*/, Format format /*= Format::kRGBA8UN*/)
		: m_usage(type), m_filePath(path), m_width(width), m_height(height), m_nrChannels(nrChannels), Base(renderDevice, data, width, height, nrChannels, type, format) {}

//====================================================================================





//------------------------------------------------------------------------------------
//		Bind: Binds the texture to the appropriate pipeline.
//------------------------------------------------------------------------------------
	void Texture::Bind(std::shared_ptr<RenderDevice>& renderDevice, AccessType type, bool compute /* = false*/, int8_t slot /* = -1*/) const noexcept
	{
		if (compute)
			Base::BindCompute(renderDevice, type, slot != -1 ? slot : m_usage);
		else
			Base::Bind(renderDevice, type, slot != -1 ? slot : m_usage);
	}

//====================================================================================





//------------------------------------------------------------------------------------
//		Unbind: Uninds the texture from the appropriate pipeline.
//------------------------------------------------------------------------------------
	void Texture::Unbind(std::shared_ptr<RenderDevice>& renderDevice, AccessType type, bool compute /* = false*/, int8_t slot /* = -1*/) const noexcept
	{
		if (compute)
			Base::UnbindCompute(renderDevice, type, slot != -1 ? slot : m_usage);
		else
			Base::Unbind(renderDevice, type, slot != -1 ? slot : m_usage);
	}

//====================================================================================





//------------------------------------------------------------------------------------
//		GetType: Returns the type of the texture.
//------------------------------------------------------------------------------------
	TextureUsage Texture::GetType()	 const noexcept
	{ 
		return m_usage; 
	}

//====================================================================================




//------------------------------------------------------------------------------------
//		GetFilePath: Returns the file path.
//------------------------------------------------------------------------------------
	const std::string& Texture::GetFilePath() const	noexcept
	{ 
		return m_filePath; 
	}

//====================================================================================
} //noctis::rdr
