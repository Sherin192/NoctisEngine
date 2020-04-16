#ifndef LIGHT_H
#define LIGHT_H
#include "Core_pch.h"
#include "NoctisRenderable.h"
#include "Transform.h"
//------------------------------------------------------------------------------------
//		Forward Declarations:
//------------------------------------------------------------------------------------
namespace noctis
{
	class Camera;
	namespace rdr
	{
		class RenderDevice;
		class PipelinePass;
		class Model;
	}
}
//====================================================================================





namespace noctis::rdr
{
//------------------------------------------------------------------------------------
//		Light: Base class for other types of Lights.
//------------------------------------------------------------------------------------
struct Light
{
	math::Nvec4 diffuse;
};

//====================================================================================





//------------------------------------------------------------------------------------
//		DirectionalLight: This light has only a direction.
//------------------------------------------------------------------------------------
struct DirectionalLight : public Light
{
	math::Nvec3 direction;
	float pad;
};

//====================================================================================






//------------------------------------------------------------------------------------
//		PointLight: This light has a position and is attenuated over distance.
//------------------------------------------------------------------------------------
struct PointLight : public Light
{
	math::Nvec3 position;
	float pad;
	math::Nvec3 attenuation;
	uint32_t enabled;
};





class NoctisLight : public IRenderable<NoctisLight>
{
public:
	enum class Type { kDirectional, kPoint, kSpot };
	NoctisLight(std::shared_ptr<RenderDevice>& renderDevice, Type type, math::Nvec4 color, math::Nvec3 position = math::Nvec3::Zero(), math::Nvec3 direction = math::Nvec3::Zero(), math::Nvec3 attenuation = math::Nvec3::Zero());
	
	void Render(std::shared_ptr<RenderDevice>& renderDevice, Camera& cam);
	Model& GetModel(std::shared_ptr<RenderDevice>& renderDevice) const noexcept;
	Transform& GetTransform() noexcept { return m_transform; }
	auto& GetColor() { return m_color; }
	auto& GetAttenuation() { return m_attenuation; }
	bool m_enabled;
private:
	Transform m_transform;
	math::Nvec4 m_color;
	math::Nvec3 m_attenuation;
	Type m_lightType;
	Model* m_model;
	};


//====================================================================================
}	//noctis::rdr
#endif //LIGHT_H