#include "Core_pch.h"
#include "Mesh.h"
#include "Material.h"

namespace noctis::rdr
{

Mesh::Mesh(std::shared_ptr<RenderDevice>& renderDevice, std::string& name, std::vector<Vertex>& vertices, std::vector<unsigned>& indices)
: m_name(name), m_textures(), m_material(), m_vertexBuffer(), m_indexBuffer()
{	
		Setup(renderDevice, vertices, indices);
}


Mesh::Mesh(std::shared_ptr<RenderDevice>& renderDevice, std::string& name, std::vector<Vertex>& vertices, std::vector<unsigned>& indices, TextureArray textures)
	: m_name(name), m_textures(std::move(textures)), m_material(), m_vertexBuffer(), m_indexBuffer()
{
	Setup(renderDevice, vertices, indices);
	for (auto& tex : textures)
	{
		if(tex)
			m_material.SetTextureBitField(tex->GetType());
	}
}


Mesh::Mesh(Mesh&& other) noexcept
	:
	m_textures(std::move(other.m_textures)),
	m_material(std::move(other.m_material)),
	m_vertexBuffer(),
	m_indexBuffer()
{
	m_name.swap(other.m_name);
	m_vertexBuffer.Swap(other.m_vertexBuffer);
	m_indexBuffer.Swap(other.m_indexBuffer);
}

void Mesh::SetMaterial(Material mat) noexcept 
{ 
	m_material = mat; 
}
void Mesh::SetTexture(std::shared_ptr<Texture>& tex) 
{ 
	m_textures[tex->GetType()] = tex;
	m_material.SetTextureBitField(tex->GetType());
}





void Mesh::Render(std::shared_ptr<RenderDevice>& renderDevice)
{
	for (auto& texture : m_textures)
	{
		if (texture)
		{
			renderDevice->SetSampler(texture->GetType(), 1, texture->GetSampler());
			renderDevice->GetDeviceContext()->PSSetShaderResources(texture->GetType() == HEIGHT ? NORMAL : texture->GetType(), 1, texture->GetSRV().GetAddressOf());
		}
	}
	m_vertexBuffer.Bind(renderDevice);
	m_indexBuffer.Bind(renderDevice);

	renderDevice->GetDeviceContext()->DrawIndexed(m_indexBuffer.Size(), 0, 0);
}


void Mesh::Setup(std::shared_ptr<RenderDevice>& renderDevice, std::vector<Vertex>& vertices, std::vector<unsigned>& indices)
{
	m_vertexBuffer.SetStride(sizeof(vertices[0]));
	m_vertexBuffer.SetOffset(0);
	m_vertexBuffer.Init(std::forward<std::shared_ptr<RenderDevice>>(renderDevice), false, vertices.data(), vertices.size());
	m_indexBuffer.Init(renderDevice, false, indices.data(), indices.size());
}

}