#ifndef NOCTIS_CUBEMAP_H_
#define NOCTIS_CUBEMAP_H_

#if defined (_WIN32)
#include "Dx11Texture.h"
#endif
#include "NoctisBuffers.h"
#include "NoctisTexture.h"
#include "Vertex.h"
#include "NoctisRenderable.h"

namespace noctis::rdr
{
	class CubeMap :  public Dx11Texture, public IRenderable<CubeMap>
	{
		using Base = Dx11Texture;
	public:
		CubeMap(std::shared_ptr<RenderDevice>& renderDevice, void* data, std::array<std::string, 6>& paths, unsigned int width, unsigned int height, unsigned int nrChannels, rdr::Format format = kRGBA8UN_SRGB);

		void Render(std::shared_ptr<RenderDevice>&, Camera&);

		const auto GetSize() const noexcept { return std::make_pair( m_width, m_height ); }
	private:
		void Init(std::shared_ptr<RenderDevice>& renderDevice);
		std::array<std::string, 6>								m_filePaths;
		unsigned int											m_width;
		unsigned int											m_height;
		unsigned int											m_nrChannels;
		ConstantBuffer<CBModelData>								m_cbuffer;
		VertexBuffer<Vertex>									m_vertexBuffer;
		IndexBuffer<>											m_indexBuffer;
	};
}



#endif //NOCTIS_CUBEMAP_H_