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
	std::string name = "_LightMaterial";
	std::shared_ptr<Material> material;
	if (material = MaterialPool::Instance().GetMaterial(name); !material)
	{
		material = std::make_shared<PhongMaterial>(name);
		
		MaterialPool::Instance().AddMaterial(name, material);
	}
	
	PhongMaterial* derived =  dynamic_cast<PhongMaterial*>(&*material);
	derived->SetAmbient(math::Nvec4(0.0f,0.0f,0.0f,0.0f));
	derived->SetDiffuse(light->diffuse);
	derived->SetSpecular(light->specular);
	derived->Update(renderDevice);
	model.SetMaterial(name, sg::kShapeNameCube);
	

	pass->Render(renderDevice, model, camera);
}


void PointLight::Render(std::shared_ptr<RenderDevice>& renderDevice, PipelinePass* pass, Camera& camera)
{
	RenderPointLight(renderDevice, pass, camera, this);
}
}