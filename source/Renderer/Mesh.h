#ifndef _MESH_H
#define _MESH_H

#include "NoctisRenderDevice.h"
#include "NoctisBuffers.h"
#include "NoctisTexture.h"
#include "Vertex.h"

//------------------------------------------------------------------------------------
//		Forward Declarations:
//------------------------------------------------------------------------------------
namespace noctis
{
	class AssetImporterImpl;
	namespace rdr
	{ 
		class Shader;
		class Material;
	}
}
//====================================================================================





namespace noctis::rdr 
{
//------------------------------------------------------------------------------------
//		Mesh:
//------------------------------------------------------------------------------------
	class Mesh
	{
	public:
		using TextureArray = std::array<std::shared_ptr<Texture>, TextureUsage::COUNT>;

		Mesh(std::shared_ptr<RenderDevice>&, std::string&, std::vector<Vertex>, std::vector<unsigned>);
		Mesh(std::shared_ptr<RenderDevice>&, std::string&, std::vector<Vertex>, std::vector<unsigned>, TextureArray);

		Mesh(const Mesh& other) = delete;
		//TODO: rule of 5
		Mesh(Mesh&& other) noexcept;

		void										Render(std::shared_ptr<RenderDevice>& renderDevice);
		VertexBuffer<Vertex>&						GetVertexBuffer() noexcept { return m_vertexBuffer; }
		IndexBuffer<>&								GetIndexBuffer() noexcept { return m_indexBuffer; }

		size_t										GetVertexCount()  const noexcept { return m_vertices.size(); }
		size_t										GetIndexCount()	const noexcept { return m_indices.size(); }
		const std::string&							GetName() const noexcept { return m_name; }
		Material&									GetMaterial() noexcept { return m_material; }
		void										SetMaterial(Material mat) noexcept;
		void										SetTexture(std::shared_ptr<Texture>& tex);
		const TextureArray&							GetTextures() const noexcept { return m_textures; }
	private:
		void										Setup(std::shared_ptr<RenderDevice>& renderDevice);

		std::string									m_name;
		std::vector<Vertex>							m_vertices;
		std::vector<unsigned>						m_indices;
		TextureArray								m_textures;
		Material									m_material;

		VertexBuffer<Vertex>						m_vertexBuffer;
		IndexBuffer<>								m_indexBuffer;

		friend class AssetImporterImpl;
	};
	
//====================================================================================
}	//noctis::rdr
#endif //_MESH_H

