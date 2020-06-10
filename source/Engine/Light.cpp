#include "Core_pch.h"
//#if _DEBUG || NOCTIS_ALWAYS_RENDER_LIGHTS
#include "Model.h" //TODO: Debug Only 
//#endif //_DEBUG || NOCTIS_ALWAYS_RENDER_LIGHTS
#include "Light.h"

namespace noctis::rdr
{
NoctisLight::NoctisLight(std::shared_ptr<RenderDevice>& renderDevice, Type type, math::Nvec4 color, math::Nvec3 position /*= math::Nvec3::Zero()*/, math::Nvec3 direction /*= math::Nvec3::Zero()*/, math::Nvec3 attenuation /*= math::Nvec3::Zero()*/)
	: m_enabled(true),
	m_transform(position, direction, { 0.1f, 0.1f, 0.1f }), 
	m_color(color), 
	m_attenuation(attenuation), 
	m_lightType(type), 
	m_model(new Model(renderDevice, sg::Shape::CUBE, "Light"))
{
}





Model& NoctisLight::GetModel(std::shared_ptr<RenderDevice>& renderDevice) const noexcept
{
	return *m_model;
}





void NoctisLight::Render(std::shared_ptr<RenderDevice>& renderDevice, Camera& cam)
{
	if (!m_enabled)
		return;
	m_model->SetTransform(m_transform);
	std::string name = "_LightMaterial";
	std::shared_ptr<Material> material;
	if (material = MaterialPool::Instance().GetMaterial(name); !material)
	{
		material = std::make_shared<PhongMaterial>(name);

		MaterialPool::Instance().AddMaterial(name, material);
	}

	PhongMaterial* derived = dynamic_cast<PhongMaterial*>(&*material);
	//derived->SetAmbient(math::Nvec4(0.0f, 0.0f, 0.0f, 0.0f));
	derived->SetDiffuse(m_color);
	derived->SetSpecular(m_color);
	derived->Update(renderDevice);
	m_model->SetMaterial(name, sg::kShapeNameCube);

	m_model->Render(renderDevice, cam);
}
} //noctis::rdr