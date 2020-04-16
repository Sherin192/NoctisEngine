#include "NoctisCubeMap.h"
#include "Core_pch.h"
#include "Noctis3DTypes.h"
#include "Engine/ShapeGenerator.h"
#include "Engine/CameraFPS.h"
#include "Transform.h"

namespace noctis::rdr
{
CubeMap::CubeMap(std::shared_ptr<RenderDevice>& renderDevice, void* data, std::array<std::string, 6>& paths, unsigned int width, unsigned int height, unsigned int nrChannels, rdr::Format format/* = kRGBA8UN_SRGB */)
	: m_filePaths(paths), m_width(width), m_height(height), m_nrChannels(nrChannels), Base(renderDevice, data, width, height, nrChannels, TextureUsage::CUBE_MAP, format)
{
	Init(renderDevice);
}





void CubeMap::Render(std::shared_ptr<RenderDevice>& renderDevice, Camera& cam)
{
	auto view = cam.GetView();
	view.data = glm::mat4(glm::mat3(view.data));
	static auto transform = Transform().AsMatrix();
	math::Nmat4 WVP = cam.GetProj() * view * transform;

	CBModelData cbData(transform, WVP);

	//Update and bind Model constant buffer
	m_cbuffer.Update(renderDevice, cbData);
	m_cbuffer.Bind(renderDevice, 0);

	//TODO: This should be stored in a struct(RenderState) and passed to the pipelinePass which will set that render state before rendering an object.
	renderDevice->SetRasterizerState(RasterizerType::SOLID_CULL_NONE);
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> dsState;
	renderDevice->GetDevice()->CreateDepthStencilState(&depthStencilDesc, &dsState);
	renderDevice->GetDeviceContext()->OMSetDepthStencilState(dsState.Get(), 0);
	
	renderDevice->SetSampler(0, 1, GetSampler());
	renderDevice->GetDeviceContext()->PSSetShaderResources(0, 1, GetSRV().GetAddressOf());
	m_vertexBuffer.Bind(renderDevice);
	m_indexBuffer.Bind(renderDevice);

	renderDevice->GetDeviceContext()->DrawIndexed(m_indexBuffer.Size(), 0, 0);

	ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
	renderDevice->GetDeviceContext()->PSSetShaderResources(0, 1, nullSRV);

	renderDevice->SetRasterizerState(RasterizerType::SOLID_CULL_BACK);
	renderDevice->GetDeviceContext()->OMSetDepthStencilState(NULL, 0);

}





void CubeMap::Init(std::shared_ptr<RenderDevice>& renderDevice)
{
	Data data;
	sg::ShapeGenerator& sGen = sg::ShapeGenerator::Instance();
	sGen.CreateCube(1.0f, 1.0f, 1.0f, data);
	m_vertexBuffer.SetStride(sizeof(data.vertices[0]));
	m_vertexBuffer.SetOffset(0);
	m_vertexBuffer.Init(std::forward<std::shared_ptr<RenderDevice>>(renderDevice), false, data.vertices.data(), data.vertices.size());
	m_indexBuffer.Init(renderDevice, false, data.indices.data(), data.indices.size());
}
}