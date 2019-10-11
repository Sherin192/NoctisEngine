#ifndef _TRANSFORM_H
#define _TRANSFORM_H

namespace noctis
{
struct Transform
{
	math::Nvec3									position;
	math::Nvec3									rotation;
	math::Nvec3									scale;

	Transform() : position(), rotation(), scale(1.0f, 1.0f, 1.0f) {}

	Transform(const math::Nvec3& pos, const math::Nvec3& rot, const math::Nvec3& scale)
		: position(pos), rotation(rot), scale(scale)
	{
	}
	void Set(Transform& t)
	{
		position = t.position;
		rotation = t.rotation;
		scale = t.scale;
	}
	auto AsMatrix()
	{
		return math::rotation(rotation) * math::translation(position) * math::scaling(scale);
	}

	void Translate(const math::Nvec3& t)
	{
		position += t;
	}

	void Rotate(const math::Nvec3& r)
	{
		rotation += r;
	}

	void Scale(const math::Nvec3& s)
	{
		scale *= s;
	}

	void UScale(float s)
	{
		scale *= s;
	}
};
}
#endif //_TRANSFORM_H
