#ifndef MODEL_H_
#define MODEL_H_

#include "NoctisRenderable.h"
#include "Mesh.h"
#include "ShapeGenerator.h"
#include "Transform.h"

namespace noctis 
{
	class AssetImporter;
	class AssetImporterImpl;
	class Camera;
	struct Transform;
}

namespace noctis::rdr {

class RenderDevice;
class Mesh;
class Material;
class Texture;
template <typename DataType> class ConstantBuffer;
struct CBModelData;

class Model : public IRenderable<Model>
{
public:
	Model() = default;
	Model(std::shared_ptr<RenderDevice>& renderDevice, sg::Shape shape);
	Model(std::shared_ptr<RenderDevice>& renderDevice, std::string_view path);

	void				Render(std::shared_ptr<RenderDevice>&, Camera&);
	bool				Generate(std::shared_ptr<RenderDevice>& renderDevice, sg::Shape shape);

	Transform&			GetTransform() noexcept;
	const Transform&	GetTransform() const noexcept;
	void				SetTransform(Transform& t) noexcept;
	void				SetTexture(std::shared_ptr<Texture>&, TextureUsage, const char* meshName);
	void				SetMaterial(Material, const char* mesh);
	Material&			GetMaterial(const char* mesh);
	const				std::vector<std::shared_ptr<Mesh>>& GetMeshes() { return m_meshes; }

	void				UpdateBuffer(std::shared_ptr<RenderDevice>& renderDevice);
	//void				BindBuffer(std::shared_ptr<RenderDevice>&, uint32_t slot);
private:
	
	ConstantBuffer<CBModelData>					m_cbuffer;
	Transform									m_transform;
	std::vector<std::shared_ptr<Mesh>>			m_meshes;

	friend AssetImporter;
	friend AssetImporterImpl;
};
}
#endif //MODEL_H_