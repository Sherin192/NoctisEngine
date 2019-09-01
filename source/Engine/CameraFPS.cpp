//#include "Core_pch.h"
#include "Renderer/NoctisRenderDevice.h"
#include "CameraFPS.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

namespace noctis
{
//===================================================================================
//	Constructor

Camera::Camera(Vector3 pos, Vector3 target, Vector3 up) :
	m_position(pos), m_up(up), m_pitch(0), m_yaw(0)
{
	m_forward = m_position - target;
	m_forward.Normalize();

	m_mouse_sensitivity = 0.005f;

	Rotate(Vector2(0.0f, 0.0f));
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

	XMStoreFloat4x4(&m_proj, XMMatrixPerspectiveFovLH(fov, aspectRatio, nearPlane, farPlane));
}

//-----------------------------------------------------------------------------------



//===================================================================================
//	UpdateViewMatrix : Updata the view matrix.

void Camera::UpdateViewMatrix()
{
	//XMMatrixLookAtRH takes a position of the camera, a target and the up vector.
	//As target I use the position of the camera plus the normalized direction that the camera is faceing.
	XMStoreFloat4x4(&m_view, XMMatrixLookAtRH(m_position, m_position + m_forward, Vector3::Up));
}

//-----------------------------------------------------------------------------------



//===================================================================================
//	Move : Moves the camera given a direction vector.

void Camera::Move(Vector3 direction)
{
	//The vector m_forward represents the facing direction of the camera.
	//Hence we get the movement in the z direction which and multiply it
	//with this vector to get the movement along our facing vector. We do 
	//the same for the right vector and the laterar movement.
	m_position += direction.x * m_right;
	m_position -= direction.z * m_forward;
	UpdateViewMatrix();
}

//-----------------------------------------------------------------------------------



//===================================================================================
//	Rotate : Rotates the cameara based on the mouse movement.

void Camera::Rotate(Vector2 mouse)
{
	//We use a relative mouse position, hence the delta is simply the mouse.xy 
	//multiplied by the mouse sensitivity defined by the user.
	Vector3 delta = Vector3(float(mouse.x), float(mouse.y), 0.f) * m_mouse_sensitivity;

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

	m_forward = Vector3(x, y, z);
	m_forward.Normalize();

	m_right = m_forward.Cross(Vector3::Up);
	m_right.Normalize();
	
	m_up = m_forward.Cross(m_right);
	m_up.Normalize();

	UpdateViewMatrix();
}

//-----------------------------------------------------------------------------------
}