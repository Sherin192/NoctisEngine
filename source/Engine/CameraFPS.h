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
	Camera(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 forward, DirectX::SimpleMath::Vector3 up);

	//-----------------------------------------------------------------------------------


	//===================================================================================
	//	Public Initializers

	void InitProjMatrix(float fov, float width, float height, float nearPlane, float farPlane);

	//-----------------------------------------------------------------------------------


	//===================================================================================
	//	Getters

	const DirectX::SimpleMath::Vector3		GetPosition()		const { return m_position; }
	const DirectX::SimpleMath::Vector3		GetTarget()			const { return m_forward; }
	const DirectX::SimpleMath::Vector3		GetUp()				const { return m_up; }
	const DirectX::XMMATRIX		GetView()			const { return DirectX::XMLoadFloat4x4(&m_view); }
	const DirectX::XMMATRIX		GetProj()			const { return DirectX::XMLoadFloat4x4(&m_proj); }
	const float			GetFOV()			const { return m_fov; }
	const float			GetAspectRatio()	const { return m_width / m_height; }
	const float			GetNear()			const { return m_near; }
	const float			GetFar()			const { return m_far; }

	//-----------------------------------------------------------------------------------

	//===================================================================================
	//	Setters

	void SetPosition(const DirectX::SimpleMath::Vector3& pos) { m_position = pos; }
	void SetMouseSensitivity(float value) { m_mouse_sensitivity = value;  }

	//-----------------------------------------------------------------------------------

	//===================================================================================
	//	Public Interactions

	void Move(DirectX::SimpleMath::Vector3);
	void Rotate(DirectX::SimpleMath::Vector2);

	//-----------------------------------------------------------------------------------

private:

	//===================================================================================
	//	Private Initializers

	void UpdateViewMatrix();

	//-----------------------------------------------------------------------------------

private:
	//===================================================================================
	//	Private Data

	DirectX::SimpleMath::Vector3					m_position;
	DirectX::SimpleMath::Vector3					m_forward;
	DirectX::SimpleMath::Vector3					m_up;
	DirectX::SimpleMath::Vector3					m_right;

	float					m_pitch;
	float					m_yaw;

	float					m_fov;
	float					m_width;
	float					m_height;
	float					m_near;
	float					m_far;

	float					m_mouse_sensitivity;

	DirectX::XMFLOAT4X4				m_view;
	DirectX::XMFLOAT4X4				m_proj;

	//-----------------------------------------------------------------------------------

};
}
#endif //_Camera_H