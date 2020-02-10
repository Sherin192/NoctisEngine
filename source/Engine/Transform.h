#ifndef _TRANSFORM_H
#define _TRANSFORM_H

namespace noctis
{
struct Transform
{
	math::Nvec3									position;
	math::Nvec3									rotation;
	math::Nvec3									scale;
	math::Nmat4									transform;
private:
	bool										dirty;
public:
	Transform() : position(), rotation(), scale(1.0f, 1.0f, 1.0f), transform(std::move(math::identity())) {}

	Transform(const math::Nvec3& pos, const math::Nvec3& rot, const math::Nvec3& scale)
		: position(pos), rotation(rot), scale(scale), transform(math::translation(position)* math::scaling(scale)* math::rotation(rotation)) {}

	Transform(const math::Nmat4& t)
		: transform(t)
	{
		position.x = t._14;
		position.y = t._24;
		position.z = t._34;
		scale.x = sqrt(pow(t._11, 2) + pow(t._21, 2) + pow(t._31, 2));
		scale.y = sqrt(pow(t._21, 2) + pow(t._22, 2) + pow(t._23, 2));
		scale.z = sqrt(pow(t._31, 2) + pow(t._32, 2) + pow(t._33, 2));
		rotation.x = atan2(t._32 / scale.y, t._33 / scale.z);
		rotation.y = -asin(t._31 / scale.x);
		rotation.z = atan2(t._21 / scale.x, t._11 / scale.x);
	}
	void Set(Transform& t)
	{
		position = t.position;
		rotation = t.rotation;
		scale = t.scale;
		transform = math::translation(position) * math::scaling(scale) * math::rotation(rotation);
		dirty = false;
	}
	auto AsMatrix()
	{
		dirty = false;
		//return transform;
		return math::translation(position) * math::scaling(scale) * math::rotation(rotation);
	}

	void Translate(const math::Nvec3& t)
	{
		position += t;
		dirty = true;
	}

	void Rotate(const math::Nvec3& r)
	{
		rotation += r;
		dirty = true;
	}

	void Scale(const math::Nvec3& s)
	{
		scale *= s;
		dirty = true;
	}

	void UScale(float s)
	{
		scale *= s;
		dirty = true;
	}
};
}
#endif //_TRANSFORM_H
