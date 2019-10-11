#include "Core_pch.h"
#if _DEBUG
#include "Model.h" //TODO: Debug Only 
#endif //_DEBUG
#include "Light.h"
#include "NoctisPipelinePass.h"

namespace noctis::rdr
{
static void RenderPointLight(std::shared_ptr<RenderDevice>& renderDevice, PipelinePass* pass, Camera& camera, PointLight* light)
{
	static Model model(renderDevice, sg::Shape::CUBE);
	const DirectX::XMVECTOR rotaxis = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	const DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationAxis(rotaxis, 0.0f);

	Transform t{ {light->position.x, light->position.y, light->position.z}, {0.0f, 0.0f, 0.0f}, {0.2f, 0.2f, 0.2f} };
	model.SetTransform(t);

	GPUMaterial gpuMaterial;
	gpuMaterial.ambient = light->ambient;
	gpuMaterial.diffuse = light->diffuse;
	gpuMaterial.specular = light->specular;

	model.SetMaterial(Material(gpuMaterial), sg::kShapeNameCube);
	pass->Render(renderDevice, model, camera);
}


void PointLight::Render(std::shared_ptr<RenderDevice>& renderDevice, PipelinePass* pass, Camera& camera)
{
	RenderPointLight(renderDevice, pass, camera, this);
}
}