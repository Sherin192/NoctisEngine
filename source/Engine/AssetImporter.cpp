#include "Core_pch.h"
#include "AssetImporter.h"
#include "AssetManager.h"
#include "Renderer/NoctisTexture.h"
#include "Renderer/Mesh.h"
#include "Renderer/Model.h"
#include "Renderer/NoctisCubeMap.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#define STB_IMAGE_IMPLEMENTATION
#include "STBImage/stb_image.h"

namespace noctis
{
//==================================================================================================
//	Implementation Class
//==================================================================================================
class AssetImporterImpl
{
	enum class MaterialRep {Legacy, PBR};
public:

	std::shared_ptr<rdr::Model>				Load(std::filesystem::path);
	void									ProcessNode(const aiScene*, aiNode*, std::shared_ptr<rdr::Model>&);
	std::shared_ptr<rdr::Mesh>				ProcessMesh(const aiScene*, aiMesh*);
	std::shared_ptr<rdr::Texture>			LoadMaterial(aiMaterial *, aiTextureType);
	std::shared_ptr<rdr::Texture>			LoadTexture(std::filesystem::path, rdr::TextureUsage);
	std::shared_ptr<rdr::CubeMap>			LoadCubeMap(std::array<std::string, 6>& paths);

	std::shared_ptr<rdr::Material>		FillMaterial(const aiMaterial* mat);

	void									SetRenderDevice(std::shared_ptr<rdr::RenderDevice>& renderDevice) { m_pRenderDevice = renderDevice; }
#if _DEBUG
	void									ShowMaterialInformations(const aiMaterial*);
#endif 
private:
	std::shared_ptr<rdr::RenderDevice>		m_pRenderDevice;
	std::filesystem::path					m_path;
	MaterialRep								m_currentRep = MaterialRep::Legacy;
};







std::shared_ptr<rdr::Model> AssetImporter::LoadModel(std::filesystem::path filePath)
{
	return m_pImpl->Load(filePath);
}





std::shared_ptr<rdr::Texture> AssetImporter::LoadTexture(std::filesystem::path filePath, rdr::TextureUsage type)
{
	return m_pImpl->LoadTexture(filePath, type);
}




std::shared_ptr<rdr::CubeMap> AssetImporter::LoadCubeMap(std::array<std::string, 6>& paths)
{
	return m_pImpl->LoadCubeMap(paths);
}




static rdr::TextureUsage ConvertAssimpToEngineType(aiTextureType type)
{
	switch (type)
	{
	case aiTextureType_DIFFUSE:
		return rdr::TextureUsage::DIFFUSE;
		break;
	case aiTextureType_SPECULAR:
		return rdr::TextureUsage::SPECULAR;
		break;
	case aiTextureType_HEIGHT:
		return rdr::TextureUsage::HEIGHT;
		break;
	case aiTextureType_NORMALS:
		return rdr::TextureUsage::NORMAL;
	case aiTextureType_OPACITY:
		return rdr::TextureUsage::OPACITY;
	case aiTextureType_EMISSIVE:
		return rdr::TextureUsage::EMISSIVE;
	case aiTextureType_LIGHTMAP:
		return rdr::TextureUsage::AMBIENT_OCCLUSION;
	case aiTextureType_UNKNOWN:
		//gltf models store metallic-roughness-ao map as one texture this will be stored in the metallic map.
		return rdr::TextureUsage::METALLIC;
	default:
		assert(0);
		return rdr::TextureUsage::UNSPECIFIED;
	}
}






//==================================================================================================
// Load : Gets a path and returns a shared_ptr to a Model stored in the AssetManager class.
//==================================================================================================
std::shared_ptr<rdr::Model> AssetImporterImpl::Load(std::filesystem::path filePath)
{
	if (std::filesystem::exists(filePath))
	{
		m_path = filePath;
		if (filePath.extension() == ".gltf")
			m_currentRep = MaterialRep::PBR;
		else
			m_currentRep = MaterialRep::Legacy;

		Assimp::Importer importer;

		//TODO: Allow the user to specify the flags instead of hardcoding them.
		const aiScene *scene = importer.ReadFile(filePath.string().c_str(), aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::string s = importer.GetErrorString();
			Log(LogLevel::Error, "ERROR::ASSIMP::" + s);
			return nullptr;
		}

		auto model = std::make_shared<rdr::Model>();
		ProcessNode(scene, scene->mRootNode, model);

		return model;
	}
	else
	{
		assert(!std::filesystem::exists(filePath));
		return {};
	}
}





//==================================================================================================
// ProcessNode : Fills the model with all the meshes from a given node then it goes to the child
//				 node recursively.
//==================================================================================================
void AssetImporterImpl::ProcessNode(const aiScene* scene, aiNode* node, std::shared_ptr<rdr::Model>& model)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		model->m_meshes.push_back(ProcessMesh(scene, mesh));
	}
	
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(scene, node->mChildren[i], model);
	}
}






std::shared_ptr<rdr::Mesh> AssetImporterImpl::ProcessMesh(const aiScene* scene, aiMesh* mesh)
{
	std::vector<rdr::Vertex> vertices;
	std::vector<unsigned int> indices;
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		DirectX::XMFLOAT3 vector;
		rdr::Vertex vertex;
		// process vertex positions, normals and texture coordinates
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;

		vertex.position = vector;
		// normals
		if (mesh->mNormals)
		{
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normal = vector;
		}
		// texture coordinates
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			DirectX::XMFLOAT2 vec;
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoord = vec;
		}
		else
			vertex.texCoord = DirectX::XMFLOAT2(0.0f, 0.0f);
		// tangent
		if (mesh->mTangents)
		{
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.tangent = vector;
		}
		//// bitangent
		if (mesh->mBitangents)
		{
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.bitangent = vector;
		}
		
		vertices.push_back(vertex);

	}
	//TODO: Move this functionality to an AssetImporter and store the textures in a TextureManager and let the mesh store pointers into the global array of textures.

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	ShowMaterialInformations(material);

	std::shared_ptr<rdr::Texture> diffuseMaps = LoadMaterial(material, aiTextureType_DIFFUSE);
	std::shared_ptr<rdr::Texture> specularMaps = LoadMaterial(material, aiTextureType_SPECULAR);
	std::shared_ptr<rdr::Texture> normalMaps = LoadMaterial(material, aiTextureType_NORMALS);
	std::shared_ptr<rdr::Texture> heightMaps = LoadMaterial(material, aiTextureType_HEIGHT);
	std::shared_ptr<rdr::Texture> opacityMap = LoadMaterial(material, aiTextureType_OPACITY);
	std::shared_ptr<rdr::Texture> emissiveMap = LoadMaterial(material, aiTextureType_EMISSIVE);
	std::shared_ptr<rdr::Texture> aOcclusionMap = LoadMaterial(material, aiTextureType_LIGHTMAP);
	std::shared_ptr<rdr::Texture> shininessMap = LoadMaterial(material, aiTextureType_SHININESS);
	std::shared_ptr<rdr::Texture> displacementMap = LoadMaterial(material, aiTextureType_DISPLACEMENT);
	std::shared_ptr<rdr::Texture> reflectionMap = LoadMaterial(material, aiTextureType_REFLECTION);
	std::shared_ptr<rdr::Texture> unknownMap;
	if (m_currentRep == MaterialRep::PBR)
		unknownMap = LoadMaterial(material, aiTextureType_UNKNOWN);

	auto _mat = FillMaterial(material);
	_mat->AddTexture(diffuseMaps);
	_mat->AddTexture(specularMaps);
	_mat->AddTexture(normalMaps);
	_mat->AddTexture(heightMaps);
	_mat->AddTexture(opacityMap);
	_mat->AddTexture(emissiveMap);
	_mat->AddTexture(aOcclusionMap);
	_mat->AddTexture(unknownMap);
	rdr::MaterialPool::Instance().AddMaterial(_mat->GetName(), _mat);

	// process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	std::string name = mesh->mName.C_Str();
	//auto shared_mesh = AssetManager::Instance().AddMesh(std::make_shared<rdr::Mesh>(rdr::Mesh(m_pRenderDevice, name, vertices, indices, textures )));
	auto shared_mesh = std::make_shared<rdr::Mesh>(rdr::Mesh(m_pRenderDevice, name, vertices, indices));
	
	shared_mesh->SetMaterial(_mat->GetName());
	return shared_mesh;
}




//Add the tetures to a unordered_map<Texture> and then store that in the AssetManager and return a pointer to it
std::shared_ptr<rdr::Texture> AssetImporterImpl::LoadMaterial(aiMaterial *material, aiTextureType type)
{
	//TODO: Optimize this function.
	//Materials should be stored in a contiguous container in the assetManager and Texture shoudl have only a handle(index) to retrieve it from the manager.

	using namespace std::literals::string_literals;
	std::shared_ptr<rdr::Texture> texture;


	if (material->GetTextureCount(type))
	{
		aiString aiPath;
		//for(int i = 0; i < material->GetTextureCount(type); i++)
		//{
			if (material->GetTexture(type, 0, &aiPath) == AI_SUCCESS)
			{
				unsigned char *data = nullptr;
				std::filesystem::path matPath(aiPath.C_Str());
				std::filesystem::path matFullPath(m_path.parent_path());
				matFullPath /= matPath;

				texture = LoadTexture(matFullPath.string().c_str(), ConvertAssimpToEngineType(type));
			}
		//}
	}
	return texture;
}


std::shared_ptr<rdr::Texture> AssetImporterImpl::LoadTexture(std::filesystem::path filePath, rdr::TextureUsage type)
{
	unsigned char *data = nullptr;
	int width, height, nrChannels;

	if (!AssetManager::Instance().ContainsTexture(filePath.string()))
	{
		data = stbi_load(filePath.string().c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);
		if (!data)
		{
			Log(LogLevel::Error, "Failed to load a texture at : " + filePath.string());
		}
		return AssetManager::Instance().AddTexture(std::make_shared<rdr::Texture>(m_pRenderDevice, data, filePath.string(), width, height, 4, type));
	}
	else
	{
		return AssetManager::Instance().GetTexture(filePath.string());
	}
}






std::shared_ptr<rdr::CubeMap> AssetImporterImpl::LoadCubeMap(std::array<std::string, 6>& paths)
{
	unsigned char* data[6]{};
	int width, height, nrChannels;

	for (int i = 0; i < paths.size(); ++i)
	{
		data[i] = stbi_load(paths[i].c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);
		if (!data[i])
			Log(LogLevel::Error, "Cube Map texture(" + paths[i] + ") failed to load.");
	}
	auto cubeMap = std::make_shared<rdr::CubeMap>(m_pRenderDevice, data, paths, width, height, nrChannels);
	for (auto d : data)
	{
		stbi_image_free(d);
	}

	return cubeMap;
}



std::shared_ptr<rdr::Material> AssetImporterImpl::FillMaterial(const aiMaterial* mat)
{
	static uint32_t index = 0;
	auto material = rdr::PhongMaterial::defaultMaterial;
	aiColor3D color;
	if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_AMBIENT, color))
	{
		material.ambient = { color.r, color.g, color.b, 1.0f };
	}
	if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_DIFFUSE, color))
	{
		material.diffuse = { color.r, color.g, color.b, 1.0f };
	}
	if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_SPECULAR, color))
	{
		material.specular = { color.r, color.g, color.b, 1.0f };
	}
	if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_REFLECTIVE, color))
	{
		material.reflect = { color.r, color.g, color.b, 1.0f };
	}

	float value;
	if (AI_SUCCESS == mat->Get(AI_MATKEY_SHININESS_STRENGTH, value))
	{
		material.specular.w = value;
	}

	aiString name;
	if (AI_SUCCESS != mat->Get(AI_MATKEY_NAME, name))
	{
		name = "material_" + index++;
	}
	return std::make_shared<rdr::PhongMaterial>(m_pRenderDevice, name.C_Str(), material);
}






#if _DEBUG
void AssetImporterImpl::ShowMaterialInformations(const aiMaterial* mat)
{
	aiString name;
	if (AI_SUCCESS == mat->Get(AI_MATKEY_NAME, name))
	{
	}
	aiColor3D color;
	if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_AMBIENT, color))
	{

	}
	if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_DIFFUSE, color))
	{
	}
	if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_EMISSIVE, color))
	{
	}
	if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_REFLECTIVE, color))
	{
	}
	if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_SPECULAR, color))
	{
	}
	float value;
	if (AI_SUCCESS == mat->Get(AI_MATKEY_SHININESS, value))
	{
	}
	if (AI_SUCCESS == mat->Get(AI_MATKEY_SHININESS_STRENGTH, value))
	{
	}
	if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_TRANSPARENT, color))
	{
	}
}
#endif 




AssetImporter::AssetImporter() : m_pImpl(new AssetImporterImpl)
{
};


void AssetImporter::Init(std::shared_ptr<rdr::RenderDevice>& renderDevice)
{
	m_pImpl->SetRenderDevice(renderDevice);
}



AssetImporter& AssetImporter::Instance()
{
	static AssetImporter instance = AssetImporter();
	return instance;
}
}

