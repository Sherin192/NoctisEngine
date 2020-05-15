#include "Core_pch.h"
#include "Material.h"
#include "NoctisTexture.h"
namespace noctis::rdr
{

PhongMaterial::GPUMaterial PhongMaterial::defaultMaterial = GPUMaterial{
										{ 1.0f, 1.0f, 1.0f, 1.0f },
										{ 1.0f, 1.0f, 1.0f, 256.0f },
										{ 1.0f, 1.0f, 1.0f, 1.0f },
										0, 0, 0, 0
};

PBRMaterial::PBRMaterialData PBRMaterial::defaultMaterial = PBRMaterialData{
	{1.0f, 1.0f, 1.0f, 1.0f},
	0.0f,
	0.05f,
	1.0f,
	0
};




void PhongMaterial::AddTexture(std::shared_ptr<Texture> texture)
{
	if (!texture)
		return;
	m_textures[texture->GetType()] = texture;
	SetTextureBitField(texture->GetType());
}




void PhongMaterial::Bind(std::shared_ptr<RenderDevice> renderDevice)
{
	for (auto& texture : m_textures)
	{
		if (texture)
		{
			renderDevice->SetSampler(texture->GetType(), 1, texture->GetSampler());
			texture->Bind(renderDevice, AccessType::Read);
			//renderDevice->GetDeviceContext()->PSSetShaderResources(texture->GetType() == HEIGHT ? NORMAL : texture->GetType(), 1, texture->GetSRV().GetAddressOf());
		}
	}

	m_cbuffer.Bind(renderDevice, 2);
}




void PBRMaterial::AddTexture(std::shared_ptr<Texture> texture)
{
	if (!texture)
		return;
	m_textures[texture->GetType()] = texture;
	SetTextureBitField(texture->GetType());
}



void PBRMaterial::Bind(std::shared_ptr<RenderDevice> renderDevice) 
{ 
	for (auto& texture : m_textures)
	{
		if (texture)
		{
			renderDevice->SetSampler(texture->GetType(), 1, texture->GetSampler());
			texture->Bind(renderDevice, AccessType::Read);
		}
	}
	m_cbuffer.Bind(renderDevice, 2); 
}

}