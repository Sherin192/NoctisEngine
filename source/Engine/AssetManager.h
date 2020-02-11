#ifndef _ASSET_MANAGER_H
#define _ASSET_MANAGER_H
#include "Core_pch.h"

//======================================================================================
//Asset importer stores a model in a unordered container 
//======================================================================================

namespace noctis
{
	namespace rdr
	{
		class Mesh;
		class Texture;
	}

class AssetManager
{
public:
	static AssetManager&								Instance();
	std::shared_ptr<rdr::Texture>						AddTexture(std::shared_ptr<rdr::Texture> texture);
	std::shared_ptr<rdr::Mesh>							AddMesh(std::shared_ptr<rdr::Mesh> mesh);
	uint8_t												AddName(std::string name);
	bool												ContainsTexture(std::string path);
	std::shared_ptr<rdr::Texture>						GetTexture(std::string path);
private:
	AssetManager() noexcept {};

	std::unordered_map<std::string, std::shared_ptr<rdr::Mesh>>					m_meshes;
	std::unordered_map<std::string, std::shared_ptr<rdr::Texture>>				m_textures;
	std::unordered_map<std::string, uint8_t>									m_names;
};
}
#endif 
