#include "Core_pch.h"
#include "Model.h" //TODO: Debug Only 
#include "Light.h"

namespace noctis::rdr
{
static void RenderPointLight(std::shared_ptr<RenderDevice>& renderDevice, Shader& shader, Camera& camera, PointLight* light)
{
	static Model model(renderDevice, sg::Shape::CUBE);
	const DirectX::XMVECTOR rotaxis = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	const DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationAxis(rotaxis, 0.0f);

	Transform t;
	XMStoreFloat4x4(&t.position, DirectX::XMMatrixTranslation(light->position.x - 0.5f, light->position.y - 0.5f, light->position.z - 0.5f));
	XMStoreFloat4x4(&t.rotation, rotation);
	XMStoreFloat4x4(&t.scale, DirectX::XMMatrixScaling(0.2f, 0.2f, 0.2f));
	model.SetTransform(t);

	GPUMaterial gpuMaterial;
	gpuMaterial.ambient = light->ambient;
	gpuMaterial.diffuse = light->diffuse;
	gpuMaterial.specular = light->specular;

	model.SetMaterial(Material(gpuMaterial), sg::kShapeNameCube);
//model.Render(renderDevice, shader, camera);
}


void PointLight::Render(std::shared_ptr<RenderDevice>& renderDevice, Shader& shader, Camera& camera)
{
	RenderPointLight(renderDevice, shader, camera, this);
}
}