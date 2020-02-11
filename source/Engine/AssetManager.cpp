//#include "Core_pch.h"
#include "AssetManager.h"

#include "Renderer/Mesh.h"
namespace noctis
{ 
AssetManager& AssetManager::Instance()
{
	static AssetManager instance;
	return instance;
}

std::shared_ptr<rdr::Texture> AssetManager::AddTexture(std::shared_ptr<rdr::Texture> texture)
{
	return m_textures.try_emplace(texture->GetFilePath(), texture).first->second;
}


std::shared_ptr<rdr::Mesh> AssetManager::AddMesh(std::shared_ptr<rdr::Mesh> mesh)
{
	return m_meshes.emplace(mesh->GetName(), mesh).first->second;
}


uint8_t AssetManager::AddName(std::string name)
{
	auto [iter, inserted] = m_names.insert({ name, 0 });
	if (!inserted)
	{
		iter->second++;
	}
	return iter->second;
}

bool AssetManager::ContainsTexture(std::string path)
{
	auto iter = m_textures.find(path);
	if (iter == std::end(m_textures))
	{
		return false;
	}
	return true;
}


std::shared_ptr<rdr::Texture> AssetManager::GetTexture(std::string path)
{
	return m_textures[path];
}
}