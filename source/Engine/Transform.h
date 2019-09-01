#ifndef _TRANSFORM_H
#define _TRANSFORM_H

namespace noctis
{
struct Transform
{
	DirectX::XMFLOAT4X4							position;
	DirectX::XMFLOAT4X4							rotation;
	DirectX::XMFLOAT4X4							scale;
	DirectX::XMMATRIX							matrix;
	
	Transform() = default;
	Transform(DirectX::FXMMATRIX _rotation, DirectX::CXMMATRIX _position, DirectX::CXMMATRIX _scale)
		: position(), rotation(), scale(), matrix()
	{
		Set(_rotation, _position, _scale);
	}

	void Set(DirectX::FXMMATRIX _rotation, DirectX::CXMMATRIX _position, DirectX::CXMMATRIX _scale)
	{
		XMStoreFloat4x4(&rotation, _rotation);
		XMStoreFloat4x4(&position, _position);
		XMStoreFloat4x4(&scale, _scale);
		matrix = _scale * _position * _rotation;
	}

	void Set(Transform& t)
	{
		rotation = t.rotation;
		position = t.position;
		scale = t.scale;
		
		DirectX::XMMATRIX rot = XMLoadFloat4x4(&rotation); 
		DirectX::XMMATRIX pos = XMLoadFloat4x4(&position);
		DirectX::XMMATRIX scl = XMLoadFloat4x4(&scale);
		matrix = scl * pos * rot;
	}
};
}
#endif //_TRANSFORM_H
