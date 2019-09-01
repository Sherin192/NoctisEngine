#include "AssimpModel.h"

//
//
//AssimpModel::AssimpModel(std::weak_ptr<RenderDevice> rDevice, std::string_view path)
//{
//	m_pRenderDevice = rDevice;
//	Load(path);
//}
//
//
//void AssimpModel::Render(Shader* shader)
//{
//	for (unsigned int i = 0; i < m_meshes.size(); i++)
//		m_meshes[i].Render(shader);
//}
//
//void AssimpModel::Load(std::string_view path)
//{
//	Assimp::Importer importer;
//	const aiScene *scene = importer.ReadFile(path.data(), aiProcess_Triangulate | aiProcess_FlipUVs);
//
//	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
//	{
//		std::string s = importer.GetErrorString();
//		std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
//		return;
//	}
//	directory = path.substr(0, path.find_last_of('/'));
//
//	ProcessNode(scene->mRootNode, scene);
//}
//void AssimpModel::ProcessNode(aiNode* node, const aiScene* scene)
//{
//	// process all the node's meshes (if any)
//	for (unsigned int i = 0; i < node->mNumMeshes; i++)
//	{
//		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
//		m_meshes.push_back(ProcessMesh(mesh, scene));
//	}
//	// then do the same for each of its children
//	for (unsigned int i = 0; i < node->mNumChildren; i++)
//	{
//		ProcessNode(node->mChildren[i], scene);
//	}
//}
//
//
//Mesh AssimpModel::ProcessMesh(aiMesh* mesh, const aiScene* scene)
//{
//	std::vector<Vertex> vertices;
//	std::vector<unsigned int> indices;
//
//	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
//	{
//		DirectX::XMFLOAT3 vector;
//		Vertex vertex;
//		// process vertex positions, normals and texture coordinates
//		vector.x = mesh->mVertices[i].x;
//		vector.y = mesh->mVertices[i].y;
//		vector.z = mesh->mVertices[i].z;
//		
//		vertex.position = vector;
//		// normals
//		vector.x = mesh->mNormals[i].x;
//		vector.y = mesh->mNormals[i].y;
//		vector.z = mesh->mNormals[i].z;
//		vertex.normal = vector;
//
//		// texture coordinates
//		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
//		{
//			DirectX::XMFLOAT2 vec;
//			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
//			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
//			vec.x = mesh->mTextureCoords[0][i].x;
//			vec.y = mesh->mTextureCoords[0][i].y;
//			vertex.texCoord = vec;
//		}
//		else
//			vertex.texCoord = DirectX::XMFLOAT2(0.0f, 0.0f);
//		// tangent
//		//vector.x = mesh->mTangents[i].x;
//		//vector.y = mesh->mTangents[i].y;
//		//vector.z = mesh->mTangents[i].z;
//		vertex.tangent = vector;
//		//// bitangent
//		//vector.x = mesh->mBitangents[i].x;
//		//vector.y = mesh->mBitangents[i].y;
//		//vector.z = mesh->mBitangents[i].z;
//		//vertex.biTangent = vector;
//		
//		vertices.push_back(vertex);
//
//	}
//	// process indices
//	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
//	{
//		aiFace face = mesh->mFaces[i];
//		// retrieve all indices of the face and store them in the indices vector
//		for (unsigned int j = 0; j < face.mNumIndices; j++)
//			indices.push_back(face.mIndices[j]);
//	}
//
//	return Mesh(m_pRenderDevice, vertices, indices);
//}