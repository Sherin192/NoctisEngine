//#include "Core_pch.h"
#include "Renderer/NoctisRenderDevice.h"
#include "CameraFPS.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;

namespace noctis
{
	using namespace math;

	//===================================================================================
//	Constructor
	Camera::Camera(Nvec3 pos, float pitch, float yaw, float fov, float width, float height, float nearPlane, float farPlane) :
		m_position(pos), 
		m_forward(), 
		m_up(), 
		m_right(),
		m_pitch(pitch), 
		m_yaw(yaw), 
		m_fov(fov), 
		m_width(width), 
		m_height(height), 
		m_near(nearPlane), 
		m_far(farPlane), 
		m_mouse_sensitivity(0.005f),
		m_view(), 
		m_proj()
	{
		InitProjMatrix(m_fov, m_width, m_height, m_near, m_far);
		Rotate((0.0f, 0.0f));
	};

	//-----------------------------------------------------------------------------------



	//===================================================================================
	//	InitProjMatrix : Initializes the projection matrix.

	void Camera::InitProjMatrix(float fov, float width, float height, float nearPlane, float farPlane)
	{
		m_fov = fov;
		m_width = width;
		m_height = height;
		m_near = nearPlane;
		m_far = farPlane;

		float aspectRatio = width / height;
		m_proj = perspectiveLH01(fov, aspectRatio, nearPlane, farPlane);
	}

	//-----------------------------------------------------------------------------------



	//===================================================================================
	//	UpdateViewMatrix : Updata the view matrix.

	void Camera::UpdateViewMatrix()
	{
		//XMMatrixLookAtRH takes a position of the camera, a target and the up vector.
		//As target I use the position of the camera plus the normalized direction that the camera is faceing.

		m_view = lookAtLH(m_position, m_position + m_forward, Nvec3::Up());
	}

	//-----------------------------------------------------------------------------------



	//===================================================================================
	//	Move : Moves the camera given a direction vector.

	void Camera::Move(math::Nvec3 direction)
	{
		//The vector m_forward represents the facing direction of the camera.
		//Hence we get the movement in the z direction which and multiply it
		//with this vector to get the movement along our facing vector. We do 
		//the same for the right vector and the laterar movement.
		m_position += direction.x * m_right;
		m_position += direction.z * m_forward;
		UpdateViewMatrix();
	}

	//-----------------------------------------------------------------------------------



	//===================================================================================
	//	Rotate : Rotates the cameara based on the mouse movement.

	void Camera::Rotate(Nvec2 mouse)
	{
		//We use a relative mouse position, hence the delta is simply the mouse.xy 
		//multiplied by the mouse sensitivity defined by the user.
		Nvec3 delta = Nvec3(float(mouse.x), float(mouse.y), 0.f) * m_mouse_sensitivity;

		m_pitch -= delta.y;
		m_yaw -= delta.x;

		//Limit pitch to straight up or straight down to avoid gimbal lock.
		float limit = XM_PI / 2.0f - 0.01f;

		m_pitch = m_pitch < -limit ? -limit : m_pitch;
		m_pitch = m_pitch > limit ? limit : m_pitch;

		//keep longitude in sane range by wrapping
		if (m_yaw > XM_PI)
		{
			m_yaw -= XM_PI * 2.0f;
		}
		else if (m_yaw < -XM_PI)
		{
			m_yaw += XM_PI * 2.0f;
		}

		float r = cosf(m_pitch);
		float x = r * cosf(m_yaw);		// x = cos(pitch) * cos(yaw)
		float y = sinf(m_pitch);		// y = sin(pith)
		float z = r * sinf(m_yaw);		// z = cos(pitch) * sin(yaw)

		m_forward = Nvec3(x, y, z);
		normalize(m_forward);

		m_right = cross(m_forward, Nvec3::Up());
		normalize(m_right);

		m_up = cross(m_forward, m_right);
		normalize(m_up);

		UpdateViewMatrix();
	}
//-----------------------------------------------------------------------------------
}