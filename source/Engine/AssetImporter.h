#ifndef _ASSET_IMPORTER_H
#define _ASSET_IMPORTER_H

#include "Renderer/Noctis3DTypes.h"
namespace noctis{

	class AssetImporterImpl;

	namespace rdr{
		class RenderDevice;
		class Texture;
		class CubeMap;
		class Model;
	}

class AssetImporter
{
public:
	
	static AssetImporter& Instance();
	void										Init(std::shared_ptr<rdr::RenderDevice>& renderDevice);
	std::shared_ptr<rdr::Model>					LoadModel(std::filesystem::path);
	std::shared_ptr<rdr::Texture>				LoadTexture(std::filesystem::path, rdr::TextureUsage type, uint8_t channels = 4);
	std::shared_ptr<rdr::CubeMap>				LoadCubeMap(std::array<std::string, 6>& paths);


private:
	AssetImporter();
	std::unique_ptr<AssetImporterImpl>			m_pImpl;
};
}
#endif
