#include "Core_pch.h"
#include "Mesh.h"
#include "Material.h"

namespace noctis::rdr
{

Mesh::Mesh(std::shared_ptr<RenderDevice>& renderDevice, std::string& name, std::vector<Vertex> vertices, std::vector<unsigned> indices)
: m_name(name), m_vertices(std::move(vertices)), m_indices(std::move(indices)), m_textures(), m_material(), m_vertexBuffer(), m_indexBuffer()
{	
		Setup(renderDevice);
}


Mesh::Mesh(std::shared_ptr<RenderDevice>& renderDevice, std::string& name, std::vector<Vertex> vertices, std::vector<unsigned> indices, TextureArray textures)
	: m_name(name), m_vertices(std::move(vertices)), m_indices(std::move(indices)), m_textures(std::move(textures)), m_material(), m_vertexBuffer(), m_indexBuffer()
{
	Setup(renderDevice);
	for (auto& tex : textures)
	{
		if(tex)
			m_material.SetTextureBitField(tex->GetType());
	}
}


Mesh::Mesh(Mesh&& other) noexcept
	:
	m_vertices(std::move(other.m_vertices)),
	m_indices(std::move(other.m_indices)),
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
			//TODO: Refactor this as it is Dx11 specific
			//renderDevice->GetDeviceContext()->PSSetSamplers(texture->GetType(), 1, texture->GetSampler().GetAddressOf());
			renderDevice->SetSampler(texture->GetType(), 1, texture->GetSampler());
			renderDevice->GetDeviceContext()->PSSetShaderResources(texture->GetType() == HEIGHT ? NORMAL : texture->GetType(), 1, texture->GetSRV().GetAddressOf());
		}
	}

	m_vertexBuffer.Bind(renderDevice);
	m_indexBuffer.Bind(renderDevice);

	renderDevice->GetDeviceContext()->DrawIndexed(m_indices.size(), 0, 0);
}


void Mesh::Setup(std::shared_ptr<RenderDevice>& renderDevice)
{
	m_vertexBuffer.SetStride(sizeof(m_vertices[0]));
	m_vertexBuffer.SetOffset(0);
	m_vertexBuffer.Init(std::forward<std::shared_ptr<RenderDevice>>(renderDevice), false, m_vertices.data(), m_vertices.size());
	m_indexBuffer.Init(renderDevice, false, m_indices.data(), m_indices.size());
}

}