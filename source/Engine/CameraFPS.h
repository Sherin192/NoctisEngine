#ifndef _Camera_H
#define _Camera_H

namespace noctis
{
class Camera
{
public:
	

	//===================================================================================
	//	Constructors

	Camera() = default;

	Camera(math::Nvec3 pos, float pitch, float yaw, float fov, float width, float height, float nearPlane, float farPlane);

	//-----------------------------------------------------------------------------------

	//===================================================================================
	//	Getters
	const auto& GetPosition()		const { return m_position; }
	const auto& GetTarget()			const { return m_forward; }
	const auto& GetUp()				const { return m_up; }
	const auto& GetView()			const { return m_view; }
	const auto& GetProj()			const { return m_proj; }
	const auto& GetFOV()			const { return m_fov; }
	const auto GetAspectRatio()		const { return m_width / m_height; }
	const auto& GetNear()			const { return m_near; }
	const auto& GetFar()			const { return m_far; }

	//-----------------------------------------------------------------------------------

	//===================================================================================
	//	Setters
	void SetPosition(const math::Nvec3& pos) { m_position = pos; }

	void SetMouseSensitivity(float value) { m_mouse_sensitivity = value;  }

	//-----------------------------------------------------------------------------------

	//===================================================================================
	//	Public Interactions
	void Move(math::Nvec3);
	void Rotate(math::Nvec2);
	//-----------------------------------------------------------------------------------

private:

	//===================================================================================
	//	Private Initializers
	void InitProjMatrix(float fov, float width, float height, float nearPlane, float farPlane);

	void UpdateViewMatrix();

	//-----------------------------------------------------------------------------------

private:
	//===================================================================================
	//	Private Data
	math::Nvec3				m_position;
	math::Nvec3				m_forward;
	math::Nvec3				m_up;
	math::Nvec3				m_right;

	float					m_pitch;
	float					m_yaw;

	float					m_fov;
	float					m_width;
	float					m_height;
	float					m_near;
	float					m_far;

	float					m_mouse_sensitivity;
	math::Nmat4				m_view;
	math::Nmat4				m_proj;
	//-----------------------------------------------------------------------------------
};
}
#endif //_Camera_H