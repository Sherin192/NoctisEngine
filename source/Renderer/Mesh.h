#ifndef _MESH_H
#define _MESH_H

#include "NoctisRenderDevice.h"
#include "NoctisBuffers.h"
#include "NoctisTexture.h"
#include "Vertex.h"
#include "Material.h"
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
		Mesh(std::shared_ptr<RenderDevice>&, std::string&, std::vector<Vertex>&, std::vector<unsigned>&);

		Mesh(const Mesh& other) = delete;
		//TODO: rule of 5
		Mesh(Mesh&& other) noexcept;

		void										Render(std::shared_ptr<RenderDevice>& renderDevice);
		VertexBuffer<Vertex>&						GetVertexBuffer() noexcept { return m_vertexBuffer; }
		IndexBuffer<>&								GetIndexBuffer() noexcept { return m_indexBuffer; }

		const std::string&							GetName() const noexcept { return m_name; }
		std::shared_ptr<Material>					GetMaterial() noexcept { return MaterialPool::Instance().GetMaterial(m_materialName); }
		void										SetMaterial(const std::string& name) { m_materialName = name; }

	private:
		void										Setup(std::shared_ptr<RenderDevice>& renderDevice, std::vector<Vertex>& vertices, std::vector<unsigned>& indices);

		std::string									m_name;
		std::string									m_materialName;
		VertexBuffer<Vertex>						m_vertexBuffer;
		IndexBuffer<>								m_indexBuffer;

		friend class AssetImporterImpl;
	};
	
//====================================================================================
}	//noctis::rdr
#endif //_MESH_H

