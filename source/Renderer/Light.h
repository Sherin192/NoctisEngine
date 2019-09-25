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
		class Shader;
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
	DirectX::SimpleMath::Vector4  ambient;
	DirectX::SimpleMath::Vector4  diffuse;
	DirectX::SimpleMath::Vector4  specular;
};

//====================================================================================





//------------------------------------------------------------------------------------
//		DirectionalLight: This light has only a direction.
//------------------------------------------------------------------------------------
struct DirectionalLight : public Light
{
	DirectX::SimpleMath::Vector3  direction;
	float pad;
};

//====================================================================================






//------------------------------------------------------------------------------------
//		PointLight: This light has a position and is attenuated over distance.
//------------------------------------------------------------------------------------
struct PointLight : public Light
{
	DirectX::SimpleMath::Vector3 position;
	float range;

	DirectX::SimpleMath::Vector3 attenuation;
	int enabled;

	void Render(std::shared_ptr<RenderDevice>& renderDevice, Shader& shader, Camera& camera);

	friend void RenderPointLight(std::shared_ptr<RenderDevice>& renderDevice, Shader& shader, Camera& camera, PointLight* light);
};

//====================================================================================
}	//noctis::rdr
//static void RenderPointLight(std::shared_ptr<RenderDevice>& renderDevice, Shader& shader, Camera& camera, PointLight* light);
#endif //LIGHT_H