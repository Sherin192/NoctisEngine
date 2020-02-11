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
class Node;

class Model : public IRenderable<Model>
{
public:
	Model() = default;
	Model(std::shared_ptr<RenderDevice>& renderDevice, sg::Shape shape);
	Model(std::shared_ptr<RenderDevice>& renderDevice, std::string_view path);

	void				Render(std::shared_ptr<RenderDevice>&, Camera&);
	void				RenderNode(std::shared_ptr<RenderDevice>&, Node*, Camera&, Transform&);
	void				Generate(std::shared_ptr<RenderDevice>& renderDevice, sg::Shape shape);

	Transform&			GetTransform() noexcept;
	const Transform&	GetTransform() const noexcept;
	void				SetTransform(Transform& t) noexcept;
	void				SetMaterial(const std::string& name, const std::string& mesh);
	std::shared_ptr<Material>	GetMaterial(const std::string& mesh);
	const				std::vector<std::shared_ptr<Mesh>>& GetMeshes() { return m_meshes; }
	Node*				GetRootNode() { return m_pRootNode.get(); }

private:
	//TODO: Model class will need to be redesigned to better represent a node based structure
	//		which will better fit a futre ECS implementation.
	ConstantBuffer<CBModelData>					m_cbuffer;
	Transform									m_transform;
	std::vector<std::shared_ptr<Mesh>>			m_meshes;
	std::unique_ptr<Node>						m_pRootNode;

	friend AssetImporter;
	friend AssetImporterImpl;
};

class Node
{
public:
	Node() : m_transform() {}
	Node(Transform& transform) : m_transform(transform) {}
	Node(Transform& transform, Model* owner) : m_transform(transform), m_pOwner(owner) {}
//private:
	std::string									m_name;
	std::vector<std::unique_ptr<Node>>			m_pNodes;
	std::vector<uint32_t>						m_meshes;
	Transform									m_transform;
	Model*										m_pOwner;

	friend Model;
};

}
#endif //MODEL_H_