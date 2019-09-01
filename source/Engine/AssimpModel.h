#pragma once
#include <memory>
#include "RenderDevice.h"
#include "Shader.h"
#include "Mesh.h"
#include "DirectXMath.h"


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>



//class Model
//{
//public:
//	Model(std::weak_ptr<RenderDevice> rDevice, std::string_view path);
//
//	void				Render(Shader* shader);
//	//bool				Generate(std::weak_ptr<RenderDevice> rDevice, sg::Shape shape);
//
//	//void			   SetWorldMtx(DirectX::FXMMATRIX mtx) { XMStoreFloat4x4(&m_worldMtx, mtx); }
//	//DirectX::XMMATRIX  GetWorldMtx() const { return XMLoadFloat4x4(&m_worldMtx); }
//
//	//void			   SetWVP(DirectX::FXMMATRIX wvp) { DirectX::XMStoreFloat4x4(&m_cb.worldViewProj, DirectX::XMMatrixTranspose(wvp)); }
//
//	//void			   SetTransform(DirectX::FXMMATRIX, DirectX::CXMMATRIX, DirectX::CXMMATRIX);
//	//void			   SetCBuffer(CBPerObject cb) { m_cb = cb; }
//	//const CBPerObject& GetCBuffer() const { return m_cb; }
//
//private:
//	void Load(std::string_view path);
//	void ProcessNode(aiNode* node, const aiScene* scene);
//	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
//	//vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
//
//	std::weak_ptr<RenderDevice> m_pRenderDevice;
//	std::vector<Mesh>							m_meshes;
//	std::string									directory;
//
//	unsigned									m_nIndices;
//
//	DirectX::XMFLOAT4X4							m_worldMtx;
//	Transform									m_transform;
//	CBPerObject									m_cb;
//};
//
