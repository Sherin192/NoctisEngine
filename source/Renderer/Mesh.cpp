#include "Core_pch.h"
#include "Mesh.h"


namespace noctis::rdr
{

//------------------------------------------------------------------------------------
//		Mesh: Constructor.
//------------------------------------------------------------------------------------
Mesh::Mesh(std::shared_ptr<RenderDevice>& renderDevice, std::string& name, std::vector<Vertex>& vertices, std::vector<unsigned>& indices)
: m_name(name), m_materialName(kDefaultMaterial), m_vertexBuffer(), m_indexBuffer()
{	
		Setup(renderDevice, vertices, indices);
}

//====================================================================================





//------------------------------------------------------------------------------------
//		Mesh: Copy Constructor.
//------------------------------------------------------------------------------------
Mesh::Mesh(Mesh&& other) noexcept
	:m_vertexBuffer(), m_indexBuffer()
{
	m_name.swap(other.m_name);
	m_materialName = other.m_materialName;
	m_vertexBuffer.Swap(other.m_vertexBuffer);
	m_indexBuffer.Swap(other.m_indexBuffer);
}

//====================================================================================





//------------------------------------------------------------------------------------
//		Render: Draw the mesh. 
//------------------------------------------------------------------------------------
void Mesh::Render(std::shared_ptr<RenderDevice>& renderDevice)
{
	MaterialPool::Instance().GetMaterial(m_materialName)->Bind(renderDevice);
	m_vertexBuffer.Bind(renderDevice);
	m_indexBuffer.Bind(renderDevice);

	renderDevice->GetDeviceContext()->DrawIndexed(m_indexBuffer.Size(), 0, 0);
}

//====================================================================================





//------------------------------------------------------------------------------------
//		Setup: Sets up the vertex and index buffer for the mesh. 
//------------------------------------------------------------------------------------
void Mesh::Setup(std::shared_ptr<RenderDevice>& renderDevice, std::vector<Vertex>& vertices, std::vector<unsigned>& indices)
{
	m_vertexBuffer.SetStride(sizeof(vertices[0]));
	m_vertexBuffer.SetOffset(0);
	m_vertexBuffer.Init(std::forward<std::shared_ptr<RenderDevice>>(renderDevice), false, vertices.data(), vertices.size());
	m_indexBuffer.Init(renderDevice, false, indices.data(), indices.size());
}

//====================================================================================
} //noctis::rdr