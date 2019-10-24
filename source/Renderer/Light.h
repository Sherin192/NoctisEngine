#ifndef LIGHT_H
#define LIGHT_H
#include "Core_pch.h"

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
	math::Nvec4 specular;
};

//====================================================================================





//------------------------------------------------------------------------------------
//		DirectionalLight: This light has only a direction.
//------------------------------------------------------------------------------------
struct DirectionalLight : public Light
{
	math::Nvec4	ambient;
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

	void Render(std::shared_ptr<RenderDevice>& renderDevice, PipelinePass* pass, Camera& camera);

	friend void RenderPointLight(std::shared_ptr<RenderDevice>& renderDevice, PipelinePass* pass, Camera& camera, PointLight* light);
};
static void RenderPointLight(std::shared_ptr<RenderDevice>& renderDevice, PipelinePass* pass, Camera& camera, PointLight* light);

//====================================================================================
}	//noctis::rdr
#endif //LIGHT_H