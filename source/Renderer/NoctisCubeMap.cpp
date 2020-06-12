#include "NoctisCubeMap.h"
#include "Core_pch.h"
#include "Noctis3DTypes.h"
#include "Engine/ShapeGenerator.h"
#include "Engine/CameraFPS.h"
#include "Transform.h"

namespace noctis::rdr
{

//------------------------------------------------------------------------------------
//		CubeMap: Constructor.
//------------------------------------------------------------------------------------
CubeMap::CubeMap(std::shared_ptr<RenderDevice>& renderDevice, void* data, std::array<std::string, 6>& paths, unsigned int width, unsigned int height, unsigned int nrChannels, rdr::Format format/* = kRGBA8UN_SRGB */)
	: m_filePaths(paths), m_width(width), m_height(height), m_nrChannels(nrChannels), Base(renderDevice, data, width, height, nrChannels, TextureUsage::CUBE_MAP, format)
{
	Init(renderDevice);
}

//====================================================================================





//------------------------------------------------------------------------------------
//		Render: Renders the CubeMap.
//------------------------------------------------------------------------------------
void CubeMap::Render(std::shared_ptr<RenderDevice>& renderDevice, Camera& cam)
{
	auto view = cam.GetView();
	//Remove the translation from the view matrix so the cube map seems infinitely away.
	view.data = glm::mat4(glm::mat3(view.data));
	static auto transform = Transform().AsMatrix();
	
	CBModelData cbData(transform, view, cam.GetProj());

	//Update and bind Model constant buffer
	m_cbuffer.Update(renderDevice, cbData);
	m_cbuffer.Bind(renderDevice, 0);

	//TODO: This should be stored in a struct(RenderState) and passed to the pipelinePass which will set that render state before rendering an object.
	//Don't cull the back faces which are neededfor the skybox effect as we want to texture internal faces as well.
	renderDevice->SetRasterizerState(RasterizerType::SOLID_CULL_NONE);
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
	//Enable the depth test as we don't want to override any scene pixels.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> dsState;
	renderDevice->GetDevice()->CreateDepthStencilState(&depthStencilDesc, &dsState);
	renderDevice->GetDeviceContext()->OMSetDepthStencilState(dsState.Get(), 0);
	
	//TODO: This will need to be removed when samplers will be decoupled from textures.
	renderDevice->SetSampler(0, 1, GetSampler());
	renderDevice->GetDeviceContext()->PSSetShaderResources(0, 1, GetSRV().GetAddressOf());
	m_vertexBuffer.Bind(renderDevice);
	m_indexBuffer.Bind(renderDevice);

	renderDevice->GetDeviceContext()->DrawIndexed(m_indexBuffer.Size(), 0, 0);
	//Clean after the skybox resource.
	ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
	renderDevice->GetDeviceContext()->PSSetShaderResources(0, 1, nullSRV);
	//Reset the previous Rasterizer sate and depth stencile.
	//TODO: when the RenderState is implemented it should be saved at the begining and restored at this moment.
	renderDevice->SetRasterizerState(RasterizerType::SOLID_CULL_BACK);
	renderDevice->GetDeviceContext()->OMSetDepthStencilState(NULL, 0);

}

//====================================================================================





//------------------------------------------------------------------------------------
//		Init: Initialize the cube model for the cube map.
//------------------------------------------------------------------------------------
void CubeMap::Init(std::shared_ptr<RenderDevice>& renderDevice)
{
	Data data;
	//TODO: The shape generator should retur Noctis internal representations of vertex and idex buffers.
	sg::ShapeGenerator& sGen = sg::ShapeGenerator::Instance();
	sGen.CreateCube(1.0f, 1.0f, 1.0f, data);
	m_vertexBuffer.SetStride(sizeof(data.vertices[0]));
	m_vertexBuffer.SetOffset(0);
	m_vertexBuffer.Init(std::forward<std::shared_ptr<RenderDevice>>(renderDevice), false, data.vertices.data(), data.vertices.size());
	m_indexBuffer.Init(renderDevice, false, data.indices.data(), data.indices.size());
}

//====================================================================================

} //noctis::rdr