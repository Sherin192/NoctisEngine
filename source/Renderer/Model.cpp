#include "Core_pch.h"
#include "Model.h"
#include "Noctis3DTypes.h"
#include "Engine/ShapeGenerator.h"
#include "Engine/CameraFPS.h"


#if defined(_TEMP)
#define STB_IMAGE_IMPLEMENTATION
#include "../Dependencies/STBImage/stb_image.h"
#endif

namespace noctis::rdr
{

	Model::Model(std::shared_ptr<RenderDevice>& renderDevice, sg::Shape shape)
		: m_cbuffer(renderDevice), m_transform(), m_meshes()
	{
		Generate(renderDevice, shape);
	}

	Model::Model(std::shared_ptr<RenderDevice>& renderDevice, std::string_view path)
		: m_cbuffer(renderDevice), m_transform(), m_meshes()

	{
	}


	Transform& Model::GetTransform() noexcept { return m_transform; }
	const Transform& Model::GetTransform() const noexcept { return m_transform; }
	void Model::SetTransform(Transform& t) noexcept { m_transform.Set(t); }



	void Model::Render(std::shared_ptr<RenderDevice>& renderDevice, Camera& cam)
	{
		math::Nmat4 WVP = cam.GetProj() * cam.GetView() * m_transform.AsMatrix();
		CBModelData cbData(m_transform.AsMatrix(), WVP);
		//renderDevice->SetRasterizerState(RasterizerType::SOLID_CULL_BACK);

		//renderDevice->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		for (auto& mesh : m_meshes)
		{
			cbData.material = mesh->GetMaterial().GetGPUMaterial();

			//Update and bind Model constant buffer
			m_cbuffer.Update(renderDevice, cbData);
			m_cbuffer.Bind(renderDevice, 0);

			mesh->Render(renderDevice);
		}
	}



	bool Model::Generate(std::shared_ptr<RenderDevice>& renderDevice, sg::Shape shape)
	{
		sg::ShapeGenerator& sGen = sg::ShapeGenerator::Instance();
		Data data;
		std::string name;
		switch (shape)
		{
		case sg::Shape::SPHERE:

			//Generate the data for the sphere.
			name = sg::kShapeNameSphere;
			sGen.CreateSphere(2.0f, 10u, 10u, data);

			break;
		case sg::Shape::CUBE:

			//Generate the data for the cube.
			name = sg::kShapeNameCube;
			sGen.CreateCube(20.0f, 20.0f, 20.0f, data);

			break;
		
		case sg::Shape::PLANE:
			name = sg::kShapeNamePlane;
			sGen.CreatePlane(10.0f, 10.0f, 2, 2, data);
		default:
			break;
		}
		//Create a Mesh.

#if defined(_TEMP)
		std::string path_str = "Models\\Sponza\\textures\\sponza_curtain_diff.png";
		int width, height, nrChannels;

		unsigned char* data_tex = stbi_load(path_str.data(), &width, &height, &nrChannels, STBI_rgb_alpha);
		std::vector<std::shared_ptr<Texture>> tex;
		Texture * texture = new Texture(m_pRenderDevice, data_tex, path_str, width, height, 4, Texture::DIFFUSE);
		auto shared_tex = std::make_shared<Texture>(*texture);
		tex.emplace_back(shared_tex);

		m_meshes.push_back(std::make_shared<Mesh>(Mesh(m_pRenderDevice, data.vertices, data.indices, tex)));

#else

		m_meshes.push_back(std::make_shared<Mesh>(Mesh(renderDevice, name, data.vertices, data.indices)));
#endif
		return true;
	}




	void Model::SetTexture(std::shared_ptr<Texture> & texture, TextureUsage type, const char* meshName)
	{
		for (auto& m : m_meshes)
		{
			if (m->GetName() == meshName)
			{
				m->SetTexture(texture);
			}
		}
	}



	void Model::SetMaterial(Material mat, const char* meshName)
	{
		for (auto& m : m_meshes)
		{
			if (m->GetName() == meshName)
			{
				m->SetMaterial(mat);
			}
		}
	}




	Material& Model::GetMaterial(const char* meshName)
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





	void Model::UpdateBuffer(std::shared_ptr<RenderDevice> & renderDevice)
	{
		//m_cbuffer.Update(renderDevice, )
	}
}