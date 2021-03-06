#include "Core_pch.h"
#include "Model.h"
#include "Renderer/Noctis3DTypes.h"
#include "Engine/ShapeGenerator.h"
#include "Engine/CameraFPS.h"
#include "Engine/AssetManager.h"

#if defined(_TEMP)
#define STB_IMAGE_IMPLEMENTATION
#include "../Dependencies/STBImage/stb_image.h"
#endif

namespace noctis::rdr
{

Model::Model(std::shared_ptr<RenderDevice>& renderDevice, sg::Shape shape, std::string name /* = "" */)
	: m_cbuffer(renderDevice), m_transform(), m_meshes(), m_pRootNode(new Node)
{
	Generate(renderDevice, shape, name);
}





Model::Model(std::shared_ptr<RenderDevice>& renderDevice, std::string_view path)
	: m_cbuffer(renderDevice), m_transform(), m_meshes()

{
}





Transform& Model::GetTransform() noexcept { return m_transform; }





const Transform& Model::GetTransform() const noexcept { return m_transform; }





void Model::SetTransform(const Transform& t) noexcept { m_transform.Set(t); m_pRootNode->m_transform.Set(t); }





void Model::Render(std::shared_ptr<RenderDevice>& renderDevice, Camera& cam)
{
	RenderNode(renderDevice, m_pRootNode.get(), cam, Transform());
}


void Model::RenderNode(std::shared_ptr<RenderDevice>& renderDevice, Node* node, Camera& cam, Transform& accumulatedTransform)
{
	renderDevice->SetRasterizerState(RasterizerType::SOLID_CULL_BACK);
	auto NodeTransform = Transform(accumulatedTransform.AsMatrix() * node->m_transform.AsMatrix());
	//math::Nmat4 WVP = cam.GetProj() * cam.GetView() * NodeTransform.AsMatrix();
	//CBModelData cbData(NodeTransform.AsMatrix(), WVP);
	CBModelData cbData(NodeTransform.AsMatrix(), cam.GetView(), cam.GetProj());

	for (auto i : node->m_meshes)
	{
		//Update and bind Model constant buffer
		m_cbuffer.Update(renderDevice, cbData);
		m_cbuffer.Bind(renderDevice, 0);

		m_meshes[i]->Render(renderDevice);
	}

	for (auto& c : node->m_pNodes)
	{
		RenderNode(renderDevice, c.get(), cam, NodeTransform);
	}
}






void Model::Generate(std::shared_ptr<RenderDevice>& renderDevice, sg::Shape shape, std::string name/* = "" */)
{
	sg::ShapeGenerator& sGen = sg::ShapeGenerator::Instance();
	Data data;
	std::string modelName;
	switch (shape)
	{
	case sg::Shape::SPHERE:

		//Generate the data for the sphere.
		modelName = sg::kShapeNameSphere;
		sGen.CreateSphere(2.0f, 10u, 10u, data);

		break;
	case sg::Shape::CUBE:

		//Generate the data for the cube.
		modelName = sg::kShapeNameCube;
		sGen.CreateCube(20.0f, 20.0f, 20.0f, data);

		break;
	
	case sg::Shape::PLANE:
		modelName = sg::kShapeNamePlane;
		sGen.CreatePlane(10.0f, 10.0f, 2, 2, data);
	default:
		break;
	}
	m_meshes.push_back(std::make_shared<Mesh>(Mesh(renderDevice, modelName, data.vertices, data.indices)));
	m_pRootNode = std::make_unique<Node>(Node(Transform(), this));
	m_pRootNode->m_meshes.push_back(0);
	if (name != "")
		modelName = name;
	auto count = AssetManager::Instance().AddName(modelName);
	if (count)
		m_pRootNode->m_name = modelName + " (" + std::to_string(count) + ")";
	else
		m_pRootNode->m_name = modelName;
}





void Model::SetMaterial(const std::string& name, const std::string& meshName)
{
	for (auto& m : m_meshes)
	{
		if (m->GetName() == meshName)
		{
			m->SetMaterial(name);
		}
	}
}


std::shared_ptr<Material> Model::GetMaterial(const std::string& meshName)
{
	for (auto& m : m_meshes)
	{
		if (m->GetName() == meshName)
		{
			return m->GetMaterial();
		}
	}
	assert(!m_meshes.empty());
}




void Model::SetName(const std::string& name) noexcept { m_pRootNode->m_name = name; }
}