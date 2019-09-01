#ifndef LIGHT_H
#define LIGHT_H
//#include "Core_pch.h"
namespace noctis
{ 
class Camera;
}

namespace noctis::rdr
{
	class RenderDevice;
class Shader;

struct Light
{
	DirectX::SimpleMath::Vector4  ambient;
	DirectX::SimpleMath::Vector4  diffuse;
	DirectX::SimpleMath::Vector4  specular;
};
struct DirectionalLight : public Light
{
	DirectX::SimpleMath::Vector3  direction;
	float pad;
};

struct PointLight : public Light
{
	DirectX::SimpleMath::Vector3 position;
	float range;

	DirectX::SimpleMath::Vector3 attenuation;
	int enabled;

	void Render(std::shared_ptr<RenderDevice>& renderDevice, Shader& shader, Camera& camera);

	friend void RenderPointLight(std::shared_ptr<RenderDevice>& renderDevice, Shader& shader, Camera& camera, PointLight* light);
};
}
//static void RenderPointLight(std::shared_ptr<RenderDevice>& renderDevice, Shader& shader, Camera& camera, PointLight* light);
#endif //LIGHT_H