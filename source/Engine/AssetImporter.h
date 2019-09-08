#ifndef _ASSET_IMPORTER_H
#define _ASSET_IMPORTER_H

#include "Renderer/Noctis3DTypes.h"
namespace noctis{

	class AssetImporterImpl;

	namespace rdr{
		class RenderDevice;
		class Texture;
		class Model;
	}

class AssetImporter
{
public:
	
	static AssetImporter& Instance(std::shared_ptr<rdr::RenderDevice>& renderDevice);

	std::shared_ptr<rdr::Model>					LoadModel(std::filesystem::path);
	std::shared_ptr<rdr::Texture>				LoadTexture(std::filesystem::path, rdr::TextureType type);

private:
	AssetImporter(std::shared_ptr<rdr::RenderDevice>& renderDevice);
	std::unique_ptr<AssetImporterImpl>			m_pImpl;
};
}
#endif
